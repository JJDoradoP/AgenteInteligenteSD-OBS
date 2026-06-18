import json
import os
import time
import random
import math
from collections import deque
import torch
import torch.nn as nn
import torch.optim as optim

IN_FILE  = "../simulations/agent_in.json"
OUT_FILE = "../simulations/agent_out.json"

L_MIN = 10000     # 10 KB mínimo
L_MAX = 100000    # 100 KB máximo

# Límites T diferenciados por clase de servicio (DiffServ)
# svc=0: Voz/EF  -> ráfagas pequeñas, latencia crítica. T de 10ms a 20ms
# svc=1: Video/AF -> balance entre throughput y delay. T de 20ms a 50ms
# svc=2: Datos/BE -> maximizar uso del canal, acepta mayor espera. T de 50ms a 100ms
T_MIN_BY_SVC = {0: 0.010, 1: 0.020, 2: 0.050}
T_MAX_BY_SVC = {0: 0.020, 1: 0.050, 2: 0.100}

# ==========================================
# 1. ARQUITECTURA DE LA RED NEURONAL (DQN)
# ==========================================
class OBS_DQN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(OBS_DQN, self).__init__()
        # P2-B: Red algo más grande para aprovechar el 5° input (coreBlk)
        self.net = nn.Sequential(
            nn.Linear(input_dim, 128),
            nn.ReLU(),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Linear(64, output_dim)
        )

    def forward(self, x):
        return self.net(x)

# ==========================================
# 2. BUFFER DE EXPERIENCIAS (Experience Replay)
# ==========================================
class ReplayBuffer:
    """P2-A: Almacena transiciones (s, a, r, s') para aprendizaje en mini-batch."""
    def __init__(self, capacity=5000):
        self.buffer = deque(maxlen=capacity)

    def push(self, state, action, reward, next_state):
        self.buffer.append((state, action, reward, next_state))

    def sample(self, batch_size=32):
        return random.sample(self.buffer, min(batch_size, len(self.buffer)))

    def __len__(self):
        return len(self.buffer)

