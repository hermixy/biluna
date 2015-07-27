/*****************************************************************
 * $Id: gv_actionfactory.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Dec 21, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfactory.h"

#include "db_actionfactory.h"
#include "db_actionsystemdatabasebrowser.h"
#include "db_actionsystemdatabasedisconnect.h"
#include "db_actionsystemdatabasemapping.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"

#include "gv_actionfilenew.h"

#include "gv_actionbase.h"
#include "gv_actioncomponent.h"
#include "gv_actiondeleteitem.h"
#include "gv_actiondrawinglist.h"
#include "gv_actiondrawline.h"
#include "gv_actiondrawsymbol.h"
#include "gv_actiondrawtext.h"
#include "gv_actionformatitemcolor.h"
#include "gv_actionformatlineattributes.h"
#include "gv_actionmodifymergeline.h"
#include "gv_actionmodifymove.h"
#include "gv_actionmodifysplitline.h"
#include "gv_actionsetsnapmode.h"
#include "gv_actionsetsnapmodeauto.h"
#include "gv_actionsetsnapmodefree.h"
#include "gv_actionsetsnapmodegrid.h"
#include "gv_actionsetsnaprestriction.h"
#include "gv_actionsetsnaprestrictionnothing.h"
#include "gv_actionsetsnaprestrictionorthogonal.h"
#include "gv_actiontestnew.h"
#include "gv_actiontestnewqt.h"
#include "gv_actionviewdraft.h"
#include "gv_actionviewgrid.h"
#include "gv_actionviewstatusbar.h"
#include "gv_actionzoomauto.h"
#include "gv_actionzoomin.h"
#include "gv_actionzoomout.h"
#include "gv_actionzoompan.h"
#include "gv_actionzoomprevious.h"
#include "gv_actionzoomredraw.h"
#include "gv_actionzoomselection.h"
#include "gv_actionzoomwindow.h"

#include "gv_actionsystemdatabaseconnect.h"
#include "gv_actionsystemselectproject.h"
#include "gv_dialogfactory.h"


GV_ActionFactory* GV_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
GV_ActionFactory::GV_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
GV_ActionFactory::~GV_ActionFactory() {
    mActiveFactory = NULL;
}

/**
 * @return unique instance of this action factory
 */
GV_ActionFactory* GV_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new GV_ActionFactory(mw);
    }
    return mActiveFactory;
}

GV_ActionBase* GV_ActionFactory::getDefaultAction() {
    return NULL;
}

void GV_ActionFactory::getFileNewMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = GV_ActionFileNew::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
//    ga->setEnabled(false);
//    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
//            ga, SLOT(slotSetEnabled(int)));
}

/**
 * Get and add the 'Test' actions to the menu and toolbar
 */
void GV_ActionFactory::getDrawingMenu(QMenu* menu, QToolBar* tb) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = GV_ActionDrawingList::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    ga = GV_ActionDrawLine::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionDrawSymbol::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionDrawText::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionDeleteItem::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    ga = GV_ActionViewGrid::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionSetSnapModeGrid::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionSetSnapModeFree::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    ga = GV_ActionZoomIn::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionZoomOut::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    ga = GV_ActionFormatItemColor::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionFormatLineAttributes::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));









}

/**
 * Get System menu items. Copied from DB_ActionFactory
 * except for: -
 * @param menu
 * @param tb toolbar, not used here
 */
void GV_ActionFactory::getSystemMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = GV_ActionSystemDatabaseConnect::createGuiAction();
    menu->addAction(ga);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsNotSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseDisconnect::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionSystemSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // create and add submenu
    subMenu=new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Administrator"));
    menu->addMenu(subMenu);

    ga = DB_ActionSystemDatabaseBrowser::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseMapping::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));
}

/**
 * Get Test menu items
 * @param menu
 * @param tb toolbar, not used here
 */
void GV_ActionFactory::getTestMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
//    QMenu* subMenu = NULL;

    ga = GV_ActionTestNew::createGuiAction();
    menu->addAction(ga);
//    connect(DB_MODELFACTORY, SIGNAL(databaseIsNotSet(int)),
//            ga, SLOT(slotSetEnabled(int)));

    ga = GV_ActionTestNewQt::createGuiAction();
    menu->addAction(ga);
}

