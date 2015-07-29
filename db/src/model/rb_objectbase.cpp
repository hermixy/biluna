/*****************************************************************
 * $Id: rb_objectbase.cpp 2241 2015-05-22 10:22:19Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_objectbase.h"

#include "rb_database.h"
#include "rb_dbvisitor.h"
#include "rb_debug.h"
#include "rb_objectcontainer.h"


/**
 * Constructor
 * TODO: after extensive testing remove debug functions
 * @param id identification
 * @param p parent container
 * @param n name of object such as PENG_Line
 * @param f object factory
 * @param woMembers create without members id parent name
 * and status created changed user
 */
RB_ObjectBase::RB_ObjectBase(const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory* f,
                            bool woMembers) {
    RB_DEBUG->addObjectBaseCreated(this);

    mId = id != "" ? id : RB_Uuid::createUuid().toString();
    mParent = p;
    mName = n;

    if (!woMembers) {
        // id, parent(id) and name are part of the member list
        addMember(this, "id", "-", mId, RB2::MemberChar40);
        if (p && p->getParent()) {
            addMember(this, "parent", "-", p->getParent()->getId(), RB2::MemberChar40);
        } else {
            addMember(this, "parent", "-", "", RB2::MemberChar40);
        }
        addMember(this, "name", "-", n, RB2::MemberChar40);
        addMember(this, "status", "-", 0 /* RB2::StatusDefault */, RB2::MemberInteger);
        addMember(this, "created", "-", "0000-00-00T00:00:00", RB2::MemberChar20);
        addMember(this, "changed", "-", "0000-00-00T00:00:00", RB2::MemberChar20);
        addMember(this, "muser", "-", "system", RB2::MemberChar20);
    }

    mFactory = f;
    mOriginal = NULL;
    mClonedObject = false;

    resetFlags();
}

/**
 * Copy constructor, does not create or set members only id, parent and name
 * TODO: why not copy values of other members?
 * TODO: after extensive testing remove debug functions
 * @param obj object to be copied
 */
RB_ObjectBase::RB_ObjectBase(RB_ObjectBase* obj) {
    RB_DEBUG->addObjectBaseCreated(this);
    if (!obj) return;

    // Note: do not use *this = *obj, but instead use:
    mParent = obj->getParent();

    // id, parent(id) and name are part of the member list
    addMember(this, "id", "-", obj->getId(), RB2::MemberChar40);
    if (obj->getParent() && obj->getParent()->getParent()) {
        addMember(this, "parent", "-", obj->getParent()->getParent()->getId(), RB2::MemberChar40);
    } else {
        addMember(this, "parent", "-", "", RB2::MemberChar40);
    }
    addMember(this, "name", "-", obj->getName(), RB2::MemberChar40);
    addMember(this, "status", "-", RB2::StatusDefault, RB2::MemberInteger);
    addMember(this, "created", "-", "0000-00-00T00:00:00", RB2::MemberChar20);
    addMember(this, "changed", "-", "0000-00-00T00:00:00", RB2::MemberChar20);
    addMember(this, "muser", "-", "noname", RB2::MemberChar20);

    mFactory = obj->getFactory();
    mOriginal = obj->getOriginal();
    mClonedObject = true;

    setFlags(obj->getFlags());
}

/**
 * Destructor
 * TODO: after extensive testing remove debug functions
 */
RB_ObjectBase::~RB_ObjectBase() {
    RB_DEBUG->addObjectBaseDeleted(this);
    removeMembers();
}


/**
 * operator = implementation
 * TODO: after extensive testing remove debug functions
 * @return *this
 */
RB_ObjectBase& RB_ObjectBase::operator= (const RB_ObjectBase& obj) {
    if (&obj != this && getName() == obj.getName()) {
        // RB_ObjectBase has no members during creation of object
        int thisCount = countMember();
        int objCount = obj.countMember();

        // do not copy id, parent and name
        for (int i = 2; i < objCount; i++) {
            if (i < thisCount) {
                // member type to be checked? 2009-08-17
                RB_ObjectMember* mem = getMember(i);
                *mem = *(obj.getMember(i));
            } else {
                // If objc has more members, add new members. EQL_Equipment
                // at creation only has a few members for equipmentList
                // but not all as per data sheet
                RB_ObjectMember* mem = obj.getMember(i);
                addMember(this,
                            mem->getName(),
                            mem->getUnit(),
                            mem->getValue(),
                            mem->getType(),
                            mem->getPreviousValue());
            }
        }
    }

    setFlags(obj.getFlags());
    return *this;	// does not return containerList in case of RB_ObjectContainer
}


/**
 * @return id identification of this object
 */
RB_String RB_ObjectBase::getId() const {
    return getValue("id").toString();
}

/**
 * Set the id of this object
 * @param id identification
 */
void RB_ObjectBase::setId(const RB_String& id) {
    setValue("id", id);
}

/**
 * Get pointer to parent object, can be a atomic, container or list
 * @return parent pointer to parent of this object or NULL if not exists
 */
RB_ObjectContainer* RB_ObjectBase::getParent() const {
    return dynamic_cast<RB_ObjectContainer*>(mParent);
}

/**
 * Replace the existing parent by the new parent,
 * sets pointer to parent and sets ID to grandparent ID if exists,
 * otherwise empty string. NOTE: does not add object to parent list.
 * @param p new parent object, usually list container
 */
void RB_ObjectBase::setParent(RB_ObjectBase* p) {
    // Remove this object from parent list but do not delete, thus detach
    RB_ObjectContainer* currentP = getParent();
    if (currentP) {
        currentP->remove(this, false);
    }
    // set new parent
    mParent = p;
    if (p && p->getParent() && !isList()) {
        setValue("parent", p->getParent()->getId());
    } else {
        setValue("parent", "");
    }
    setFlag(RB2::FlagIsDirty);
}

/**
 * Removes object from parent list container and sets
 * parent pointer to NULL and ID to empty string.
 */
void RB_ObjectBase::detachFromParent() {
    setParent(NULL);
}

/**
 * Get pointer to grand parent object, can be a container or list,
 * for example if the object is an atomic object and you do not want the
 * list container but the real parent object, use this function
 * @return pointer to grand parent of this object or NULL if not exists
 */
RB_ObjectContainer* RB_ObjectBase::getGrandParent() const {
    if (mParent) {
        return mParent->getParent();
    }

    return NULL;
}

/**
 * @return name the name of this object
 */
RB_String RB_ObjectBase::getName() const {
    return getValue("name").toString();
}

/**
 * Set the name of this object
 * @param n name
 */
void RB_ObjectBase::setName(const RB_String& n) {
    setValue("name", n);
}

/**
 * @return status of this record in the database,
 * such as RB2::StatusDefault
 */
int RB_ObjectBase::getStatus() const {
    return getValue("status").toInt();
}

/**
 * Set the status of this record in the database
 * @param s status such as RB2::StatusDefault
 */
void RB_ObjectBase::setStatus(int s) {
    setValue("status", s);
}

/**
 * @return creation date of this record in the database
 */
RB_String RB_ObjectBase::getCreated() const {
    return getValue("created").toString();
}

/**
 * Set the creation date of this object in the database
 * @param c creation date
 */
void RB_ObjectBase::setCreated(const RB_String& c) {
    setValue("created", c);
}

/**
 * @return changed date of this record in the database
 */
RB_String RB_ObjectBase::getChanged() const {
    return getValue("changed").toString();
}

/**
 * Set the changed date of this record in the database
 * @param c changed date
 */
void RB_ObjectBase::setChanged(const RB_String& c) {
    setValue("changed", c);
}

/**
 * @return name of user that made the last change
 * to this record in the database
 */
RB_String RB_ObjectBase::getUser() const {
    return getValue("muser").toString();
}

/**
 * Set the username of this record in the database
 * @param u username
 */
void RB_ObjectBase::setUser(const RB_String& u) {
    setValue("muser", u);
}

/**
 * @return name the name of this objectfactory
 */
RB_ObjectFactory* RB_ObjectBase::getFactory() const {
    return mFactory;
}

/**
 * @return name the name of this objectfactory
 */
void RB_ObjectBase::setFactory(RB_ObjectFactory* f) {
    mFactory = f;
}

/**
 * Revert changes to member values. If changed reverted to previous value
 */
