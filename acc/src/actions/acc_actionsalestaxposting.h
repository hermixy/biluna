/*****************************************************************
 * $Id: acc_actionsalestaxposting.h 1803 2012-11-22 21:43:16Z rutger $
 * Created: Jan 19, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESTAXPOSTING_H
#define ACC_ACTIONSALESTAXPOSTING_H

#include "rb_action.h"


/**
 * Action opens sales tax postings edit dialog
 */
class ACC_ActionSalesTaxPosting : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesTaxPosting();
    virtual ~ACC_ActionSalesTaxPosting() {}

    static RB_String getName() {
        return "Sales/Purchase Tax Postings";
    }

    virtual RB_String name() {
        return ACC_ActionSalesTaxPosting::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESTAXPOSTING_H
