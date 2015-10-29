/*****************************************************************
 * $Id: crm_modelfactory.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_modelfactory.h"

#include "crm_objectfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"


CRM_ModelFactory* CRM_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
CRM_ModelFactory::CRM_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("CRM_ModelFactory::CRM_ModelFactory()");
    this->setObjectFactory(CRM_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
CRM_ModelFactory::~CRM_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("CRM_ModelFactory::~CRM_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
CRM_ModelFactory* CRM_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new CRM_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as unique/single/shared model
 * @return model manager
 */
RB_MmProxy* CRM_ModelFactory::getModel(int type, bool shared) {
    RB_MmProxy* model = NULL;

    if (shared) {
        std::map<int, RB_MmProxy*>::iterator iter;
        iter = mModelList.find(type);
        if (iter != mModelList.end()) {
            // shared model 1- exists or 2- has been removed/deleted and set to NULL
            model = (*iter).second;
        }
        // Model already created
        if (model) return model;
    }

    QSqlDatabase db = RB_DATABASE->database(mDatabaseConnection);

    switch (type) {
    case ModelActivity:
        model = getTableModel(db, mObjectFactory, type, "CRM_ActivityList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "mend");
        model->setDisplayRole(model->fieldIndex("mvalue"), RB2::AlignFinancialAmount);
        break;
    case ModelCampaign:
        model = getTableModel(db, mObjectFactory, type, "CRM_CampaignList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "campaigncode");
        break;
    case ModelCampaignTarget:
        model = getTableModel(db, mObjectFactory, type, "CRM_CampaignTargetList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "group_idx");
        break;
    case ModelCampaignType:
        model = getTableModel(db, mObjectFactory, type, "CRM_CampaignTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelContact:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelContactDetail:
        model = getTableModel(db, mObjectFactory, type, "CRM_ContactDetailList", shared);
        // no sorting necessary, should be one row only as an extension to ACC_Contact
        break;
    case ModelCustomer:
        model = getTableModel(db, mObjectFactory, type, "ACC_CustomerList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "customerno");
        break;
    case ModelCustomerDetail:
        model = getTableModel(db, mObjectFactory, type, "CRM_CustomerDetailList", shared);
        // no sorting necessary, should be one row only as an extension to ACC_Customer
        break;
    case ModelDocument:
        model = getTableModel(db, mObjectFactory, type, "CRM_DocumentList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "refno");
        break;
    case ModelGroup:
        model = getTableModel(db, mObjectFactory, type, "CRM_GroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelGroupContact:
        model = getTableModel(db, mObjectFactory, type, "CRM_GroupContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "contact_idx");
        break;
    case ModelLeadSourceType:
        model = getTableModel(db, mObjectFactory, type, "CRM_LeadSourceTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelProject:
        // ACC_ProjectList actually does not exist but required to create model
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "created");
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "created");
        break;
    case ModelSysSeqNo:
        model = getTableModel(db, mObjectFactory, type, "CRM_SysSeqNoList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "seqnumbername");
        break;
    case ModelTemplate:
        model = getTableModel(db, mObjectFactory, type, "CRM_TemplateList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "description");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_ModelFactory::getModel()"
                        "non-existing model ERROR");
        break;
    }

    return model;
}

/**
 * Get parent model. Used in child models or models depending on selection of
 * a parent model.
 * @param type child model type
 * @return parent model or NULL if not exists
 */
RB_MmProxy* CRM_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelActivity:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCampaign:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCampaignTarget:
        iter = mModelList.find(ModelCampaign);
        break;
    case ModelCampaignType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelContact:
        iter = mModelList.find(ModelNone);
        break;
    case ModelContactDetail:
        iter = mModelList.find(ModelContact);
        break;
    case ModelCustomer:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCustomerDetail:
        iter = mModelList.find(ModelCustomer);
        break;
    case ModelDocument:
        iter = mModelList.find(ModelNone);
        break;
    case ModelGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelGroupContact:
        iter = mModelList.find(ModelGroup);
        break;
    case ModelLeadSourceType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSysSeqNo:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTemplate:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

