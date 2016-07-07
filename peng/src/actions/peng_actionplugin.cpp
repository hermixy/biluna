/*****************************************************************
 * $Id: peng_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 16, 2011 11:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "peng_actionfactory.h"
#include "peng_actionselectproject.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"

/**
 * Constructor
 */
PENG_ActionPlugin::PENG_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
PENG_ActionPlugin::~PENG_ActionPlugin() {
    PENG_DIALOGFACTORY->deleteAllMdiWindows();
    PENG_DIALOGFACTORY->deleteAllDockWidgets();

    delete PENG_ACTIONFACTORY;
    delete PENG_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete PENG_MODELFACTORY;
    // TODO: create utility factory
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String PENG_ActionPlugin::getName() {
    return RB_String(tr("Plant Engineering"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String PENG_ActionPlugin::getDescription() {
    return RB_String(tr("Plant engineering perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void PENG_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from PENG_MainWindow
    PENG_ModelFactory::getInstance(mw);
    PENG_DialogFactory::getInstance(mw);
    PENG_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&PlantEng"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_peng");
    tb->setWindowTitle(tr("Plant Engineering"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    PENG_ACTIONFACTORY->getPlantEngineeringMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    PENG_ActionSelectProject a;
    a.trigger();
}

// Q_EXPORT_PLUGIN2(peng_actionplugin, PENG_ActionPlugin);
