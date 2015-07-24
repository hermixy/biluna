/*****************************************************************
 * $Id: acc_actiontaxcategory.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 22, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONTAXCATEGORY_H
#define ACC_ACTIONTAXCATEGORY_H

#include "rb_action.h"


/**
 * Action opens tax categories edit dialog
 */
class ACC_ActionTaxCategory : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionTaxCategory();
    virtual ~ACC_ActionTaxCategory() {}

    static RB_String getName() {
        return "Currency";
    }

    virtual RB_String name() {
        return ACC_ActionTaxCategory::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONTAXCATEGORY_H
