/*****************************************************************
 * $Id: scan_actionresult.h 1402 2011-04-28 16:34:46Z rutger $
 * Created: Apr 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONRESULT_H
#define SCAN_ACTIONRESULT_H

#include "rb_action.h"


/**
 * Action opens the scan result processing window
 */
class SCAN_ActionResult : public RB_Action {

    Q_OBJECT

public:
    SCAN_ActionResult();
    virtual ~SCAN_ActionResult() {}

    static RB_String getName() {
        return "SCAN Result";
    }

    virtual RB_String name() {
        return SCAN_ActionResult::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SCAN_ACTIONRESULT_H
