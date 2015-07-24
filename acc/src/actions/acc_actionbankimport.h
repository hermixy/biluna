/*****************************************************************
 * $Id: acc_actionbankimport.h 1803 2012-11-22 21:43:16Z rutger $
 * Created: Oct 15, 2012 18:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONBANKIMPORT_H
#define ACC_ACTIONBANKIMPORT_H

#include "rb_action.h"


/**
 * Action importing bank statements such as MT940 format
 */
class ACC_ActionBankImport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionBankImport();
    virtual ~ACC_ActionBankImport() {}

    static RB_String getName() {
        return "Import bank statements";
    }

    virtual RB_String name() {
        return ACC_ActionBankImport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONBANKIMPORT_H
