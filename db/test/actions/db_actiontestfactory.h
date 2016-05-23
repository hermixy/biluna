/*****************************************************************
 * $Id: db_actiontestfactory.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTFACTORY_H
#define DB_ACTIONTESTFACTORY_H

#include "rb_action.h"
#include "db_testtextwidget.h"


/**
 * Test action class for object factory.
 */
class DB_ActionTestFactory : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestFactory(DB_TestTextWidget* tw);
    virtual ~DB_ActionTestFactory() {}

    static RB_String getName() {
        return "Test Object Factory";
    }

    virtual RB_String name() {
        return DB_ActionTestFactory::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

private:
    void writeModel(RB_String& str, RB_ObjectBase* obj, bool writeMembers);

    DB_TestTextWidget* mTextWidget;

};

#endif
