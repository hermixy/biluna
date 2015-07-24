/*****************************************************************
 * $Id: sail_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionfactory.h"

#include "sail_actionclose.h"
#include "sail_actionmapnavigation.h"
#include "sail_actionpolardiagram.h"
#include "sail_actionprojectedit.h"
#include "sail_actionselectproject.h"
#include "sail_actiontrackdistancereport.h"
#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"


SAIL_ActionFactory* SAIL_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
SAIL_ActionFactory::SAIL_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectiveCRM);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SAIL_ActionFactory::~SAIL_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SAIL_ActionFactory::~SAIL_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
SAIL_ActionFactory* SAIL_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SAIL_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void SAIL_ActionFactory::getMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
//    QMenu* subMenu = NULL;

    ga = SAIL_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)), // Note: DB instead of SAIL
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

//    subMenu = new QMenu(SAIL_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("Customers"));
//    menu->addMenu(subMenu);

    ga = SAIL_ActionMapNavigation::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SAIL_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // reports

    ga = SAIL_ActionTrackDistanceReport::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SAIL_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // settings

    ga = SAIL_ActionPolarDiagram::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(SAIL_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SAIL_ActionProjectEdit::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    // Requires valid DB model ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = SAIL_ActionClose::createGuiAction();
    menu->addAction(ga);
}

