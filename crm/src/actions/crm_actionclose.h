/*****************************************************************
 * $Id: crm_actionclose.h 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 8, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCLOSE_H
#define CRM_ACTIONCLOSE_H

#include "rb_action.h"


/**
 * Action closes the perspective
 */
class CRM_ActionClose : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionClose();
    virtual ~CRM_ActionClose() {}

    static RB_String getName() {
        return "Close perspective";
    }

    virtual RB_String name() {
        return CRM_ActionClose::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONCLOSE_H
