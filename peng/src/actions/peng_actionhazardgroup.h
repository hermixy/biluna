/*****************************************************************
 * $Id: peng_actionfluid.h 2146 2014-05-23 20:27:06Z rutger $
 * Created: June 16, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONHAZARDGROUP_H
#define PENG_ACTIONHAZARDGROUP_H

#include "rb_action.h"


/**
 * Action opens hazard group edit dialog
 */
class PENG_ActionHazardGroup : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionHazardGroup();
    virtual ~PENG_ActionHazardGroup() {}

    static RB_String getName() {
        return "Hazard Group";
    }

    virtual RB_String name() {
        return PENG_ActionHazardGroup::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONHAZARDGROUP_H
