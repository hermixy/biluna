/*****************************************************************
 * $Id: pcalc_actiondimension.h 0 2016-07-20 15:43:20Z rutger $
 * Created: July 20, 2016 10:43:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONDIMENSION_H
#define PCALC_ACTIONDIMENSION_H

#include "rb_action.h"


/**
 * Action opens dimension property window
 */
class PCALC_ActionDimension : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionDimension();
    virtual ~PCALC_ActionDimension() {}

    static RB_String getName() {
        return "Dimension";
    }

    virtual RB_String name() {
        return PCALC_ActionDimension::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONDIMENSION_H
