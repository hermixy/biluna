/*****************************************************************
 * $Id: scan_actioneditscan.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONEDITSCAN_H
#define SCAN_ACTIONEDITSCAN_H

#include "rb_action.h"


/**
 * Action opens scan edit dialog
 */
class SCAN_ActionEditScan : public RB_Action {

    Q_OBJECT

public:
    SCAN_ActionEditScan();
    virtual ~SCAN_ActionEditScan() {}

    static RB_String getName() {
        return "Edit Scan";
    }

    virtual RB_String name() {
        return SCAN_ActionEditScan::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SCAN_ACTIONEDITSCAN_H
