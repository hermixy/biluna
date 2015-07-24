/*****************************************************************
 * $Id: acc_actioncurrency.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCURRENCY_H
#define ACC_ACTIONCURRENCY_H

#include "rb_action.h"


/**
 * Action opens currency edit dialog
 */
class ACC_ActionCurrency : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCurrency();
    virtual ~ACC_ActionCurrency() {}

    static RB_String getName() {
        return "Currency";
    }

    virtual RB_String name() {
        return ACC_ActionCurrency::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCURRENCY_H
