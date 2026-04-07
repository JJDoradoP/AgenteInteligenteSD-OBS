//
// Generated file, do not edit! Created by opp_msgtool 6.1 from msgs/Obs.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Obs_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(DataPkt)

DataPkt::DataPkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

DataPkt::DataPkt(const DataPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DataPkt::~DataPkt()
{
}

DataPkt& DataPkt::operator=(const DataPkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataPkt::copy(const DataPkt& other)
{
    this->svc = other.svc;
}

void DataPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->svc);
}

void DataPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->svc);
}

int DataPkt::getSvc() const
{
    return this->svc;
}

void DataPkt::setSvc(int svc)
{
    this->svc = svc;
}

class DataPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_svc,
    };
  public:
    DataPktDescriptor();
    virtual ~DataPktDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DataPktDescriptor)

DataPktDescriptor::DataPktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DataPkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

DataPktDescriptor::~DataPktDescriptor()
{
    delete[] propertyNames;
}

bool DataPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataPkt *>(obj)!=nullptr;
}

const char **DataPktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DataPktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DataPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int DataPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_svc
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *DataPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "svc",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int DataPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "svc") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *DataPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_svc
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **DataPktDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DataPktDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DataPktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void DataPktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DataPkt'", field);
    }
}

const char *DataPktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataPktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_svc: return long2string(pp->getSvc());
        default: return "";
    }
}

void DataPktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_svc: pp->setSvc(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

omnetpp::cValue DataPktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_svc: return pp->getSvc();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DataPkt' as cValue -- field index out of range?", field);
    }
}

void DataPktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_svc: pp->setSvc(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

const char *DataPktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr DataPktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DataPktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

Register_Class(BCP)

BCP::BCP(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

BCP::BCP(const BCP& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BCP::~BCP()
{
    delete [] this->path;
}

BCP& BCP::operator=(const BCP& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BCP::copy(const BCP& other)
{
    this->burstId = other.burstId;
    this->srcId = other.srcId;
    this->dst = other.dst;
    this->ingressCore = other.ingressCore;
    this->bytes = other.bytes;
    this->offset = other.offset;
    this->srcName = other.srcName;
    this->srcEdge = other.srcEdge;
    this->dstEdge = other.dstEdge;
    this->pathLen = other.pathLen;
    delete [] this->path;
    this->path = (other.path_arraysize==0) ? nullptr : new int[other.path_arraysize];
    path_arraysize = other.path_arraysize;
    for (size_t i = 0; i < path_arraysize; i++) {
        this->path[i] = other.path[i];
    }
    this->hopIndex = other.hopIndex;
}

void BCP::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->burstId);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->dst);
    doParsimPacking(b,this->ingressCore);
    doParsimPacking(b,this->bytes);
    doParsimPacking(b,this->offset);
    doParsimPacking(b,this->srcName);
    doParsimPacking(b,this->srcEdge);
    doParsimPacking(b,this->dstEdge);
    doParsimPacking(b,this->pathLen);
    b->pack(path_arraysize);
    doParsimArrayPacking(b,this->path,path_arraysize);
    doParsimPacking(b,this->hopIndex);
}

void BCP::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->burstId);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->dst);
    doParsimUnpacking(b,this->ingressCore);
    doParsimUnpacking(b,this->bytes);
    doParsimUnpacking(b,this->offset);
    doParsimUnpacking(b,this->srcName);
    doParsimUnpacking(b,this->srcEdge);
    doParsimUnpacking(b,this->dstEdge);
    doParsimUnpacking(b,this->pathLen);
    delete [] this->path;
    b->unpack(path_arraysize);
    if (path_arraysize == 0) {
        this->path = nullptr;
    } else {
        this->path = new int[path_arraysize];
        doParsimArrayUnpacking(b,this->path,path_arraysize);
    }
    doParsimUnpacking(b,this->hopIndex);
}

int BCP::getBurstId() const
{
    return this->burstId;
}

void BCP::setBurstId(int burstId)
{
    this->burstId = burstId;
}

int BCP::getSrcId() const
{
    return this->srcId;
}

void BCP::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int BCP::getDst() const
{
    return this->dst;
}

void BCP::setDst(int dst)
{
    this->dst = dst;
}

int BCP::getIngressCore() const
{
    return this->ingressCore;
}

void BCP::setIngressCore(int ingressCore)
{
    this->ingressCore = ingressCore;
}

int BCP::getBytes() const
{
    return this->bytes;
}

void BCP::setBytes(int bytes)
{
    this->bytes = bytes;
}

double BCP::getOffset() const
{
    return this->offset;
}

void BCP::setOffset(double offset)
{
    this->offset = offset;
}

const char * BCP::getSrcName() const
{
    return this->srcName.c_str();
}

void BCP::setSrcName(const char * srcName)
{
    this->srcName = srcName;
}

int BCP::getSrcEdge() const
{
    return this->srcEdge;
}

void BCP::setSrcEdge(int srcEdge)
{
    this->srcEdge = srcEdge;
}

int BCP::getDstEdge() const
{
    return this->dstEdge;
}

void BCP::setDstEdge(int dstEdge)
{
    this->dstEdge = dstEdge;
}

int BCP::getPathLen() const
{
    return this->pathLen;
}

void BCP::setPathLen(int pathLen)
{
    this->pathLen = pathLen;
}

