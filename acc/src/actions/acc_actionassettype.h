/*****************************************************************
 * $Id: acc_actionassettype.h 1473 2011-10-24 17:16:18Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONASSETTYPE_H
#define ACC_ACTIONASSETTYPE_H

#include "rb_action.h"


/**
 * Action opens asset type edit dialog
 */
class ACC_ActionAssetType : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAssetType();
    virtual ~ACC_ActionAssetType() {}

    static RB_String getName() {
        return "Asset types";
    }

    virtual RB_String name() {
        return ACC_ActionAssetType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONASSETTYPE_H
