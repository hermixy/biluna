/*****************************************************************
 * $Id: crm_actionprojectedit.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONPROJECTEDIT_H
#define CRM_ACTIONPROJECTEDIT_H

#include "rb_action.h"


/**
 * Action opens project (company) edit dialog
 */
class CRM_ActionProjectEdit : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionProjectEdit();
    virtual ~CRM_ActionProjectEdit() {}

    static RB_String getName() {
        return "Project Edit";
    }

    virtual RB_String name() {
        return CRM_ActionProjectEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONPROJECTEDIT_H
