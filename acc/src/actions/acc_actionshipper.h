/*****************************************************************
 * $Id: acc_actionshipper.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 20, 2010 10:44:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSHIPPER_H
#define ACC_ACTIONSHIPPER_H

#include "rb_action.h"


/**
 * Action opens shipper edit dialog
 */
class ACC_ActionShipper : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionShipper();
    virtual ~ACC_ActionShipper() {}

    static RB_String getName() {
        return "Currency";
    }

    virtual RB_String name() {
        return ACC_ActionShipper::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSHIPPER_H
