/*****************************************************************
 * $Id: db_actionsystemtoolssignalspy.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSSIGNALSPY_H
#define DB_ACTIONSYSTEMTOOLSSIGNALSPY_H

#include "rb_action.h"


/**
 * Tools action class for monitoring the signal/slot communication between QObjects
 */
class DB_EXPORT DB_ActionSystemToolsSignalSpy : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsSignalSpy();
    virtual ~DB_ActionSystemToolsSignalSpy() {}

    static RB_String getName() {
        return "Tools Signal Spy";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsSignalSpy::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
