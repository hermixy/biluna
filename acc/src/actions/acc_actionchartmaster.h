/*****************************************************************
 * $Id: acc_actionchartmaster.h 1106 2010-03-04 21:34:11Z rutger $
 * Created: Feb 25, 2010 10:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCHARTMASTER_H
#define ACC_ACTIONCHARTMASTER_H

#include "rb_action.h"


/**
 * Action opens chart of accounts (master) edit dialog
 */
class ACC_ActionChartMaster : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionChartMaster();
    virtual ~ACC_ActionChartMaster() {}

    static RB_String getName() {
        return "Chart of Accounts";
    }

    virtual RB_String name() {
        return ACC_ActionChartMaster::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCHARTMASTER_H
