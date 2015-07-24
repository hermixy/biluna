/*****************************************************************
 * $Id: acc_actiontaxauthrate.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 22, 2010 1:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONTAXAUTHRATE_H
#define ACC_ACTIONTAXAUTHRATE_H

#include "rb_action.h"


/**
 * Action opens tax authorities and ratesedit dialog
 */
class ACC_ActionTaxAuthRate : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionTaxAuthRate();
    virtual ~ACC_ActionTaxAuthRate() {}

    static RB_String getName() {
        return "Tax Authorities and Rates";
    }

    virtual RB_String name() {
        return ACC_ActionTaxAuthRate::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONTAXAUTHRATE_H
