/*****************************************************************
 * $Id: db_actiontestobject.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTOBJECT_H
#define DB_ACTIONTESTOBJECT_H

#include "rb_action.h"
#include "db_testtextwidget.h"


/**
 * Test action class for data object of Biluna.
 */
class DB_ActionTestObject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestObject();
    virtual ~DB_ActionTestObject() {}

    static RB_String getName() {
        return "Test object";
    }

    virtual RB_String name() {
        return DB_ActionTestObject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

private:
    void writeModel(RB_String& str, RB_ObjectBase* obj, bool writeMembers);
    void writeModelFunctionTest(DB_TestTextWidget* tw);

};

#endif
