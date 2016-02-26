/*****************************************************************
 * $Id: acc_modelfactory.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Dec 6, 2009 11:29 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_modelfactory.h"

#include "acc_objectfactory.h"
#include "acc_qasysacctcontrol.h"
#include "db_modelfactory.h"
#include "rb_database.h"

ACC_ModelFactory* ACC_ModelFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
ACC_ModelFactory::ACC_ModelFactory(RB_MainWindow* mw) : RB_ModelFactory(mw) {
    RB_DEBUG->print("ACC_ModelFactory::ACC_ModelFactory()");
    this->setObjectFactory(ACC_OBJECTFACTORY);
    DB_MODELFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_ModelFactory::~ACC_ModelFactory() {
    DB_MODELFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("ACC_ModelFactory::~ACC_ModelFactory() OK");
}

/**
 * Create instance of factory
 */
ACC_ModelFactory* ACC_ModelFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new ACC_ModelFactory(mw);
    }
    return mActiveFactory;
}

/**
 * @param type model type
 * @param shared true if model is to be created as unique/single/shared model
 * @return model manager
 */
RB_MmProxy* ACC_ModelFactory::getModel(int type, bool shared) {
    RB_MmProxy* model = NULL;

    if (shared) {
        std::map<int, RB_MmProxy*>::iterator iter;
        iter = mModelList.find(type);
        if (iter != mModelList.end()) {
            // shared model 1- exists or 2- has been removed/deleted and set to NULL
            model = (*iter).second;
        }
        // Model already created
        if (model) return model;
    }

    QSqlDatabase db = getDatabase();

    switch (type) {
    case ModelAccountControl:
        model = getTableModel(db, mObjectFactory, type, "ACC_AccountControlList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "controlnumber");
        break;
    case ModelAccountGroup:
        model = getTreeModel(db, mObjectFactory, type, "ACC_AccountGroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "sequenceintb");
        break;
    case ModelArea:
        model = getTableModel(db, mObjectFactory, type, "ACC_AreaList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "areacode");
        break;
    case ModelAsset:
        model = getTableModel(db, mObjectFactory, type, "ACC_AssetList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "assetcode");
        break;
    case ModelAssetCustAttr:
        model = getTableModel(db, mObjectFactory, type, "ACC_AssetCustAttrList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "seqno");
        break;
    case ModelAssetSupplAttr:
        model = getTableModel(db, mObjectFactory, type, "ACC_AssetSupplAttrList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "seqno");
        break;
    case ModelAssetType:
        model = getTableModel(db, mObjectFactory, type, "ACC_AssetTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "mname");
        break;
    case ModelBankMapping :
        model = getTableModel(db, mObjectFactory, type, "ACC_BankMappingList", shared);
        break;
    case ModelBankTrans :
        model = getTableModel(db, mObjectFactory, type, "ACC_BankTransList", shared);
        model->setDisplayRole(11, RB2::AlignFinancialAmount);
        break;
    case ModelChartMaster:
        model = getTableModel(db, mObjectFactory, type, "ACC_ChartMasterList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "accountcode");
        break;
    case ModelCogsGlPosting:
        model = getTableModel(db, mObjectFactory, type, "ACC_CogsGlPostingList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "area_id"); // relational name
        break;
    case ModelContactCustomer:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelContactSupplier:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelCostBudget:
        model = getTableModel(db, mObjectFactory, type, "ACC_CostBudgetList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "period");
        model->setDisplayRole(9, RB2::AlignFinancialAmount);
        model->setDisplayRole(10, RB2::AlignFinancialAmount);
        break;
    case ModelCostCenter:
        model = getTableModel(db, mObjectFactory, type, "ACC_CostCenterList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "centercode");
        break;
    case ModelCostGroup:
        model = getTreeModel(db, mObjectFactory, type, "ACC_CostGroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "sequenceno");
        break;
    case ModelCreditorTrans :
        model = getTableModel(db, mObjectFactory, type, "ACC_CreditorTransList", shared);
        model->setDisplayRole(11, RB2::AlignFinancialAmount);
        model->setDisplayRole(13, RB2::AlignFinancialAmount);
        model->setDisplayRole(15, RB2::AlignFinancialAmount);
        model->setDisplayRole(17, RB2::AlignFinancialAmount);
        break;
    case ModelCurrency:
        model = getTableModel(db, mObjectFactory, type, "ACC_CurrencyList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "currcode");
        break;
    case ModelCustCostPart:
        model = getTableModel(db, mObjectFactory, type, "ACC_CustCostPartList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "shortname");
        break;
    case ModelCustomerNote:
        model = getTableModel(db, mObjectFactory, type, "ACC_CustomerNoteList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "subject");
        break;
    case ModelCustomer:
        model = getTableModel(db, mObjectFactory, type, "ACC_CustomerList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "customerno");
        break;
    case ModelDebtorTrans :
        model = getTableModel(db, mObjectFactory, type, "ACC_DebtorTransList", shared);
        model->setDisplayRole(11, RB2::AlignFinancialAmount);
        model->setDisplayRole(13, RB2::AlignFinancialAmount);
        model->setDisplayRole(15, RB2::AlignFinancialAmount);
        model->setDisplayRole(17, RB2::AlignFinancialAmount);
        break;
    case ModelDebtorType:
        model = getTableModel(db, mObjectFactory, type, "ACC_DebtorTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "typename");
        break;
    case ModelFactorCompany:
        model = getTableModel(db, mObjectFactory, type, "ACC_FactorCompanyList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "company");
        break;
//    case ModelGlTrans :
//        model = getTableModel(db, mObjectFactory, type, "ACC_GlTransList", shared);
//        model->setSourceSortOrder(RB2::SortOrderDescending, "transdate");
//        model->setDisplayRole(15, RB2::AlignFinancialAmount);
//        break;
    case ModelHoldReason:
        model = getTableModel(db, mObjectFactory, type, "ACC_HoldReasonList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "reasoncode");
        break;
    case ModelImportBank:
        // Custom in-memory model, mObjectFactory not used, root set later
        model = getInMemoryTableModel(NULL, mObjectFactory,
                                      type, "ACC_ImportBankList", shared);
        break;
    case ModelImportBankTrans:
        // In-memory model, root set later
        model = getInMemoryTableModel(NULL, mObjectFactory,
                                      type, "ACC_BankTransList", shared);
        model->setDisplayRole(11, RB2::AlignFinancialAmount);
        break;
    case ModelImportTransDoc:
        // In-memory model, root set later
        model = getInMemoryTableModel(NULL, mObjectFactory,
                                      type, "ACC_TransDocList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "transno");
        model->setDisplayRole(16, RB2::AlignFinancialAmount);
        model->setDisplayRole(17, RB2::AlignFinancialAmount);
        model->setDisplayRole(18, RB2::AlignFinancialAmount);
        model->setDisplayRole(22, RB2::AlignFinancialAmount);
        break;
    case ModelLocation:
        model = getTableModel(db, mObjectFactory, type, "ACC_LocationList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "locationname");
        break;
    case ModelMemoTrans:
        model = getTableModel(db, mObjectFactory, type, "ACC_MemoTransList", shared);
        model->setDisplayRole(11, RB2::AlignFinancialAmount);
        break;
    case ModelPaymentTerm:
        model = getTableModel(db, mObjectFactory, type, "ACC_PaymentTermList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "termsindicator");
        break;
    case ModelPrice:
        model = getTableModel(db, mObjectFactory, type, "ACC_PriceList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "salestype_id"); // also relational name
        break;
    case ModelProject:
        // ACC_ProjectList actually does not exist but required to create model
        model = getTableModel(db, mObjectFactory, type, "ACC_ProjectList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "created");
        break;
    case ModelPurchaseGlPosting:
        model = getTableModel(db, mObjectFactory, type, "ACC_PurchaseGlPostingList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "taxgroup_id"); // relational name
        break;
    case ModelPurchaseOrderDetail :
        model = getTableModel(db, mObjectFactory, type, "ACC_PurchaseOrderDetailList", shared);
        break;
    case ModelPurchaseOrder :
        model = getTableModel(db, mObjectFactory, type, "ACC_PurchaseOrderList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "orderno");
        break;
    case ModelPurchData:
        model = getTableModel(db, mObjectFactory, type, "ACC_PurchDataList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "supplier_partno");
        model->setDisplayRole(10, RB2::AlignFinancialAmount); // price
        break;
    case ModelSalesGlPosting:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesGlPostingList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "area_id"); // relational name
        break;
    case ModelSalesMan:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesManList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "salesmanname");
        break;
    case ModelSalesOrderDetail:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesOrderDetailList", shared);
        break;
    case ModelSalesOrder:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesOrderList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "orderno");
        break;
    case ModelSalesTaxPosting:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesTaxPostingList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "taxauth_id"); // relational name
        break;
    case ModelSalesType:
        model = getTableModel(db, mObjectFactory, type, "ACC_SalesTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "typeabbrev");
        break;
    case ModelSelectContact:
        model = getTableModel(db, mObjectFactory, type, "ACC_ContactList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "lastname");
        break;
    case ModelShipper:
        model = getTableModel(db, mObjectFactory, type, "ACC_ShipperList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "shippername");
        break;
    case ModelStockCategory:
        model = getTreeModel(db, mObjectFactory, type, "ACC_StockCategoryList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "categorycode");
        break;
    case ModelStockMaster:
        model = getTableModel(db, mObjectFactory, type, "ACC_StockMasterList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "stockcode");
        break;
    case ModelSupplCostPart:
        model = getTableModel(db, mObjectFactory, type, "ACC_SupplCostPartList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "shortname");
        break;
    case ModelSupplierNote:
        model = getTableModel(db, mObjectFactory, type, "ACC_SupplierNoteList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "subject");
        break;
    case ModelSupplier:
        model = getTableModel(db, mObjectFactory, type, "ACC_SupplierList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "suppliercode");
        break;
    case ModelSystemAccountControl: // Refer ACC_AccountControlDialog
        // Custom in-memory model, mObjectFactory not used
        model = getInMemoryTableModel(ACC_QASYSACCTCONTROL->getRoot(), mObjectFactory,
                                      type, "ACC_SysAcctControlList", shared);
        break;
    case ModelSysType:
        model = getTableModel(db, mObjectFactory, type, "ACC_SysTypeList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "systypename");
        break;
    case ModelTaxAuthority:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxAuthorityList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "description");
        break;
    case ModelTaxAuthRate:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxAuthRateList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "dispatchtaxprovince_id"); // relational name
        break;
    case ModelTaxCategory:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxCategoryList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "taxcatname");
        break;
    case ModelTaxGroup:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxGroupList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "taxgroupdescr");
        break;
    case ModelTaxGroupAuth:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxGroupAuthList", shared);
        break;
    case ModelTaxProvince:
        model = getTableModel(db, mObjectFactory, type, "ACC_TaxProvinceList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "taxprovincename");
        break;
    case ModelTransAllocn:
        model = getTableModel(db, mObjectFactory, type, "ACC_TransAllocnList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "datealloc");
        break;
    case ModelTransDoc:
        model = getTableModel(db, mObjectFactory, type, "ACC_TransDocList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "transno");
        model->setDisplayRole(16, RB2::AlignFinancialAmount);
        model->setDisplayRole(17, RB2::AlignFinancialAmount);
        model->setDisplayRole(18, RB2::AlignFinancialAmount);
        model->setDisplayRole(22, RB2::AlignFinancialAmount);
        break;
    case ModelTransDocSelect:
        model = getTableModel(db, mObjectFactory, type, "ACC_TransDocList", shared);
        model->setSourceSortOrder(RB2::SortOrderDescending, "transno");
        model->setDisplayRole(16, RB2::AlignFinancialAmount);
        model->setDisplayRole(17, RB2::AlignFinancialAmount);
        model->setDisplayRole(18, RB2::AlignFinancialAmount);
        model->setDisplayRole(22, RB2::AlignFinancialAmount);
        break;
    case ModelUnitOfMeasure:
        model = getTableModel(db, mObjectFactory, type, "ACC_UnitOfMeasureList", shared);
        model->setSourceSortOrder(RB2::SortOrderAscending, "unitname");
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_ModelFactory::getModel()"
                        "non-existing model ERROR");
        break;
    }

    return model;
}

/**
 * Get parent model. Used in child models or models depending on selection of
 * a parent model. Done separately here and not in MDI window because
 * the parent model could be in a different navigation dock widget
 * @param type child model type
 * @return parent model or NULL if not exists
 */
RB_MmProxy* ACC_ModelFactory::getParentModel(int type) {
    RB_MmProxy* model = NULL;
    std::map<int, RB_MmProxy*>::iterator iter;

    switch (type) {
    case ModelAccountControl:
        iter = mModelList.find(ModelNone);
        break;
    case ModelAccountGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelArea:
        iter = mModelList.find(ModelNone);
        break;
    case ModelAsset:
        iter = mModelList.find(ModelNone);
        break;
    case ModelAssetCustAttr:
        iter = mModelList.find(ModelAsset);
        break;
    case ModelAssetSupplAttr:
        iter = mModelList.find(ModelAsset);
        break;
    case ModelAssetType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelBankMapping :
        iter = mModelList.find(ModelNone);
        break;
    case ModelBankTrans :
        iter = mModelList.find(ModelNone);
        break;
    case ModelChartMaster:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCogsGlPosting:
        iter = mModelList.find(ModelNone);
        break;
    case ModelContactCustomer:
        iter = mModelList.find(ModelCustomer);
        break;
    case ModelContactSupplier:
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelCostBudget:
        iter = mModelList.find(ModelCostCenter);
        break;
    case ModelCostCenter:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCostGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCreditorTrans :
        iter = mModelList.find(ModelNone);
        break;
    case ModelCurrency:
        iter = mModelList.find(ModelNone);
        break;
    case ModelCustCostPart:
        iter = mModelList.find(ModelCustomer);
        break;
    case ModelCustomerNote:
        iter = mModelList.find(ModelCustomer);
        break;
    case ModelCustomer:
        iter = mModelList.find(ModelNone);
        break;
    case ModelDebtorTrans :
        iter = mModelList.find(ModelNone);
        break;
    case ModelDebtorType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelFactorCompany:
        iter = mModelList.find(ModelNone);
        break;
//    case ModelGlTrans :
//        iter = mModelList.find(ModelNone);
//        break;
    case ModelHoldReason:
        iter = mModelList.find(ModelNone);
        break;
    case ModelImportBank:
        iter = mModelList.find(ModelNone);
        break;
    case ModelImportBankTrans: // actual parent is ACC_Project
        iter = mModelList.find(ModelImportTransDoc);
        break;
    case ModelImportTransDoc:
        iter = mModelList.find(ModelNone);
        break;
    case ModelLocation:
        iter = mModelList.find(ModelNone);
        break;
    case ModelMemoTrans:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPaymentTerm:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPrice:
        iter = mModelList.find(ModelStockMaster);
        break;
    case ModelProject:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPurchaseGlPosting:
        iter = mModelList.find(ModelNone);
        break;
    case ModelPurchaseOrderDetail :
        iter = mModelList.find(ModelPurchaseOrder);
        break;
    case ModelPurchaseOrder :
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelPurchData:
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelSalesGlPosting:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSalesMan:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSalesOrderDetail:
        iter = mModelList.find(ModelSalesOrder);
        break;
    case ModelSalesOrder:
        iter = mModelList.find(ModelCustomer);
        break;
    case ModelSalesTaxPosting:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSalesType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSelectContact:
        iter = mModelList.find(ModelNone);
        break;
    case ModelShipper:
        iter = mModelList.find(ModelNone);
        break;
    case ModelStockCategory:
        iter = mModelList.find(ModelNone);
        break;
    case ModelStockMaster:
        iter = mModelList.find(ModelStockCategory);
        break;
    case ModelSystemAccountControl:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSupplCostPart:
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelSupplier:
        iter = mModelList.find(ModelNone);
        break;
    case ModelSupplierNote:
        iter = mModelList.find(ModelSupplier);
        break;
    case ModelSysType:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTaxAuthority:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTaxAuthRate:
        iter = mModelList.find(ModelTaxAuthority);
        break;
    case ModelTaxCategory:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTaxGroup:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTaxGroupAuth:
        iter = mModelList.find(ModelTaxGroup);
        break;
    case ModelTaxProvince:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTransAllocn:
        iter = mModelList.find(ModelNone);
        break;
    case ModelTransDoc:
        iter = mModelList.find(ModelChartMaster);
        break;
    case ModelTransDocSelect:
        iter = mModelList.find(ModelNone);
        break;
    case ModelUnitOfMeasure:
        iter = mModelList.find(ModelNone);
        break;
    default:
        iter = mModelList.find(ModelNone);
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_ModelFactory::getParentModel() "
                        "non-existing (parent) model ERROR");
        break;
    }
    if (iter != mModelList.end()) {
        model = (*iter).second;
    }
    return model;
}

