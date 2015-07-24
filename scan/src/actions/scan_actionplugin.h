/*****************************************************************
 * $Id: scan_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SCAN_ACTIONPLUGIN_H
#define SCAN_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Scan, questionair or interview plugin (perspective) interface class.
 * The actions for the menu and toolbar are the starting point for all plugins
 */
class SCAN_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.scan" FILE "scan_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    SCAN_ActionPlugin();
    virtual ~SCAN_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // SCAN_ACTIONPLUGIN_H
