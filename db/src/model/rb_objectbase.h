/*****************************************************************
 * $Id: rb_objectbase.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OBJECTBASE_H
#define RB_OBJECTBASE_H

#include <string>
#include <vector>
#include <QSqlDatabase>

#include "rb.h"
#include "rb_object.h"
#include "rb_objectfactory.h"
#include "rb_objectmember.h"
#include "rb_string.h"
#include "rb_uuid.h"
#include "rb_variant.h"
#include "rb_visitor.h"


// classes referenced with circular reference to this class
class RB_ObjectContainer;
class RB_ObjectIterator;
class RB_ObjectMember;


/**
 * Template of base object in model, has one or more members in a std::vector.
 * Can be root or has a parent container object.
 */
class DB_EXPORT RB_ObjectBase : public RB_Object {

public:
    RB_ObjectBase(const RB_String& id = "", RB_ObjectBase* p = NULL,
                  const RB_String& n = "RB_ObjectBase",
                  RB_ObjectFactory* f = NULL,
                  bool woMembers = false);
    RB_ObjectBase(RB_ObjectBase* obj);
    virtual ~RB_ObjectBase();
    virtual RB_ObjectBase& operator= (const RB_ObjectBase& obj);

//    virtual void init() { }

    virtual RB_String getId() const;
    virtual void setId(const RB_String& id);
    virtual RB_ObjectContainer* getParent() const;
    virtual void setParent(RB_ObjectBase* p);
    virtual void detachFromParent();
    virtual RB_ObjectContainer* getGrandParent() const;
    virtual RB_String getName() const;
    virtual void setName(const RB_String& n);
    virtual int getStatus() const;
    virtual void setStatus(int s);
    virtual RB_String getCreated() const;
    virtual void setCreated(const RB_String& c);
    virtual RB_String getChanged() const;
    virtual void setChanged(const RB_String& c);
    virtual RB_String getUser() const;
    virtual void setUser(const RB_String& c);
    virtual RB_ObjectFactory* getFactory() const;
    virtual void setFactory(RB_ObjectFactory* f);

    virtual RB_ObjectBase* copy(RB2::ResolveLevel level = RB2::ResolveNone) = 0;
    virtual RB_ObjectBase* clone(RB2::ResolveLevel level = RB2::ResolveNone) = 0;
    virtual void revert();
    virtual bool isClone();
    virtual void setCloned(bool cloned);
    virtual RB_ObjectBase* getOriginal();
    virtual void setOriginal(RB_ObjectBase* orig);
    virtual void deleteOriginal();

    virtual bool isAtomic() { return false; }
    virtual bool isContainer() { return false; }
    virtual bool isList() { return false; }

    // abstract container definitions
    virtual int objectCount() const;
    virtual RB_ObjectBase* getObject(const RB_String& id = "") = 0;
    virtual RB_ObjectBase* getObject(const RB_String& memberName,
                                     const RB_Variant& value);
    virtual RB_ObjectBase* newObject(const RB_String& id = "") = 0;
    virtual RB_ObjectContainer* getContainer(const RB_String& name = "") = 0;
    virtual RB_ObjectContainer* newContainer(const RB_String& name) = 0;

    virtual bool addObject(RB_ObjectBase* obj) = 0;
    virtual RB_ObjectIterator* createIterator() = 0;
    virtual bool remove(RB_ObjectBase* obj, bool deleteObj = true) = 0;
    virtual RB_ObjectBase* getChildObject(const RB_String& objName) = 0;
    virtual RB_ObjectBase* getDescendantObject(const RB_String& id) = 0;

    virtual bool eraseChildren() = 0;

    virtual int memberCount() const;
    virtual RB_ObjectMember* getMember(int number) const;
    virtual RB_ObjectMember* getMember(const RB_String& name) const;
    virtual int getMemberNo(const RB_String& name) const;
    virtual RB_ObjectMember* addMember(const RB_String& name,
                           const RB_String& unit, const RB_Variant& value,
                           RB2::MemberType type = RB2::MemberNone,
                           const RB_Variant& prevValue = RB_Variant());

    virtual RB_Variant getValue(int number) const;
    virtual RB_Variant getValue(const RB_String& name) const;
    virtual void setValue(int number, const RB_Variant& var);
    virtual void setValue(const RB_String& name, const RB_Variant& var);
    virtual RB_Variant getPValue(int number) const;
    virtual RB_Variant getPValue(const RB_String& name) const;
    virtual void setPValue(int number, const RB_Variant& var);
    virtual void setPValue(const RB_String& name, const RB_Variant& var);
    virtual RB_Variant getDValue(int number) const;
    virtual RB_Variant getDValue(const RB_String& name) const;
    virtual RB_Variant getIdValue(int number) const;
    virtual RB_Variant getIdValue(const RB_String& name) const;

    virtual void deleteFlag(unsigned int f);

    virtual bool dbReadWhere(QSqlDatabase db, const QString& whereStatement);
    virtual bool dbRead(QSqlDatabase db,
                        RB2::ResolveLevel level = RB2::ResolveNone,
                        bool calledFromList = false, bool useParentId = false);
    virtual bool dbUpdate(QSqlDatabase db,
                          RB2::ResolveLevel level = RB2::ResolveNone);
    virtual bool dbDelete(QSqlDatabase db, bool calledFromList = false);
    virtual bool dbReadList(QSqlDatabase db,
                            RB2::ResolveLevel level = RB2::ResolveNone,
                            const RB_String& fieldName = "parent");
    virtual bool dbUpdateList(QSqlDatabase db,
                              RB2::ResolveLevel level = RB2::ResolveNone);
    virtual bool dbDeleteList(QSqlDatabase db,
                              const RB_String& fieldName = "parent");
    /**
     * Accept visitor of the visitor design pattern,
     * such as XML writer, DB read/write, is an abstract function.
     * @param visitor
     */
    virtual void acceptVisitor(RB_Visitor& visitor) = 0;

    virtual RB_String toString();

protected:
    //! Create object members, to be overloaded. Function does nothing.
    virtual void createMembers() {}
    virtual void removeMembers();
    virtual void createCopy(RB_ObjectBase* obj, RB2::ResolveLevel level);
    virtual RB2::ResolveLevel resolveNext(RB2::ResolveLevel level);

    //! identification of object
    RB_String mId;
    //! pointer to parent object
    RB_ObjectBase* mParent;
    //! model object factory
    RB_ObjectFactory* mFactory;
    //! pointer to original object in case this object is the clone
    RB_ObjectBase* mOriginal;

private:
    //! whether this object is a clone
    bool mClonedObject;
    //! vector container of members
    std::vector<RB_ObjectMember*> mMemberVector;
};

#endif // RB_OBJECTBASE_H