void RB_ObjectBase::revert() {
    std::vector<RB_ObjectMember*>::iterator iter;
    iter = mMemberVector.begin();
    while (iter != mMemberVector.end()) {
        if (*iter != NULL) {
            RB_ObjectMember* mem = dynamic_cast<RB_ObjectMember*>(*iter);

            if (mem) {
                mem->revert();
            }
        }
        ++iter;
    }

    delFlag(RB2::FlagIsDirty);
}

/**
 * Helper function to create members of object,
 * in case of no object factory.
 * @param copy copy object to be created
 * @param level resolve level, not applicable here
 */
void RB_ObjectBase::createCopy(RB_ObjectBase* copy, RB2::ResolveLevel /*level*/) {
    RB_ObjectMember* mem = NULL;
    int memCount = this->countMember();

    for (int i = RB2::HIDDENCOLUMNS; i < memCount; ++i) {
        mem = mMemberVector.at(i);
        copy->addMember(copy, mem->getName(), mem->getUnit(),
                        mem->getValue(), mem->getType(),
                        mem->getPreviousValue(), mem->getDisplayValue());
    }
}

/**
 * @return true if this object is clone
 */
bool RB_ObjectBase::isClone() {
    return mClonedObject;
}

/**
 * @param isCloned true if object is a clone
 */
void RB_ObjectBase::setCloned(bool cloned) {
    mClonedObject = cloned;
}

/**
 * Get original object
 * @return original object or NULL
 */
RB_ObjectBase* RB_ObjectBase::getOriginal() {
    if (isClone() && mOriginal) {
        return mOriginal;
    }
    return NULL;
}

/**
 * Set original object
 * @param orig original object
 */
void RB_ObjectBase::setOriginal(RB_ObjectBase* orig) {
    mOriginal = orig;
}

/**
 * Delete original object
 */
void RB_ObjectBase::deleteOriginal() {
    if (isClone() && mOriginal) {
        RB_ObjectBase* parent = mOriginal->getParent();
        parent->remove(mOriginal);
        setOriginal(NULL);
    }
}

/**
 * @return total number of members in this object
 */
int RB_ObjectBase::countMember() const {
    return (int)mMemberVector.size();
//    return mMemberMap.size();
}

/**
 * Get member object
 * @param number sequence number of member, number relates to the column number
 * of a table in a database. Numbers start with 0 (zero).
 * @return selected object member
 */
RB_ObjectMember* RB_ObjectBase::getMember(int number) const {
    RB_ObjectMember* member = NULL;

    if (number < 0 || number >= countMember()) {
        RB_String str = "RB_ObjectBase::getMember(int="
                        + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
    } else {
        member = mMemberVector.at(number);

//        std::map<std::string, RB_ObjectMember*>::const_iterator iter;
//        int count = 0;

//        for (iter = mMemberMap.begin(); iter != mMemberMap.end(); ++iter) {
//            if (count == number) {
//                return iter->second;
//            }
//            ++count;
//        }

    }

    return member;
}

/**
 * Get member object
 * @param name name of member, name corresponds with column name of table
 * in a database
 * @return selected object member
 */
RB_ObjectMember* RB_ObjectBase::getMember(const RB_String& name) const {
    // Do not use RB_ObjectIterator since members are part of std::vector
    RB_ObjectMember* member = NULL;
    RB_ObjectMember* tmpMember = NULL;
    int memCount = countMember();

    for (int i = 0; !member && i < memCount; i++) {
        tmpMember = getMember(i);

        if (tmpMember->getName() == name) {
            member = tmpMember;
            i = memCount;
        }
    }
    // Debug only
    //    if (!member) {
    //        RB_DEBUG->debug("RB_ObjectBase::getMember(RB_Stringname=" + name + ") ERROR");
    //        for (int i = 0; i < noMem; i++) {
    //            tmpMember = getMember(i);
    //            RB_DEBUG->debug("  with membername=" + tmpMember->getName());
    //        }
    //    }
    // end Debug
    return member;

//    std::map<std::string, RB_ObjectMember*>::const_iterator iter;
//    iter = mMemberMap.find(name.toStdString());

//    if (iter != mMemberMap.end()) {
//        return iter->second;
//    }

//    return NULL;
}

/**
 * Get member number
 * @param name name of member, name corresponds with column name of table
 * in a database
 * @return sequence number of the member with name
 */
