/*****************************************************************
 * $Id: crm_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "crm_actionfactory.h"
#include "crm_actionselectproject.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"

/**
 * Constructor
 */
CRM_ActionPlugin::CRM_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
CRM_ActionPlugin::~CRM_ActionPlugin() {
    CRM_DIALOGFACTORY->deleteAllMdiWindows();
    CRM_DIALOGFACTORY->deleteAllDockWidgets();

    delete CRM_ACTIONFACTORY;
    delete CRM_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete CRM_MODELFACTORY;
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String CRM_ActionPlugin::getName() {
    return RB_String(tr("Customer Relation Management"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String CRM_ActionPlugin::getDescription() {
    return RB_String(tr("Customer Relation Management Perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void CRM_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from CRM_MainWindow
    CRM_ModelFactory::getInstance(mw);
    CRM_DialogFactory::getInstance(mw);
    CRM_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&CRM"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_crm");
    tb->setWindowTitle(tr("CRM"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    CRM_ACTIONFACTORY->getMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    CRM_ActionSelectProject a;
    a.trigger();
}

// Q_EXPORT_PLUGIN2(crm_actionplugin, CRM_ActionPlugin);