size_t BCP::getPathArraySize() const
{
    return path_arraysize;
}

int BCP::getPath(size_t k) const
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    return this->path[k];
}

void BCP::setPathArraySize(size_t newSize)
{
    int *path2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = path_arraysize < newSize ? path_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        path2[i] = this->path[i];
    for (size_t i = minSize; i < newSize; i++)
        path2[i] = 0;
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void BCP::setPath(size_t k, int path)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    this->path[k] = path;
}

void BCP::insertPath(size_t k, int path)
{
    if (k > path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize + 1;
    int *path2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    path2[k] = path;
    for (i = k + 1; i < newSize; i++)
        path2[i] = this->path[i-1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void BCP::appendPath(int path)
{
    insertPath(path_arraysize, path);
}

void BCP::erasePath(size_t k)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize - 1;
    int *path2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    for (i = k; i < newSize; i++)
        path2[i] = this->path[i+1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

int BCP::getHopIndex() const
{
    return this->hopIndex;
}

void BCP::setHopIndex(int hopIndex)
{
    this->hopIndex = hopIndex;
}

class BCPDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_burstId,
        FIELD_srcId,
        FIELD_dst,
        FIELD_ingressCore,
        FIELD_bytes,
        FIELD_offset,
        FIELD_srcName,
        FIELD_srcEdge,
        FIELD_dstEdge,
        FIELD_pathLen,
        FIELD_path,
        FIELD_hopIndex,
    };
  public:
    BCPDescriptor();
    virtual ~BCPDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(BCPDescriptor)

BCPDescriptor::BCPDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BCP)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

BCPDescriptor::~BCPDescriptor()
{
    delete[] propertyNames;
}

bool BCPDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BCP *>(obj)!=nullptr;
}

const char **BCPDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BCPDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BCPDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 12+base->getFieldCount() : 12;
}

unsigned int BCPDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_burstId
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_dst
        FD_ISEDITABLE,    // FIELD_ingressCore
        FD_ISEDITABLE,    // FIELD_bytes
        FD_ISEDITABLE,    // FIELD_offset
        FD_ISEDITABLE,    // FIELD_srcName
        FD_ISEDITABLE,    // FIELD_srcEdge
        FD_ISEDITABLE,    // FIELD_dstEdge
        FD_ISEDITABLE,    // FIELD_pathLen
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_path
        FD_ISEDITABLE,    // FIELD_hopIndex
    };
    return (field >= 0 && field < 12) ? fieldTypeFlags[field] : 0;
}

const char *BCPDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "burstId",
        "srcId",
        "dst",
        "ingressCore",
        "bytes",
        "offset",
        "srcName",
        "srcEdge",
        "dstEdge",
        "pathLen",
        "path",
        "hopIndex",
    };
    return (field >= 0 && field < 12) ? fieldNames[field] : nullptr;
}

int BCPDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "dst") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "ingressCore") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "bytes") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "offset") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "srcName") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "srcEdge") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "dstEdge") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "pathLen") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "path") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "hopIndex") == 0) return baseIndex + 11;
    return base ? base->findField(fieldName) : -1;
}

const char *BCPDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_burstId
        "int",    // FIELD_srcId
        "int",    // FIELD_dst
        "int",    // FIELD_ingressCore
        "int",    // FIELD_bytes
        "double",    // FIELD_offset
        "string",    // FIELD_srcName
        "int",    // FIELD_srcEdge
        "int",    // FIELD_dstEdge
        "int",    // FIELD_pathLen
        "int",    // FIELD_path
        "int",    // FIELD_hopIndex
    };
    return (field >= 0 && field < 12) ? fieldTypeStrings[field] : nullptr;
}

const char **BCPDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BCPDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BCPDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_path: return pp->getPathArraySize();
        default: return 0;
    }
}

void BCPDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_path: pp->setPathArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BCP'", field);
    }
}

const char *BCPDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BCPDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return long2string(pp->getBurstId());
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_dst: return long2string(pp->getDst());
        case FIELD_ingressCore: return long2string(pp->getIngressCore());
        case FIELD_bytes: return long2string(pp->getBytes());
        case FIELD_offset: return double2string(pp->getOffset());
        case FIELD_srcName: return oppstring2string(pp->getSrcName());
        case FIELD_srcEdge: return long2string(pp->getSrcEdge());
        case FIELD_dstEdge: return long2string(pp->getDstEdge());
        case FIELD_pathLen: return long2string(pp->getPathLen());
        case FIELD_path: return long2string(pp->getPath(i));
        case FIELD_hopIndex: return long2string(pp->getHopIndex());
        default: return "";
    }
}

void BCPDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(string2long(value)); break;
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_dst: pp->setDst(string2long(value)); break;
        case FIELD_ingressCore: pp->setIngressCore(string2long(value)); break;
        case FIELD_bytes: pp->setBytes(string2long(value)); break;
        case FIELD_offset: pp->setOffset(string2double(value)); break;
        case FIELD_srcName: pp->setSrcName((value)); break;
        case FIELD_srcEdge: pp->setSrcEdge(string2long(value)); break;
        case FIELD_dstEdge: pp->setDstEdge(string2long(value)); break;
        case FIELD_pathLen: pp->setPathLen(string2long(value)); break;
        case FIELD_path: pp->setPath(i,string2long(value)); break;
        case FIELD_hopIndex: pp->setHopIndex(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BCP'", field);
    }
}

