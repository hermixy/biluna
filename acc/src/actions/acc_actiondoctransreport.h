/*****************************************************************
 * $Id: acc_actiondoctransreport.h 1328 2010-12-06 21:55:13Z rutger $
 * Created: Nov 10, 2010 10:57:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONDOCTRANSREPORT_H
#define ACC_ACTIONDOCTRANSREPORT_H

#include "rb_action.h"


/**
 * Action opens document transaction report MDI window
 */
class ACC_ActionDocTransReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionDocTransReport();
    virtual ~ACC_ActionDocTransReport() {}

    static RB_String getName() {
        return "View document transaction report";
    }

    virtual RB_String name() {
        return ACC_ActionDocTransReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONJOURNALREPORT_H
