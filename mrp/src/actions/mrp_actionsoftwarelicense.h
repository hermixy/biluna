/*****************************************************************
 * $Id: sail_actionmapnavigation.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_ACTIONSOFTWARELICENSE_H
#define MRP_ACTIONSOFTWARELICENSE_H

#include "rb_action.h"


/**
 * Action opens map navigation window
 */
class MRP_ActionSoftwareLicense : public RB_Action {

    Q_OBJECT

public:
    MRP_ActionSoftwareLicense();
    virtual ~MRP_ActionSoftwareLicense() {}

    static RB_String getName() {
        return "Map Navigation";
    }

    virtual RB_String name() {
        return MRP_ActionSoftwareLicense::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // MRP_ACTIONSOFTWARELICENSE_H
