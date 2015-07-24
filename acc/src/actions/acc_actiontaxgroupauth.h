/*****************************************************************
 * $Id: acc_actiontaxgroupauth.h 1719 2012-07-25 10:26:12Z rutger $
 * Created: Feb 23, 2010 12:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONTAXGROUPAUTH_H
#define ACC_ACTIONTAXGROUPAUTH_H

#include "rb_action.h"


/**
 * Action opens tax group authorities edit dialog
 */
class ACC_ActionTaxGroupAuth : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionTaxGroupAuth();
    virtual ~ACC_ActionTaxGroupAuth() {}

    static RB_String getName() {
        return "Tax Group Authorities";
    }

    virtual RB_String name() {
        return ACC_ActionTaxGroupAuth::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONTAXGROUPAUTH_H
