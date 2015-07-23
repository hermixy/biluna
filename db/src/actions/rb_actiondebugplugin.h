/*****************************************************************
 * $Id: rb_actiondebugplugin.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 16, 2009 12:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONDEBUGPLUGIN_H
#define RB_ACTIONDEBUGPLUGIN_H

#include "rb_action.h"

/**
 * Debug plugins action class
 */
class DB_EXPORT RB_ActionDebugPlugin : public RB_Action {

    Q_OBJECT

public:
    RB_ActionDebugPlugin();
    virtual ~RB_ActionDebugPlugin() {}

    static RB_String getName() {
        return "Debug plugins";
    }

    virtual RB_String name() {
        return RB_ActionDebugPlugin::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif // RB_ACTIONDEBUGPLUGIN_H
