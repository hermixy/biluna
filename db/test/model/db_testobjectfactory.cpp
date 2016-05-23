/*****************************************************************
 * $Id: db_testobjectfactory.cpp 1433 2011-07-04 20:09:05Z rutger $
 * Created: Sep 1, 2009 8:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testobjectfactory.h"

#include "db_objectfactory.h"
#include "db_test.h"
#include "db_testchild.h"
#include "db_testdlg.h"
#include "db_testproject.h"
#include "db_testrelation.h"
#include "rb_debug.h"
#include "rb_uuid.h"

DB_TestObjectFactory* DB_TestObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor, register with DB_ObjectFactory
 */
DB_TestObjectFactory::DB_TestObjectFactory() {
    RB_DEBUG->print("DB_TestObjectFactory::DB_TestObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
DB_TestObjectFactory::~DB_TestObjectFactory() {
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("DB_TestObjectFactory::~DB_TestObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
DB_TestObjectFactory* DB_TestObjectFactory::getInstance() {
    RB_DEBUG->print("DB_TestObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new DB_TestObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("DB_TestProject");
        mActiveObjectFactory->setName("DB_TestObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object connected to and part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *          will be returned
 * @param name name of the parent list (container)
 * @param addToParent add object to parent
 * @param withContainers create object with list containers
 * @return object
 */
RB_ObjectBase* DB_TestObjectFactory::newObject(const RB_String& id,
                                               RB_ObjectBase* parent,
                                               const RB_String& /* typeName */,
                                               bool addToParent) {
    RB_DEBUG->print("DB_TestObjectFactory::newObject()");

    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "DB_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "DB_") { // root
        obj = new DB_TestProject(uuid, NULL, "DB_TestProject", this);
        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "DB_TestList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_TestRelationList", this);
        obj->addObject(list);
    } else if (str == "DB_TestList") {
        obj = new DB_Test(uuid, parent, "DB_Test", this);
        uuid = "";

        // For tree model only
        list = new RB_ObjectContainer(uuid, obj, "DB_TestList", this);
        obj->addObject(list);

        list = new RB_ObjectContainer(uuid, obj, "DB_TestChildList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_TestDlgList", this);
        obj->addObject(list);
    } else if (str == "DB_TestRelationList") {
        obj = new DB_TestRelation(uuid, parent, "DB_TestRelation", this);
    } else if (str == "DB_TestChildList") {
        obj = new DB_TestChild(uuid, parent, "DB_TestChild", this);
        // For tree model only
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "DB_TestChildList", this);
        obj->addObject(list);
    } else if (str == "DB_TestDlgList") {
        obj = new DB_TestDlg(uuid, parent, "DB_TestDlg", this);
    }

    // Add object to parent and set object parent 2009-08-16
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
