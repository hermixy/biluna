/*****************************************************************
 * $Id: db_testmodelfactory.cpp 1771 2012-09-06 21:13:20Z rutger $
 * Created: Nov 12, 2009 11:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testmodelfactory.h"

#include "db_modelfactory.h"
#include "db_testobjectfactory.h"
// #include "rb_debug.h"

DB_TestModelFactory* DB_TestModelFactory::mActiveFactory = NULL;


/**
 * Constructor, register with DB_ModelFactory
 */
DB_TestModelFactory::DB_TestModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("DB_TestModelFactory::DB_TestModelFactory()");
    this->setObjectFactory(DB_TESTOBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
DB_TestModelFactory::~DB_TestModelFactory() {
    // nothing yet
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("DB_TestModelFactory::~DB_TestModelFactory() OK");
}

/**
 * Create instance of factory
 */
DB_TestModelFactory* DB_TestModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new DB_TestModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @return model manager
 */
RB_MmProxy* DB_TestModelFactory::getModel(int type, bool shared) {
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
    case ModelTest:
        model = getTableModel(db, mObjectFactory, type, "DB_TestList", shared);
        break;
    case ModelTestChild:
        model = getTableModel(db, mObjectFactory, type, "DB_TestChildList", shared);
        break;
    case ModelTestDialog:
        model = getTableModel(db, mObjectFactory, type, "DB_TestDlgList", shared);
        break;
    case ModelTestProject:
        model = getTableModel(db, mObjectFactory, type, "DB_TestProjectList", shared);
        break;
    case ModelTestRelation:
        model = getTableModel(db, mObjectFactory, type, "DB_TestRelationList", shared);
        break;
    case ModelTestTree:
        model = getTreeModel(db, mObjectFactory, type, "DB_TestList", shared);
        break;
    case ModelTestTreeChild:
        model = getTableModel(db, mObjectFactory, type, "DB_TestChildList", shared);
        break;
    case ModelTvTrv:
        model = getTableModel(db, mObjectFactory, type, "DB_TestList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "value1");
        break;
    case ModelTvTrvChild:
        model = getTreeModel(db, mObjectFactory, type, "DB_TestChildList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "number");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_TestModelFactory::getModel()"
                        "non-existing model ERROR");
        break;
    }

    return model;
}

/**
 * Get parent model
 * @param type child model type
 * @return parent model or NULL if not exists
 */
RB_MmProxy* DB_TestModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
        case ModelTest:
            // mModelList is a std::map<int, RB_MmObjectInterface*>
            iter = mModelList.find(ModelNone);
            break;
        case ModelTestChild:
            iter = mModelList.find(ModelTest);
            break;
        case ModelTestDialog:
            iter = mModelList.find(ModelTest);
            break;
        case ModelTestProject:
            iter = mModelList.find(ModelNone);
            break;
        case ModelTestRelation:
            iter = mModelList.find(ModelNone);
            break;
        case ModelTestTree:
            iter = mModelList.find(ModelNone);
            break;
        case ModelTestTreeChild:
            iter = mModelList.find(ModelTestTree);
            break;
        case ModelTvTrv:
            iter = mModelList.find(ModelTestTree);
            break;
        case ModelTvTrvChild:
            iter = mModelList.find(ModelTvTrv);
            break;
        default:
            iter = mModelList.find(ModelNone);
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_TestModelFactory::getParentModel()"
                            "non-existing (parent) model ERROR");
            break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }

    return model;
}

