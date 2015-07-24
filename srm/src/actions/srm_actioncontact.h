/*****************************************************************
 * $Id: srm_actioncontact.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONCONTACT_H
#define SRM_ACTIONCONTACT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Edit/view contact
 */
class SRM_ActionContact : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionContact();
    virtual ~SRM_ActionContact() { }

    static RB_String getName() {
        return "Edit/view contact";
    }

    virtual RB_String name() {
        return SRM_ActionContact::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONCONTACT_H
