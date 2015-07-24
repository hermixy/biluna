/*****************************************************************
 * $Id: pcalc_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Apr 29, 2015 11:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "pcalc_actionfactory.h"
#include "pcalc_actionselectproject.h"
#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"

/**
 * Constructor
 */
PCALC_ActionPlugin::PCALC_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
PCALC_ActionPlugin::~PCALC_ActionPlugin() {
    PCALC_DIALOGFACTORY->deleteAllMdiWindows();
    PCALC_DIALOGFACTORY->deleteAllDockWidgets();

    delete PCALC_ACTIONFACTORY;
    delete PCALC_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete PCALC_MODELFACTORY;
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String PCALC_ActionPlugin::getName() {
    return RB_String(tr("Plant Calculation"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String PCALC_ActionPlugin::getDescription() {
    return RB_String(tr("Plant calculation perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void PCALC_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from PCALC_MainWindow
    PCALC_ModelFactory::getInstance(mw);
    PCALC_DialogFactory::getInstance(mw);
    PCALC_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("PlantC&alc"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_pcalc");
    tb->setWindowTitle(tr("Plant Calculation"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    PCALC_ACTIONFACTORY->getPlantCalculationMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    PCALC_ActionSelectProject a;
    a.trigger();
}

// Q_EXPORT_PLUGIN2(pcalc_actionplugin, PCALC_ActionPlugin);
