//
// Generated file, do not edit! Created by opp_msgtool 6.1 from src/msgs/Obs.msg.
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
    this->bytes = other.bytes;
    this->offset = other.offset;
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
    doParsimPacking(b,this->bytes);
    doParsimPacking(b,this->offset);
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
    doParsimUnpacking(b,this->bytes);
    doParsimUnpacking(b,this->offset);
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
        FIELD_bytes,
        FIELD_offset,
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
    return base ? 8+base->getFieldCount() : 8;
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
        FD_ISEDITABLE,    // FIELD_bytes
        FD_ISEDITABLE,    // FIELD_offset
        FD_ISEDITABLE,    // FIELD_srcEdge
        FD_ISEDITABLE,    // FIELD_dstEdge
        FD_ISEDITABLE,    // FIELD_pathLen
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_path
        FD_ISEDITABLE,    // FIELD_hopIndex
    };
    return (field >= 0 && field < 8) ? fieldTypeFlags[field] : 0;
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
        "bytes",
        "offset",
        "srcEdge",
        "dstEdge",
        "pathLen",
        "path",
        "hopIndex",
    };
    return (field >= 0 && field < 8) ? fieldNames[field] : nullptr;
}

int BCPDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "bytes") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "offset") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "srcEdge") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "dstEdge") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "pathLen") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "path") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "hopIndex") == 0) return baseIndex + 7;
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
        "int",    // FIELD_bytes
        "double",    // FIELD_offset
        "int",    // FIELD_srcEdge
        "int",    // FIELD_dstEdge
        "int",    // FIELD_pathLen
        "int",    // FIELD_path
        "int",    // FIELD_hopIndex
    };
    return (field >= 0 && field < 8) ? fieldTypeStrings[field] : nullptr;
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
        case FIELD_bytes: return long2string(pp->getBytes());
        case FIELD_offset: return double2string(pp->getOffset());
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
        case FIELD_bytes: pp->setBytes(string2long(value)); break;
        case FIELD_offset: pp->setOffset(string2double(value)); break;
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
        case FIELD_bytes: return pp->getBytes();
        case FIELD_offset: return pp->getOffset();
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
        case FIELD_bytes: pp->setBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_offset: pp->setOffset(value.doubleValue()); break;
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
    this->bytes = other.bytes;
    this->srcEdge = other.srcEdge;
    this->dstEdge = other.dstEdge;
    this->svc = other.svc;
    this->assemblyTime = other.assemblyTime;
    this->sendTime = other.sendTime;
    this->nVoice = other.nVoice;
    this->nData = other.nData;
    this->nVideo = other.nVideo;
}

void Burst::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->burstId);
    doParsimPacking(b,this->bytes);
    doParsimPacking(b,this->srcEdge);
    doParsimPacking(b,this->dstEdge);
    doParsimPacking(b,this->svc);
    doParsimPacking(b,this->assemblyTime);
    doParsimPacking(b,this->sendTime);
    doParsimPacking(b,this->nVoice);
    doParsimPacking(b,this->nData);
    doParsimPacking(b,this->nVideo);
}

void Burst::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->burstId);
    doParsimUnpacking(b,this->bytes);
    doParsimUnpacking(b,this->srcEdge);
    doParsimUnpacking(b,this->dstEdge);
    doParsimUnpacking(b,this->svc);
    doParsimUnpacking(b,this->assemblyTime);
    doParsimUnpacking(b,this->sendTime);
    doParsimUnpacking(b,this->nVoice);
    doParsimUnpacking(b,this->nData);
    doParsimUnpacking(b,this->nVideo);
}

int Burst::getBurstId() const
{
    return this->burstId;
}

void Burst::setBurstId(int burstId)
{
    this->burstId = burstId;
}

int Burst::getBytes() const
{
    return this->bytes;
}

