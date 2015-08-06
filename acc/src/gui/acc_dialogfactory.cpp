/*****************************************************************
 * $Id: acc_dialogfactory.cpp 2202 2015-01-11 19:39:36Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_dialogfactory.h"

#include "acc_accountcontroldialog.h"
#include "acc_accountgroupdialog.h"
#include "acc_allocnreportwidget.h"
#include "acc_areadialog.h"
#include "acc_assetvaluationwidget.h"
#include "acc_assetwidget.h"
#include "acc_assettypedialog.h"
#include "acc_bankmappingdialog.h"
#include "acc_bankimportcheckdialog.h"
#include "acc_chartmasterdialog.h"
#include "acc_cogsglpostingdialog.h"
#include "acc_contacteditwidget.h"
#include "acc_costcenterbalancereportwidget.h"
#include "acc_costcenterbudgetdialog.h"
#include "acc_costcenterreportwidget.h"
#include "acc_costgroupdialog.h"
#include "acc_currencydialog.h"
#include "acc_custbranchwidget.h"
#include "acc_custcostpartwidget.h"
#include "acc_custmasterwidget.h"
#include "acc_debtortypedialog.h"
#include "acc_doctransreportwidget.h"
#include "acc_factorcompanydialog.h"
#include "acc_gltransactionwidget.h"
#include "acc_gltransreportwidget.h"
#include "acc_invoiceaccruedreportwidget.h"
#include "acc_invoiceagedreportwidget.h"
#include "acc_modelfactory.h"
#include "acc_holdreasondialog.h"
#include "acc_navigationwidget.h"
#include "acc_orderreportwidget.h"
#include "acc_locationdialog.h"
#include "acc_paymenttermdialog.h"
#include "acc_pricewidget.h"
#include "acc_projectdialog.h"
#include "acc_projecteditdialog.h"
#include "acc_purchaseglpostingdialog.h"
#include "acc_purchaseorderwidget.h"
#include "acc_salesglpostingdialog.h"
#include "acc_salesmandialog.h"
#include "acc_salesorderwidget.h"
#include "acc_salestaxpostingdialog.h"
#include "acc_salestaxreportwidget.h"
#include "acc_salestypedialog.h"
#include "acc_selectchartmasterdialog.h"
#include "acc_selectcontactdialog.h"
#include "acc_selectcostcenterdialog.h"
#include "acc_selectcostgroupdialog.h"
#include "acc_selectcustbranchdialog.h"
#include "acc_selectstockitemdialog.h"
#include "acc_selectsupplierdialog.h"
#include "acc_settingreportwidget.h"
#include "acc_shipperdialog.h"
#include "acc_stockcategorywidget.h"
#include "acc_stockitemwidget.h"
#include "acc_stockmasterwidget.h"
#include "acc_supplcostpartwidget.h"
#include "acc_supplierdetailwidget.h"
#include "acc_suppliermasterwidget.h"
#include "acc_systypedialog.h"
#include "acc_taxauthratedialog.h"
#include "acc_taxcategorydialog.h"
#include "acc_taxgroupauthdialog.h"
#include "acc_taxprovincedialog.h"
#include "acc_trialbalancewidget.h"
#include "acc_unitofmeasuredialog.h"
//#include "db_browserdialog.h"
//#include "db_connectiondialog.h"
#include "db_dialogfactory.h"
//#include "db_mappingdialog.h"
//#include "db_selectmappingdialog.h"
//#include "rb_commandwidget.h"
//#include "rb_debug.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
//#include "rb_settings.h"

ACC_DialogFactory* ACC_DialogFactory::mActiveFactory = 0;

/**
 * Constructor
 */
