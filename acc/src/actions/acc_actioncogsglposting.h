/*****************************************************************
 * $Id: acc_actioncogsglposting.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Apr 05, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCOGSGLPOSTING_H
#define ACC_ACTIONCOGSGLPOSTING_H

#include "rb_action.h"


/**
 * Action opens cost of goods sold general ledger posting edit dialog
 */
class ACC_ActionCogsGlPosting : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCogsGlPosting();
    virtual ~ACC_ActionCogsGlPosting() {}

    static RB_String getName() {
        return "COGS GL Postings";
    }

    virtual RB_String name() {
        return ACC_ActionCogsGlPosting::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCOGSGLPOSTING_H
