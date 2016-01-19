/*****************************************************************
 * $Id: rb_objectatomic.cpp 1607 2012-04-06 09:56:51Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_objectatomic.h"

#include "rb_objectcontainer.h"

/**
 * Constructor
 * TODO: after extensive testing remove debug functions
 * @param id identification
 * @param p parent container
 * @param n name of object such as LDT_Line
 * @param f object factory
 * @param woMembers, create without members id parent and name if true
 */
RB_ObjectAtomic::RB_ObjectAtomic(const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f, bool woMembers)
                        : RB_ObjectBase(id, p, n, f, woMembers)  {
	// test only
	// std::cout << "->RB_ObjectAtomic::RB_ObjectAtomic() " << this 
	// 			<<  " - " << name << std::endl;
}

/**
 * Copy constructor
 * TODO: after extensive testing remove debug functions
 * @param obj object to be copied
 */
RB_ObjectAtomic::RB_ObjectAtomic(RB_ObjectBase* obj) : RB_ObjectBase(obj) {
	// test only
	// std::cout << "->RB_ObjectAtomic::RB_ObjectAtomic() " << this 
	//			<<  " - copy " << name << std::endl;
}

/**
 * Destructor
 * TODO: after extensive testing remove debug functions
 */
RB_ObjectAtomic::~RB_ObjectAtomic() {
	// test only
	// std::cout << "<-RB_ObjectAtomic::~RB_ObjectAtomic() " << this 
	//			<<  " - " << name << std::endl;
}

/**
 * Copy of the object, does copy the members but not the ID (=Uuid) and parent
 * @return obj copy of this object, the copy is a shallow copy. Deep copies
 * are only applicable to RB_ObjectContainer
 */
RB_ObjectBase* RB_ObjectAtomic::copy(RB2::ResolveLevel level) {
    RB_ObjectBase* obj = NULL;

    if (!mFactory) {
        RB_DEBUG->warning("RB_ObjectAtomic::copy() factory NULL WARNING");
        obj = new RB_ObjectAtomic(RB_Uuid::createUuid().toString(), NULL,
                                 getName(), NULL, false);
        createCopy(obj, level);
        return obj;
    }

    // use object factory
    mFactory->setCreateNewId(true);
    obj = clone(level);
    mFactory->setCreateNewId(false);
    return obj;
}

/**
 * Clone exact copy including the ID and parent
 * @param level of clone, not used deep copies are only applicable to RB_ObjectContainer
 * @return obj clone of this object, the clone is a shallow copy.
 */
RB_ObjectBase* RB_ObjectAtomic::clone(RB2::ResolveLevel level) {
    RB_ObjectBase* obj = NULL;

    if (!mFactory) {
        RB_DEBUG->warning("RB_ObjectBase::clone() factory NULL WARNING");
        obj = new RB_ObjectAtomic(getId(), getParent(), getName(), NULL, false);
        obj->setCloned(true);
        obj->setOriginal(this);
        createCopy(obj, level);
        return obj;
    }

    // use object factory
    obj = mFactory->shallowCopy(this);
    return obj;
}

/**
 * Object is atomic: has members but no children (objects or lists)
 * @return true
 */
bool RB_ObjectAtomic::isAtomic() {
    return true;
}

/**
 * Object is a container: has members and can have a container of lists
 * @return false
 */
bool RB_ObjectAtomic::isContainer() {
    return false;
}

/**
 * Object is a list: has no members and is a list of same type of objects
 * @return false
 */
bool RB_ObjectAtomic::isList() {
    return false;
}

/**
 * Accept visitor of the visitor design pattern,
 * such as XML writer.
 * @param visitor
 */
void RB_ObjectAtomic::acceptVisitor(RB_Visitor& visitor) {
    visitor.visitObject(this);
    visitor.leaveObject(this);
}

