/*****************************************************************
 * $Id: db_actiontest.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTEST_H
#define DB_ACTIONTEST_H

#include "rb_action.h"

/**
 * Action to test the software design for actions as per QCad
 */
class DB_ActionTest : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTest();
    ~DB_ActionTest() {}

    static RB_String getName() {
        return "File Quit";
    }

    virtual RB_String name() {
        return DB_ActionTest::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif // DB_ACTIONTEST_H
