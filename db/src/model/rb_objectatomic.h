/*****************************************************************
 * $Id: rb_objectatomic.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef RB_OBJECTATOMIC_H
#define RB_OBJECTATOMIC_H

#include <iostream>
#include <list>
#include <string>

#include "rb_objectbase.h"

/**
 * Template of atomic object in model, has no children containers only members
 */
class DB_EXPORT RB_ObjectAtomic : public RB_ObjectBase {

public:

    RB_ObjectAtomic(const RB_String& id = "", RB_ObjectBase* p = 0,
                    const RB_String& n = "", RB_ObjectFactory* f = NULL,
                    bool woMembers = false);
    RB_ObjectAtomic(RB_ObjectAtomic* obja);
    virtual ~RB_ObjectAtomic();

    virtual RB_ObjectBase* copy(RB2::ResolveLevel level = RB2::ResolveNone);
    virtual RB_ObjectBase* clone(RB2::ResolveLevel level = RB2::ResolveNone);

    virtual bool isAtomic();
    virtual bool isContainer();
    virtual bool isList();

    //! Does nothing, returns NULL
    virtual RB_ObjectBase* getObject(const RB_String&) {
        return NULL;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectBase* getObject(const RB_String&, const RB_Variant&) {
        return NULL;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectBase* newObject(const RB_String&) {
        return NULL;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectContainer* getContainer(const RB_String&) {
        return NULL;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectContainer* newContainer(const RB_String&) {
        return NULL;
    }
    //! Does nothing, returns false
    virtual bool addObject(RB_ObjectBase*) {
        RB_DEBUG->error("RB_ObjectAtomic::addObject() ERROR");
        return false;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectIterator* createIterator() {
        return NULL;
    }
    //! Does nothing, returns false
    virtual bool remove(RB_ObjectBase*, bool) {
        RB_DEBUG->error("RB_ObjectAtomic::remove() ERROR");
        return false;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectBase* getChildObject(const RB_String&) {
        return NULL;
    }
    //! Does nothing, returns NULL
    virtual RB_ObjectBase* getDescendantObject(const RB_String&) {
        return NULL;
    }

    //! Does nothing, returns true
    virtual bool eraseChildren() {
        return true;
    }

    virtual void acceptVisitor(RB_Visitor& visitor);
};

#endif // RB_OBJECTATOMIC_H
