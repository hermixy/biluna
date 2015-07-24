/*****************************************************************
 * $Id: peng_actioneqtype.h 1368 2011-02-17 14:40:37Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONEQTYPE_H
#define PENG_ACTIONEQTYPE_H

#include "rb_action.h"


/**
 * Action opens equipmenttype edit window
 */
class PENG_ActionEqType : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionEqType();
    virtual ~PENG_ActionEqType() {}

    static RB_String getName() {
        return "Equipment Type";
    }

    virtual RB_String name() {
        return PENG_ActionEqType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONEQTYPE_H
