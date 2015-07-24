/*****************************************************************
 * $Id: sail_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionfactory.h"

#include "mrp_actionclose.h"
#include "mrp_actionselectproject.h"
#include "mrp_actionsoftwarelicense.h"
#include "mrp_actionsoftwarelicensereport.h"
#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"


MRP_ActionFactory* MRP_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
MRP_ActionFactory::MRP_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectiveCRM);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
MRP_ActionFactory::~MRP_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("MRP_ActionFactory::~MRP_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
MRP_ActionFactory* MRP_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new MRP_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void MRP_ActionFactory::getMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
//    QMenu* subMenu = NULL;

    ga = MRP_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)), // Note: DB instead of MRP
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

//    subMenu = new QMenu(MRP_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("Customers"));
//    menu->addMenu(subMenu);

    ga = MRP_ActionSoftwareLicense::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(MRP_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // reports

    ga = MRP_ActionSoftwareLicenseReport::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(MRP_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // settings
    // none

    menu->addSeparator();

    ga = MRP_ActionClose::createGuiAction();
    menu->addAction(ga);
}

