/*****************************************************************
 * $Id: acc_actioncustcostpart.h 1782 2012-09-25 15:02:39Z rutger $
 * Created: Sept 25, 2012 12:20:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCUSTCOSTPART_H
#define ACC_ACTIONCUSTCOSTPART_H

#include "rb_action.h"


/**
 * Action opens customer coster center part (settings) window
 */
class ACC_ActionCustCostPart : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCustCostPart();
    virtual ~ACC_ActionCustCostPart() {}

    static RB_String getName() {
        return "Customer Cost Part";
    }

    virtual RB_String name() {
        return ACC_ActionCustCostPart::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCUSTCOSTPART_H
