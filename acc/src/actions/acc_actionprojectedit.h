/*****************************************************************
 * $Id: acc_actionprojectedit.h 1120 2010-03-24 22:05:29Z rutger $
 * Created: Mar 24, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPROJECTEDIT_H
#define ACC_ACTIONPROJECTEDIT_H

#include "rb_action.h"


/**
 * Action opens project (company) edit dialog
 */
class ACC_ActionProjectEdit : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionProjectEdit();
    virtual ~ACC_ActionProjectEdit() {}

    static RB_String getName() {
        return "Company Edit";
    }

    virtual RB_String name() {
        return ACC_ActionProjectEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPROJECTEDIT_H
