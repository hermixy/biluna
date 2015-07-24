/*****************************************************************
 * $Id: crm_actioncampaigntarget.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCAMPAIGNTARGET_H
#define CRM_ACTIONCAMPAIGNTARGET_H

#include "rb_action.h"


/**
 * Action opens campaign target edit window
 */
class CRM_ActionCampaignTarget : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionCampaignTarget();
    virtual ~CRM_ActionCampaignTarget() {}

    static RB_String getName() {
        return "Campaign Target";
    }

    virtual RB_String name() {
        return CRM_ActionCampaignTarget::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONCAMPAIGNTARGET_H
