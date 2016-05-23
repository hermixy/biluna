/*****************************************************************
 * $Id: db_actiontestinmemorytreemodel.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTINMEMORYTREEMODEL_H
#define DB_ACTIONTESTINMEMORYTREEMODEL_H

#include "rb_action.h"


/**
 * Test action for in-memory tree model.
 */
class DB_ActionTestInMemoryTreeModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestInMemoryTreeModel(/*DB_TestTreeWidget* tw*/);
    virtual ~DB_ActionTestInMemoryTreeModel() {}

    static RB_String getName() {
        return "Test In-memory Tree Model";
    }

    virtual RB_String name() {
        return DB_ActionTestInMemoryTreeModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
