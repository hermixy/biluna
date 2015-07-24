/*****************************************************************
 * $Id: acc_actioncustbranch.h 1091 2010-02-10 09:46:08Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCUSTBRANCH_H
#define ACC_ACTIONCUSTBRANCH_H

#include "rb_action.h"


/**
 * Action opens customer branch edit window
 */
class ACC_ActionCustBranch : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCustBranch();
    virtual ~ACC_ActionCustBranch() {}

    static RB_String getName() {
        return "Customer Branch";
    }

    virtual RB_String name() {
        return ACC_ActionCustBranch::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCUSTBRANCH_H