omnetpp::cValue BCPDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return pp->getBurstId();
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_dst: return pp->getDst();
        case FIELD_ingressCore: return pp->getIngressCore();
        case FIELD_bytes: return pp->getBytes();
        case FIELD_offset: return pp->getOffset();
        case FIELD_srcName: return pp->getSrcName();
        case FIELD_srcEdge: return pp->getSrcEdge();
        case FIELD_dstEdge: return pp->getDstEdge();
        case FIELD_pathLen: return pp->getPathLen();
        case FIELD_path: return pp->getPath(i);
        case FIELD_hopIndex: return pp->getHopIndex();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BCP' as cValue -- field index out of range?", field);
    }
}

void BCPDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dst: pp->setDst(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ingressCore: pp->setIngressCore(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_bytes: pp->setBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_offset: pp->setOffset(value.doubleValue()); break;
        case FIELD_srcName: pp->setSrcName(value.stringValue()); break;
        case FIELD_srcEdge: pp->setSrcEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dstEdge: pp->setDstEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_pathLen: pp->setPathLen(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_path: pp->setPath(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopIndex: pp->setHopIndex(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BCP'", field);
    }
}

const char *BCPDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr BCPDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BCPDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BCP *pp = omnetpp::fromAnyPtr<BCP>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BCP'", field);
    }
}

Register_Class(Burst)

Burst::Burst(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

Burst::Burst(const Burst& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Burst::~Burst()
{
    delete [] this->path;
}

Burst& Burst::operator=(const Burst& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Burst::copy(const Burst& other)
{
    this->burstId = other.burstId;
    this->srcId = other.srcId;
    this->dst = other.dst;
    this->bytes = other.bytes;
    this->nVoice = other.nVoice;
    this->nData = other.nData;
    this->nVideo = other.nVideo;
    this->srcName = other.srcName;
    this->srcEdge = other.srcEdge;
    this->dstEdge = other.dstEdge;
    this->pathLen = other.pathLen;
    this->hopIndex = other.hopIndex;
    delete [] this->path;
    this->path = (other.path_arraysize==0) ? nullptr : new int[other.path_arraysize];
    path_arraysize = other.path_arraysize;
    for (size_t i = 0; i < path_arraysize; i++) {
        this->path[i] = other.path[i];
    }
    this->creationTime = other.creationTime;
}

void Burst::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->burstId);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->dst);
    doParsimPacking(b,this->bytes);
    doParsimPacking(b,this->nVoice);
    doParsimPacking(b,this->nData);
    doParsimPacking(b,this->nVideo);
    doParsimPacking(b,this->srcName);
    doParsimPacking(b,this->srcEdge);
    doParsimPacking(b,this->dstEdge);
    doParsimPacking(b,this->pathLen);
    doParsimPacking(b,this->hopIndex);
    b->pack(path_arraysize);
    doParsimArrayPacking(b,this->path,path_arraysize);
    doParsimPacking(b,this->creationTime);
}

void Burst::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->burstId);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->dst);
    doParsimUnpacking(b,this->bytes);
    doParsimUnpacking(b,this->nVoice);
    doParsimUnpacking(b,this->nData);
    doParsimUnpacking(b,this->nVideo);
    doParsimUnpacking(b,this->srcName);
    doParsimUnpacking(b,this->srcEdge);
    doParsimUnpacking(b,this->dstEdge);
    doParsimUnpacking(b,this->pathLen);
    doParsimUnpacking(b,this->hopIndex);
    delete [] this->path;
    b->unpack(path_arraysize);
    if (path_arraysize == 0) {
        this->path = nullptr;
    } else {
        this->path = new int[path_arraysize];
        doParsimArrayUnpacking(b,this->path,path_arraysize);
    }
    doParsimUnpacking(b,this->creationTime);
}

int Burst::getBurstId() const
{
    return this->burstId;
}

void Burst::setBurstId(int burstId)
{
    this->burstId = burstId;
}

int Burst::getSrcId() const
{
    return this->srcId;
}

void Burst::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int Burst::getDst() const
{
    return this->dst;
}

void Burst::setDst(int dst)
{
    this->dst = dst;
}

int Burst::getBytes() const
{
    return this->bytes;
}

void Burst::setBytes(int bytes)
{
    this->bytes = bytes;
}

int Burst::getNVoice() const
{
    return this->nVoice;
}

void Burst::setNVoice(int nVoice)
{
    this->nVoice = nVoice;
}

int Burst::getNData() const
{
    return this->nData;
}

void Burst::setNData(int nData)
{
    this->nData = nData;
}

int Burst::getNVideo() const
{
    return this->nVideo;
}

void Burst::setNVideo(int nVideo)
{
    this->nVideo = nVideo;
}

const char * Burst::getSrcName() const
{
    return this->srcName.c_str();
}

void Burst::setSrcName(const char * srcName)
{
    this->srcName = srcName;
}

int Burst::getSrcEdge() const
{
    return this->srcEdge;
}

void Burst::setSrcEdge(int srcEdge)
{
    this->srcEdge = srcEdge;
}

int Burst::getDstEdge() const
{
    return this->dstEdge;
}

void Burst::setDstEdge(int dstEdge)
{
    this->dstEdge = dstEdge;
}

int Burst::getPathLen() const
{
    return this->pathLen;
}

