/*****************************************************************
 * $Id: std_objectfactory.cpp 0001 2016-01-06 00:31:33Z rutger $
 * Created: Jan 06, 2016 8:34:25 PM - rutger
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_objectfactory.h"

#include "db_objectfactory.h"

#include "std_project.h"
#include "rb_debug.h"
#include "rb_uuid.h"

STD_ObjectFactory* STD_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
STD_ObjectFactory::STD_ObjectFactory() {
    RB_DEBUG->print("STD_ObjectFactory::STD_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
STD_ObjectFactory::~STD_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("STD_ObjectFactory::~STD_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
STD_ObjectFactory* STD_ObjectFactory::getInstance() {
    RB_DEBUG->print("STD_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new STD_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("STD_Project");
        mActiveObjectFactory->setName("STD_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object connected to and part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param name name of the parent list (container)
 * @return object, connected to parent
 */
RB_ObjectBase* STD_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "STD_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "STD_") { // root
        obj = new STD_Project(uuid, NULL, "STD_Project", this);

        list = new RB_ObjectContainer (uuid, obj, "STD_GasketEN13555List", this);
        obj->addObject(list);
    } else if (str == "STD_STD_GasketEN13555List") {
        obj = new STD_STD_GasketEN13555(uuid, parent, "STD_STD_GasketEN13555", this);
/*
        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "STD_EN1591_BoltNutWasherList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN1591_FlangeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN1591_GasketList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN1591_LoadCaseList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "STD_EN1591_ShellList", this);
        obj->addObject(list);
    } else if (str == "STD_EN1591_BoltNutWasherList") {
        obj = new STD_EN1591_BoltNutWasher(uuid, parent, "STD_EN1591_BoltNutWasher", this);
    } else if (str == "STD_EN1591_FlangeList") {
        obj = new STD_EN1591_Flange(uuid, parent, "STD_EN1591_Flange", this);
    } else if (str == "STD_EN1591_GasketList") {
        obj = new STD_EN1591_Gasket(uuid, parent, "STD_EN1591_Gasket", this);
    } else if (str == "STD_EN1591_LoadCaseList") {
        obj = new STD_EN1591_LoadCase(uuid, parent, "STD_EN1591_LoadCase", this);
    } else if (str == "STD_EN1591_ShellList") {
        obj = new STD_EN1591_Shell(uuid, parent, "STD_EN1591_Shell", this);
*/
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
