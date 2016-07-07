/*****************************************************************
 * $Id: srm_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "srm_actionfactory.h"
#include "srm_actionselectproject.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"

/**
 * Constructor
 */
SRM_ActionPlugin::SRM_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
SRM_ActionPlugin::~SRM_ActionPlugin() {
    SRM_DIALOGFACTORY->deleteAllMdiWindows();
    SRM_DIALOGFACTORY->deleteAllDockWidgets();

    delete SRM_ACTIONFACTORY;
    delete SRM_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete SRM_MODELFACTORY;
    // TODO: create utility factory
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String SRM_ActionPlugin::getName() {
    return RB_String(tr("Supplier Relation Management"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String SRM_ActionPlugin::getDescription() {
    return RB_String(tr("Supplier Relation Management Perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void SRM_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from SRM_MainWindow
    SRM_ModelFactory::getInstance(mw);
    SRM_DialogFactory::getInstance(mw);
    SRM_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&SRM"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_crm");
    tb->setWindowTitle(tr("SRM"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    SRM_ACTIONFACTORY->getMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    SRM_ActionSelectProject a;
    a.trigger();
}

//Q_EXPORT_PLUGIN2(SRM_actionplugin, SRM_ActionPlugin);
