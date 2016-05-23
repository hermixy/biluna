/*****************************************************************
 * $Id: db_testobjectfactory.h 1433 2011-07-04 20:09:05Z rutger $
 * Created: Sep 1, 2009 8:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTOBJECTFACTORY_H
#define DB_TESTOBJECTFACTORY_H

#include <iostream>
#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"


#define DB_TESTOBJECTFACTORY DB_TestObjectFactory::getInstance()


/**
 * Instance (static) class which acts as an object factory.
 * This class is instantiated once and uses the interface
 * RB_ObjectFactoryInterface to interact with general Rb_ObjectAtomic,
 * RB_ObjectBase, RB_ObjectContainer, RB_ObjectIterator, RB_ObjectMember
 * functions. In the model use DB_OBJECTFACTORY->function() to create objects
 * or test whether it is an object of this model.
 */
class DB_TestObjectFactory : public RB_ObjectFactory {

public:
    virtual ~DB_TestObjectFactory();
    static DB_TestObjectFactory* getInstance();

    virtual RB_ObjectBase* newObject(const RB_String& id,
                                     RB_ObjectBase* parent,
                                     const RB_String& typeName = "",
                                     bool addToParent = true);

    RB_String mainListName() const { return "DB_TestList"; }

private:
    DB_TestObjectFactory();
    static DB_TestObjectFactory* mActiveObjectFactory;

};

#endif // DB_TESTOBJECTFACTORY_H
