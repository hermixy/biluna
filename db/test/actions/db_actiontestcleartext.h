/*****************************************************************
 * $Id: db_actiontestcleartext.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTCLEARTEXT_H
#define RB_ACTIONTESTCLEARTEXT_H

#include "rb_action.h"


/**
 * Test action class for clearing the text in the text window.
 */
class DB_ActionTestClearText : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestClearText();
    virtual ~DB_ActionTestClearText() {}

    static RB_String getName() {
        return "Clear Text";
    }

    virtual RB_String name() {
        return DB_ActionTestClearText::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

private:

};

#endif
