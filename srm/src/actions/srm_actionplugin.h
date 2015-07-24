/*****************************************************************
 * $Id: srm_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef SRM_ACTIONPLUGIN_H
#define SRM_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Customer Relationship Management CRM plugin (perspective) plugin class.
 * The actions for the menu and toolbar are the starting point for all plugins
 */
class SRM_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.srm" FILE "srm_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    SRM_ActionPlugin();
    virtual ~SRM_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // SRM_ACTIONPLUGIN_H
