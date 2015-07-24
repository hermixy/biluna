/*****************************************************************
 * $Id: peng_actionpaint.h 2163 2014-09-04 08:03:35Z rutger $
 * Created: May 23, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONPAINT_H
#define PENG_ACTIONPAINT_H

#include "rb_action.h"


/**
 * Action opens paint system edit/selection dialog
 */
class PENG_ActionPaint : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionPaint();
    virtual ~PENG_ActionPaint() {}

    static RB_String getName() {
        return "Paint System";
    }

    virtual RB_String name() {
        return PENG_ActionPaint::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONPAINT_H
