/*****************************************************************
 * $Id: db_actiontoolsplugins.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSPLUGINS_H
#define DB_ACTIONTOOLSPLUGINS_H

#include "rb_action.h"


/**
 * Tools action class for reporting the plugins status
 */
class DB_ActionToolsPlugins : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsPlugins();
    virtual ~DB_ActionToolsPlugins() {}

    static RB_String getName() {
        return "Tools Plugins";
    }

    virtual RB_String name() {
        return DB_ActionToolsPlugins::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
