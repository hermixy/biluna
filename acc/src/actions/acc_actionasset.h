/*****************************************************************
 * $Id: acc_actionasset.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONASSET_H
#define ACC_ACTIONASSET_H

#include "rb_action.h"


/**
 * Action opens asset edit dialog
 */
class ACC_ActionAsset : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAsset();
    virtual ~ACC_ActionAsset() {}

    static RB_String getName() {
        return "Asset";
    }

    virtual RB_String name() {
        return ACC_ActionAsset::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONASSET_H