int RB_ObjectBase::getMemberNo(const RB_String& name) const {
    int number = -1;
    int memCount = countMember();

    // Do not use RB_ObjectIterator since members are part of std::vector
    for (int i = 0; i < memCount && number < 0; i++) {
        if (getMember(i)->getName() == name) {
            number = i;
        }
    }

    if (number < 0) {
        RB_String str = "RB_ObjectBase::getMemberNo(RB_String=" + name + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
    }

    return number;
}

/**
 * Creation, addition and initialization of members. No action if member
 * member name already exists.
 * @param id unique identification of member, used to interface with database
 * @param parent parent (base) object of member
 * @param name name of member
 * @param unit unit of measurement
 * @param value current value
 * @param prevValue previous value
 * @return member
 */
RB_ObjectMember* RB_ObjectBase::addMember(RB_ObjectBase* parent,
                        const RB_String& name, const RB_String& unit,
                        const RB_Variant& value, RB2::MemberType type,
                        const RB_Variant& prevValue, const RB_Variant& dispValue) {
    // For debug only, slows down the creation of objects
    /*
    if (getMember(name)) {
        RB_DEBUG->print("RB_ObjectBase::addMember() member '%s' "
                            "already exists ERROR", name.toStdString().c_str());
        return false;
    }
    */

    RB_ObjectMember* newMember = new RB_ObjectMember(parent, name, unit,
                                                     value, type, prevValue,
                                                     dispValue);
    mMemberVector.push_back(newMember);
//    mMemberMap[name.toStdString()] = newMember;
    return newMember;
}

/**
 * Delete all members in memberVector and clear vector
 */
void RB_ObjectBase::removeMembers() {
    std::vector<RB_ObjectMember*>::iterator iter;
    iter = mMemberVector.begin();
    while (iter != mMemberVector.end()) {
        if (*iter != NULL) {
            delete *iter;
            *iter = NULL;
        }
        ++iter;
    }

    // empty memberVector, this is not overdone because otherwise
    // vector contains invalid pointers
    mMemberVector.clear();

//    std::map<std::string, RB_ObjectMember*>::const_iterator iter;
//    iter = mMemberMap.begin();
//    while (iter != mMemberMap.end()) {
//        if (iter->second != NULL) {
//            delete iter->second;
//        }
//        ++iter;
//    }
//    mMemberMap.clear();
}

/**
 * Short notation for getMember(int number)->getValue()
 * @param number index number of value
 * @return RB_Variant member 'value' at position 'number'
 */
RB_Variant RB_ObjectBase::getValue(int number) const {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::getValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getValue();
}

/**
 * Short notation for getMember(RB_String name)->getValue()
 * NOTE: getValue("parent") returns the id of the actual parent container
 * as in the database, not the id of the intermediate list. If this is a list
 * the returned id will be of the direct parent container. This function will
 * first search for the member and return the value or return the mId and mName
 * in case of membername is 'id' or 'name'.
 * @param name name of value member
 * @return RB_Variant member 'value' with label 'name'
 */
RB_Variant RB_ObjectBase::getValue(const RB_String& name) const {
    RB_ObjectMember* member = getMember(name);
    if (!member && (name == "id")) {
        return mId;
    } else if (mName.endsWith("List") && mParent && name == "parent") {
        return mParent->getId();
    } else if (!member && (name == "name")) {
        return mName;
    } else if (!member) {
        RB_String str = "RB_ObjectBase::getValue(RB_String=" + name + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getValue();
}

/**
 * Short notation for getMember(int number)->setValue(RB_variant var)
 * @param number index number of previous value member
 * @param var value
 */
void RB_ObjectBase::setValue(int number, const RB_Variant& var) {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::setValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }
    if (var != member->getValue()) {
        member->setValue(var);
    }
}

/**
 * Short notation for getMember(RB_String name)->setValue(RB_variant var)
 * NOTE: the values for 'id' and 'name' members will be set to the private
 * mId and mName always, even if the members do not exist.
 * @param name name of previous value member
 * @param var value
 */
