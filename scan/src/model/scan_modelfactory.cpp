/*****************************************************************
 * $Id: scan_modelfactory.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_modelfactory.h"

#include "db_modelfactory.h"
#include "rb_database.h"
#include "scan_objectfactory.h"


SCAN_ModelFactory* SCAN_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
SCAN_ModelFactory::SCAN_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("SCAN_ModelFactory::SCAN_ModelFactory()");
    this->setObjectFactory(SCAN_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SCAN_ModelFactory::~SCAN_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SCAN_ModelFactory::~SCAN_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
SCAN_ModelFactory* SCAN_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SCAN_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return model manager
 */
RB_MmProxy* SCAN_ModelFactory::getModel(int type, bool shared) {
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
    case ModelAnswer:
        model = getTableModel(db, mObjectFactory, type, "SCAN_AnswerList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelAnswerResult:
        model = getTableModel(db, mObjectFactory, type, "SCAN_AnswerResultList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelBlock:
        model = getTreeModel(db, mObjectFactory, type, "SCAN_BlockList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelProject:
        // false because project is not parent on this dialog and is not shared with others
        model = getTableModel(db, mObjectFactory, type, "SCAN_ProjectList", false);
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelProjectEdit:
        // false because project is not parent on this dialog and is not shared with others
        model = getTableModel(db, mObjectFactory, type, "SCAN_ProjectList", false);
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelQuestion:
        model = getTableModel(db, mObjectFactory, type, "SCAN_QuestionList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelQuestionResult:
        model = getTableModel(db, mObjectFactory, type, "SCAN_QuestionResultList");
        model->setSourceSortOrder(RB2::SortOrderAscending, "number");
        break;
    case ModelScan:
        model = getTableModel(db, mObjectFactory, type, "SCAN_ScanList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelScanResult:
        model = getTableModel(db, mObjectFactory, type, "SCAN_ScanResultList");
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SCAN_ModelFactory::getModel()"
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
RB_MmProxy* SCAN_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelAnswer:
        iter = mModelList.find(ModelQuestion);
        break;
    case ModelAnswerResult:
        iter = mModelList.find(ModelQuestionResult);
        break;
    case ModelBlock:
        iter = mModelList.find(ModelScan);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProjectEdit:
        iter = mModelList.find(ModelNone);
        break;
    case ModelQuestion:
        iter = mModelList.find(ModelScan);
        break;
    case ModelQuestionResult:
        iter = mModelList.find(ModelScanResult);
        break;
    case ModelScan:
        iter = mModelList.find(ModelNone);
        break;
    case ModelScanResult:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "SCAN_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

