/*****************************************************************
 * $Id: scan_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actionfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "scan_actionclose.h"
#include "scan_actioneditscan.h"
#include "scan_actionresult.h"
#include "scan_actionreviewchart.h"
#include "scan_actioneditproject.h"
#include "scan_actionselectproject.h"
#include "scan_dialogfactory.h"
#include "scan_modelfactory.h"


SCAN_ActionFactory* SCAN_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
SCAN_ActionFactory::SCAN_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectivePENG);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SCAN_ActionFactory::~SCAN_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SCAN_ActionFactory::~SCAN_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
SCAN_ActionFactory* SCAN_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SCAN_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void SCAN_ActionFactory::getScanMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
//    QMenu* subMenu = NULL;

    ga = SCAN_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    // Needs a valid DB_Project ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = SCAN_ActionResult::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SCAN_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // Settings

    ga = SCAN_ActionEditProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    // Needs a valid DB_Project ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SCAN_ActionEditScan::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SCAN_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SCAN_ActionReviewChart::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SCAN_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Reports


    menu->addSeparator();

    ga = SCAN_ActionClose::createGuiAction();
    menu->addAction(ga);
}

/**
 * Connect signal/slots for enabling all the GUI actions,
 * except Select Project and Close Perspective. These are
 * handled individualy.
 */
void SCAN_ActionFactory::connectSignalSlot(RB_GuiAction* ga) {
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            this, SLOT(slotDbIdValid(int)));
    connect(SCAN_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));
}

/**
 * Slot handles DB_ModelFactory rootIsSet() signal
 * @param valid RB2::ValidTrue or RB2::ValidFalse
 */
void SCAN_ActionFactory::slotDbIdValid(int valid) {
    if (valid == RB2::ValidFalse) {
        emit dbIdValid(RB2::ValidFalse);
    }
}
