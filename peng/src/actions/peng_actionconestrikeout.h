/*****************************************************************
 * $Id: peng_actionconestrikeout.h 1448 2011-08-24 19:16:28Z rutger $
 * Created: Aug 10, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONCONESTRIKEOUT_H
#define PENG_ACTIONCONESTRIKEOUT_H

#include "rb_action.h"


/**
 * Action opens cone strikeout design window
 */
class PENG_ActionConeStrikeOut : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionConeStrikeOut();
    virtual ~PENG_ActionConeStrikeOut() {}

    static RB_String getName() {
        return "Cone Strikeout";
    }

    virtual RB_String name() {
        return PENG_ActionConeStrikeOut::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONCONESTRIKEOUT_H
