/*****************************************************************
 * $Id: acc_actionfactory.cpp 2202 2015-01-11 19:39:36Z rutger $
 * Created: Dec 21, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionfactory.h"

#include "acc_actionaccountcontrol.h"
#include "acc_actionaccountgroup.h"
#include "acc_actionallocnreport.h"
#include "acc_actionarea.h"
#include "acc_actionasset.h"
#include "acc_actionassettype.h"
#include "acc_actionassetvaluation.h"
#include "acc_actionbankcsvmapping.h"
#include "acc_actionbankimport.h"
#include "acc_actionchartmaster.h"
#include "acc_actionclose.h"
#include "acc_actioncogsglposting.h"
#include "acc_actioncontactcustomeredit.h"
#include "acc_actioncontactsupplieredit.h"
#include "acc_actioncostcenterbalance.h"
#include "acc_actioncostcenterbudget.h"
#include "acc_actioncostcentersum.h"
#include "acc_actioncostgroup.h"
#include "acc_actioncurrency.h"
#include "acc_actioncustbranch.h"
#include "acc_actioncustcostpart.h"
#include "acc_actiondebtortype.h"
#include "acc_actionsettingsdefault.h"
#include "acc_actiondoctransreport.h"
#include "acc_actionfactorcompany.h"
#include "acc_actiongltransaction.h"
#include "acc_actiongltransreport.h"
#include "acc_actionholdreason.h"
#include "acc_actioninvoiceaccruedreport.h"
#include "acc_actionlocation.h"
#include "acc_actionmaintexportsettings.h"
#include "acc_actionmaintimportsettings.h"
#include "acc_actionpaymentterm.h"
#include "acc_actionpoinvoiceagedreport.h"
#include "acc_actionprice.h"
#include "acc_actionprojectedit.h"
#include "acc_actionpurchaseglposting.h"
#include "acc_actionpurchaseorderreport.h"
#include "acc_actionpurchaseorder.h"
#include "acc_actionrecreatebalance.h"
#include "acc_actionsalesglposting.h"
#include "acc_actionsalesman.h"
#include "acc_actionsalesorderreport.h"
#include "acc_actionsalesorder.h"
#include "acc_actionsalestaxposting.h"
#include "acc_actionsalestaxreport.h"
#include "acc_actionsalestype.h"
#include "acc_actionshipper.h"
#include "acc_actionselectproject.h"
#include "acc_actionsettingsreport.h"
#include "acc_actionsoinvoiceagedreport.h"
#include "acc_actionstockcategory.h"
#include "acc_actionstockitem.h"
#include "acc_actionsupplcostpart.h"
#include "acc_actionsupplier.h"
#include "acc_actionsystype.h"
#include "acc_actiontaxauthrate.h"
#include "acc_actiontaxcategory.h"
#include "acc_actiontaxgroupauth.h"
#include "acc_actiontaxprovince.h"
#include "acc_actiontrialbalance.h"
#include "acc_actionunitofmeasure.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"


ACC_ActionFactory* ACC_ActionFactory::mActiveFactory = 0;


/**
 * Constructor
 */
ACC_ActionFactory::ACC_ActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    this->setPerspective(RB2::PerspectiveACC);
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_ActionFactory::~ACC_ActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("ACC_ActionFactory::~ACC_ActionFactory() OK");
}

/**
 * @return unique instance of this action factory
 */
ACC_ActionFactory* ACC_ActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new ACC_ActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the actions to the menu and toolbar
 */
void ACC_ActionFactory::getAccountingMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;
    QMenu* subSubMenu = NULL;

    ga = ACC_ActionSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)), // Note: DB instead of ACC
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // Customer and supplier

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Customer"));
    menu->addMenu(subMenu);

    ga = ACC_ActionCustBranch::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionContactCustomerEdit::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("S&upplier"));
    menu->addMenu(subMenu);

    ga = ACC_ActionSupplier::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionContactSupplierEdit::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    // Assets

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("Assets"));
    menu->addMenu(subMenu);

    ga = ACC_ActionAsset::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

