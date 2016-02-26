/*****************************************************************
 * $Id: gv_modelfactory.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Apr 17, 2010 11:29 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_modelfactory.h"

#include "gv_drawing.h"
#include "gv_objectfactory.h"
#include "db_modelfactory.h"

GV_ModelFactory* GV_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
GV_ModelFactory::GV_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("GV_ModelFactory::GV_ModelFactory()");
    setObjectFactory(GV_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
GV_ModelFactory::~GV_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("GV_ModelFactory::~GV_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
GV_ModelFactory* GV_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new GV_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as a shared/single model
 * @return model manager
 */
RB_MmProxy* GV_ModelFactory::getModel(int type, bool shared) {
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
    case ModelDrawing:
        model = getTableModel(db, mObjectFactory, type, "GV_DrawingList", shared);
        break;
    case ModelProject:
        // GV_ProjectList does not exist but required to get list of projects
        model = getTableModel(db, mObjectFactory, type, "GV_ProjectList", shared);
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_ModelFactory::getModel()"
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
RB_MmProxy* GV_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelDrawing:
        iter = mModelList.find(ModelNone);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}