void Burst::setBytes(int bytes)
{
    this->bytes = bytes;
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

int Burst::getSvc() const
{
    return this->svc;
}

void Burst::setSvc(int svc)
{
    this->svc = svc;
}

double Burst::getAssemblyTime() const
{
    return this->assemblyTime;
}

void Burst::setAssemblyTime(double assemblyTime)
{
    this->assemblyTime = assemblyTime;
}

double Burst::getSendTime() const
{
    return this->sendTime;
}

void Burst::setSendTime(double sendTime)
{
    this->sendTime = sendTime;
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

class BurstDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_burstId,
        FIELD_bytes,
        FIELD_srcEdge,
        FIELD_dstEdge,
        FIELD_svc,
        FIELD_assemblyTime,
        FIELD_sendTime,
        FIELD_nVoice,
        FIELD_nData,
        FIELD_nVideo,
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
    return base ? 10+base->getFieldCount() : 10;
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
        FD_ISEDITABLE,    // FIELD_bytes
        FD_ISEDITABLE,    // FIELD_srcEdge
        FD_ISEDITABLE,    // FIELD_dstEdge
        FD_ISEDITABLE,    // FIELD_svc
        FD_ISEDITABLE,    // FIELD_assemblyTime
        FD_ISEDITABLE,    // FIELD_sendTime
        FD_ISEDITABLE,    // FIELD_nVoice
        FD_ISEDITABLE,    // FIELD_nData
        FD_ISEDITABLE,    // FIELD_nVideo
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
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
        "bytes",
        "srcEdge",
        "dstEdge",
        "svc",
        "assemblyTime",
        "sendTime",
        "nVoice",
        "nData",
        "nVideo",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int BurstDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "bytes") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "srcEdge") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "dstEdge") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "svc") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "assemblyTime") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "nVoice") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "nData") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "nVideo") == 0) return baseIndex + 9;
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
        "int",    // FIELD_bytes
        "int",    // FIELD_srcEdge
        "int",    // FIELD_dstEdge
        "int",    // FIELD_svc
        "double",    // FIELD_assemblyTime
        "double",    // FIELD_sendTime
        "int",    // FIELD_nVoice
        "int",    // FIELD_nData
        "int",    // FIELD_nVideo
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
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
        case FIELD_bytes: return long2string(pp->getBytes());
        case FIELD_srcEdge: return long2string(pp->getSrcEdge());
        case FIELD_dstEdge: return long2string(pp->getDstEdge());
        case FIELD_svc: return long2string(pp->getSvc());
        case FIELD_assemblyTime: return double2string(pp->getAssemblyTime());
        case FIELD_sendTime: return double2string(pp->getSendTime());
        case FIELD_nVoice: return long2string(pp->getNVoice());
        case FIELD_nData: return long2string(pp->getNData());
        case FIELD_nVideo: return long2string(pp->getNVideo());
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
        case FIELD_bytes: pp->setBytes(string2long(value)); break;
        case FIELD_srcEdge: pp->setSrcEdge(string2long(value)); break;
        case FIELD_dstEdge: pp->setDstEdge(string2long(value)); break;
        case FIELD_svc: pp->setSvc(string2long(value)); break;
        case FIELD_assemblyTime: pp->setAssemblyTime(string2double(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2double(value)); break;
        case FIELD_nVoice: pp->setNVoice(string2long(value)); break;
        case FIELD_nData: pp->setNData(string2long(value)); break;
        case FIELD_nVideo: pp->setNVideo(string2long(value)); break;
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
        case FIELD_bytes: return pp->getBytes();
        case FIELD_srcEdge: return pp->getSrcEdge();
        case FIELD_dstEdge: return pp->getDstEdge();
        case FIELD_svc: return pp->getSvc();
        case FIELD_assemblyTime: return pp->getAssemblyTime();
        case FIELD_sendTime: return pp->getSendTime();
        case FIELD_nVoice: return pp->getNVoice();
        case FIELD_nData: return pp->getNData();
        case FIELD_nVideo: return pp->getNVideo();
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
        case FIELD_bytes: pp->setBytes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_srcEdge: pp->setSrcEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dstEdge: pp->setDstEdge(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_svc: pp->setSvc(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_assemblyTime: pp->setAssemblyTime(value.doubleValue()); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        case FIELD_nVoice: pp->setNVoice(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nData: pp->setNData(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nVideo: pp->setNVideo(omnetpp::checked_int_cast<int>(value.intValue())); break;
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

Register_Class(BurstAck)

BurstAck::BurstAck(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

BurstAck::BurstAck(const BurstAck& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

BurstAck::~BurstAck()
{
}

BurstAck& BurstAck::operator=(const BurstAck& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void BurstAck::copy(const BurstAck& other)
{
    this->burstId = other.burstId;
}

void BurstAck::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->burstId);
}

void BurstAck::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->burstId);
}

int BurstAck::getBurstId() const
{
    return this->burstId;
}

void BurstAck::setBurstId(int burstId)
{
    this->burstId = burstId;
}

class BurstAckDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_burstId,
    };
  public:
    BurstAckDescriptor();
    virtual ~BurstAckDescriptor();

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

Register_ClassDescriptor(BurstAckDescriptor)

BurstAckDescriptor::BurstAckDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BurstAck)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

BurstAckDescriptor::~BurstAckDescriptor()
{
    delete[] propertyNames;
}

bool BurstAckDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BurstAck *>(obj)!=nullptr;
}

const char **BurstAckDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BurstAckDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BurstAckDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int BurstAckDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_burstId
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *BurstAckDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "burstId",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int BurstAckDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "burstId") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *BurstAckDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_burstId
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **BurstAckDescriptor::getFieldPropertyNames(int field) const
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

const char *BurstAckDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int BurstAckDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BurstAckDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BurstAck'", field);
    }
}

const char *BurstAckDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BurstAckDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return long2string(pp->getBurstId());
        default: return "";
    }
}

void BurstAckDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BurstAck'", field);
    }
}

omnetpp::cValue BurstAckDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: return pp->getBurstId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BurstAck' as cValue -- field index out of range?", field);
    }
}

void BurstAckDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        case FIELD_burstId: pp->setBurstId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BurstAck'", field);
    }
}

const char *BurstAckDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr BurstAckDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BurstAckDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BurstAck *pp = omnetpp::fromAnyPtr<BurstAck>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BurstAck'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

