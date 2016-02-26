/*****************************************************************
 * $Id: sail_modelfactory.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_modelfactory.h"

#include "db_modelfactory.h"
#include "mrp_objectfactory.h"
#include "rb_database.h"

MRP_ModelFactory* MRP_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
MRP_ModelFactory::MRP_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("MRP_ModelFactory::MRP_ModelFactory()");
    this->setObjectFactory(MRP_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
MRP_ModelFactory::~MRP_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("MRP_ModelFactory::~MRP_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
MRP_ModelFactory* MRP_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new MRP_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as unique/single/shared model
 * @return model manager
 */
RB_MmProxy* MRP_ModelFactory::getModel(int type, bool shared) {
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

    QSqlDatabase db = getDatabase();

    switch (type) {
    case ModelContact:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelCustomer:
        model = getTableModel(db, mObjectFactory, type, "ACC_CustomerList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "customerno");
        break;
    case ModelProject:
        // ACC_ProjectList actually does not exist but required to create model
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelSalesOrder:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesOrderList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "orderno");
        break;
    case ModelSoftwareLicense:
        model = getTableModel(db, mObjectFactory, type, "MRP_SoftwareLicenseList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "startdate");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "MRP_ModelFactory::getModel()"
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
RB_MmProxy* MRP_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelContact:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCustomer:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSalesOrder:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSoftwareLicense:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "MRP_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

