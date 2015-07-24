/*****************************************************************
 * $Id: peng_actionlinedetail.h 2146 2014-05-23 20:27:06Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONLINEDETAIL_H
#define PENG_ACTIONLINEDETAIL_H

#include "rb_action.h"


/**
 * Action opens line designation table detail window
 */
class PENG_ActionLineDetail : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionLineDetail();
    virtual ~PENG_ActionLineDetail() {}

    static RB_String getName() {
        return "Line Designation Table Detail";
    }

    virtual RB_String name() {
        return PENG_ActionLineDetail::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONLINEDETAIL_H
