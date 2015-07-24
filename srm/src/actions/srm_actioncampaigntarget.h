/*****************************************************************
 * $Id: srm_actioncampaigntarget.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONCAMPAIGNTARGET_H
#define SRM_ACTIONCAMPAIGNTARGET_H

#include "rb_action.h"


/**
 * Action opens campaign target edit window
 */
class SRM_ActionCampaignTarget : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionCampaignTarget();
    virtual ~SRM_ActionCampaignTarget() {}

    static RB_String getName() {
        return "Campaign Target";
    }

    virtual RB_String name() {
        return SRM_ActionCampaignTarget::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONCAMPAIGNTARGET_H
