/*****************************************************************
 * $Id: db_actiontestdbtabletree.h 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTDBTABLETREE_H
#define DB_ACTIONTESTDBTABLETREE_H

#include "rb_action.h"


/**
 * Test action class for database table and child tree model.
 */
class DB_ActionTestDbTableTree : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestDbTableTree();
    virtual ~DB_ActionTestDbTableTree() {}

    static RB_String getName() {
        return "Test DB Table Treechild Model";
    }

    virtual RB_String name() {
        return DB_ActionTestDbTableTree::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