ACC_DialogFactory::ACC_DialogFactory(RB_MainWindow* mw)
        : RB_DialogFactory(mw) {
    RB_DEBUG->print("ACC_DialogFactory::ACC_DialogFactory()");

    this->setPerspective(RB2::PerspectiveACC);
    DB_DIALOGFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_DialogFactory::~ACC_DialogFactory() {
    DB_DIALOGFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("ACC_DialogFactory::~ACC_DialogFactory() OK");
}

/**
 * Get unique (static) instance of dialog factory
 */
ACC_DialogFactory* ACC_DialogFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new ACC_DialogFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @return DB model factory
 */
RB_ModelFactory* ACC_DialogFactory::getModelFactory() {
    return ACC_MODELFACTORY;
}

/**
 * Get or create widget for dock- or MDI window,
 * could also be for dialog or mainwindow
 * @param type type of widget
 * @param parent parent for constructor of widget
 * @return widget
 */
RB_Widget* ACC_DialogFactory::getWidget(int type, QWidget* parent) {
    RB_Widget* wgt = findWidget(type);
    if (wgt) return wgt; // all widgets are unique

    switch (type) {
    case WidgetAllocnReport: {
        wgt = new ACC_AllocnReportWidget(parent);
        break;
    }
    case WidgetAsset: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabAsset);
        dw->show();

        // create requested widget
        wgt = new ACC_AssetWidget(parent);
        break;

    }
    case WidgetAssetValuation: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabAsset);
        dw->show();

        // create requested widget
        wgt = new ACC_AssetValuationWidget(parent);
        break;

    }
    case WidgetContactCustomerEdit: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabCustomer);
        dw->show();

        // create requested widget
        wgt = new ACC_ContactEditWidget(parent);
        break;
    }
    case WidgetContactSupplierEdit: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabSupplier);
        dw->show();

        // create requested widget
        bool parentIsCustomer = false;
        wgt = new ACC_ContactEditWidget(parent, parentIsCustomer);
        break;
    }
    case WidgetCostCenterBalance: {
        wgt = new ACC_CostCenterBalanceReportWidget(parent);
        break;

    }
    case WidgetCostCenterSummary: {
        wgt = new ACC_CostCenterReportWidget(parent);
        break;

    }
    case WidgetCustBranch: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabCustomer);
        dw->show();

        // create requested widget
        wgt = new ACC_CustBranchWidget(parent);
        break;

    }
    case WidgetCustCostPart: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabCustomer);
        dw->show();

        // create requested widget
        wgt = new ACC_CustCostPartWidget(parent);
        break;

    }
    case WidgetDocTransReport: {
        wgt = new ACC_DocTransReportWidget(parent);
        break;
    }
    case WidgetGlTransaction: {
        wgt = new ACC_GlTransactionWidget(parent);
        break;
    }
    case WidgetGlTransReport: {
        wgt = new ACC_GlTransReportWidget(parent);
        break;
    }
    case WidgetInvoiceAccruedReport: {
        wgt = new ACC_InvoiceAccruedReportWidget(parent);
        break;
    }
    case WidgetNavigation: {
        // DockWidget debtor (customer) main location and branch location
        wgt = new ACC_NavigationWidget(parent);
        break;
    }
    case WidgetPoInvoiceAgedReport : {
        wgt = new ACC_InvoiceAgedReportWidget(parent);
        ACC_InvoiceAgedReportWidget* wdgt = dynamic_cast<ACC_InvoiceAgedReportWidget*>(wgt);
        wdgt->setDocType(ACC2::TransCreditor);
        break;
    }
    case WidgetPrice: {
        // activate parent dockwidget product categories and product items
        QDockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabStockItem);
        dw->show();
        // Create requested widget
        wgt = new ACC_PriceWidget(parent);
        break;
    }
    case WidgetPurchaseOrderReport : {
        wgt = new ACC_OrderReportWidget(parent);
        break;
    }
    case WidgetPurchaseOrder: {
        // activate parent dockwidget supplier list
        QDockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabSupplier);
        dw->show();
        // Create requested widget
        wgt = new ACC_PurchaseOrderWidget(parent);
        break;
    }
    case WidgetSalesOrderReport : {
        wgt = new ACC_OrderReportWidget(parent);
        break;
    }
    case WidgetSalesOrder: {
        // activate parent dockwidget customer main location and branches
        QDockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabCustomer);
        dw->show();
        // Create requested widget
        wgt = new ACC_SalesOrderWidget(parent);
        break;
    }
    case WidgetSalesTaxReport: {
        wgt = new ACC_SalesTaxReportWidget(parent);
        break;
    }
    case WidgetSettingsReport : {
        wgt = new ACC_SettingReportWidget(parent);
        break;
    }
    case WidgetSoInvoiceAgedReport : {
        wgt = new ACC_InvoiceAgedReportWidget(parent);
        ACC_InvoiceAgedReportWidget* wdgt = dynamic_cast<ACC_InvoiceAgedReportWidget*>(wgt);
        wdgt->setDocType(ACC2::TransDebtor);
        break;
    }
    case WidgetStockCategory: {
        // activate parent dockwidget product categories and product items
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabStockItem);
        dw->show();
        // Create requested widget
        wgt = new ACC_StockCategoryWidget(parent);
        break;
    }
    case WidgetStockItem: {
        // activate parent dockwidget product categories and product items
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabStockItem);
        dw->show();
        // Create requested widget
        wgt = new ACC_StockItemWidget(parent);
        break;
    }
    case WidgetSupplCostPart: {
        // activate parent dockwidget, will findWidget() (dockWidget) first
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                        "dockwidget_navigation",
                                        Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabSupplier);
        dw->show();

        // create requested widget
        wgt = new ACC_SupplCostPartWidget(parent);
        break;

    }
    case WidgetSupplierDetail: {
        // activate parent dockwidget product categories and product items
        RB_DockWidget* dw = getDockWidget(ACC_DialogFactory::WidgetNavigation,
                                    "dockwidget_navigation",
                                    Qt::LeftDockWidgetArea);
        ACC_NavigationWidget* wdgt = dynamic_cast<ACC_NavigationWidget*>(dw->widget());
        wdgt->slotShowTab(ACC_NavigationWidget::TabSupplier);
        dw->show();
        // Create requested widget
        wgt = new ACC_SupplierDetailWidget(parent);
        break;
    }
    case WidgetTrialBalance: {
        wgt = new ACC_TrialBalanceWidget(parent);
        break;

    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_DialogFactory::getWidget() "
                        "non-existing widget requested ERROR");
        break;
    }

    if (wgt) {
        wgt->setWidgetType(type);
        wgt->setDialogFactory(this);
        // Set all widgets as unique widget, set 'inactive' via removeWidget()
        mWidgetList[type] = wgt;
        // not required here wgt->init();
    }

    return wgt;
}

