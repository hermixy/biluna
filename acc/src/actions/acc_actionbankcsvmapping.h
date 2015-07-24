/*****************************************************************
 * $Id: acc_actionbankcsvmapping.h 1946 2013-07-01 19:33:25Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONBANKCSVMAPPING_H
#define ACC_ACTIONBANKCSVMAPPING_H

#include "rb_action.h"


/**
 * Action to set the bank CSV import settings
 */
class ACC_ActionBankCsvMapping : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionBankCsvMapping();
    virtual ~ACC_ActionBankCsvMapping() {}

    static RB_String getName() {
        return "Bank import CSV mapping";
    }

    virtual RB_String name() {
        return ACC_ActionBankCsvMapping::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONBANKCSVMAPPING_H
