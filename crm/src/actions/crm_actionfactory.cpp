/*****************************************************************
 * $Id: crm_actionfactory.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionfactory.h"

#include "crm_actionactivityreport.h"
#include "crm_actionactivity.h"
#include "crm_actioncampaigntarget.h"
#include "crm_actionclose.h"
#include "crm_actioncontact.h"
#include "crm_actioncontactbycustreport.h"
#include "crm_actioncontactreport.h"
#include "crm_actioncustomer.h"
#include "crm_actioncustomerreport.h"
#include "crm_actiondocument.h"
#include "crm_actiongroupcontact.h"
#include "crm_actionleadsourcetype.h"
#include "crm_actionprojectedit.h"
#include "crm_actionselectproject.h"
#include "crm_actionsysseqno.h"
#include "crm_actiontemplate.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"


CRM_ActionFactory* CRM_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
CRM_ActionFactory::CRM_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectiveCRM);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
CRM_ActionFactory::~CRM_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("CRM_ActionFactory::~CRM_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
CRM_ActionFactory* CRM_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new CRM_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void CRM_ActionFactory::getMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = CRM_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)), // Note: DB instead of CRM
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

//    subMenu = new QMenu(CRM_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("Customer"));
//    menu->addMenu(subMenu);

    ga = CRM_ActionCustomer::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionContact::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionActivity::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionCampaignTarget::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionDocument::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = CRM_ActionGroupContact::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionTemplate::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // Reports

    subMenu = new QMenu(CRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Reports"));
    menu->addMenu(subMenu);

    ga = CRM_ActionCustomerReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionContactReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionContactByCustReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionActivityReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings

    subMenu = new QMenu(CRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("S&ettings"));
    menu->addMenu(subMenu);

    ga = CRM_ActionProjectEdit::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    // Requires valid DB model ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = CRM_ActionLeadSourceType::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Maintenance

    subMenu = new QMenu(CRM_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Maintenance"));
    menu->addMenu(subMenu);

    ga = CRM_ActionSysSeqNo::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(CRM_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = CRM_ActionClose::createGuiAction();
    menu->addAction(ga);
}

