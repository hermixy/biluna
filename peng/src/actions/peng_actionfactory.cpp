/*****************************************************************
 * $Id: peng_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Feb 16, 2011 11:34:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "peng_actionclose.h"
#include "peng_actionconestrikeout.h"
#include "peng_actioneditproject.h"
#include "peng_actioneqtype.h"
#include "peng_actionequipment.h"
#include "peng_actionfluid.h"
#include "peng_actionfluidstability.h"
#include "peng_actionhazardgroup.h"
#include "peng_actioninsulation.h"
#include "peng_actionlinedetail.h"
#include "peng_actionpaint.h"
#include "peng_actionpedclassification.h"
#include "peng_actionpedeqsummary.h"
#include "peng_actionpipeclass.h"
#include "peng_actionselectproject.h"
#include "peng_actionsettings.h"
#include "peng_actionweldvolcalc.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"



PENG_ActionFactory* PENG_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
PENG_ActionFactory::PENG_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectivePENG);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PENG_ActionFactory::~PENG_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PENG_ActionFactory::~PENG_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
PENG_ActionFactory* PENG_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new PENG_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void PENG_ActionFactory::getPlantEngineeringMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = PENG_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    // Needs a valid DB_Project ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

//    subMenu = new QMenu(PENG_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("Customers"));
//    menu->addMenu(subMenu);

//    ga = PENG_ActionCustBranch::createGuiAction();
//    subMenu->addAction(ga);
//    tb->addAction(ga);
//    ga->setEnabled(false);
//    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
//            ga, SLOT(slotSetEnabled(int)));

//    ga = PENG_ActionEquipment::createGuiAction();
//    menu->addAction(ga);
//    tb->addAction(ga);
//    ga->setEnabled(false);
//    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
//            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionLineDetail::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = PENG_ActionConeStrikeOut::createGuiAction();
    menu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionPedClassification::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionWeldVolCalc::createGuiAction();
    menu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // reports
    //
    subMenu = new QMenu(PENG_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("Reports"));
    menu->addMenu(subMenu);

    ga = PENG_ActionPedEqSummary::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings
    //
    subMenu = new QMenu(PENG_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("PENG Settings"));
    menu->addMenu(subMenu);

    ga = PENG_ActionEditProject::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionFluid::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionInsulation::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionPaint::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionPipeClass::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionSettings::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();    // name settings only

    ga = PENG_ActionEqType::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionFluidStability::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = PENG_ActionHazardGroup::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(PENG_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = PENG_ActionClose::createGuiAction();
    menu->addAction(ga);
}
