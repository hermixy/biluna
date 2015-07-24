/*****************************************************************
 * $Id: srm_actionleadsourcetype.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 3, 2012 15:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONLEADSOURCETYPE_H
#define SRM_ACTIONLEADSOURCETYPE_H

#include "rb_action.h"


/**
 * Action to edit lead source type from database
 */
class SRM_ActionLeadSourceType : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionLeadSourceType();
    virtual ~SRM_ActionLeadSourceType() { }

    static RB_String getName() {
        return "Edit lead source types";
    }

    virtual RB_String name() {
        return SRM_ActionLeadSourceType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONLEADSOURCETYPE_H
