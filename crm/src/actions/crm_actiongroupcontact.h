/*****************************************************************
 * $Id: crm_actiongroupcontact.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONGROUPCONTACT_H
#define CRM_ACTIONGROUPCONTACT_H

#include "rb_action.h"


/**
 * Action opens group contact edit window
 */
class CRM_ActionGroupContact : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionGroupContact();
    virtual ~CRM_ActionGroupContact() {}

    static RB_String getName() {
        return "Group Contact";
    }

    virtual RB_String name() {
        return CRM_ActionGroupContact::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONGROUPCONTACT_H
