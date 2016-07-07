/*****************************************************************
 * $Id: scan_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "scan_actionfactory.h"
#include "scan_actionselectproject.h"
#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"

/**
 * Constructor
 */
SCAN_ActionPlugin::SCAN_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
SCAN_ActionPlugin::~SCAN_ActionPlugin() {
    SCAN_DIALOGFACTORY->deleteAllMdiWindows();
    SCAN_DIALOGFACTORY->deleteAllDockWidgets();

    delete SCAN_ACTIONFACTORY;
    delete SCAN_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete SCAN_MODELFACTORY;
    // TODO: create utility factory
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String SCAN_ActionPlugin::getName() {
    return RB_String(tr("Scan, questionair or interview"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String SCAN_ActionPlugin::getDescription() {
    return RB_String(tr("Scan perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void SCAN_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from SCAN_MainWindow
    SCAN_ModelFactory::getInstance(mw);
    SCAN_DialogFactory::getInstance(mw);
    SCAN_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&Scan"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_Scan");
    tb->setWindowTitle(tr("Scan"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    SCAN_ACTIONFACTORY->getScanMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    SCAN_ActionSelectProject a;
    a.trigger();
}

// Q_EXPORT_PLUGIN2(peng_actionplugin, SCAN_ActionPlugin);
