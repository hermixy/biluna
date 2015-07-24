/*****************************************************************
 * $Id: acc_actionstockitem.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSTOCKITEM_H
#define ACC_ACTIONSTOCKITEM_H

#include "rb_action.h"


/**
 * Action opens stock item edit window
 */
class ACC_ActionStockItem : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionStockItem();
    virtual ~ACC_ActionStockItem() {}

    static RB_String getName() {
        return "Stock Item";
    }

    virtual RB_String name() {
        return ACC_ActionStockItem::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSTOCKITEM_H