void RB_ObjectBase::setValue(const RB_String& name, const RB_Variant& var) {
    if (name == "id") {
        mId = var.toString();
    // } else if (name == "parent") {
        // nothing
    } else if (name == "name") {
        mName = var.toString();
    }
    RB_ObjectMember* member = getMember(name);
    if (!member) {
        if (name != "parent") { // 'parent' in case without member creation
            RB_String str = "RB_ObjectBase::setValue(RB_String=" + name + ", "
                            + var.toString() + ") ERROR";
            RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        }
        return;
    }
    if (var != member->getValue()) {
        member->setValue(var);
    }
}

/**
 * Short notation for getMember(int number)->getPreviousValue()
 * @param number index number of previous value member
 * @return RB_Variant member 'previousValue' at position 'number'
 */
RB_Variant RB_ObjectBase::getPValue(int number) const {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::getPValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getPreviousValue();
}

/**
 * Short notation for getMember(RB_String number)->getPreviousValue()
 * @param name name of previous value member
 * @return RB_Variant member 'previousValue' with label 'name'
 */
RB_Variant RB_ObjectBase::getPValue(const RB_String& name) const {
    RB_ObjectMember* member = getMember(name);
    if (!member) {
        RB_String str = "RB_ObjectBase::getPValue(RB_String=" + name + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getPreviousValue();
}

/**
 * Short notation for getMember(int number)->setPreviousValue(RB_variant var)
 * @param number index number of previous value member
 * @param var value
 */
void RB_ObjectBase::setPValue(int number, const RB_Variant& var) {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::setPValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }
    member->setPreviousValue(var);
}

/**
 * Short notation for getMember(RB_String name)->setPreviousValue(RB_variant var)
 * @param name name of previous value member
 * @param var value
 */
void RB_ObjectBase::setPValue(const RB_String& name, const RB_Variant& var) {
    RB_ObjectMember* member = getMember(name);
    if (!member) {
        RB_String str = "RB_ObjectBase::setPValue(RB_String=" + name + ", var) ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }
    member->setPreviousValue(var);
}

/**
 * Short notation for getMember(int number)->getDisplayValue()
 * @param number index number of display value member
 * @return RB_Variant member 'displayValue' at position 'number'
 */
RB_Variant RB_ObjectBase::getDValue(int number) const {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::getDValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getDisplayValue();
}

/**
 * Short notation for getMember(RB_String number)->getDisplayValue()
 * @param name name of display value member
 * @return RB_Variant member 'displayValue' with label 'name'
 */
RB_Variant RB_ObjectBase::getDValue(const RB_String& name) const {
    RB_ObjectMember* member = getMember(name);
    if (!member) {
        RB_String str = "RB_ObjectBase::getDValue(RB_String=" + name + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return RB_Variant();
    }
    return member->getDisplayValue();
}

/**
 * Short notation for getMember(int number)->setDisplayValue(RB_variant var)
 * @param number index number of display value member
 * @param var value
 */
void RB_ObjectBase::setDValue(int number, const RB_Variant& var) {
    RB_ObjectMember* member = getMember(number);
    if (!member) {
        RB_String str = "RB_ObjectBase::setDValue(int=" + RB_String::number(number) + ") ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }
    member->setDisplayValue(var);
}

/**
 * Short notation for getMember(RB_String name)->setDisplayValue(RB_variant var)
 * @param name name of display value member
 * @param var value
 */
void RB_ObjectBase::setDValue(const RB_String& name, const RB_Variant& var) {
    RB_ObjectMember* member = getMember(name);
    if (!member) {
        RB_String str = "RB_ObjectBase::setDValue(RB_String=" + name + ", var) ERROR";
        RB_DEBUG->print(RB_Debug::D_ERROR, str.toStdString().c_str());
        return;
    }
    member->setDisplayValue(var);
}

/**
 * Read this object data from database
 * @param db database connection
 * @param level resolve level
 * @param calledFromList if true objects have already been read
 * with dbReadList, only the child objects in the list containers
 * need to be read
 * @param useParentId if true use parent ID instead of object ID in query
 */
bool RB_ObjectBase::dbRead(QSqlDatabase db, RB2::ResolveLevel level,
                           bool calledFromList, bool useParentId) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setResolveLevel(level);
    vis->setCalledFromList(calledFromList);
    vis->setUseParentId(useParentId);
    vis->setDbRead();
    this->acceptVisitor(*vis);
    delete vis;
    return true;
}

/**
 * To be overriden, does nothing
 * @return true
 */
bool RB_ObjectBase::dbReadList(QSqlDatabase, RB2::ResolveLevel,
                               const RB_String& /*fieldName*/) {
    return true;
}

/**
 * Update database or insert this object data.
 * NOTE: A tree (database) model creates a dummy root of the same class as
 * the tree objects. This root object has the same ID as the actual parent.
 * The parent ID is set to "". This dummy root object is not stored in the
 * database based on the "" parent ID. Therefore make sure that objects
 * that need to be stored to the database should always have a parent ID.
 * The root XXX_Project objects have standard "0" as parent ID and can be
 * stored to the database. Objects for which the flag FlagIsDeleted is set,
 * will call dbDelete() and for the children.
 * @param db database connection
 * @param resolveLevel not used here
 */
bool RB_ObjectBase::dbUpdate(QSqlDatabase db, RB2::ResolveLevel level) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setResolveLevel(level);
    vis->setDbUpdate();
    vis->setUpdateNewId(false); // if true new ID will be created, default is false
    this->acceptVisitor(*vis);
    delete vis;
    return true;
}

