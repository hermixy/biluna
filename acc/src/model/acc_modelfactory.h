/*****************************************************************
 * $Id: acc_modelfactory.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Dec 20, 2009 11:29 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MODELFACTORY_H
#define ACC_MODELFACTORY_H

#include "rb_modelfactory.h"

#define ACC_MODELFACTORY ACC_ModelFactory::getInstance()

/**
 * Model factory for the Accounting ACC plugin
 */
class ACC_ModelFactory : public RB_ModelFactory {

    Q_OBJECT

public:
    virtual ~ACC_ModelFactory();
    static ACC_ModelFactory* getInstance(RB_MainWindow* mw = 0);

    virtual RB_MmProxy* getModel(int type, bool shared = true);
    virtual RB_MmProxy* getParentModel(int type);

    /**
     * Model manager types, example = 111XXX:
     * - 11 = ACC,
     * - 1 = model,
     * - XXX = sequence number
     * 111100 is starting number leaving 111000 to 111099 for test/debug cases
     * Refer also WidgetType and DialogType
     */
    enum ModelType {
        // start numbering from 111000
//        ModelTest = 111000,             /**< Table model */
//        ModelTestChild,                 /**< Child table model */
//        ModelTestDialog,                /**< Dialog model */
//        ModelTestTree,                  /**< Tree model */
//        ModelTestTreeChild,             /**< Child Tree model */
        // start numbering from 111100
        ModelNone = 111100,             /**< Invalid model */
        ModelAccountControl,            /**< Account control model, identifies journal and sales tax type */
        ModelAccountGroup,              /**< Account groups TREE model, fixed assets, liabilities */
        ModelArea,                      /**< Sales area model */
        ModelAsset,                     /**< Asset model */
        ModelAssetType,                 /**< Assettype model */
        ModelAssetCustAttr,             /**< Asset-customer attribute model */
        ModelAssetSupplAttr,            /**< Asset-supplier attribute model */
        ModelBankMapping,               /**< Bank import mapping model */
        ModelBankTrans,                 /**< Bank transaction model or item of bank document */
        ModelChartMaster,               /**< Chart of accounts model, used in ACC_GlTransactionWidget as parent */
        ModelCogsGlPosting,             /**< Cost of goods sold postings to General Ledger */
        ModelContactCustomer,           /**< Contacts of a customer */
        ModelContactSupplier,           /**< Contacts of a supplier */
        ModelCostBudget,                /**< Cost center budget */
        ModelCostCenter,                /**< Cost or profit center */
        ModelCostGroup,                 /**< Cost center group */
        ModelCreditorTrans,             /**< Creditor transaction model or item of creditor document (purchase invoice) */
        ModelCurrency,                  /**< Currency model */
        ModelCustCostPart,              /**< Customer cost center part model */
        ModelCustomerNote,              /**< Customer notes model */
        ModelCustomer,                  /**< Customer branch model */
        ModelDebtorTrans,               /**< Debtor transaction model or item debtor document (sales invoice) */
        ModelDebtorType,                /**< Debtor type model, for additional filtering of customer */
        ModelFactorCompany,             /**< Factor companies model */
//        ModelGlTrans,
        ModelHoldReason,                /**< Credit status or hold reasons for invoice of customer */
        ModelImportBank,                /**< Import bank statements model, in-memory model */
        ModelImportBankTrans,           /**< Import bank transaction model, in-memory model */
        ModelImportTransDoc,            /**< Import bank transaction document model, in-memory model */
        ModelLocation,                  /**< Warehouse location model */
        ModelMemoTrans,                 /**< Memorial transaction model or item of memorial document */
        ModelPaymentTerm,               /**< Payment terms model */
        ModelPrice,                     /**< Selling price model */
        ModelProject,                   /**< Project model */
        ModelPurchaseGlPosting,         /**< Purchase general ledger postings */
        ModelPurchaseOrderDetail,       /**< Supplier purchase order details */
        ModelPurchaseOrder,             /**< Supplier purchase orders */
        ModelPurchData,                 /**< Supplier purchase data model, about items supplier can supply */
        ModelSalesGlPosting,            /**< Sales general ledger postings */
        ModelSalesMan,                  /**< Sales person model */
        ModelSalesOrderDetail,          /**< Sales order details */
        ModelSalesOrder,                /**< Sales orders */
        ModelSalesTaxPosting,           /**< Sales tax posting model */
        ModelSalesType,                 /**< Sales types or price list model */
        ModelSelectContact,             /**< Selection of contact only */
        ModelShipper,                   /**< Shipper or freight forwarder model */
        ModelStockCategory,             /**< Stock categories and subcategories TREE for stock items */
        ModelStockMaster,               /**< Stock master or stock items, the actual products */
        ModelSupplCostPart,             /**< Supplier cost center part model */
        ModelSupplierNote,              /**< Supplier notes model */
        ModelSupplier,                  /**< Supplier of items, material or services */
        ModelSystemAccountControl,      /**< System account control model, in-memory */
        ModelSysType,                   /**< System type number model */
        ModelTaxAuthority,              /**< Tax authorities model */
        ModelTaxAuthRate,               /**< Tax authority rates model */
        ModelTaxCategory,               /**< Tax categories model, for example luxury items, exempt items */
        ModelTaxGroup,                  /**< Tax groups containing taxgroups taxes or tax authorities */
        ModelTaxGroupAuth,              /**< Tax group authorities, is container of tax authorities */
        ModelTaxProvince,               /**< Tax provinces of stock location or warehouses */
        ModelTransAllocn,               /**< Transaction allocation, e.g. invoice to bank receipt */
        ModelTransDoc,                  /**< Transaction documents, e.g. reference to invoice */
        ModelUnitOfMeasure,             /**< Units of measurement for stock items */
        ModelDefault                    /**< Default- or all models */
    };

private:
    ACC_ModelFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static ACC_ModelFactory* mActiveFactory;

};

#endif // ACC_MODELFACTORY_H
