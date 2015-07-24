/*****************************************************************
 * $Id: srm_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionfactory.h"

#include "srm_actionactivityreport.h"
#include "srm_actionactivity.h"
#include "srm_actioncampaigntarget.h"
#include "srm_actionclose.h"
#include "srm_actioncontact.h"
#include "srm_actioncontactbysuppreport.h"
#include "srm_actioncontactreport.h"
#include "srm_actiondocument.h"
#include "srm_actiongroupcontact.h"
#include "srm_actionleadsourcetype.h"
#include "srm_actionprojectedit.h"
#include "srm_actionselectproject.h"
#include "srm_actionsupplier.h"
#include "srm_actionsupplierreport.h"
#include "srm_actionsysseqno.h"
#include "srm_actiontemplate.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"


SRM_ActionFactory* SRM_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
SRM_ActionFactory::SRM_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectiveSRM);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
SRM_ActionFactory::~SRM_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("SRM_ActionFactory::~SRM_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
SRM_ActionFactory* SRM_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new SRM_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void SRM_ActionFactory::getMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = SRM_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)), // Note: DB instead of CRM
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

//    subMenu = new QMenu(SRM_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("Customer"));
//    menu->addMenu(subMenu);

    ga = SRM_ActionSupplier::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionContact::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionActivity::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionCampaignTarget::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionDocument::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = SRM_ActionGroupContact::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionTemplate::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // Reports

    subMenu = new QMenu(SRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Reports"));
    menu->addMenu(subMenu);

    ga = SRM_ActionSupplierReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionContactReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionContactBySuppReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionActivityReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings

    subMenu = new QMenu(SRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("S&ettings"));
    menu->addMenu(subMenu);

    ga = SRM_ActionProjectEdit::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    // Requires valid DB model ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = SRM_ActionLeadSourceType::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Maintenance

    subMenu = new QMenu(SRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Maintenance"));
    menu->addMenu(subMenu);

    ga = SRM_ActionSysSeqNo::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(SRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = SRM_ActionClose::createGuiAction();
    menu->addAction(ga);
}

