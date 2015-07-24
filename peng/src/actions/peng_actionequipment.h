/*****************************************************************
 * $Id: peng_actionequipment.h 1366 2011-02-16 21:09:00Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONEQUIPMENT_H
#define PENG_ACTIONEQUIPMENT_H

#include "rb_action.h"


/**
 * Action opens equipment edit window
 */
class PENG_ActionEquipment : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionEquipment();
    virtual ~PENG_ActionEquipment() {}

    static RB_String getName() {
        return "Equipment";
    }

    virtual RB_String name() {
        return PENG_ActionEquipment::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONEQUIPMENT_H
