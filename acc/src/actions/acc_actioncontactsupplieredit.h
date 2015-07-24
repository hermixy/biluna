/*****************************************************************
 * $Id: acc_actioncontactsupplieredit.h 2153 2014-07-25 16:38:28Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCONTACTSUPPLIEREDIT_H
#define ACC_ACTIONCONTACTSUPPLIEREDIT_H

#include "rb_action.h"


/**
 * Action opens Contact edit supplier contact MDI window
 */
class ACC_ActionContactSupplierEdit: public RB_Action {

    Q_OBJECT

public:
    ACC_ActionContactSupplierEdit();
    virtual ~ACC_ActionContactSupplierEdit() {}

    static RB_String getName() {
        return "Supplier Contact";
    }

    virtual RB_String name() {
        return ACC_ActionContactSupplierEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCONTACTSUPPLIEREDIT_H
