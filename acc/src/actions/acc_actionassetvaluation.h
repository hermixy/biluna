/*****************************************************************
 * $Id: acc_actionassetvaluation.h 1476 2011-11-07 22:08:48Z rutger $
 * Created: Nov 7, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONASSETVALUATION_H
#define ACC_ACTIONASSETVALUATION_H

#include "rb_action.h"


/**
 * Action opens asset edit dialog
 */
class ACC_ActionAssetValuation : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAssetValuation();
    virtual ~ACC_ActionAssetValuation() {}

    static RB_String getName() {
        return "Asset Valuation";
    }

    virtual RB_String name() {
        return ACC_ActionAssetValuation::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONASSETVALUATION_H
