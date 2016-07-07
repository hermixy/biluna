/*****************************************************************
 * $Id: sail_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "sail_actionselectproject.h"
#include "sail_actionfactory.h"
#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"

/**
 * Constructor
 */
SAIL_ActionPlugin::SAIL_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
SAIL_ActionPlugin::~SAIL_ActionPlugin() {
    SAIL_DIALOGFACTORY->deleteAllMdiWindows();
    SAIL_DIALOGFACTORY->deleteAllDockWidgets();

    delete SAIL_ACTIONFACTORY;
    delete SAIL_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete SAIL_MODELFACTORY;
    // TODO: create utility factory
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String SAIL_ActionPlugin::getName() {
    return RB_String(tr("Sailing Navigation"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String SAIL_ActionPlugin::getDescription() {
    return RB_String(tr("Sailing Navigation Perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void SAIL_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from SAIL_MainWindow
    SAIL_ModelFactory::getInstance(mw);
    SAIL_DialogFactory::getInstance(mw);
    SAIL_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("Sai&ling"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_sail");
    tb->setWindowTitle(tr("SAIL"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    SAIL_ACTIONFACTORY->getMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    SAIL_ActionSelectProject a;
    a.trigger();
}

//Q_EXPORT_PLUGIN2(sail_actionplugin, SAIL_ActionPlugin);
