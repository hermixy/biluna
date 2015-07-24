/*****************************************************************
 * $Id: acc_actionplugin.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Dec 22, 2010 11:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionplugin.h"

#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "acc_actionfactory.h"
#include "acc_actionselectproject.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_utilityfactory.h"


/**
 * Constructor
 */
ACC_ActionPlugin::ACC_ActionPlugin() {
    mMenuAction = NULL;
    mToolBar = NULL;
}

/**
 * Destructor
 */
ACC_ActionPlugin::~ACC_ActionPlugin() {
    ACC_DIALOGFACTORY->deleteAllMdiWindows();
    ACC_DIALOGFACTORY->deleteAllDockWidgets();

    delete ACC_ACTIONFACTORY;
    delete ACC_DIALOGFACTORY; // before modelfactory otherwise models invalid
    delete ACC_MODELFACTORY;
    delete ACC_UTILITYFACTORY;
    delete mMenuAction;
    delete mToolBar;
}

/**
 * @returns name of perspective (plugin)
 */
RB_String ACC_ActionPlugin::getName() {
    return RB_String(tr("Accounting"));
}

/**
 * @returns description of perspective (plugin)
 */
RB_String ACC_ActionPlugin::getDescription() {
    return RB_String(tr("Accounting perspective"));
}

/**
 * Get menu and toolbar actions
 * @param mw main window
 */
void ACC_ActionPlugin::getGuiActions(RB_MainWindow* mw) {
    // from BIL_MainWindow
    ACC_UtilityFactory::getInstance();
    ACC_ModelFactory::getInstance(mw);
    ACC_DialogFactory::getInstance(mw);
    ACC_ActionFactory::getInstance(mw);

    QMenu* menu = new QMenu(mw);
    menu->setTitle(tr("&Accounting"));
    mw->menuBar()->insertMenu(DB_ACTIONFACTORY->getPerspectiveAction(), menu);
    mMenuAction = menu->menuAction();

    QToolBar* tb = new QToolBar(mw);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_accounting");
    tb->setWindowTitle(tr("Accounting"));
    mw->addToolBar(Qt::TopToolBarArea, tb);
    mToolBar = tb;

    ACC_ACTIONFACTORY->getAccountingMenu(menu, tb);
    // more menus if applicable

    DB_MODELFACTORY->emitState();
    // QApplication::setOverrideCursor(Qt::WaitCursor)
    // was set in DB_ActionFactory::slotPluginClicked()
    QApplication::restoreOverrideCursor();

    // Open select project dialog
    ACC_ActionSelectProject a;
    a.trigger();
}

// Q_EXPORT_PLUGIN2(acc_actionplugin, ACC_ActionPlugin);
