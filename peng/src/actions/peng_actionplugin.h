/*****************************************************************
 * $Id: peng_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 16, 2011 11:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef PENG_ACTIONPLUGIN_H
#define PENG_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Plant Engineering PENG plugin (perspective) interface class. The actions
 * for the menu and toolbar are the starting point for all plugins
 */
class PENG_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.peng" FILE "peng_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    PENG_ActionPlugin();
    virtual ~PENG_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // PENG_ACTIONPLUGIN_H
