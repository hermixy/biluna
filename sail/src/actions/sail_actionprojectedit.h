/*****************************************************************
 * $Id: sail_actionprojectedit.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONPROJECTEDIT_H
#define SAIL_ACTIONPROJECTEDIT_H

#include "rb_action.h"


/**
 * Action opens project edit dialog
 */
class SAIL_ActionProjectEdit : public RB_Action {

    Q_OBJECT

public:
    SAIL_ActionProjectEdit();
    virtual ~SAIL_ActionProjectEdit() {}

    static RB_String getName() {
        return "Project Edit";
    }

    virtual RB_String name() {
        return SAIL_ActionProjectEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SAIL_ACTIONPROJECTEDIT_H
