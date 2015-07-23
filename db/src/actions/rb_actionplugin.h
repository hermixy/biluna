/*****************************************************************
 * $Id: rb_actionplugin.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 16, 2010 11:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONPLUGIN_H
#define RB_ACTIONPLUGIN_H

#include "db_global.h"
#include "rb_mainwindow.h"
#include "rb_string.h"


/**
 * Plugin (perspective) interface class. The actions
 * for the menu and toolbar are the starting point for all plugins
 */
class DB_EXPORT RB_ActionPlugin {

public:
    virtual ~RB_ActionPlugin() { }

    //! Get name of perspective
    virtual RB_String getName() = 0;
    //! Get description of perspective
    virtual RB_String getDescription() = 0;
    //! Get menu and toolbar actions
    virtual void getGuiActions(RB_MainWindow* mw) = 0;
};
Q_DECLARE_INTERFACE(RB_ActionPlugin,
                    "com.biluna.plugin.RB_ActionPlugin/1.0")
#endif // RB_ACTIONPLUGIN_H
