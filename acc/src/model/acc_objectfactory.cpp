/*****************************************************************
 * $Id: acc_objectfactory.cpp 2191 2014-10-27 20:31:51Z rutger $
 * Created: Sep 1, 2009 8:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_objectfactory.h"

#include "acc_accountcontrol.h" // not part of database
#include "acc_accountgroup.h"
#include "acc_area.h"
#include "acc_asset.h"
#include "acc_assetattribute.h"
#include "acc_assetcustattr.h"
#include "acc_assetcustinter.h"
#include "acc_assetsupplattr.h"
#include "acc_assetsupplinter.h"
#include "acc_assettype.h"
#include "acc_bankmapping.h"
#include "acc_banktrans.h"
#include "acc_chartmaster.h"
#include "acc_cogsglposting.h"
#include "acc_contact.h"
#include "acc_costbudget.h"
#include "acc_costcenter.h"
#include "acc_costgroup.h"
#include "acc_costsum.h"
#include "acc_creditortrans.h"
#include "acc_currency.h"
#include "acc_custcostpart.h"
#include "acc_customernote.h"
#include "acc_customer.h"
//#include "acc_debtorsmaster.h"
#include "acc_debtortrans.h"
//#include "acc_debtortranstaxes.h"
#include "acc_debtortype.h"
#include "acc_factorcompany.h"
#include "acc_glsum.h"
#include "acc_gltrans.h"
#include "acc_holdreason.h"
#include "acc_location.h"
#include "acc_memotrans.h"
#include "acc_paymentterm.h"
#include "acc_price.h"
#include "acc_project.h"
#include "acc_purchaseglposting.h"
#include "acc_purchaseorderdetail.h"
#include "acc_purchaseorder.h"
#include "acc_purchdata.h"
#include "acc_salesglposting.h"
#include "acc_salesman.h"
#include "acc_salesorder.h"
#include "acc_salesorderdetail.h"
#include "acc_salestaxposting.h"
#include "acc_salestype.h"
#include "acc_shipper.h"
#include "acc_stockcategory.h"
#include "acc_stockmaster.h"
#include "acc_stockmove.h"
//#include "acc_stockmovetaxes.h"
#include "acc_supplcostpart.h"
#include "acc_suppliernote.h"
#include "acc_supplier.h"
//#include "acc_supptranstaxes.h"
#include "acc_sysacctcontrol.h"
//#include "acc_syssetting.h"
#include "acc_systype.h"
#include "acc_taxauthority.h"
#include "acc_taxauthrate.h"
#include "acc_taxcategory.h"
#include "acc_taxgroup.h"
#include "acc_taxgroupauth.h"
#include "acc_taxprovince.h"
#include "acc_transallocn.h"
#include "acc_transdoc.h"
#include "acc_unitofmeasure.h"
#include "db_objectfactory.h"
#include "db_project.h" // for system (DB global) settings
#include "rb_debug.h"
#include "rb_uuid.h"

ACC_ObjectFactory* ACC_ObjectFactory::mActiveObjectFactory = 0;


/**
 * Constructor
 */
