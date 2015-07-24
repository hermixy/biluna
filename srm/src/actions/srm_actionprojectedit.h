/*****************************************************************
 * $Id: srm_actionprojectedit.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONPROJECTEDIT_H
#define SRM_ACTIONPROJECTEDIT_H

#include "rb_action.h"


/**
 * Action opens project (company) edit dialog
 */
class SRM_ActionProjectEdit : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionProjectEdit();
    virtual ~SRM_ActionProjectEdit() {}

    static RB_String getName() {
        return "Project Edit";
    }

    virtual RB_String name() {
        return SRM_ActionProjectEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONPROJECTEDIT_H