#ifdef BILUNA_DEBUG
    // TODO: crashes
    ga = ACC_ActionAssetValuation::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));
#endif

    // Inventory

    ga = ACC_ActionStockItem::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
    tb->addSeparator();

    // Sales, purchase order and GL accounting/bookings

    ga = ACC_ActionSalesOrder::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionPurchaseOrder::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionGlTransaction::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    // Import/Export

    ga = ACC_ActionBankImport::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();
//    tb->addSeparator();

    // Reports

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Reports"));
    menu->addMenu(subMenu);

    ga = ACC_ActionTrialBalance::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionGlTransReport::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionDocTransReport::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionAllocnReport::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionInvoiceAccruedReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();

    ga = ACC_ActionSoInvoiceAgedReport::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionPoInvoiceAgedReport::createGuiAction();
    subMenu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionSalesOrderReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionPurchaseOrderReport::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();

    subSubMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subSubMenu->setTitle(tr("Sales Ta&x Report"));
    subMenu->addMenu(subSubMenu);

    ga = ACC_ActionSalesTaxReport::createGuiAction(); // Netherlands
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();

    ga = ACC_ActionCostCenterSum::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionCostCenterBalance::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subMenu->addSeparator();

    ga = ACC_ActionSettingsReport::createGuiAction();
    subMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("ACC S&ettings")); // Settings does not work with OSX
    menu->addMenu(subMenu);

    // Settings > General

    subSubMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subSubMenu->setTitle(tr("&General"));
    subMenu->addMenu(subSubMenu);

    ga = ACC_ActionProjectEdit::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    // Requires valid DB model ID
    connect(DB_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionUnitOfMeasure::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionCurrency::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subSubMenu->addSeparator();

    ga = ACC_ActionAccountControl::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionAccountGroup::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionChartMaster::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subSubMenu->addSeparator();

    ga = ACC_ActionTaxCategory::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionTaxProvince::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionTaxAuthRate::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionTaxGroupAuth::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionSalesTaxPosting::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subSubMenu->addSeparator();

    ga = ACC_ActionStockCategory::createGuiAction();
    subSubMenu->addAction(ga);
    // tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionLocation::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionShipper::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionFactorCompany::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subSubMenu->addSeparator();

    ga = ACC_ActionAssetType::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings > Sales/Purchasing

    subSubMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subSubMenu->setTitle(tr("&Sales/Purchasing"));
    subMenu->addMenu(subSubMenu);

    ga = ACC_ActionSalesType::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionArea::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionSalesGlPosting::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionPurchaseGlPosting::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionCogsGlPosting::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionSalesMan::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    subSubMenu->addSeparator();

    ga = ACC_ActionPrice::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionPaymentTerm::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionHoldReason::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionDebtorType::createGuiAction();
    subSubMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings > Cost Centers

    subSubMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subSubMenu->setTitle(tr("&Cost Centers"));
    subMenu->addMenu(subSubMenu);

    ga = ACC_ActionCostGroup::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionCostCenterBudget::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionCustCostPart::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionSupplCostPart::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings > Mapping

    subSubMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subSubMenu->setTitle(tr("&Mapping"));
    subMenu->addMenu(subSubMenu);

    ga = ACC_ActionBankCsvMapping::createGuiAction();
    subSubMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Settings

    ga = ACC_ActionSettingsDefault::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // Maintenance

    subMenu = new QMenu(ACC_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Maintenance"));
    menu->addMenu(subMenu);

    ga = ACC_ActionSysType::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionMaintImportSettings::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionMaintExportSettings::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = ACC_ActionRecreateBalance::createGuiAction();
    subMenu->addAction(ga);
//    tb->addAction(ga);
    ga->setEnabled(false);
    connect(ACC_MODELFACTORY, SIGNAL(rootIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = ACC_ActionClose::createGuiAction();
    menu->addAction(ga);
}

