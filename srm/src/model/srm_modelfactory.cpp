/*****************************************************************
 * $Id: srm_modelfactory.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_modelfactory.h"

#include "srm_objectfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"

SRM_ModelFactory* SRM_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
SRM_ModelFactory::SRM_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("SRM_ModelFactory::SRM_ModelFactory()");
    this->setObjectFactory(SRM_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SRM_ModelFactory::~SRM_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SRM_ModelFactory::~SRM_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
SRM_ModelFactory* SRM_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SRM_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as unique/single/shared model
 * @return model manager
 */
RB_MmProxy* SRM_ModelFactory::getModel(int type, bool shared) {
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
        model = getTableModel(db, mObjectFactory, type, "SRM_ActivityList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "mend");
        model->setDisplayRole(model->fieldIndex("mvalue"), RB2::AlignFinancialAmount);
        break;
    case ModelCampaign:
        model = getTableModel(db, mObjectFactory, type, "SRM_CampaignList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "campaigncode");
        break;
    case ModelCampaignTarget:
        model = getTableModel(db, mObjectFactory, type, "SRM_CampaignTargetList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "group_idx");
        break;
    case ModelCampaignType:
        model = getTableModel(db, mObjectFactory, type, "SRM_CampaignTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelContact:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelContactDetail:
        model = getTableModel(db, mObjectFactory, type, "SRM_ContactDetailList", shared);
        // no sorting necessary, should be one row only as an extension to ACC_Contact
        break;
    case ModelDocument:
        model = getTableModel(db, mObjectFactory, type, "SRM_DocumentList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "refno");
        break;
    case ModelGroup:
        model = getTableModel(db, mObjectFactory, type, "SRM_GroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelGroupContact:
        model = getTableModel(db, mObjectFactory, type, "SRM_GroupContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "contact_idx");
        break;
    case ModelLeadSourceType:
        model = getTableModel(db, mObjectFactory, type, "SRM_LeadSourceTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelProject:
        // SRM_ProjectList actually does not exist but required to create model
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "created");
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "created");
        break;
    case ModelPurchData:
        model = getTableModel(db, mObjectFactory, type, "ACC_PurchDataList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "supplier_partno");
        model->setDisplayRole(10, RB2::AlignFinancialAmount); // price
        break;
    case ModelSupplier:
        model = getTableModel(db, mObjectFactory, type, "ACC_SupplierList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "suppliercode");
        break;
    case ModelSupplierDetail:
        model = getTableModel(db, mObjectFactory, type, "SRM_SupplierDetailList", shared);
        // no sorting necessary, should be one row only as an extension to ACC_Supplier
        break;
    case ModelSysSeqNo:
        model = getTableModel(db, mObjectFactory, type, "SRM_SysSeqNoList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "seqnumbername");
        break;
    case ModelTemplate:
        model = getTableModel(db, mObjectFactory, type, "SRM_TemplateList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "description");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SRM_ModelFactory::getModel()"
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
RB_MmProxy* SRM_ModelFactory::getParentModel(int type) {
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
    case ModelPurchData:
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelSupplier:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSupplierDetail:
        iter = mModelList.find(ModelSupplier);
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
                        "SRM_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

