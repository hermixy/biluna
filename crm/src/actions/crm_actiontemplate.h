/*****************************************************************
 * $Id: crm_actiontemplate.h 2067 2014-01-21 20:10:28Z rutger $
 * Created: Mar 6, 2012 11:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONTEMPLATE_H
#define CRM_ACTIONTEMPLATE_H

#include "rb_action.h"


/**
 * Action opens template edit window
 */
class CRM_ActionTemplate : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionTemplate();
    virtual ~CRM_ActionTemplate() {}

    static RB_String getName() {
        return "Template";
    }

    virtual RB_String name() {
        return CRM_ActionTemplate::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONTEMPLATE_H
