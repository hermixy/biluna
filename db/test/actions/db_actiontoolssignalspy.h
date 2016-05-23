/*****************************************************************
 * $Id: db_actiontoolssignalspy.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSSIGNALSPY_H
#define DB_ACTIONTOOLSSIGNALSPY_H

#include "rb_action.h"


/**
 * Tools action class for monitoring the signal/slot communication between QObjects
 */
class DB_ActionToolsSignalSpy : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsSignalSpy();
    virtual ~DB_ActionToolsSignalSpy() {}

    static RB_String getName() {
        return "Tools Signal Spy";
    }

    virtual RB_String name() {
        return DB_ActionToolsSignalSpy::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