ACC_ObjectFactory::ACC_ObjectFactory() {
    RB_DEBUG->print("ACC_ObjectFactory::ACC_ObjectFactory()");
    DB_OBJECTFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_ObjectFactory::~ACC_ObjectFactory() {
    DB_OBJECTFACTORY->unregisterFactory(this);
    mActiveObjectFactory = NULL;
    RB_DEBUG->print("ACC_ObjectFactory::~ACC_ObjectFactory() OK");
}

/**
 * Create or get instance
 * @return factory
 */
ACC_ObjectFactory* ACC_ObjectFactory::getInstance() {
    if (!mActiveObjectFactory) {
        mActiveObjectFactory = new ACC_ObjectFactory();
        // initiate factory
        mActiveObjectFactory->createModelStructure("ACC_Project");
        mActiveObjectFactory->setName("ACC_ObjectFactory");
    }
    return mActiveObjectFactory;
}

/**
 * Create new object, connect to- and make part of parent.
 * NOTE:
 *  ACC_AccountControlList
 *  ACC_AccountGroupList
 *  ACC_AreaList
 *  ACC_CostGroupList
 *  ACC_CurrencyList
 *  ACC_DebtorTypeList
 *  ACC_FactorCompanyList
 *  ACC_HoldReasonList
 *  ACC_LocationList
 *  ACC_PaymentTermList
 *  ACC_SalesManList
 *  ACC_SalesTypeList
 *  ACC_ShipperList
 *  ACC_TaxAuthorityList
 *  ACC_TaxCategoryList
 *  ACC_TaxGroupList
 *  ACC_TaxProvinceList
 *  ACC_UnitOfMeasureList
 * are part of DB_Project but are global system settings.
 * The relevant global models will set the filter to WHERE id<>'0'
 * instead of WHERE parent='acc_project.id'. Their parent
 * is set to 'default'.
 * @param id identification of object (Uuid)
 * @param parent parent list (container) of the object, if NULL the root object
 *        will be returned
 * @param name name of the parent list (container)
 * @param addToParent add new object to parent container
 * @return object, connected to parent
 */
RB_ObjectBase* ACC_ObjectFactory::newObject(const RB_String& id,
                                            RB_ObjectBase* parent,
                                            const RB_String& /* typeName */,
                                            bool addToParent) {
    RB_String uuid = id;
    RB_String str = "";

    if (parent != NULL) {
        str = parent->getName();
    } else {
        str = "ACC_"; // root
    }

    RB_ObjectContainer* list = NULL;
    RB_ObjectBase* obj = NULL;

    if (str == "ACC_") { // root
        obj = new ACC_Project(uuid, NULL, "ACC_Project", this);

        uuid = ""; // RB_Uuid::createUuid().toString(); No Uuid for ..Lists
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_BankMappingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_BankTransList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_ChartMasterList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CogsGlPostingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CostCenterList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CreditorTransList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CustomerList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_DebtorTransList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_GlTransList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_MemoTransList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_PurchaseGlPostingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesGlPostingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesTaxPostingList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_StockCategoryList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SupplierList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SysTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_TransAllocnList", this);
        obj->addObject(list);
        // for ACC global system settings only, the current DB project is root id
        list = new RB_ObjectContainer(uuid, obj, "DB_ProjectList", this);
        obj->addObject(list);

    } else if (str == "ACC_AccountControlList") {
        obj = new ACC_AccountControl(uuid, parent, "ACC_AccountControl", this);
    } else if (str == "ACC_AccountGroupList") {
        obj = new ACC_AccountGroup(uuid, parent, "ACC_AccountGroup", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_AccountGroupList", this);
        obj->addObject(list);
    } else if (str == "ACC_AreaList") {
        obj = new ACC_Area(uuid, parent, "ACC_Area", this);
    } else if (str == "ACC_AssetAttributeList") {
        obj = new ACC_AssetAttribute(uuid, parent, "ACC_AssetAttribute", this);
    } else if (str == "ACC_AssetList") {
        obj = new ACC_Asset(uuid, parent, "ACC_Asset", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetAttributeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetCustInterList", this);
        obj->addObject(list);
        // Not used, for tree model can have same type of children:
        //        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetList", this);
        //        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetSupplInterList", this);
        obj->addObject(list);
    } else if (str == "ACC_AssetCustAttrList") {
        obj = new ACC_AssetCustAttr(uuid, parent, "ACC_AssetCustAttr", this);
    } else if (str == "ACC_AssetCustInterList") {
        obj = new ACC_AssetCustInter(uuid, parent, "ACC_AssetCustInter", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetCustAttrList", this);
        obj->addObject(list);
    } else if (str == "ACC_AssetSupplAttrList") {
        obj = new ACC_AssetSupplAttr(uuid, parent, "ACC_AssetSupplAttr", this);
    } else if (str == "ACC_AssetSupplInterList") {
        obj = new ACC_AssetSupplInter(uuid, parent, "ACC_AssetSupplInter", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetSupplAttrList", this);
        obj->addObject(list);
    } else if (str == "ACC_AssetTypeList") {
        obj = new ACC_AssetType(uuid, parent, "ACC_AssetType", this);
    } else if (str == "ACC_BankMappingList") {
        obj = new ACC_BankMapping(uuid, parent, "ACC_BankMapping", this);
    } else if (str == "ACC_BankTransList") {
        obj = new ACC_BankTrans(uuid, parent, "ACC_BankTrans", this);
    } else if (str == "ACC_ChartMasterList") {
        obj = new ACC_ChartMaster(uuid, parent, "ACC_ChartMaster", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_TransDocList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_GlSumList", this);
        obj->addObject(list);
    } else if (str == "ACC_CogsGlPostingList") {
        obj = new ACC_CogsGlPosting(uuid, parent, "ACC_CogsGlPosting", this);
    } else if (str == "ACC_ContactList") {
        obj = new ACC_Contact(uuid, parent, "ACC_Contact", this);
    } else if (str == "ACC_CostBudgetList") {
        obj = new ACC_CostBudget(uuid, parent, "ACC_CostBudget", this);
    } else if (str == "ACC_CostCenterList") {
        obj = new ACC_CostCenter(uuid, parent, "ACC_CostCenter", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_CostBudgetList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CostSumList", this);
        obj->addObject(list);
    } else if (str == "ACC_CostGroupList") {
        obj = new ACC_CostGroup(uuid, parent, "ACC_CostGroup", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_CostGroupList", this);
        obj->addObject(list);
    } else if (str == "ACC_CostSumList") {
        obj = new ACC_CostSum(uuid, parent, "ACC_CostSum", this);
    } else if (str == "ACC_CreditorTransList") {
        obj = new ACC_CreditorTrans(uuid, parent, "ACC_CreditorTrans", this);
    } else if (str == "ACC_CurrencyList") {
        obj = new ACC_Currency(uuid, parent, "ACC_Currency", this);
    } else if (str == "ACC_CustCostPartList") {
        obj = new ACC_CustCostPart(uuid, parent, "ACC_CustCostPart", this);
    } else if (str == "ACC_CustomerNoteList") {
        obj = new ACC_CustomerNote(uuid, parent, "ACC_CustomerNote", this);
    } else if (str == "ACC_CustomerList") {
        obj = new ACC_Customer(uuid, parent, "ACC_Customer", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_ContactList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CustCostPartList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CustomerNoteList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesOrderList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SupplCostPartList", this);
        obj->addObject(list);
    } else if (str == "ACC_DebtorTransList") {
        obj = new ACC_DebtorTrans(uuid, parent, "ACC_DebtorTrans", this);
    } else if (str == "ACC_DebtorTypeList") {
        obj = new ACC_DebtorType(uuid, parent, "ACC_DebtorType", this);
    } else if (str == "ACC_FactorCompanyList") {
        obj = new ACC_FactorCompany(uuid, parent, "ACC_FactorCompany", this);
    } else if (str == "ACC_GlSumList") {
        obj = new ACC_GlSum(uuid, parent, "ACC_GlSum", this);
    } else if (str == "ACC_GlTransList") {
        obj = new ACC_GlTrans(uuid, parent, "ACC_GlTrans", this);
    } else if (str == "ACC_HoldReasonList") {
        obj = new ACC_HoldReason(uuid, parent, "ACC_HoldReason", this);
    } else if (str == "ACC_LocationList") {
        obj = new ACC_Location(uuid, parent, "ACC_Location", this);
    } else if (str == "ACC_MemoTransList") {
        obj = new ACC_MemoTrans(uuid, parent, "ACC_MemoTrans", this);
    } else if (str == "ACC_PaymentTermList") {
        obj = new ACC_PaymentTerm(uuid, parent, "ACC_PaymentTerm", this);
    } else if (str == "ACC_PriceList") {
        obj = new ACC_Price(uuid, parent, "ACC_Price", this);
    } else if (str == "ACC_PurchaseGlPostingList") {
        obj = new ACC_PurchaseGlPosting(uuid, parent, "ACC_PurchaseGlPosting", this);
    } else if (str == "ACC_PurchaseOrderDetailList") {
        obj = new ACC_PurchaseOrderDetail(uuid, parent, "ACC_PurchaseOrderDetail", this);
    } else if (str == "ACC_PurchaseOrderList") {
        obj = new ACC_PurchaseOrder(uuid, parent, "ACC_PurchaseOrder", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_PurchaseOrderDetailList", this);
        obj->addObject(list);
    } else if (str == "ACC_PurchDataList") {
        obj = new ACC_PurchData(uuid, parent, "ACC_PurchData", this);
    } else if (str == "ACC_SalesGlPostingList") {
        obj = new ACC_SalesGlPosting(uuid, parent, "ACC_SalesGlPosting", this);
    } else if (str == "ACC_SalesManList") {
        obj = new ACC_SalesMan(uuid, parent, "ACC_SalesMan", this);
    } else if (str == "ACC_SalesOrderDetailList") {
        obj = new ACC_SalesOrderDetail(uuid, parent, "ACC_SalesOrderDetail", this);
    } else if (str == "ACC_SalesOrderList") {
        obj = new ACC_SalesOrder(uuid, parent, "ACC_SalesOrder", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesOrderDetailList", this);
        obj->addObject(list);
    } else if (str == "ACC_SalesTaxPostingList") {
        obj = new ACC_SalesTaxPosting(uuid, parent, "ACC_SalesTaxPosting", this);
    } else if (str == "ACC_SalesTypeList") {
        obj = new ACC_SalesType(uuid, parent, "ACC_SalesType", this);
    } else if (str == "ACC_ShipperList") {
        obj = new ACC_Shipper(uuid, parent, "ACC_Shipper", this);
    } else if (str == "ACC_StockCategoryList") {
        obj = new ACC_StockCategory(uuid, parent, "ACC_StockCategory", this);

        uuid = "";
        // for tree model can have same type of children:
        list = new RB_ObjectContainer(uuid, obj, "ACC_StockCategoryList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_StockMasterList", this);
        obj->addObject(list);
    } else if (str == "ACC_StockMasterList") {
        obj = new ACC_StockMaster(uuid, parent, "ACC_StockMaster", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_PriceList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_StockMoveList", this);
        obj->addObject(list);
    } else if (str == "ACC_StockMoveList") {
        obj = new ACC_StockMove(uuid, parent, "ACC_StockMove", this);

//        uuid = "";
//        list = new RB_ObjectContainer(uuid, obj, "ACC_StockMoveTaxesList", this);
//        obj->addObject(list);
//    } else if (str == "ACC_StockMoveTaxesList") {
//        obj = new ACC_StockMoveTaxes(uuid, parent, "ACC_StockMoveTaxes", this);
    } else if (str == "ACC_SupplCostPartList") {
        obj = new ACC_SupplCostPart(uuid, parent, "ACC_SupplCostPart", this);
    } else if (str == "ACC_SupplierNoteList") {
        obj = new ACC_SupplierNote(uuid, parent, "ACC_SupplierNote", this);
    } else if (str == "ACC_SupplierList") {
        obj = new ACC_Supplier(uuid, parent, "ACC_Supplier", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_ContactList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_PurchDataList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_PurchaseOrderList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SupplierNoteList", this);
        obj->addObject(list);
    } else if (str == "ACC_SysAcctControlList") {
        // not part of the database, only for in-memory model
        obj = new ACC_SysAcctControl(uuid, parent, "ACC_SysAcctControl", this);
    } else if (str == "ACC_SysTypeList") {
        obj = new ACC_SysType(uuid, parent, "ACC_SysType", this);
    } else if (str == "ACC_TaxAuthorityList") {
        obj = new ACC_TaxAuthority(uuid, parent, "ACC_TaxAuthority", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxAuthRateList", this);
        obj->addObject(list);
    } else if (str == "ACC_TaxAuthRateList") {
        obj = new ACC_TaxAuthRate(uuid, parent, "ACC_TaxAuthRate", this);
    } else if (str == "ACC_TaxCategoryList") {
        obj = new ACC_TaxCategory(uuid, parent, "ACC_TaxCategory", this);
    } else if (str == "ACC_TaxGroupList") {
        obj = new ACC_TaxGroup(uuid, parent, "ACC_TaxGroup", this);

        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxGroupAuthList", this);
        obj->addObject(list);
    } else if (str == "ACC_TaxGroupAuthList") {
        obj = new ACC_TaxGroupAuth(uuid, parent, "ACC_TaxGroupAuth", this);
    } else if (str == "ACC_TaxProvinceList") {
        obj = new ACC_TaxProvince(uuid, parent, "ACC_TaxProvince", this);
    } else if (str == "ACC_TransAllocnList") {
        obj = new ACC_TransAllocn(uuid, parent, "ACC_TransAllocn", this);
    } else if (str == "ACC_TransDocList") {
        obj = new ACC_TransDoc(uuid, parent, "ACC_TransDoc", this);
    } else if (str == "ACC_UnitOfMeasureList") {
        obj = new ACC_UnitOfMeasure(uuid, parent, "ACC_UnitOfMeasure", this);
    } else if (str == "DB_ProjectList") {
        // for ACC global system settings only,
        // the current DB project is always root id
        obj = new DB_Project(uuid, parent, "DB_Project", this);

        // part of global relations
        uuid = "";
        list = new RB_ObjectContainer(uuid, obj, "ACC_AccountControlList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_AccountGroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_AreaList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_AssetTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CostGroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_CurrencyList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_DebtorTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_FactorCompanyList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_HoldReasonList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_LocationList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_PaymentTermList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesManList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_SalesTypeList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_ShipperList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxAuthorityList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxCategoryList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxGroupList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_TaxProvinceList", this);
        obj->addObject(list);
        list = new RB_ObjectContainer(uuid, obj, "ACC_UnitOfMeasureList", this);
        obj->addObject(list);

    } else {
        RB_DEBUG->error("ACC_ObjectFactory::newObject() " + parent->getName() + " ERROR");
    }

    // Add object to parent and set object parent
    if (parent && addToParent) parent->addObject(obj);
    return obj;
}

/**
 * Is field or object name a system setting
 * @param name field or object name
 * @returns true if name is part of the system settings
 */
bool ACC_ObjectFactory::isSystemSetting(const RB_String& name) {
    if (name == "ACC_AccountControlList" || name == "ACC_AccountControl") {
        return true;
    } else if (name == "ACC_AccountGroupList" || name == "ACC_AccountGroup") {
        return true;
    } else if (name == "ACC_AreaList" || name == "ACC_Area") {
        return true;
    } else if (name == "ACC_AssetTypeList" || name == "ACC_AssetType") {
        return true;
    } else if (name == "ACC_CostGroupList" || name == "ACC_CostGroup") {
        return true;
    } else if (name == "ACC_CurrencyList" || name == "ACC_Currency") {
        return true;
    } else if (name == "ACC_DebtorTypeList" || name == "ACC_DebtorType") {
        return true;
    } else if (name == "ACC_FactorCompanyList" || name == "ACC_FactorCompany") {
        return true;
    } else if (name == "ACC_HoldReasonList" || name == "ACC_HoldReason") {
        return true;
    } else if (name == "ACC_LocationList" || name == "ACC_Location") {
        return true;
    } else if (name == "ACC_PaymentTermList" || name == "ACC_PaymentTerm") {
        return true;
    } else if (name == "ACC_SalesManList" || name == "ACC_SalesMan") {
        return true;
    } else if (name == "ACC_SalesTypeList" || name == "ACC_SalesType") {
        return true;
    } else if (name == "ACC_ShipperList" || name == "ACC_Shipper") {
        return true;
    } else if (name == "ACC_TaxAuthorityList" || name == "ACC_TaxAuthority") {
        return true;
    } else if (name == "ACC_TaxAuthRateList" || name == "ACC_TaxAuthRate") {
        return true;
    } else if (name == "ACC_TaxCategoryList" || name == "ACC_TaxCategory") {
        return true;
    } else if (name == "ACC_TaxGroupList" || name == "ACC_TaxGroup") {
        return true;
    } else if (name == "ACC_TaxGroupAuthList" || name == "ACC_TaxGroupAuth") {
        return true;
    } else if (name == "ACC_TaxProvinceList" || name == "ACC_TaxProvince") {
        return true;
    } else if (name == "ACC_UnitOfMeasureList" || name == "ACC_UnitOfMeasure") {
        return true;
    }

    return false;
}

/**
 * Is field or object name a project setting
 * @param name field or object name
 * @returns true if name is part of the project settings
 */
bool ACC_ObjectFactory::isProjectSetting(const RB_String& name) {
    if (name == "ACC_ChartMasterList" || name == "ACC_ChartMaster") {
        return true;
    }
    return false;
}