/**
 * Request a dialog
 * @param type type of dialog
 * @return Dialog
 */
RB_Dialog* ACC_DialogFactory::
getDialog(int type) {
    RB_DEBUG->print("ACC_DialogFactory::getDialog()");

    RB_Dialog* dlg = NULL;

    switch (type) {
    case DialogAccountControl: {
        dlg = new ACC_AccountControlDialog(mMainWindow);
        break;
    }
    case DialogAccountGroup: {
        dlg = new ACC_AccountGroupDialog(mMainWindow);
        break;
    }
    case DialogArea: {
        dlg = new ACC_AreaDialog(mMainWindow);
        break;
    }
    case DialogAssetType: {
        dlg = new ACC_AssetTypeDialog(mMainWindow);
        break;
    }
    case DialogBankCsvMapping: {
        dlg = new ACC_BankMappingDialog(mMainWindow);
        break;
    }
    case DialogBankImport: {
        dlg = new ACC_BankImportCheckDialog(mMainWindow);
        break;
    }
    case DialogChartMaster: {
        dlg = new ACC_ChartMasterDialog(mMainWindow);
        break;
    }
    case DialogCogsGlPosting: {
        dlg = new ACC_CogsGlPostingDialog(mMainWindow);
        break;
    }
    case DialogCostCenterBudget: {
        dlg = new ACC_CostCenterBudgetDialog(mMainWindow);
        break;
    }
    case DialogCostGroup: {
        dlg = new ACC_CostGroupDialog(mMainWindow);
        break;
    }
    case DialogCurrency: {
        dlg = new ACC_CurrencyDialog(mMainWindow);
        break;
    }
    case DialogDebtorType: {
        dlg = new ACC_DebtorTypeDialog(mMainWindow);
        break;
    }
    case DialogFactorCompany: {
        dlg = new ACC_FactorCompanyDialog(mMainWindow);
        break;
    }
    case DialogHoldReason: {
        dlg = new ACC_HoldReasonDialog(mMainWindow);
        break;
    }
    case DialogLocation: {
        dlg = new ACC_LocationDialog(mMainWindow);
        break;
    }
    case DialogPaymentTerm: {
        dlg = new ACC_PaymentTermDialog(mMainWindow);
        break;
    }
    case DialogProject: {
        dlg = new ACC_ProjectDialog(mMainWindow);
        break;
    }
    case DialogProjectEdit: {
        dlg = new ACC_ProjectEditDialog(mMainWindow);
        break;
    }
    case DialogPurchaseGlPosting: {
        dlg = new ACC_PurchaseGlPostingDialog(mMainWindow);
        break;
    }
    case DialogSalesGlPosting: {
        dlg = new ACC_SalesGlPostingDialog(mMainWindow);
        break;
    }
    case DialogSalesMan: {
        dlg = new ACC_SalesManDialog(mMainWindow);
        break;
    }
    case DialogSalesTaxPosting: {
        dlg = new ACC_SalesTaxPostingDialog(mMainWindow);
        break;
    }
    case DialogSalesType: {
        dlg = new ACC_SalesTypeDialog(mMainWindow);
        break;
    }
    case DialogSelectChartMaster: {
        dlg = new ACC_SelectChartMasterDialog(mMainWindow);
        break;
    }
    case DialogSelectContact: {
        dlg = new ACC_SelectContactDialog(mMainWindow);
        break;
    }
    case DialogSelectCostCenter: {
        dlg = new ACC_SelectCostCenterDialog(mMainWindow);
        break;
    }
    case DialogSelectCostGroup: {
        dlg = new ACC_SelectCostGroupDialog(mMainWindow);
        break;
    }
    case DialogSelectCustBranch: {
        dlg = new ACC_SelectCustBranchDialog(mMainWindow);
        break;
    }
    case DialogSelectStockMaster: {
        dlg = new ACC_SelectStockItemDialog(mMainWindow);
        break;
    }
    case DialogSelectSupplier: {
        dlg = new ACC_SelectSupplierDialog(mMainWindow);
        break;
    }
    case DialogShipper: {
        dlg = new ACC_ShipperDialog(mMainWindow);
        break;
    }
    case DialogSysType: {
        dlg = new ACC_SysTypeDialog(mMainWindow);
        break;
    }
    case DialogTaxAuthRate: {
        dlg = new ACC_TaxAuthRateDialog(mMainWindow);
        break;
    }
    case DialogTaxCategory: {
        dlg = new ACC_TaxCategoryDialog(mMainWindow);
        break;
    }
    case DialogTaxGroupAuth: {
        dlg = new ACC_TaxGroupAuthDialog(mMainWindow);
        break;
    }
    case DialogTaxProvince: {
        dlg = new ACC_TaxProvinceDialog(mMainWindow);
        break;
    }
    case DialogUnitOfMeasure: {
        dlg = new ACC_UnitOfMeasureDialog(mMainWindow);
        break;
    }
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_DialogFactory::getDialog(type)"
                        "non-existing dialog requested ERROR");
        requestWarningDialog("ACC_DialogFactory::getDialog() ERROR\n"
                             "Non-existing dialog requested.");
        break;
    }

    if (dlg) {
        dlg->init();
    }
    return dlg;
}

/**
 * Update signal slot user interface connections
 */
void ACC_DialogFactory::updateUiConnections() {
    RB_DEBUG->print("ACC_DialogFactory::updateUiConnections()");

    // For dock windows

}

/**
 * Called when an action needs to communicate 'message' to the user.
 */
void ACC_DialogFactory::commandMessage(const RB_String& message) {
    DB_DIALOGFACTORY->commandMessage(message);
}

/**
 * Called when an action needs to communicate a 'message' list to the user.
 */
void ACC_DialogFactory::commandMessage(const RB_StringList& messageList) {
    DB_DIALOGFACTORY->commandMessage(messageList);
}

/**
 * @brief ACC_DialogFactory::slotEmitBankStatementsImported
 */
void ACC_DialogFactory::emitBankStatementsImported() {
    emit bankStatementsImported();
}
