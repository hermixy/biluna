/*****************************************************************
 * $Id: acc_actionsalesglposting.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Apr 05, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESGLPOSTING_H
#define ACC_ACTIONSALESGLPOSTING_H

#include "rb_action.h"


/**
 * Action opens sales general ledger postings edit dialog
 */
class ACC_ActionSalesGlPosting : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesGlPosting();
    virtual ~ACC_ActionSalesGlPosting() {}

    static RB_String getName() {
        return "Sales GL Postings";
    }

    virtual RB_String name() {
        return ACC_ActionSalesGlPosting::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESGLPOSTING_H
