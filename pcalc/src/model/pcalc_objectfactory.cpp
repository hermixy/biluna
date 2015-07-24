/*****************************************************************
 * $Id: pcalc_objectfactory.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Apr 14, 2010 8:34:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_objectfactory.h"

#include "db_objectfactory.h"
#include "pcalc_en1591_assembly.h"
#include "pcalc_en1591_boltnutwasher.h"
#include "pcalc_en1591_flange.h"
#include "pcalc_en1591_gasket.h"
#include "pcalc_en1591_loadcase.h"
#include "pcalc_en1591_shell.h"
#include "peng_project.h"
#include "rb_debug.h"
#include "rb_uuid.h"

PCALC_ObjectFactory* PCALC_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
PCALC_ObjectFactory::PCALC_ObjectFactory() {
    RB_DEBUG->print("PCALC_ObjectFactory::PCALC_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_ObjectFactory::~PCALC_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("PCALC_ObjectFactory::~PCALC_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
PCALC_ObjectFactory* PCALC_ObjectFactory::getInstance() {
    RB_DEBUG->print("PCALC_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new PCALC_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("PCALC_Project");
        mActiveObjectFactory->setName("PCALC_ObjectFactory");
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
RB_ObjectBase* PCALC_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "PCALC_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "PCALC_") { // root
        obj = new PENG_Project(uuid, NULL, "PENG_Project", this);

        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_AssemblyList", this);
        obj->addObject(list);
    } else if (str == "PCALC_EN1591_AssemblyList") {
        obj = new PCALC_EN1591_Assembly(uuid, parent, "PCALC_EN1591_Assembly", this);

        uuid = "";
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_BoltNutWasherList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_FlangeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_GasketList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_LoadCaseList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer (uuid, obj, "PCALC_EN1591_ShellList", this);
        obj->addObject(list);
    } else if (str == "PCALC_EN1591_BoltNutWasherList") {
        obj = new PCALC_EN1591_BoltNutWasher(uuid, parent, "PCALC_EN1591_BoltNutWasher", this);
    } else if (str == "PCALC_EN1591_FlangeList") {
        obj = new PCALC_EN1591_Flange(uuid, parent, "PCALC_EN1591_Flange", this);
    } else if (str == "PCALC_EN1591_GasketList") {
        obj = new PCALC_EN1591_Gasket(uuid, parent, "PCALC_EN1591_Gasket", this);
    } else if (str == "PCALC_EN1591_LoadCaseList") {
        obj = new PCALC_EN1591_LoadCase(uuid, parent, "PCALC_EN1591_LoadCase", this);
    } else if (str == "PCALC_EN1591_ShellList") {
        obj = new PCALC_EN1591_Shell(uuid, parent, "PCALC_EN1591_Shell", this);
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}
