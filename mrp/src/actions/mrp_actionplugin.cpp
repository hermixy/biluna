/*****************************************************************
 * $Id: sail_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "mrp_actionselectproject.h"
#include "mrp_actionfactory.h"
#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"

/**
 * Constructor
 */
MRP_ActionPlugin::MRP_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
MRP_ActionPlugin::~MRP_ActionPlugin() {
    MRP_DIALOGFACTORY->deleteAllMdiWindows();
    MRP_DIALOGFACTORY->deleteAllDockWidgets();

    delete MRP_ACTIONFACTORY;
    delete MRP_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete MRP_MODELFACTORY;
    // TODO: create utility factory
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String MRP_ActionPlugin::getName() {
    return RB_String(tr("Manufacturing Resource Planning"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String MRP_ActionPlugin::getDescription() {
    return RB_String(tr("Manufacturing Resource Planning Perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void MRP_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from MRP_MainWindow
    MRP_ModelFactory::getInstance(mw);
    MRP_DialogFactory::getInstance(mw);
    MRP_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&Man.Res.Planning"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_mrp");
    tb->setWindowTitle(tr("MRP"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    MRP_ACTIONFACTORY->getMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    MRP_ActionSelectProject a;
    a.trigger();
}

//Q_EXPORT_PLUGIN2(mrp_actionplugin, MRP_ActionPlugin);
