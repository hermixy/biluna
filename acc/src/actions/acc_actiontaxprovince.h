/*****************************************************************
 * $Id: acc_actiontaxprovince.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 18, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONTAXPROVINCE_H
#define ACC_ACTIONTAXPROVINCE_H

#include "rb_action.h"


/**
 * Action opens tax provinces edit dialog
 */
class ACC_ActionTaxProvince : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionTaxProvince();
    virtual ~ACC_ActionTaxProvince() {}

    static RB_String getName() {
        return "Tax provinces";
    }

    virtual RB_String name() {
        return ACC_ActionTaxProvince::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONTAXPROVINCE_H
