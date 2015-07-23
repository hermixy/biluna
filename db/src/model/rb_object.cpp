/*****************************************************************
 * $Id: rb_object.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Created: 2015-06-29 rutger
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the data base (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_object.h"
#include "rb_debug.h"

RB_Object::RB_Object(const RB_String& name) {
    RB_DEBUG->addObjectCreated(this);
    this->mName = name;
}

RB_Object::RB_Object(RB_Object* obj) {
    RB_DEBUG->addObjectCreated(this);
    mName = obj->getName();
    setFlags(obj->getFlags());
}

RB_Object::~RB_Object() {
    RB_DEBUG->addObjectDeleted(this);
    std::vector<RB_Object*>::iterator iter;
    iter = mChildList.begin();

    while (iter != mChildList.end()) {
        if (*iter != NULL) {
            delete *iter;
            *iter = NULL;
        }
        ++iter;
    }

    mChildList.clear();
}

RB_Object& RB_Object::operator= (const RB_Object& obj) {
    mName = obj.getName();
    setFlags(obj.getFlags());
    return *this;
}

void RB_Object::init() { }

RB_Object* RB_Object::getParent() const {
    return mParent;
}

std::vector<RB_Object*> RB_Object::getChildList() const {
    return mChildList;
}

RB_String RB_Object::getName() const {
    return mName;
}

void RB_Object::setName(const RB_String& name) {
    mName = name;
}


