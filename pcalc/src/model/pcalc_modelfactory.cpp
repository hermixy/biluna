/*****************************************************************
 * $Id: pcalc_modelfactory.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Apr 15, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_modelfactory.h"

#include "db_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_sqlcommonfunctions.h"
#include "rb_database.h"

PCALC_ModelFactory* PCALC_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
PCALC_ModelFactory::PCALC_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("PCALC_ModelFactory::PCALC_ModelFactory()");
    this->setObjectFactory(PCALC_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_ModelFactory::~PCALC_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PCALC_ModelFactory::~PCALC_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
PCALC_ModelFactory* PCALC_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new PCALC_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return model manager
 */
RB_MmProxy* PCALC_ModelFactory::getModel(int type, bool shared) {
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
    case ModelEN13555EGeG:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555EGeGList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "qa", "temp");
        break;
    case ModelEN13555Gasket:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555GasketList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "type");
        break;
    case ModelEN13555Manuf:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555ManufacturerList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "manufacturer");
        break;
    case ModelEN13555PQRdeltaeGC:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555PqrDeltaeGCList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "qa", "temp");
        break;
    case ModelEN13555QminL:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555QminLList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "testpress", "leakrate");
        break;
    case ModelEN13555QsminL:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN13555QsminLList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "testpress", "qa", "leakrate");
        break;
    case ModelEN1591Assembly:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_AssemblyList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591BoltNutWasher:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_BoltNutWasherList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Flange:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_FlangeList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Gasket:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_GasketList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591LoadCase:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_LoadCaseList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelEN1591Shell:
        model = getTableModel(db, mObjectFactory, type, "PCALC_EN1591_ShellList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelProject:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelProjectEdit:
        model = getTableModel(db, mObjectFactory, type, "PENG_ProjectList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_ModelFactory::getModel()"
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
RB_MmProxy* PCALC_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelEN13555EGeG:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555Gasket:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555Manuf:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555PQRdeltaeGC:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555QminL:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN13555QsminL:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN1591Assembly:
        iter = mModelList.find(ModelNone);
        break;
    case ModelEN1591BoltNutWasher:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Flange:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Gasket:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591LoadCase:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelEN1591Shell:
        iter = mModelList.find(ModelEN1591Assembly);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

/**
 * Add default rows, for example equipment types
 * \todo Base the default rows on SQL data file, not hardcoded.
 * @returns true on success
 */
bool PCALC_ModelFactory::addDefaultRows() {
//    PCALC_SqlCommonFunctions f;

    bool success = true;

    return success;
}

