/*****************************************************************
 * $Id: acc_actionstockcategory.h 1093 2010-02-10 11:45:55Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSTOCKCATEGORY_H
#define ACC_ACTIONSTOCKCATEGORY_H

#include "rb_action.h"


/**
 * Action opens stockcategory and -item edit window
 */
class ACC_ActionStockCategory : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionStockCategory();
    virtual ~ACC_ActionStockCategory() {}

    static RB_String getName() {
        return "Stock Category";
    }

    virtual RB_String name() {
        return ACC_ActionStockCategory::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSTOCKCATEGORY_H
