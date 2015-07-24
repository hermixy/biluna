/*****************************************************************
 * $Id: crm_actionleadsourcetype.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 3, 2012 15:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONLEADSOURCETYPE_H
#define CRM_ACTIONLEADSOURCETYPE_H

#include "rb_action.h"


/**
 * Action to edit lead source type from database
 */
class CRM_ActionLeadSourceType : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionLeadSourceType();
    virtual ~CRM_ActionLeadSourceType() { }

    static RB_String getName() {
        return "Edit lead source types";
    }

    virtual RB_String name() {
        return CRM_ActionLeadSourceType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONLEADSOURCETYPE_H