# ==========================================
# 3. AGENTE DE APRENDIZAJE PROFUNDO (DQN mejorado)
# ==========================================
class NeuralNetAgent:
    def __init__(self, actions, lr=0.001, gamma=0.9,
                 epsilon_start=0.3, epsilon_end=0.02, epsilon_decay=3000):
        self.actions  = actions
        self.gamma    = gamma

        # P2-C: Parámetros de epsilon con decaimiento exponencial
        self.epsilon_start = epsilon_start
        self.epsilon_end   = epsilon_end
        self.epsilon_decay = epsilon_decay
        self.steps_done    = 0

        # P1-A: input_dim=5 (rate, blk, coreBlk, qbytes, delay)
        # Antes era 4 sin coreBlk
        self.model        = OBS_DQN(input_dim=5, output_dim=len(actions))
        # P2-B: Red objetivo separada para estabilizar el cálculo de Bellman
        self.target_model = OBS_DQN(input_dim=5, output_dim=len(actions))
        self.target_model.load_state_dict(self.model.state_dict())
        self.target_model.eval()  # La target network no entrena directamente

        self.optimizer = optim.Adam(self.model.parameters(), lr=lr)
        self.loss_fn   = nn.MSELoss()

        self.last_state  = {}
        self.last_action = {}

        # P2-A: Buffer de experiencias
        self.replay_buffer    = ReplayBuffer(capacity=5000)
        self.batch_size       = 32
        self.min_replay_size  = 100   # No aprender hasta tener suficientes muestras

        # P2-B: Frecuencia de actualización de la target network (soft update)
        self.target_update_freq = 200
        self.train_step_count   = 0

        self.model_file = '../simulations/results/dqn_model.pth'
        self.load_model()

    def load_model(self):
        if os.path.exists(self.model_file):
            try:
                self.model.load_state_dict(
                    torch.load(self.model_file, weights_only=True))
                self.target_model.load_state_dict(self.model.state_dict())
                print(f"[AGENT DQN] Modelo cargado de {self.model_file}")
            except Exception as e:
                print(f"[AGENT DQN] No se pudo cargar el modelo anterior "
                      f"(posiblemente por cambio de arquitectura): {e}")
                print("[AGENT DQN] Iniciando entrenamiento desde cero.")

    def save_model(self):
        try:
            torch.save(self.model.state_dict(), self.model_file)
        except Exception as e:
            print(f"[AGENT DQN] Error guardando modelo: {e}")

    def get_state_tensor(self, rate, blk, core_blk, qbytes, delay):
        """P1-A: Estado ampliado con información global del Core."""
        norm_rate     = rate / 1e8         # ~100 Mbps -> ~1.0
        norm_q        = qbytes / 100000.0  # ~100 KB   -> ~1.0
        norm_delay    = delay / 0.1        # ~100 ms   -> ~1.0
        # core_blk: fracción EWMA de bloqueos en el Core [0, 1]
        # blk: fracción local de bloqueo en el Edge      [0, 1]
        return torch.tensor(
            [norm_rate, blk, core_blk, norm_q, norm_delay],
            dtype=torch.float32)

    def get_epsilon(self):
        """P2-C: Decaimiento exponencial de epsilon."""
        eps = self.epsilon_end + (self.epsilon_start - self.epsilon_end) * \
              math.exp(-1.0 * self.steps_done / self.epsilon_decay)
        self.steps_done += 1
        return eps

    def choose_action(self, state_tensor):
        """Exploración (epsilon-greedy con decaimiento) vs Explotación."""
        if random.random() < self.get_epsilon():
            return random.randint(0, len(self.actions) - 1)
        with torch.no_grad():
            q_values = self.model(state_tensor)
        return torch.argmax(q_values).item()

    def update_target_network(self):
        """P2-B: Soft update de la target network: θ_t = τ·θ + (1-τ)·θ_t"""
        tau = 0.01
        for t_param, m_param in zip(self.target_model.parameters(),
                                    self.model.parameters()):
            t_param.data.copy_(tau * m_param.data + (1.0 - tau) * t_param.data)

    def learn_from_replay(self):
        """P2-A: Aprendizaje por mini-batch desde el replay buffer."""
        if len(self.replay_buffer) < self.min_replay_size:
            return

        batch = self.replay_buffer.sample(self.batch_size)
        self.model.train()

        for state, action_idx, reward, next_state in batch:
            q_values  = self.model(state)
            current_q = q_values[action_idx]

            # P2-B: Usar target_model para el cálculo del target de Bellman
            with torch.no_grad():
                next_q_values = self.target_model(next_state)
                target_q = reward + self.gamma * torch.max(next_q_values)

            target_q = torch.tensor(target_q.item(), dtype=torch.float32)
            loss = self.loss_fn(current_q, target_q)

            self.optimizer.zero_grad()
            loss.backward()
            # Gradient clipping para estabilidad
            torch.nn.utils.clip_grad_norm_(self.model.parameters(), max_norm=1.0)
            self.optimizer.step()

        self.train_step_count += 1
        # P2-B: Actualizar target network periódicamente
        if self.train_step_count % self.target_update_freq == 0:
            self.update_target_network()


# ==========================================
# 4. ESPACIO DE ACCIONES Y CONFIGURACIÓN
# ==========================================
L_STEPS = [-10000, 0, 10000]
T_STEPS = [-0.001, 0.0,  0.001]
ACTION_SPACE = [(l, t) for l in L_STEPS for t in T_STEPS]

# Instanciar el agente mejorado
agent_nn = NeuralNetAgent(actions=ACTION_SPACE)

# Configuración del CSV para el Dataset
DECISION_COUNT = 0
CSV_DIR  = "../simulations/results"
os.makedirs(CSV_DIR, exist_ok=True)
CSV_FILE = os.path.join(CSV_DIR, "dataset_tesis.csv")

file_exists_init = os.path.exists(CSV_FILE)
f_csv = open(CSV_FILE, "a", buffering=8192)

if not file_exists_init:
    f_csv.write("time,edge_id,svc,rate_bps,blk,core_blk,blk_prob,q_bytes,"
                "delay,reward,epsilon,action_idx,new_L,new_T\n")

def clamp(v, lo, hi):
    return max(lo, min(hi, v))

# ==========================================
# 5. BUCLE PRINCIPAL DE DECISIÓN
# ==========================================
def decide(obs):
    global DECISION_COUNT
    actions = []

    sim_time = float(obs.get("t", 0.0))

    for e in obs.get("edges", []):
        edge_id  = e.get("edgeId", 0)
        svc      = e.get("svc", 0)
        rate     = float(e.get("rateBps", 0.0))
        blk      = float(e.get("blk", 0.0))
        # P1-A: nuevo campo del estado global del Core
        core_blk = float(e.get("coreBlk", 0.0))
        qbytes   = int(e.get("qBytes", 0))
        delay    = float(e.get("delay", 0.0))
        L        = int(e.get("L", 18000))
        T        = float(e.get("T", 0.05))

        agent_id = f"{edge_id}_{svc}"

        # Estado actual con 5 dimensiones
        current_state = agent_nn.get_state_tensor(rate, blk, core_blk, qbytes, delay)

        # -------------------------------------------------------
        # P1-B: Función de recompensa mejorada con overhead BCP
        # -------------------------------------------------------
        # bcp_overhead: penaliza ráfagas cortas (más BCPs por segundo)
        # Normalizado: L=10KB -> ~1.0, L=100KB -> ~0.1
        bcp_overhead_norm = (1e8 / max(L, 1)) / 30000.0
        # core_blk_penalty: penaliza bloqueos en el Core de forma proporcional
        core_blk_penalty = core_blk * 50.0

        reward = (rate / 1e6) \
                 - (blk * 100.0) \
                 - (delay * 10.0) \
                 - (bcp_overhead_norm * 2.0) \
                 - core_blk_penalty

        # P2-A: Guardar experiencia en el replay buffer y aprender
        if agent_id in agent_nn.last_state:
            last_s = agent_nn.last_state[agent_id]
            last_a = agent_nn.last_action[agent_id]
            # Añadir al buffer
            agent_nn.replay_buffer.push(last_s, last_a, reward, current_state)
            # Aprender de un mini-batch aleatorio del buffer
            agent_nn.learn_from_replay()

        # Elegir la siguiente acción
        action_idx = agent_nn.choose_action(current_state)
        delta_l, delta_t = agent_nn.actions[action_idx]

        # Calcular los nuevos valores con las restricciones DiffServ
        t_min = T_MIN_BY_SVC.get(svc, 0.005)
        t_max = T_MAX_BY_SVC.get(svc, 0.010)

        newL = clamp(L + delta_l, L_MIN, L_MAX)
        newT = clamp(T + delta_t, t_min, t_max)

        # Guardar estado y acción para la próxima iteración
        agent_nn.last_state[agent_id]  = current_state
        agent_nn.last_action[agent_id] = action_idx

        DECISION_COUNT += 1

        # Guardar el modelo y vaciar buffer periódicamente
        if DECISION_COUNT % 100 == 0:
            agent_nn.save_model()
            f_csv.flush()

        # Registrar en el dataset (añade columnas core_blk y epsilon)
        current_eps = agent_nn.epsilon_end + \
            (agent_nn.epsilon_start - agent_nn.epsilon_end) * \
            math.exp(-1.0 * agent_nn.steps_done / agent_nn.epsilon_decay)

        f_csv.write(f"{sim_time:.4f},{edge_id},{svc},{rate:.2f},{blk:.4f},"
                    f"{core_blk:.4f},{blk:.4f},{qbytes},{delay:.6f},"
                    f"{reward:.4f},{current_eps:.4f},{action_idx},{newL},{newT:.6f}\n")

        print(f"[AGENT] t={sim_time:.2f} edge={edge_id} svc={svc} "
              f"r={reward:.2f} ε={current_eps:.3f} "
              f"coreBlk={core_blk:.3f} -> (L={newL}, T={newT:.4f})")

        actions.append({
            "edgeId": edge_id,
            "svc":    svc,
            "L":      int(newL),
            "T":      float(newT)
        })

    return {"actions": actions}

# ==========================================
# 6. INTEGRACIÓN CON OMNeT++ VÍA ARCHIVOS
# ==========================================
print("[AGENT DQN v2] Iniciado — Experience Replay + Target Network + "
      "Estado Global Core. Esperando OMNeT++...")
print(f"  Espacio de estados: 5D (rate, blk_local, blk_core, qBytes, delay)")
print(f"  Espacio de acciones: {len(ACTION_SPACE)} acciones")
print(f"  Epsilon: {agent_nn.epsilon_start} → {agent_nn.epsilon_end} "
      f"(decay={agent_nn.epsilon_decay} pasos)")
print(f"  Replay buffer: {agent_nn.replay_buffer.buffer.maxlen} transiciones")

last_mtime = None

while True:
    if os.path.exists(IN_FILE):
        mtime = os.path.getmtime(IN_FILE)
        if last_mtime is None or mtime != last_mtime:
            last_mtime = mtime

            try:
                time.sleep(0.001)  # Esperar a que OMNeT++ libere el archivo

                with open(IN_FILE, "r", encoding="utf-8") as f:
                    obs = json.load(f)

                resp = decide(obs)

                with open(OUT_FILE, "w", encoding="utf-8") as f:
                    json.dump(resp, f, separators=(",", ":"))

            except Exception as ex:
                print(f"[AGENT DQN] Error leyendo JSON, reintentando: {ex}")
                last_mtime = None

    time.sleep(0.005)
