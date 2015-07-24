/*****************************************************************
 * $Id: acc_actionprice.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPRICE_H
#define ACC_ACTIONPRICE_H

#include "rb_action.h"


/**
 * Action opens item price edit dialog
 */
class ACC_ActionPrice : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPrice();
    virtual ~ACC_ActionPrice() {}

    static RB_String getName() {
        return "Price";
    }

    virtual RB_String name() {
        return ACC_ActionPrice::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPRICE_H
