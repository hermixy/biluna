/*****************************************************************
 * $Id: acc_actioncurrency.h 1091 2010-02-10 09:46:08Z rutger $
 * Created: Aug 29, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONGLTRANSACTION_H
#define ACC_ACTIONGLTRANSACTION_H

#include "rb_action.h"


/**
 * Action opens General Ledger edit MDI window
 */
class ACC_ActionGlTransaction: public RB_Action {

    Q_OBJECT

public:
    ACC_ActionGlTransaction();
    virtual ~ACC_ActionGlTransaction() {}

    static RB_String getName() {
        return "General Ledger Transactions";
    }

    virtual RB_String name() {
        return ACC_ActionGlTransaction::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONGLTRANSACTION_H