void Burst::setPathLen(int pathLen)
{
    this->pathLen = pathLen;
}

int Burst::getHopIndex() const
{
    return this->hopIndex;
}

void Burst::setHopIndex(int hopIndex)
{
    this->hopIndex = hopIndex;
}

size_t Burst::getPathArraySize() const
{
    return path_arraysize;
}

int Burst::getPath(size_t k) const
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    return this->path[k];
}

void Burst::setPathArraySize(size_t newSize)
{
    int *path2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = path_arraysize < newSize ? path_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        path2[i] = this->path[i];
    for (size_t i = minSize; i < newSize; i++)
        path2[i] = 0;
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void Burst::setPath(size_t k, int path)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    this->path[k] = path;
}

void Burst::insertPath(size_t k, int path)
{
    if (k > path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize + 1;
    int *path2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    path2[k] = path;
    for (i = k + 1; i < newSize; i++)
        path2[i] = this->path[i-1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void Burst::appendPath(int path)
{
    insertPath(path_arraysize, path);
}

void Burst::erasePath(size_t k)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize - 1;
    int *path2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    for (i = k; i < newSize; i++)
        path2[i] = this->path[i+1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

omnetpp::simtime_t Burst::getCreationTime() const
{
    return this->creationTime;
}

void Burst::setCreationTime(omnetpp::simtime_t creationTime)
{
    this->creationTime = creationTime;
}

class BurstDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_burstId,
        FIELD_srcId,
        FIELD_dst,
        FIELD_bytes,
        FIELD_nVoice,
        FIELD_nData,
        FIELD_nVideo,
        FIELD_srcName,
        FIELD_srcEdge,
        FIELD_dstEdge,
        FIELD_pathLen,
        FIELD_hopIndex,
        FIELD_path,
        FIELD_creationTime,
    };
  public:
    BurstDescriptor();
    virtual ~BurstDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(BurstDescriptor)

BurstDescriptor::BurstDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Burst)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

BurstDescriptor::~BurstDescriptor()
{
    delete[] propertyNames;
}

bool BurstDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Burst *>(obj)!=nullptr;
}

const char **BurstDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BurstDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BurstDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 14+base->getFieldCount() : 14;
}

unsigned int BurstDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_burstId
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_dst
        FD_ISEDITABLE,    // FIELD_bytes
        FD_ISEDITABLE,    // FIELD_nVoice
        FD_ISEDITABLE,    // FIELD_nData
        FD_ISEDITABLE,    // FIELD_nVideo
        FD_ISEDITABLE,    // FIELD_srcName
        FD_ISEDITABLE,    // FIELD_srcEdge
        FD_ISEDITABLE,    // FIELD_dstEdge
        FD_ISEDITABLE,    // FIELD_pathLen
        FD_ISEDITABLE,    // FIELD_hopIndex
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_path
        FD_ISEDITABLE,    // FIELD_creationTime
    };
    return (field >= 0 && field < 14) ? fieldTypeFlags[field] : 0;
}

const char *BurstDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "burstId",
        "srcId",
        "dst",
        "bytes",
        "nVoice",
        "nData",
        "nVideo",
        "srcName",
        "srcEdge",
        "dstEdge",
        "pathLen",
        "hopIndex",
        "path",
        "creationTime",
    };
    return (field >= 0 && field < 14) ? fieldNames[field] : nullptr;
}

int BurstDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "dst") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "bytes") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "nVoice") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "nData") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "nVideo") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "srcName") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "srcEdge") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "dstEdge") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "pathLen") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "hopIndex") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "path") == 0) return baseIndex + 12;
    if (strcmp(fieldName, "creationTime") == 0) return baseIndex + 13;
    return base ? base->findField(fieldName) : -1;
}

const char *BurstDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_burstId
        "int",    // FIELD_srcId
        "int",    // FIELD_dst
        "int",    // FIELD_bytes
        "int",    // FIELD_nVoice
        "int",    // FIELD_nData
        "int",    // FIELD_nVideo
        "string",    // FIELD_srcName
        "int",    // FIELD_srcEdge
        "int",    // FIELD_dstEdge
        "int",    // FIELD_pathLen
        "int",    // FIELD_hopIndex
        "int",    // FIELD_path
        "omnetpp::simtime_t",    // FIELD_creationTime
    };
    return (field >= 0 && field < 14) ? fieldTypeStrings[field] : nullptr;
}

const char **BurstDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BurstDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BurstDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_path: return pp->getPathArraySize();
        default: return 0;
    }
}

void BurstDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_path: pp->setPathArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Burst'", field);
    }
}

const char *BurstDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BurstDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return long2string(pp->getBurstId());
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_dst: return long2string(pp->getDst());
        case FIELD_bytes: return long2string(pp->getBytes());
        case FIELD_nVoice: return long2string(pp->getNVoice());
        case FIELD_nData: return long2string(pp->getNData());
        case FIELD_nVideo: return long2string(pp->getNVideo());
        case FIELD_srcName: return oppstring2string(pp->getSrcName());
        case FIELD_srcEdge: return long2string(pp->getSrcEdge());
        case FIELD_dstEdge: return long2string(pp->getDstEdge());
        case FIELD_pathLen: return long2string(pp->getPathLen());
        case FIELD_hopIndex: return long2string(pp->getHopIndex());
        case FIELD_path: return long2string(pp->getPath(i));
        case FIELD_creationTime: return simtime2string(pp->getCreationTime());
        default: return "";
    }
}

void BurstDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(string2long(value)); break;
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_dst: pp->setDst(string2long(value)); break;
        case FIELD_bytes: pp->setBytes(string2long(value)); break;
        case FIELD_nVoice: pp->setNVoice(string2long(value)); break;
        case FIELD_nData: pp->setNData(string2long(value)); break;
        case FIELD_nVideo: pp->setNVideo(string2long(value)); break;
        case FIELD_srcName: pp->setSrcName((value)); break;
        case FIELD_srcEdge: pp->setSrcEdge(string2long(value)); break;
        case FIELD_dstEdge: pp->setDstEdge(string2long(value)); break;
        case FIELD_pathLen: pp->setPathLen(string2long(value)); break;
        case FIELD_hopIndex: pp->setHopIndex(string2long(value)); break;
        case FIELD_path: pp->setPath(i,string2long(value)); break;
        case FIELD_creationTime: pp->setCreationTime(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Burst'", field);
    }
}

omnetpp::cValue BurstDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return pp->getBurstId();
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_dst: return pp->getDst();
        case FIELD_bytes: return pp->getBytes();
        case FIELD_nVoice: return pp->getNVoice();
        case FIELD_nData: return pp->getNData();
        case FIELD_nVideo: return pp->getNVideo();
        case FIELD_srcName: return pp->getSrcName();
        case FIELD_srcEdge: return pp->getSrcEdge();
        case FIELD_dstEdge: return pp->getDstEdge();
        case FIELD_pathLen: return pp->getPathLen();
        case FIELD_hopIndex: return pp->getHopIndex();
        case FIELD_path: return pp->getPath(i);
        case FIELD_creationTime: return pp->getCreationTime().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Burst' as cValue -- field index out of range?", field);
    }
}

void BurstDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dst: pp->setDst(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_bytes: pp->setBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nVoice: pp->setNVoice(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nData: pp->setNData(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nVideo: pp->setNVideo(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_srcName: pp->setSrcName(value.stringValue()); break;
        case FIELD_srcEdge: pp->setSrcEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dstEdge: pp->setDstEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_pathLen: pp->setPathLen(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopIndex: pp->setHopIndex(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_path: pp->setPath(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_creationTime: pp->setCreationTime(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Burst'", field);
    }
}

const char *BurstDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr BurstDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BurstDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Burst *pp = omnetpp::fromAnyPtr<Burst>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Burst'", field);
    }
}

Register_Class(CoreStatus)

CoreStatus::CoreStatus(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

CoreStatus::CoreStatus(const CoreStatus& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

CoreStatus::~CoreStatus()
{
}

CoreStatus& CoreStatus::operator=(const CoreStatus& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void CoreStatus::copy(const CoreStatus& other)
{
    this->coreId = other.coreId;
    this->queuedBursts = other.queuedBursts;
    this->estDelay = other.estDelay;
}

void CoreStatus::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->coreId);
    doParsimPacking(b,this->queuedBursts);
    doParsimPacking(b,this->estDelay);
}

void CoreStatus::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->coreId);
    doParsimUnpacking(b,this->queuedBursts);
    doParsimUnpacking(b,this->estDelay);
}

int CoreStatus::getCoreId() const
{
    return this->coreId;
}

void CoreStatus::setCoreId(int coreId)
{
    this->coreId = coreId;
}

int CoreStatus::getQueuedBursts() const
{
    return this->queuedBursts;
}

void CoreStatus::setQueuedBursts(int queuedBursts)
{
    this->queuedBursts = queuedBursts;
}

double CoreStatus::getEstDelay() const
{
    return this->estDelay;
}

void CoreStatus::setEstDelay(double estDelay)
{
    this->estDelay = estDelay;
}

class CoreStatusDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_coreId,
        FIELD_queuedBursts,
        FIELD_estDelay,
    };
  public:
    CoreStatusDescriptor();
    virtual ~CoreStatusDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(CoreStatusDescriptor)

CoreStatusDescriptor::CoreStatusDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(CoreStatus)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

CoreStatusDescriptor::~CoreStatusDescriptor()
{
    delete[] propertyNames;
}

bool CoreStatusDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CoreStatus *>(obj)!=nullptr;
}

const char **CoreStatusDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *CoreStatusDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int CoreStatusDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int CoreStatusDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_coreId
        FD_ISEDITABLE,    // FIELD_queuedBursts
        FD_ISEDITABLE,    // FIELD_estDelay
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *CoreStatusDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "coreId",
        "queuedBursts",
        "estDelay",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int CoreStatusDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "coreId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "queuedBursts") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "estDelay") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *CoreStatusDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_coreId
        "int",    // FIELD_queuedBursts
        "double",    // FIELD_estDelay
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **CoreStatusDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CoreStatusDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CoreStatusDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void CoreStatusDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'CoreStatus'", field);
    }
}

const char *CoreStatusDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CoreStatusDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        case FIELD_coreId: return long2string(pp->getCoreId());
        case FIELD_queuedBursts: return long2string(pp->getQueuedBursts());
        case FIELD_estDelay: return double2string(pp->getEstDelay());
        default: return "";
    }
}

void CoreStatusDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        case FIELD_coreId: pp->setCoreId(string2long(value)); break;
        case FIELD_queuedBursts: pp->setQueuedBursts(string2long(value)); break;
        case FIELD_estDelay: pp->setEstDelay(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CoreStatus'", field);
    }
}

omnetpp::cValue CoreStatusDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        case FIELD_coreId: return pp->getCoreId();
        case FIELD_queuedBursts: return pp->getQueuedBursts();
        case FIELD_estDelay: return pp->getEstDelay();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'CoreStatus' as cValue -- field index out of range?", field);
    }
}

void CoreStatusDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        case FIELD_coreId: pp->setCoreId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_queuedBursts: pp->setQueuedBursts(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_estDelay: pp->setEstDelay(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CoreStatus'", field);
    }
}

const char *CoreStatusDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr CoreStatusDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void CoreStatusDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    CoreStatus *pp = omnetpp::fromAnyPtr<CoreStatus>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CoreStatus'", field);
    }
}

Register_Class(EdgeStatus)

EdgeStatus::EdgeStatus(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

EdgeStatus::EdgeStatus(const EdgeStatus& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

EdgeStatus::~EdgeStatus()
{
}

EdgeStatus& EdgeStatus::operator=(const EdgeStatus& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void EdgeStatus::copy(const EdgeStatus& other)
{
    this->edgeId = other.edgeId;
    this->totalBuffered = other.totalBuffered;
    this->nVoice = other.nVoice;
    this->nData = other.nData;
    this->nVideo = other.nVideo;
}

void EdgeStatus::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->edgeId);
    doParsimPacking(b,this->totalBuffered);
    doParsimPacking(b,this->nVoice);
    doParsimPacking(b,this->nData);
    doParsimPacking(b,this->nVideo);
}

void EdgeStatus::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->edgeId);
    doParsimUnpacking(b,this->totalBuffered);
    doParsimUnpacking(b,this->nVoice);
    doParsimUnpacking(b,this->nData);
    doParsimUnpacking(b,this->nVideo);
}

int EdgeStatus::getEdgeId() const
{
    return this->edgeId;
}

void EdgeStatus::setEdgeId(int edgeId)
{
    this->edgeId = edgeId;
}

int EdgeStatus::getTotalBuffered() const
{
    return this->totalBuffered;
}

void EdgeStatus::setTotalBuffered(int totalBuffered)
{
    this->totalBuffered = totalBuffered;
}

int EdgeStatus::getNVoice() const
{
    return this->nVoice;
}

void EdgeStatus::setNVoice(int nVoice)
{
    this->nVoice = nVoice;
}

int EdgeStatus::getNData() const
{
    return this->nData;
}

void EdgeStatus::setNData(int nData)
{
    this->nData = nData;
}

int EdgeStatus::getNVideo() const
{
    return this->nVideo;
}

void EdgeStatus::setNVideo(int nVideo)
{
    this->nVideo = nVideo;
}

class EdgeStatusDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_edgeId,
        FIELD_totalBuffered,
        FIELD_nVoice,
        FIELD_nData,
        FIELD_nVideo,
    };
  public:
    EdgeStatusDescriptor();
    virtual ~EdgeStatusDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(EdgeStatusDescriptor)

EdgeStatusDescriptor::EdgeStatusDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(EdgeStatus)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

EdgeStatusDescriptor::~EdgeStatusDescriptor()
{
    delete[] propertyNames;
}

bool EdgeStatusDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EdgeStatus *>(obj)!=nullptr;
}

const char **EdgeStatusDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *EdgeStatusDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int EdgeStatusDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int EdgeStatusDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_edgeId
        FD_ISEDITABLE,    // FIELD_totalBuffered
        FD_ISEDITABLE,    // FIELD_nVoice
        FD_ISEDITABLE,    // FIELD_nData
        FD_ISEDITABLE,    // FIELD_nVideo
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *EdgeStatusDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "edgeId",
        "totalBuffered",
        "nVoice",
        "nData",
        "nVideo",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int EdgeStatusDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "edgeId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "totalBuffered") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "nVoice") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "nData") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "nVideo") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *EdgeStatusDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_edgeId
        "int",    // FIELD_totalBuffered
        "int",    // FIELD_nVoice
        "int",    // FIELD_nData
        "int",    // FIELD_nVideo
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **EdgeStatusDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *EdgeStatusDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int EdgeStatusDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void EdgeStatusDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'EdgeStatus'", field);
    }
}

const char *EdgeStatusDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EdgeStatusDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: return long2string(pp->getEdgeId());
        case FIELD_totalBuffered: return long2string(pp->getTotalBuffered());
        case FIELD_nVoice: return long2string(pp->getNVoice());
        case FIELD_nData: return long2string(pp->getNData());
        case FIELD_nVideo: return long2string(pp->getNVideo());
        default: return "";
    }
}

void EdgeStatusDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: pp->setEdgeId(string2long(value)); break;
        case FIELD_totalBuffered: pp->setTotalBuffered(string2long(value)); break;
        case FIELD_nVoice: pp->setNVoice(string2long(value)); break;
        case FIELD_nData: pp->setNData(string2long(value)); break;
        case FIELD_nVideo: pp->setNVideo(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EdgeStatus'", field);
    }
}

omnetpp::cValue EdgeStatusDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: return pp->getEdgeId();
        case FIELD_totalBuffered: return pp->getTotalBuffered();
        case FIELD_nVoice: return pp->getNVoice();
        case FIELD_nData: return pp->getNData();
        case FIELD_nVideo: return pp->getNVideo();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'EdgeStatus' as cValue -- field index out of range?", field);
    }
}

void EdgeStatusDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: pp->setEdgeId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_totalBuffered: pp->setTotalBuffered(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nVoice: pp->setNVoice(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nData: pp->setNData(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nVideo: pp->setNVideo(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EdgeStatus'", field);
    }
}

const char *EdgeStatusDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr EdgeStatusDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void EdgeStatusDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    EdgeStatus *pp = omnetpp::fromAnyPtr<EdgeStatus>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'EdgeStatus'", field);
    }
}

Register_Class(PathReply)

PathReply::PathReply(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

PathReply::PathReply(const PathReply& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

PathReply::~PathReply()
{
    delete [] this->path;
}

PathReply& PathReply::operator=(const PathReply& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PathReply::copy(const PathReply& other)
{
    this->burstId = other.burstId;
    this->srcEdge = other.srcEdge;
    this->dstEdge = other.dstEdge;
    this->pathLen = other.pathLen;
    delete [] this->path;
    this->path = (other.path_arraysize==0) ? nullptr : new int[other.path_arraysize];
    path_arraysize = other.path_arraysize;
    for (size_t i = 0; i < path_arraysize; i++) {
        this->path[i] = other.path[i];
    }
}

void PathReply::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->burstId);
    doParsimPacking(b,this->srcEdge);
    doParsimPacking(b,this->dstEdge);
    doParsimPacking(b,this->pathLen);
    b->pack(path_arraysize);
    doParsimArrayPacking(b,this->path,path_arraysize);
}

void PathReply::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->burstId);
    doParsimUnpacking(b,this->srcEdge);
    doParsimUnpacking(b,this->dstEdge);
    doParsimUnpacking(b,this->pathLen);
    delete [] this->path;
    b->unpack(path_arraysize);
    if (path_arraysize == 0) {
        this->path = nullptr;
    } else {
        this->path = new int[path_arraysize];
        doParsimArrayUnpacking(b,this->path,path_arraysize);
    }
}

int PathReply::getBurstId() const
{
    return this->burstId;
}

void PathReply::setBurstId(int burstId)
{
    this->burstId = burstId;
}

int PathReply::getSrcEdge() const
{
    return this->srcEdge;
}

void PathReply::setSrcEdge(int srcEdge)
{
    this->srcEdge = srcEdge;
}

int PathReply::getDstEdge() const
{
    return this->dstEdge;
}

void PathReply::setDstEdge(int dstEdge)
{
    this->dstEdge = dstEdge;
}

int PathReply::getPathLen() const
{
    return this->pathLen;
}

void PathReply::setPathLen(int pathLen)
{
    this->pathLen = pathLen;
}

size_t PathReply::getPathArraySize() const
{
    return path_arraysize;
}

int PathReply::getPath(size_t k) const
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    return this->path[k];
}

void PathReply::setPathArraySize(size_t newSize)
{
    int *path2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = path_arraysize < newSize ? path_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        path2[i] = this->path[i];
    for (size_t i = minSize; i < newSize; i++)
        path2[i] = 0;
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void PathReply::setPath(size_t k, int path)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    this->path[k] = path;
}

void PathReply::insertPath(size_t k, int path)
{
    if (k > path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize + 1;
    int *path2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    path2[k] = path;
    for (i = k + 1; i < newSize; i++)
        path2[i] = this->path[i-1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

void PathReply::appendPath(int path)
{
    insertPath(path_arraysize, path);
}

void PathReply::erasePath(size_t k)
{
    if (k >= path_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)path_arraysize, (unsigned long)k);
    size_t newSize = path_arraysize - 1;
    int *path2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        path2[i] = this->path[i];
    for (i = k; i < newSize; i++)
        path2[i] = this->path[i+1];
    delete [] this->path;
    this->path = path2;
    path_arraysize = newSize;
}

class PathReplyDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_burstId,
        FIELD_srcEdge,
        FIELD_dstEdge,
        FIELD_pathLen,
        FIELD_path,
    };
  public:
    PathReplyDescriptor();
    virtual ~PathReplyDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PathReplyDescriptor)

PathReplyDescriptor::PathReplyDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(PathReply)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PathReplyDescriptor::~PathReplyDescriptor()
{
    delete[] propertyNames;
}

bool PathReplyDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PathReply *>(obj)!=nullptr;
}

const char **PathReplyDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PathReplyDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PathReplyDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int PathReplyDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_burstId
        FD_ISEDITABLE,    // FIELD_srcEdge
        FD_ISEDITABLE,    // FIELD_dstEdge
        FD_ISEDITABLE,    // FIELD_pathLen
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_path
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *PathReplyDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "burstId",
        "srcEdge",
        "dstEdge",
        "pathLen",
        "path",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int PathReplyDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "srcEdge") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "dstEdge") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "pathLen") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "path") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *PathReplyDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_burstId
        "int",    // FIELD_srcEdge
        "int",    // FIELD_dstEdge
        "int",    // FIELD_pathLen
        "int",    // FIELD_path
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **PathReplyDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PathReplyDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PathReplyDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_path: return pp->getPathArraySize();
        default: return 0;
    }
}

void PathReplyDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_path: pp->setPathArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PathReply'", field);
    }
}

