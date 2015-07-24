/*****************************************************************
 * $Id: acc_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 22, 2010 11:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef ACC_ACTIONPLUGIN_H
#define ACC_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Accounting ACC plugin (perspective) interface class. The actions
 * for the menu and toolbar are the starting point for all plugins
 */
class ACC_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.acc" FILE "acc_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    ACC_ActionPlugin();
    virtual ~ACC_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // ACC_ACTIONPLUGIN_H
