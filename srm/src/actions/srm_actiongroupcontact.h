/*****************************************************************
 * $Id: srm_actiongroupcontact.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONGROUPCONTACT_H
#define SRM_ACTIONGROUPCONTACT_H

#include "rb_action.h"


/**
 * Action opens group contact edit window
 */
class SRM_ActionGroupContact : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionGroupContact();
    virtual ~SRM_ActionGroupContact() {}

    static RB_String getName() {
        return "Group Contact";
    }

    virtual RB_String name() {
        return SRM_ActionGroupContact::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONGROUPCONTACT_H
