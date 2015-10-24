/*****************************************************************
 * $Id: acc_dialogfactory.h 2202 2015-01-11 19:39:36Z rutger $
 * Created: Dec 20, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_DIALOGFACTORY_H
#define ACC_DIALOGFACTORY_H

#include <QObject>
#include "rb_dialogfactory.h"
#include "rb_modelfactory.h"

class RB_ModelFactory;

#define ACC_DIALOGFACTORY ACC_DialogFactory::getInstance()

/**
 * Dialog and widget factory for DB project
 */
class ACC_DialogFactory : public RB_DialogFactory {

    Q_OBJECT

public:
    virtual ~ACC_DialogFactory();
    static ACC_DialogFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_ModelFactory* getModelFactory();

    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual RB_Dialog* getDialog(int type);
    virtual void updateUiConnections();
    virtual void commandMessage(const RB_String& message);
    virtual void commandMessage(const RB_StringList& message);

    void emitBankStatementsImported();


    /**
     * DB Dialog types, example = 113XXX:
     * - 11 = ACC,
     * - 3 = dialog,
     * - XXX = sequence number
     * 113100 is starting number leaving 113000 to 113099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum DialogType {
        // start number from 113000
//        DialogTest = 113000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */
        // start numbering from 113100
        DialogNone = 113100,            /**< Invalid dialog */
        DialogAccountGroup,             /**< Edit account groups dialog */
        DialogAccountControl,           /**< Edit account control dialog */
        DialogArea,                     /**< Edit sales area dialog */
        DialogAssetType,                /**< Edit assettype dialog */
        DialogBankCsvMapping,           /**< Edit CSV bank data mapping dialog */
        DialogBankImport,               /**< Import bank statement dialog */
        DialogChartMaster,              /**< Edit chart of accounts */
        DialogCogsGlPosting,            /**< Edit COGS postings dialog */
        DialogCostCenterBudget,         /**< Edit Cost center and budget dialog */
        DialogCostGroup,                /**< Edit Cost center group dialog */
        DialogCurrency,                 /**< Edit currency dialog */
        DialogDebtorType,               /**< Edit customer type dialog */
        DialogFactorCompany,            /**< Edit factor companies dialog */
        DialogHoldReason,               /**< Edit credit status or hold reasons customer dialog */
        DialogLocation,                 /**< Edit stock location dialog */
        DialogPaymentTerm,              /**< Edit payment terms dialog */
        DialogProject,                  /**< Select project (company) from database dialog */
        DialogProjectEdit,              /**< Edit project (company) dialog */
        DialogPurchaseGlPosting,        /**< Edit purchase postings dialog */
        DialogSalesGlPosting,           /**< Edit sales postings dialog */
        DialogSalesMan,                 /**< Edit salesman dialog */
        DialogSalesTaxPosting,          /**< Edit sales tax posting dialog */
        DialogSalesType,                /**< Edit sales type or price lists dialog */
        DialogSelectChartMaster,        /**< Select account from chart master */
        DialogSelectContact,            /**< Select contact */
        DialogSelectCostCenter,         /**< Select cost center */
        DialogSelectCostGroup,          /**< Select cost center group */
        DialogSelectCustBranch,         /**< Select customer branch dialog */
        DialogSelectMapping,            /**< Select mapping dialog */
        DialogSelectStockMaster,        /**< Select stock item dialog */
        DialogSelectSupplier,           /**< Select supplier dialog */
        DialogShipper,                  /**< Edit shipper dialog */
        DialogSysType,                  /**< Edit system type numbers */
        DialogTaxAuthRate,              /**< Edit tax authorities and rates dialog */
        DialogTaxCategory,              /**< Edit tax categories for stock items dialog */
        DialogTaxGroupAuth,             /**< Edit tax group and authority taxes for customer and supplier */
        DialogTaxProvince,              /**< Edit tax provinces for warehouses dialog */
        DialogUnitOfMeasure,            /**< Edit units of measurement dialog */
        DialogDefault                   /**< Default- or all dialogs */
    };

    /**
     * DB Widget/dialog manager types, example = 112XXX:
     * - 11 = DB,
     * - 2 = widget,
     * - XXX = sequence number
     * 112100 is starting number leaving 112000 to 112099 for test/debug cases
     * Refer also ModelType and DialogType
     */
    enum WidgetType {
        // start numbering from 112000
//        WidgetTestDialog = 112000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
        // start numbering from 112100
        WidgetNone = 112100,            /**< Invalid model */
        WidgetAllocnReport,             /**< Allocation report widget */
        WidgetAsset,                    /**< Asset widget */
        WidgetAssetValuation,           /**< Asset or property valuation widget */
        WidgetBankPaymentReport,        /**< Customer or supplier bank payment report */
        WidgetContactCustomerEdit,      /**< Edit customer contacts widget */
        WidgetContactSupplierEdit,      /**< Edit supplier contacts widget */
        WidgetCostCenterBalance,        /**< Cost center balance report widget */
        WidgetCostCenterSummary,        /**< Cost center summary report widget */
        WidgetCustBranch,               /**< Customer and branch details */
        WidgetCustCostPart,             /**< Customer cost center part (setting) widget */
//        WidgetDebtorsMaster,            /**< Debtor master (customer) and branch list */
        WidgetDocTransReport,           /**< Document transaction report widget */
        WidgetGlTransaction,            /**< General Ledger transaction widget */
        WidgetGlTransReport,            /**< GL transaction report widget */
        WidgetInvoiceAccruedReport,     /**< Sales/purchase accrued (still open) report widget */
        WidgetNavigation,               /**< Dockwidget with customer/branch, stockitem, supplier, etc. */
        WidgetPoInvoiceAgedReport,      /**< Purchase invoices aged report widget */
        WidgetPrice,                    /**< Item price (setting) widget */
        WidgetPurchaseOrderReport,      /**< Purchase order report widget */
        WidgetPurchaseOrder,            /**< Purchase orders, detail list and details */
        WidgetSalesOrderReport,         /**< Sales order report widget */
        WidgetSalesOrder,               /**< Sales orders, detail list and details */
        WidgetSalesTaxReport,           /**< Sales tax report, summary and detailed */
        WidgetSettingsReport,           /**< Various settings report */
        WidgetSoInvoiceAgedReport,      /**< Sales invoices aged report widget */
        WidgetStockCategory,            /**< Stock master categories and subcategories */
        WidgetStockItem,                /**< Stock master item details*/
        WidgetSupplCostPart,            /**< Supplier cost center part (setting) widget */
        WidgetSupplierDetail,           /**< Supplier details */
        WidgetTrialBalance,             /**< View trial balance */
        WidgetDefault                   /**< Default- or all models */
    };

signals:
    void bankStatementsImported();

private:
    //! Private constructor
    ACC_DialogFactory(RB_MainWindow* mw);
    //! Unique (static) instance of factory
    static ACC_DialogFactory* mActiveFactory;

};

#endif // ACC_DIALOGFACTORY_H
