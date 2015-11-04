/*****************************************************************
 * $Id: srm_objectfactory.cpp 2101 2014-02-19 11:55:29Z rutger $
 * Created: Feb 12, 2012 15:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_objectfactory.h"

#include "acc_contact.h"
#include "acc_project.h"
#include "acc_purchdata.h"
#include "acc_supplier.h"
#include "db_project.h"
#include "srm_activity.h"
#include "srm_campaign.h"
#include "srm_campaigntarget.h"
#include "srm_campaigntype.h"
#include "srm_contactdetail.h"
#include "srm_document.h"
#include "srm_group.h"
#include "srm_groupcontact.h"
#include "srm_leadsourcetype.h"
#include "srm_supplierdetail.h"
#include "srm_sysseqno.h"
//#include "srm_syssetting.h"
#include "srm_template.h"
#include "db_objectfactory.h"
#include "rb_debug.h"
#include "rb_uuid.h"

SRM_ObjectFactory* SRM_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
SRM_ObjectFactory::SRM_ObjectFactory() {
    RB_DEBUG->print("SRM_ObjectFactory::SRM_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SRM_ObjectFactory::~SRM_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("SRM_ObjectFactory::~SRM_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
SRM_ObjectFactory* SRM_ObjectFactory::getInstance() {
    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new SRM_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("SRM_Project");
        mActiveObjectFactory->setName("SRM_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object, connect to- and make part of parent.
 * NOTE:
 *  SRM_CampaignType
 *  SRM_LeadSourceType
 *  SRM_SysSeqNo
 * are part of DB_ProjectList because they are global system settings.
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
RB_ObjectBase* SRM_ObjectFactory::newObject(const RB_String& id,
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
        list = new RB_ObjectContainer(uuid, obj, "ACC_SupplierList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "DB_ProjectList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_ActivityList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_CampaignList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_DocumentList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_GroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_TemplateList", this);
        obj->addObject(list);


    } else if (str == "DB_ProjectList") {
        obj = new DB_Project(uuid, parent, "DB_Project", this);

        // system setting objects, usually get the XXX_Project.id as parentId
        // because they are perspective globals
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SRM_CampaignTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_LeadSourceTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_SysSeqNoList", this);
        obj->addObject(list);
    } else if (str == "ACC_ContactList") { // from ACC Accounting
        obj = new ACC_Contact(uuid, parent, "ACC_Contact", this);
        // in ACC also children lists

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SRM_ContactDetailList", this);
        obj->addObject(list);
    } else if (str == "ACC_PurchDataList") { // from ACC Accounting
        obj = new ACC_PurchData(uuid, parent, "ACC_PurchData", this);
    } else if (str == "ACC_SupplierList") { // from ACC Accounting
        obj = new ACC_Supplier(uuid, parent, "ACC_Supplier", this);
        // in ACC also children lists

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_ContactList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_PurchDataList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "SRM_SupplierDetailList", this);
        obj->addObject(list);
    } else if (str == "SRM_ActivityList") {
        obj = new SRM_Activity(uuid, parent, "SRM_Activity", this);
    } else if (str == "SRM_CampaignList") {
        obj = new SRM_Campaign(uuid, parent, "SRM_Campaign", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SRM_CampaignTargetList", this);
        obj->addObject(list);
    } else if (str == "SRM_CampaignTargetList") {
        obj = new SRM_CampaignTarget(uuid, parent, "SRM_CampaignTarget", this);
    } else if (str == "SRM_CampaignTypeList") {
        obj = new SRM_CampaignType(uuid, parent, "SRM_CampaignType", this);
    } else if (str == "SRM_ContactDetailList") {
        obj = new SRM_ContactDetail(uuid, parent, "SRM_ContactDetail", this);
    } else if (str == "SRM_DocumentList") {
        obj = new SRM_Document(uuid, parent, "SRM_Document", this);
    } else if (str == "SRM_GroupContactList") {
        obj = new SRM_GroupContact(uuid, parent, "SRM_GroupContact", this);
    } else if (str == "SRM_GroupList") {
        obj = new SRM_Group(uuid, parent, "SRM_Group", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "SRM_GroupContactList", this);
        obj->addObject(list);
    } else if (str == "SRM_LeadSourceTypeList") {
        obj = new SRM_LeadSourceType(uuid, parent, "SRM_LeadSourceType", this);
    } else if (str == "SRM_SupplierDetailList") {
        obj = new SRM_SupplierDetail(uuid, parent, "SRM_SupplierDetail", this);
    } else if (str == "SRM_SysSeqNoList") {
        obj = new SRM_SysSeqNo(uuid, parent, "SRM_SysSeqNo", this);
    } else if (str == "SRM_TemplateList") {
        obj = new SRM_Template(uuid, parent, "SRM_Template", this);
    } else {
        RB_DEBUG->error("SRM_ObjectFactory::newObject() " + parent->getName() + " ERROR");
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
bool SRM_ObjectFactory::isSystemSetting(const RB_String& name) {
    if (name == "SRM_CampaignTypeList" || name == "SRM_CampaignType") {
        return true;
    } else if (name == "SRM_LeadSourceTypeList" || name == "SRM_LeadSourceType") {
        return true;
    } else if (name == "SRM_SysSeqNoList" || name == "SRM_SysSeqNo") {
        return true;
    }

    return false;
}

/**
 * Is field or object name a project setting
 * @param name field or object name
 * @returns true if name is part of the project settings
 */
bool SRM_ObjectFactory::isProjectSetting(const RB_String& name) {
    //    if (name == "SRM_ChartMasterList" || name == "SRM_ChartMaster") {
    //        return true;
    //    }
    return false;
}
