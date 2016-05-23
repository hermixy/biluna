/*****************************************************************
 * $Id: db_testmodelfactory.h 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 12, 2009 11:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTMODELFACTORY_H
#define DB_TESTMODELFACTORY_H

#include "rb_modelfactory.h"

#define DB_TESTMODELFACTORY DB_TestModelFactory::getInstance()

/**
 * Test model factory
 */
class DB_TestModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~DB_TestModelFactory();
    static DB_TestModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 101XXX:
     * - 10 = DB,
     * - 1 = model,
     * - XXX = sequence number
     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 101000
        ModelTest = 101000,             /**< Table model */
        ModelTestChild,                 /**< Child table model */
        ModelTestDialog,                /**< Dialog model */
        ModelTestProject,               /**< Test project model */
        ModelTestRelation,              /**< Test relation table model */
        ModelTestTree,                  /**< Tree model */
        ModelTestTreeChild,             /**< Child Tree model */
        ModelTvTrv,                     /**< Tableparent (treechild) model */
        ModelTvTrvChild,                /**< (Tableparent) treechild model */
        ModelNone                       /**< Invalid model */
    };

private:
    DB_TestModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static DB_TestModelFactory* mActiveFactory;

};

#endif // DB_TESTMODELFACTORY_H
