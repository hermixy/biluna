/*****************************************************************
 * $Id: db_objectfactory.cpp 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_objectfactory.h"

#include "db_activity.h"
#include "db_mapping.h"
#include "db_map.h"
#include "db_permissiongroup.h"
#include "db_permissionperspectiveproject.h"
#include "db_permissionproject.h"
#include "db_project.h"
#include "db_simplereport.h"
#include "db_systemgroup.h"
#include "db_systemuser.h"
#include "db_systemusergroup.h"
#include "db_systemuserpermission.h"
#include "db_test.h"
#include "db_testchild.h"
#include "db_testdlg.h"
#include "db_testrelation.h"
#include "db_version.h"
#include "rb_debug.h"
#include "rb_uuid.h"


DB_ObjectFactory* DB_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor, registers itself
 */
DB_ObjectFactory::DB_ObjectFactory() : RB_ObjectFactory() {
    RB_DEBUG->print("DB_ObjectFactory::DB_ObjectFactory()");

    // This is the only factory that registers itself
    this->registerFactory(this);
}

/**
 * Destructor
 */
DB_ObjectFactory::~DB_ObjectFactory() {
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("DB_ObjectFactory::~DB_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
DB_ObjectFactory* DB_ObjectFactory::getInstance() {
	// RB_DEBUG->print("DB_ObjectFactory::getInstance()");

    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new DB_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("DB_Project");
        mActiveObjectFactory->setName("DB_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object connected to and part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *          will be returned
 * @param name name of the parent list (container)
 * @param addToParent true (default) if object is to be added to parent
 * @param withContainers create object with containers, default is true
 * @return object, connected to parent
 */
RB_ObjectBase* DB_ObjectFactory::newObject(const RB_String& id, 
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
	RB_String uuid = id;
	if (uuid == "") {
            uuid = RB_Uuid::createUuid().toString();
	}
	
	RB_String str = "";
        if (parent != NULL) {
            str = parent->getName();
        } else {
            str = "DB_";
	}
	
    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "DB_") {
        obj = new DB_Project(uuid, NULL, "DB_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "DB_ActivityList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_MapList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_PermissionProjectList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_SimpleReportList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_SystemGroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_SystemUserList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_VersionList", this);
        obj->addObject(list);
#ifdef DB_TEST
        list = new RB_ObjectContainer(uuid, obj, "DB_TestList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_TestRelationList", this);
        obj->addObject(list);
#endif
    } else if (str == "DB_ActivityList") {
        // only place holder for database creation and DB_Project as parent
        // the parent DB_Calendar will be made in the view only
        obj = new DB_Activity(uuid, parent, "DB_Activity", this);
    } else if (str == "DB_MapList") {
        obj = new DB_Map(uuid, parent, "DB_Map", this);
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "DB_MappingList", this);
        obj->addObject(list);
    } else if (str == "DB_MappingList") {
        obj = new DB_Mapping(uuid, parent, "DB_Mapping", this);
    } else if (str == "DB_PermissionGroupList") {
        obj = new DB_PermissionGroup(uuid, parent, "DB_PermissionGroup", this);
    } else if (str == "DB_PermissionPerspectiveProjectList") {
        // only for in memory model, not part of object hierachy
        obj = new DB_PermissionPerspectiveProject(uuid, parent, "DB_PermissionPerspectiveProject", this);
    } else if (str == "DB_PermissionProjectList") {
        obj = new DB_PermissionProject(uuid, parent, "DB_PermissionProject", this);
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "DB_PermissionGroupList", this);
        obj->addObject(list);
    } else if (str == "DB_SimpleReportList") {
        obj = new DB_SimpleReport(uuid, parent, "DB_SimpleReport", this);
    } else if (str == "DB_SystemGroupList") {
        obj = new DB_SystemGroup(uuid, parent, "DB_SystemGroup", this);
    } else if (str == "DB_SystemUserGroupList") {
        obj = new DB_SystemUserGroup(uuid, parent, "DB_SystemUserGroup", this);
    } else if (str == "DB_SystemUserList") {
        obj = new DB_SystemUser(uuid, parent, "DB_SystemUser", this);
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "DB_SystemUserGroupList", this);
        obj->addObject(list);
    } else if (str == "DB_SystemUserPermissionList") {
        // only for in memory model, not part of object hierachy
        obj = new DB_SystemUserPermission(uuid, parent, "DB_SystemUserPermission", this);
    } else if (str == "DB_VersionList") {
        // Only place holder for database creation and DB_Project as parent
        // The actual parent is NULL and will be made in the view only
        obj = new DB_Version(uuid, parent, "DB_Version", this);
    }
#ifdef DB_TEST
    else if (str == "DB_TestList") {
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
#endif
    else {
        RB_DEBUG->error("DB_ObjectFactory::newObject() " + parent->getName() + " ERROR");
    }


    // Add object to parent and set object parent 2009-08-16
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}

/**
 * @returns objectfactory base on perspective name
 */
RB_ObjectFactory* DB_ObjectFactory::getFactory(const RB_String& perspective) {
    RB_ObjectFactory* f = NULL;

    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !f) {
        RB_String name = (*iter)->getName();
        RB_StringList strL = name.split("_");
        if(perspective.toLower() == strL.at(0).toLower()) {
            f = *iter;
        }
        ++iter;
    }

    return f;
}

/**
 * @return factory list
 */
std::vector<RB_ObjectFactory*> DB_ObjectFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * Register factory
 * @param f Objectfactory
 */
void DB_ObjectFactory::registerFactory(RB_ObjectFactory* f) {
    bool exists = false;

    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_ObjectFactory::registerFactory() "
                            "objectfactory already registered ERROR");
        }
        ++iter;
    }

    if (!exists) {
        mFactoryList.push_back(f);
    }
}

/**
 * Unregister factory
 * @param f Objectfactory
 */
void DB_ObjectFactory::unregisterFactory(RB_ObjectFactory* f) {
    bool exists = false;

    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (!exists && iter != mFactoryList.end()) {
        if (f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        } else {
            ++iter;
        }
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_ObjectFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}
