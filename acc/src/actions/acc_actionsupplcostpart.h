/*****************************************************************
 * $Id: acc_actionsupplcostpart.h 1782 2012-09-25 15:02:39Z rutger $
 * Created: Sept 25, 2012 12:20:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSUPPLCOSTPART_H
#define ACC_ACTIONSUPPLCOSTPART_H

#include "rb_action.h"


/**
 * Action opens supplier cost part (settings) window
 */
class ACC_ActionSupplCostPart : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSupplCostPart();
    virtual ~ACC_ActionSupplCostPart() {}

    static RB_String getName() {
        return "Supplier Cost Part";
    }

    virtual RB_String name() {
        return ACC_ActionSupplCostPart::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSUPPLCOSTPART_H
