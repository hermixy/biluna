/*****************************************************************
 * $Id: sail_objectfactory.cpp 1727 2012-07-30 22:01:50Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_objectfactory.h"

#include "sail_coordinate.h"
#include "sail_map.h"
#include "sail_project.h"
#include "sail_route.h"
#include "sail_routecoordinate.h"
#include "sail_symbol.h"
#include "sail_syssetting.h"
#include "sail_track.h"
#include "db_objectfactory.h"
#include "rb_debug.h"
#include "rb_uuid.h"

SAIL_ObjectFactory* SAIL_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
SAIL_ObjectFactory::SAIL_ObjectFactory() {
    RB_DEBUG->print("SAIL_ObjectFactory::SAIL_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SAIL_ObjectFactory::~SAIL_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("SAIL_ObjectFactory::~SAIL_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
SAIL_ObjectFactory* SAIL_ObjectFactory::getInstance() {
    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new SAIL_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("SAIL_Project");
        mActiveObjectFactory->setName("SAIL_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object, connect to- and make part of parent.
 * NOTE:
 *  SAIL_Map
 *  SAIL_Symbol
 * are part of SAIL_SysSetting but are global system settings.
 * The relevant global models will set the filter to WHERE id<>'0'
 * instead of WHERE parent='acc_project.id'. Their parent
 * is set to 'default'.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param name name of the parent list (container)
 * @param addToParent add new object to parent container
 * @return object, connected to parent
 */
RB_ObjectBase* SAIL_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "SAIL_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "SAIL_") { // root
        obj = new SAIL_Project(uuid, NULL, "SAIL_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "SAIL_CoordinateList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SAIL_RouteList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SAIL_SysSettingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SAIL_TrackList", this);
        obj->addObject(list);
    } else if (str == "SAIL_CoordinateList") {
        obj = new SAIL_Coordinate(uuid, parent, "SAIL_Coordinate", this);
    } else if (str == "SAIL_MapList") {
        obj = new SAIL_Map(uuid, parent, "SAIL_Map", this);
    } else if (str == "SAIL_RouteList") {
        obj = new SAIL_Route(uuid, parent, "SAIL_Route", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SAIL_RouteCoordinateList", this);
        obj->addObject(list);
    } else if (str == "SAIL_RouteCoordinateList") {
        obj = new SAIL_RouteCoordinate(uuid, parent, "SAIL_RouteCoordinate", this);
    } else if (str == "SAIL_SymbolList") {
        obj = new SAIL_Symbol(uuid, parent, "SAIL_Symbol", this);
    } else if (str == "SAIL_SysSettingList") {
        obj = new SAIL_SysSetting(uuid, parent, "SAIL_SysSetting", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SAIL_MapList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SAIL_SymbolList", this);
        obj->addObject(list);
    } else if (str == "SAIL_TrackList") {
        obj = new SAIL_Track(uuid, parent, "SAIL_Track", this);
    } else {
        RB_DEBUG->error("SAIL_ObjectFactory::newObject() " + parent->getName() + " ERROR");
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}

/**
 * Is field or object name a system setting
 * @param name field or object name
 * @returns true if name is part of the system settings
 */
bool SAIL_ObjectFactory::isSystemSetting(const RB_String& name) {
    if (name == "SAIL_MapList" || name == "SAIL_Map") {
        return true;
    } else if (name == "SAIL_SymbolList" || name == "SAIL_Symbol") {
        return true;
    }

    return false;
}

/**
 * Is field or object name a project setting
 * @param name field or object name
 * @returns true if name is part of the project settings
 */
bool SAIL_ObjectFactory::isProjectSetting(const RB_String& name) {
    //    if (name == "SAIL_ChartMasterList" || name == "SAIL_ChartMaster") {
    //        return true;
    //    }
    return false;
}
