/*****************************************************************
 * $Id: acc_actionpurchaseglposting.h 1889 2013-01-31 17:41:21Z rutger $
 * Created: Jan 28, 2013 17:49:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPURCHASEGLPOSTING_H
#define ACC_ACTIONPURCHASEGLPOSTING_H

#include "rb_action.h"


/**
 * Action opens purchase general ledger postings edit dialog
 */
class ACC_ActionPurchaseGlPosting : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPurchaseGlPosting();
    virtual ~ACC_ActionPurchaseGlPosting() {}

    static RB_String getName() {
        return "Purchase GL Postings";
    }

    virtual RB_String name() {
        return ACC_ActionPurchaseGlPosting::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPURCHASEGLPOSTING_H
