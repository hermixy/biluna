/*****************************************************************
 * $Id: pcalc_actionplugin.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 29, 2015 11:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONPLUGIN_H
#define PCALC_ACTIONPLUGIN_H

#include <QObject>
#include "rb_actionplugin.h"


/**
 * Plant Engineering PCALC plugin (perspective) interface class. The actions
 * for the menu and toolbar are the starting point for all plugins
 */
class PCALC_ActionPlugin : public QObject, RB_ActionPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.biluna.pcalc" FILE "pcalc_actionplugin.json")
    Q_INTERFACES(RB_ActionPlugin)

public:
    PCALC_ActionPlugin();
    virtual ~PCALC_ActionPlugin();
    virtual RB_String getName();
    virtual RB_String getDescription();
    virtual void getGuiActions(RB_MainWindow* mw);

private:
    QAction* mMenuAction;
    QToolBar* mToolBar;
};

#endif // PCALC_ACTIONPLUGIN_H
