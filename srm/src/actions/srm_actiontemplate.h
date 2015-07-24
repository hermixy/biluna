/*****************************************************************
 * $Id: srm_actiontemplate.h 2069 2014-01-21 21:54:24Z rutger $
 * Created: Mar 6, 2012 11:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONTEMPLATE_H
#define SRM_ACTIONTEMPLATE_H

#include "rb_action.h"


/**
 * Action opens template edit window
 */
class SRM_ActionTemplate : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionTemplate();
    virtual ~SRM_ActionTemplate() {}

    static RB_String getName() {
        return "Template";
    }

    virtual RB_String name() {
        return SRM_ActionTemplate::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONTEMPLATE_H
