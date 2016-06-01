/*****************************************************************
 * $Id: peng_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "pcalc_actionclose.h"
#include "pcalc_actioneditproject.h"
#include "pcalc_actionen1591flange.h"
#include "pcalc_actionengasket.h"
#include "pcalc_actionmaterial.h"
#include "pcalc_actionmatcreep.h"
#include "pcalc_actionmatelasmodul.h"
#include "pcalc_actionmatthermexp.h"
#include "pcalc_actionselectproject.h"
#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"



PCALC_ActionFactory* PCALC_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
PCALC_ActionFactory::PCALC_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectivePCALC);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_ActionFactory::~PCALC_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PCALC_ActionFactory::~PCALC_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
PCALC_ActionFactory* PCALC_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new PCALC_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void PCALC_ActionFactory::getPlantCalculationMenu(QMenu* menu,
                                                  QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = PCALC_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    // Needs a valid DB_Project ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();


    ga = PCALC_ActionEN1591Flange::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // menu->addSeparator();

    // reports
    //

    // ...

    // Settings
    //
    subMenu = new QMenu(PCALC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("PCALC Settings"));
    menu->addMenu(subMenu);

    ga = PCALC_ActionEditProject::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));


    ga = PCALC_ActionENGasket::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // menu->addSeparator();

    ga = PCALC_ActionMaterial::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PCALC_ActionMatCreep::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PCALC_ActionMatElasModul::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));


    ga = PCALC_ActionMatThermExp::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PCALC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    //

    ga = PCALC_ActionClose::createGuiAction();
    menu->addAction(ga);
}
