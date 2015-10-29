/*****************************************************************
 * $Id: mrp_objectfactory.cpp 1727 2012-07-30 22:01:50Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_objectfactory.h"

#include "acc_contact.h"
#include "acc_customer.h"
#include "acc_project.h"
#include "acc_salesorder.h"
#include "db_objectfactory.h"
#include "mrp_softwarelicense.h"
#include "rb_debug.h"
#include "rb_uuid.h"

MRP_ObjectFactory* MRP_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
MRP_ObjectFactory::MRP_ObjectFactory() {
    RB_DEBUG->print("MRP_ObjectFactory::MRP_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
MRP_ObjectFactory::~MRP_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("MRP_ObjectFactory::~MRP_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
MRP_ObjectFactory* MRP_ObjectFactory::getInstance() {
    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new MRP_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("ACC_Project");
        mActiveObjectFactory->setName("MRP_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object, connect to- and make part of parent.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param name name of the parent list (container)
 * @param addToParent add new object to parent container
 * @return object, connected to parent
 */
RB_ObjectBase* MRP_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "ACC_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "ACC_") { // root
        obj = new ACC_Project(uuid, NULL, "ACC_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "ACC_CustomerList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "MRP_SoftwareLicenseList", this);
        obj->addObject(list);
    } else if (str == "ACC_ContactList") {
        obj = new ACC_Contact(uuid, parent, "ACC_Contact", this);
    } else if (str == "ACC_CustomerList") {
        obj = new ACC_Customer(uuid, parent, "ACC_Customer", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_ContactList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesOrderList", this); // TODO: view only? How about Purchase Order?
        obj->addObject(list);
    } else if (str == "ACC_SalesOrderList") {
        obj = new ACC_SalesOrder(uuid, parent, "ACC_SalesOrder", this);
    } else if (str == "MRP_SoftwareLicenseList") {
        obj = new MRP_SoftwareLicense(uuid, parent, "MRP_SoftwareLicense", this);
    } else {
        RB_DEBUG->error("MRP_ObjectFactory::newObject() " + parent->getName() + " ERROR");
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
bool MRP_ObjectFactory::isSystemSetting(const RB_String& /* name */) {
    return false;
}

/**
 * Is field or object name a project setting
 * @param name field or object name
 * @returns true if name is part of the project settings
 */
bool MRP_ObjectFactory::isProjectSetting(const RB_String& /* name */) {
    return false;
}
