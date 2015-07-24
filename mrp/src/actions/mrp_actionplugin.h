/*****************************************************************
 * $Id: sail_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef MRP_ACTIONPLUGIN_H
#define MRP_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Material Requirements Planning or
 * Manufacturing Resoursce planning MRP plugin (perspective) plugin class.
 * The actions for the menu and toolbar are the starting point for all plugins
 */
class MRP_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.mrp" FILE "mrp_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    MRP_ActionPlugin();
    virtual ~MRP_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // MRP_ACTIONPLUGIN_H
