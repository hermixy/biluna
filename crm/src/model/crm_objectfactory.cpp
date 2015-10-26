/*****************************************************************
 * $Id: crm_objectfactory.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 12, 2012 15:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_objectfactory.h"

#include "acc_contact.h"
#include "acc_customer.h"
#include "acc_project.h"
#include "crm_activity.h"
#include "crm_campaign.h"
#include "crm_campaigntarget.h"
#include "crm_campaigntype.h"
#include "crm_contactdetail.h"
#include "crm_customerdetail.h"
#include "crm_document.h"
#include "crm_group.h"
#include "crm_groupcontact.h"
#include "crm_leadsourcetype.h"
#include "crm_sysseqno.h"
#include "crm_syssetting.h"
#include "crm_template.h"
#include "db_objectfactory.h"
#include "rb_debug.h"
#include "rb_uuid.h"

CRM_ObjectFactory* CRM_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
CRM_ObjectFactory::CRM_ObjectFactory() {
    RB_DEBUG->print("CRM_ObjectFactory::CRM_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
CRM_ObjectFactory::~CRM_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("CRM_ObjectFactory::~CRM_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
CRM_ObjectFactory* CRM_ObjectFactory::getInstance() {
    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new CRM_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("CRM_Project");
        mActiveObjectFactory->setName("CRM_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object, connect to- and make part of parent.
 * NOTE:
 *  CRM_CampaignType
 *  CRM_LeadSourceType
 * are part of CRM_SysSetting but are global system settings.
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
RB_ObjectBase* CRM_ObjectFactory::newObject(const RB_String& id,
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
        list = new RB_ObjectContainer(uuid, obj, "CRM_ActivityList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_CampaignList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_DocumentList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_GroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_SysSettingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_TemplateList", this);
        obj->addObject(list);
    } else if (str == "ACC_ContactList") { // from ACC Accounting
        obj = new ACC_Contact(uuid, parent, "ACC_Contact", this);
        // in ACC also children lists

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "CRM_ContactDetailList", this);
        obj->addObject(list);
    } else if (str == "ACC_CustomerList") { // from ACC Accounting
        obj = new ACC_Customer(uuid, parent, "ACC_Customer", this);
        // in ACC also children lists

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_ContactList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_CustomerDetailList", this);
        obj->addObject(list);
    } else if (str == "CRM_ActivityList") {
        obj = new CRM_Activity(uuid, parent, "CRM_Activity", this);
    } else if (str == "CRM_CampaignList") {
        obj = new CRM_Campaign(uuid, parent, "CRM_Campaign", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "CRM_CampaignTargetList", this);
        obj->addObject(list);
    } else if (str == "CRM_CampaignTargetList") {
        obj = new CRM_CampaignTarget(uuid, parent, "CRM_CampaignTarget", this);
    } else if (str == "CRM_CampaignTypeList") {
        obj = new CRM_CampaignType(uuid, parent, "CRM_CampaignType", this);
    } else if (str == "CRM_ContactDetailList") {
        obj = new CRM_ContactDetail(uuid, parent, "CRM_ContactDetail", this);
    } else if (str == "CRM_CustomerDetailList") {
        obj = new CRM_CustomerDetail(uuid, parent, "CRM_CustomerDetail", this);
    } else if (str == "CRM_DocumentList") {
        obj = new CRM_Document(uuid, parent, "CRM_Document", this);
    } else if (str == "CRM_GroupContactList") {
        obj = new CRM_GroupContact(uuid, parent, "CRM_GroupContact", this);
    } else if (str == "CRM_GroupList") {
        obj = new CRM_Group(uuid, parent, "CRM_Group", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "CRM_GroupContactList", this);
        obj->addObject(list);
    } else if (str == "CRM_LeadSourceTypeList") {
        obj = new CRM_LeadSourceType(uuid, parent, "CRM_LeadSourceType", this);
    } else if (str == "CRM_SysSeqNoList") {
        obj = new CRM_SysSeqNo(uuid, parent, "CRM_SysSeqNo", this);
    } else if (str == "CRM_SysSettingList") {
        obj = new CRM_SysSetting(uuid, parent, "CRM_SysSetting", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "CRM_CampaignTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_LeadSourceTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "CRM_SysSeqNoList", this);
        obj->addObject(list);
    } else if (str == "CRM_TemplateList") {
        obj = new CRM_Template(uuid, parent, "CRM_Template", this);
    } else {
        RB_DEBUG->error("CRM_ObjectFactory::newObject() " + parent->getName() + " ERROR");
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
bool CRM_ObjectFactory::isSystemSetting(const RB_String& name) {
    if (name == "CRM_CampaignTypeList" || name == "CRM_CampaignType") {
        return true;
    } else if (name == "CRM_LeadSourceTypeList" || name == "CRM_LeadSourceType") {
        return true;
    }

    return false;
}

/**
 * Is field or object name a project setting
 * @param name field or object name
 * @returns true if name is part of the project settings
 */
bool CRM_ObjectFactory::isProjectSetting(const RB_String& /*name*/) {
    //    if (name == "CRM_ChartMasterList" || name == "CRM_ChartMaster") {
    //        return true;
    //    }
    return false;
}
