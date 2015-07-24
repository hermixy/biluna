/*****************************************************************
 * $Id: acc_actiondebtortype.h 1099 2010-02-17 20:53:59Z rutger $
 * Created: Feb 17, 2010 11:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONDEBTORTYPE_H
#define ACC_ACTIONDEBTORTYPE_H

#include "rb_action.h"


/**
 * Action opens customer(or debtor) type edit dialog
 */
class ACC_ActionDebtorType : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionDebtorType();
    virtual ~ACC_ActionDebtorType() {}

    static RB_String getName() {
        return "Debtor type";
    }

    virtual RB_String name() {
        return ACC_ActionDebtorType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONDEBTORTYPE_H
