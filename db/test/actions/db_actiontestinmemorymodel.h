/*****************************************************************
 * $Id: db_actiontestinmemorymodel.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTINMEMORYMODEL_H
#define DB_ACTIONTESTINMEMORYMODEL_H

#include "rb_action.h"


/**
 * Test class for in-memory table model.
 */
class DB_ActionTestInMemoryModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestInMemoryModel(/*DB_TestTableWidget* tw*/);
    virtual ~DB_ActionTestInMemoryModel() {}

    static RB_String getName() {
        return "Test In-memory Model";
    }

    virtual RB_String name() {
        return DB_ActionTestInMemoryModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