/**
 * To be overriden, does nothing
 * @return true
 */
bool RB_ObjectBase::dbUpdateList(QSqlDatabase, RB2::ResolveLevel) {
    return true;
}

/**
 * Delete this object from database
 * TODO: no flags are set, because maybe not necessary after deletion of objects
 * @param db database connection
 * @param calledFromList true if called from list, if false will delete single object
 */
bool RB_ObjectBase::dbDelete(QSqlDatabase db, bool calledFromList) {
    RB_DbVisitor* vis = new RB_DbVisitor();
    vis->setDatabase(db);
    vis->setCalledFromList(calledFromList);
    vis->setDbDelete();
    this->acceptVisitor(*vis);
    delete vis;
    return true;
}

/**
 * To be overriden, does nothing
 * @return true
 */
bool RB_ObjectBase::dbDeleteList(QSqlDatabase, const RB_String& /* fieldName */) {
    return true;
}

/**
 * Resolve next level e.g. RB2::ResolveTwo becomes RB2::ResolveOne
 * @param level current resolve level
 * @return next resolve level
 */
RB2::ResolveLevel RB_ObjectBase::resolveNext(RB2::ResolveLevel level) {
    switch (level) {
        case RB2::ResolveNone:
            return RB2::ResolveNone;
            break;
        case RB2::ResolveOne:
            return RB2::ResolveNone;
            break;
        case RB2::ResolveTwo:
            return RB2::ResolveOne;
            break;
        case RB2::ResolveThree:
            return RB2::ResolveTwo;
            break;
        case RB2::ResolveFour:
            return RB2::ResolveThree;
            break;
        case RB2::ResolveFive:
            return RB2::ResolveFour;
            break;
        case RB2::ResolveSix:
            return RB2::ResolveFive;
            break;
        case RB2::ResolveSeven:
            return RB2::ResolveSix;
            break;
        case RB2::ResolveEight:
            return RB2::ResolveSeven;
            break;
        case RB2::ResolveNine:
            return RB2::ResolveEight;
            break;
        case RB2::ResolveTen:
            return RB2::ResolveNine;
            break;
        case RB2::ResolveAll:
            return RB2::ResolveAll;
            break;
        default:
            return RB2::ResolveNone;
            break;
    }
    return RB2::ResolveNone;
}

/**
 * Debug only: return object data in a string
 * @return string with object and member data
 */
RB_String RB_ObjectBase::toString() {
    RB_String str = "";

    str += getId() + ", ";
    str += getParent() ? getParent()->getId() + ", " : "NULL, ";
    str += getName() + "\n";

    for (unsigned int i = 2; i < mMemberVector.size(); ++i) {
        str += "    " + mMemberVector.at(i)->toString() + "\n";
    }

//    std::map<std::string, RB_ObjectMember*>::const_iterator iter;
//    iter = mMemberMap.begin();
//    while (iter != mMemberMap.end()) {
//        str += "    " + iter->second->toString() + "\n";
//        ++iter;
//    }

    return str;
}

// EOF
