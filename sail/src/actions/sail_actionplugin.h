/*****************************************************************
 * $Id: sail_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SAIL_ACTIONPLUGIN_H
#define SAIL_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Sailing navigation SAIL plugin (perspective) plugin class.
 * The actions for the menu and toolbar are the starting point for all plugins
 */
class SAIL_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.sail" FILE "sail_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    SAIL_ActionPlugin();
    virtual ~SAIL_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // SAIL_ACTIONPLUGIN_H
