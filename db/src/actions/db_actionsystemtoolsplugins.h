/*****************************************************************
 * $Id: db_actionsystemtoolsplugins.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSPLUGINS_H
#define DB_ACTIONSYSTEMTOOLSPLUGINS_H

#include "rb_action.h"


/**
 * Tools action class for reporting the plugins status
 */
class DB_EXPORT DB_ActionSystemToolsPlugins : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsPlugins();
    virtual ~DB_ActionSystemToolsPlugins() {}

    static RB_String getName() {
        return "Tools Plugins";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsPlugins::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