const char *PathReplyDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PathReplyDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return long2string(pp->getBurstId());
        case FIELD_srcEdge: return long2string(pp->getSrcEdge());
        case FIELD_dstEdge: return long2string(pp->getDstEdge());
        case FIELD_pathLen: return long2string(pp->getPathLen());
        case FIELD_path: return long2string(pp->getPath(i));
        default: return "";
    }
}

void PathReplyDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(string2long(value)); break;
        case FIELD_srcEdge: pp->setSrcEdge(string2long(value)); break;
        case FIELD_dstEdge: pp->setDstEdge(string2long(value)); break;
        case FIELD_pathLen: pp->setPathLen(string2long(value)); break;
        case FIELD_path: pp->setPath(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PathReply'", field);
    }
}

omnetpp::cValue PathReplyDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return pp->getBurstId();
        case FIELD_srcEdge: return pp->getSrcEdge();
        case FIELD_dstEdge: return pp->getDstEdge();
        case FIELD_pathLen: return pp->getPathLen();
        case FIELD_path: return pp->getPath(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PathReply' as cValue -- field index out of range?", field);
    }
}

void PathReplyDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_srcEdge: pp->setSrcEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dstEdge: pp->setDstEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_pathLen: pp->setPathLen(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_path: pp->setPath(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PathReply'", field);
    }
}

const char *PathReplyDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr PathReplyDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PathReplyDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PathReply *pp = omnetpp::fromAnyPtr<PathReply>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PathReply'", field);
    }
}

Register_Class(ParamUpdate)

ParamUpdate::ParamUpdate(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ParamUpdate::ParamUpdate(const ParamUpdate& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ParamUpdate::~ParamUpdate()
{
}

ParamUpdate& ParamUpdate::operator=(const ParamUpdate& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ParamUpdate::copy(const ParamUpdate& other)
{
    this->edgeId = other.edgeId;
    this->LmaxTotal = other.LmaxTotal;
    this->TmaxTotal = other.TmaxTotal;
}

void ParamUpdate::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->edgeId);
    doParsimPacking(b,this->LmaxTotal);
    doParsimPacking(b,this->TmaxTotal);
}

void ParamUpdate::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->edgeId);
    doParsimUnpacking(b,this->LmaxTotal);
    doParsimUnpacking(b,this->TmaxTotal);
}

int ParamUpdate::getEdgeId() const
{
    return this->edgeId;
}

void ParamUpdate::setEdgeId(int edgeId)
{
    this->edgeId = edgeId;
}

int ParamUpdate::getLmaxTotal() const
{
    return this->LmaxTotal;
}

void ParamUpdate::setLmaxTotal(int LmaxTotal)
{
    this->LmaxTotal = LmaxTotal;
}

double ParamUpdate::getTmaxTotal() const
{
    return this->TmaxTotal;
}

void ParamUpdate::setTmaxTotal(double TmaxTotal)
{
    this->TmaxTotal = TmaxTotal;
}

class ParamUpdateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_edgeId,
        FIELD_LmaxTotal,
        FIELD_TmaxTotal,
    };
  public:
    ParamUpdateDescriptor();
    virtual ~ParamUpdateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ParamUpdateDescriptor)

ParamUpdateDescriptor::ParamUpdateDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ParamUpdate)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ParamUpdateDescriptor::~ParamUpdateDescriptor()
{
    delete[] propertyNames;
}

bool ParamUpdateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ParamUpdate *>(obj)!=nullptr;
}

const char **ParamUpdateDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ParamUpdateDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ParamUpdateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int ParamUpdateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_edgeId
        FD_ISEDITABLE,    // FIELD_LmaxTotal
        FD_ISEDITABLE,    // FIELD_TmaxTotal
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *ParamUpdateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "edgeId",
        "LmaxTotal",
        "TmaxTotal",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int ParamUpdateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "edgeId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "LmaxTotal") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "TmaxTotal") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *ParamUpdateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_edgeId
        "int",    // FIELD_LmaxTotal
        "double",    // FIELD_TmaxTotal
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **ParamUpdateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ParamUpdateDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ParamUpdateDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ParamUpdateDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ParamUpdate'", field);
    }
}

const char *ParamUpdateDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ParamUpdateDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: return long2string(pp->getEdgeId());
        case FIELD_LmaxTotal: return long2string(pp->getLmaxTotal());
        case FIELD_TmaxTotal: return double2string(pp->getTmaxTotal());
        default: return "";
    }
}

void ParamUpdateDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: pp->setEdgeId(string2long(value)); break;
        case FIELD_LmaxTotal: pp->setLmaxTotal(string2long(value)); break;
        case FIELD_TmaxTotal: pp->setTmaxTotal(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ParamUpdate'", field);
    }
}

omnetpp::cValue ParamUpdateDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: return pp->getEdgeId();
        case FIELD_LmaxTotal: return pp->getLmaxTotal();
        case FIELD_TmaxTotal: return pp->getTmaxTotal();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ParamUpdate' as cValue -- field index out of range?", field);
    }
}

void ParamUpdateDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        case FIELD_edgeId: pp->setEdgeId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_LmaxTotal: pp->setLmaxTotal(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_TmaxTotal: pp->setTmaxTotal(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ParamUpdate'", field);
    }
}

const char *ParamUpdateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ParamUpdateDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ParamUpdateDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ParamUpdate *pp = omnetpp::fromAnyPtr<ParamUpdate>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ParamUpdate'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

