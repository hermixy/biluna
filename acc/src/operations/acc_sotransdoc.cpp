/*****************************************************************
 * $Id: acc_sotransdoc.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Mar 25, 2010 5:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_sotransdoc.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_SoTransDoc::ACC_SoTransDoc() {

}

/**
 * Destructor
 */
ACC_SoTransDoc::~ACC_SoTransDoc() {

}

/* *
 * Execute the creation of line items and setting their taxes. Creates a
 * temporary stockmove (or stockserialmoves) with stockmovetaxes model
 * @param stockMovesList temporary container for the results
 * @return true on success
 * /
bool ACC_SoTransDoc::execute(RB_ObjectBase* stockMovesList) {
    if (!stockMovesList) return false;
    // Remove previous stockmove with tax objects
    RB_ObjectContainer* smList = dynamic_cast<RB_ObjectContainer*>(stockMovesList);
    smList->erase();

    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    RB_String taxGroupId = custModel->getCurrentValue("taxgroup_id", RB2::RoleOrigData).toString();
    RB_String defaultLocation = custModel->getCurrentValue("defaultlocation_id", RB2::RoleOrigData).toString();

    // get taxprovince id
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String qStr = "";
    qStr = "SELECT taxprovinceid FROM acc_location WHERE id='" + defaultLocation + "';";

    if (!query.exec(qStr) || !query.first()) {
        return false;
    }

    RB_String taxProvince = query.value(0).toString();
    query.clear();
    ACC_SqlCommonFunctions f;

    // Create from sales line items the stockmove list
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);
    for (int row = 0; row < odModel->rowCount(); row++) {
        QModelIndex idx =  odModel->index(row, odModel->fieldIndex("qtydispatched"));
        double qtyDispatched = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed
        if (qtyDispatched > 0.0 && completed == 0) {
            // ACC_StockMove object
            RB_ObjectBase* sm = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                              stockMovesList, "", true);
            setStockMoveData(sm, row);
            f.getSalesTaxes(query, taxGroupId, taxProvince,
                               sm->getValue("taxcat_id").toString());

            while (query.next()) {
                RB_ObjectBase* smTaxList = sm->getContainer("ACC_StockMoveTaxesList");
                RB_ObjectBase* smTaxObj = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                                     smTaxList, "", true);
                smTaxObj->setValue("taxauth_id", query.value(2));
                smTaxObj->setValue("taxrate", query.value(5));
                smTaxObj->setValue("taxontax", query.value(4));
                smTaxObj->setValue("taxcalculationorder", query.value(0));
                smTaxObj->setValue("salestaxglacct_idx", query.value(3));
            }
        }

        query.clear();
    }

    // Freightcost is included in the container and handled separately,
    // the container itself is not send to the database.
    // TODO: revisite in the future because containers should not have members
    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);
    double freightCost =  soModel->getCurrentValue("freightcost", Qt::DisplayRole).toDouble();
    stockMovesList->addMember(stockMovesList, "freightcost", "-",
                              freightCost, RB2::MemberDouble);

    return true;
}
*/

/* *
 * Set stockmove object data
 * @param sm stockmove object
 * @param row rownumber in the salesorderdetail model
 * /
void ACC_SoTransDoc::setStockMoveData(RB_ObjectBase *sm, int row) {
    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);

    QModelIndex idx =  odModel->index(row, odModel->fieldIndex("stkid"));
    RB_String stockMasterId = idx.data(Qt::UserRole + 1).toString();
    sm->setValue("parent", stockMasterId);
    sm->setValue("type", 10);
    sm->setValue("transno", 0);                        // invoice is not set, done in oper.exec()
    sm->setValue("loccode", custModel->getCurrentValue("defaultlocation_id", Qt::UserRole + 1));
    sm->setValue("transdate", "0000-00-00T00:00:00");   // transaction date not set, done in oper.exec()
    // TODO: sm->setValue("debtorno", custModel->getCurrentId());
    sm->setValue("branchcode", custModel->getCurrentId());
    idx = odModel->index(row, odModel->fieldIndex("unitprice"));
    sm->setValue("price", idx.data(Qt::DisplayRole));
    sm->setValue("prd", 0);                            // period not set, done in oper.exec()
    sm->setValue("reference", soModel->getCurrentId());
    idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
    sm->setValue("qty", idx.data(Qt::DisplayRole));
    idx = odModel->index(row, odModel->fieldIndex("discountpercent"));
    sm->setValue("discountpercent", idx.data(Qt::DisplayRole));
    // Get standard cost
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String qStr = "";
    qStr = "SELECT materialcost, labourcost, overheadcost FROM acc_stockmaster "
           "WHERE id='" + stockMasterId + "';";
    if (!query.exec(qStr) || !query.first()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,"ACC_SoTransDoc::setStockMoveData() "
                        "query ERROR");
    }
    double stdCost = query.value(0).toDouble() + query.value(1).toDouble()
                     + query.value(2).toDouble();
    query.clear();
    sm->setValue("standardcost", stdCost);


    // TODO: new quantity on hand not set
    sm->setValue("newqoh", 0);


    idx = odModel->index(row, odModel->fieldIndex("narrative"));
    sm->setValue("narrative", idx.data(Qt::DisplayRole));
    idx = odModel->index(row, odModel->fieldIndex("taxcat_id"));
    sm->setValue("taxcat_id", idx.data(Qt::UserRole + 1));
    idx = odModel->index(row, 0); // id
    sm->setValue("solineid", odModel->hiddenData(idx, Qt::UserRole + 1));
}
*/

/**
 * Execute the creation of line items and setting their taxes. Creates a
 * temporary debtor transactions with taxes
 * @param transDoc transaction document (or sales invoice)
 * @param dtList debtor transaction list, temporary container for the results
 * @param isPost true if posting of invoice is done, false if only preview of invoice
 * @return true on success
 */ /*
bool ACC_SoTransDoc::execute(RB_ObjectBase* transDoc, RB_ObjectContainer* dtList, bool isPost) {
    if (!dtList || !transDoc) return false;

    dtList->setValue("parent", ACC_MODELFACTORY->getRootId());

    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);

    // The tax group id of the customer
    RB_String taxGroupId = custModel->getCurrentValue("taxgroup_id", RB2::RoleOrigData).toString();
    // Selected location for delivery to customer
    RB_String stkLocId = soModel->getCurrentValue("fromstkloc_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxGroupId) || !RB_DEBUG->isValidId(stkLocId)) {
        return false;
    }

    ACC_SqlCommonFunctions f;

    // set transaction document members
    RB_String whereStr = "accountcontrol=" + RB_String::number((int)ACC2::ControlDebtor);
    whereStr += " AND parent='" + ACC_MODELFACTORY->getRootId() + "'";
    RB_String parentId = f.selectFromWhere("id", "acc_chartmaster", whereStr).toString();
    transDoc->setValue("parent", parentId);
    RB_String actName = f.selectFromWhereId("accountname", "acc_chartmaster", parentId).toString();
    transDoc->setValue("doctype", (int)ACC2::TransSalesOrder);
    transDoc->setValue("docname", actName);
    transDoc->setValue("salestype_id", custModel->getCurrentValue("salestype_id", RB2::RoleOrigData));
    QDateTime today = QDateTime::currentDateTime();
    transDoc->setValue("transdate", today);
    RB_String paymentId = custModel->getCurrentValue("paymentterm_id", RB2::RoleOrigData).toString();
    QDateTime dueDate;
    f.setDueDate(paymentId, dueDate);
    transDoc->setValue("duedate", dueDate);
    int number = isPost ? f.getNextTransNo((int)ACC2::TransDebtor, "none", "Debtor") : -1; // only at post
    transDoc->setValue("transno", number);
    transDoc->setValue("debtor_idx", custModel->getCurrentId());
    transDoc->setDValue("debtor_idx", custModel->getCurrentValue("mname").toString()); // display value
    transDoc->setValue("creditor_idx", "0");
    transDoc->setValue("refno", soModel->getCurrentValue("customerref", RB2::RoleOrigData));
    RB_String str = "SO";
    str +=  " " + soModel->getCurrentValue("orderno").toString();
    transDoc->setValue("description", str);
    // duedate depending payment terms at invoice processing
    // TODO: rate exchange rate
    // TODO: diffonexch
    transDoc->setValue("alloc", 0.0);
    transDoc->setValue("settled", 0);
    transDoc->setValue("hold", 0);
    transDoc->setValue("edisent", 0);
    transDoc->setValue("consignment", 0);
    transDoc->setValue("order_id", soModel->getCurrentValue("id", RB2::RoleOrigData));
    transDoc->setValue("invoicecomment", soModel->getCurrentValue("invoicecomment", RB2::RoleOrigData));

    double totalAmount = 0.0;
    double totalTax = 0.0;
    double amount = 0.0;

    dtList->erase();

    // get dispatch taxprovince ID, sales area ID and sales type ID
    RB_String taxProvinceId = f.selectFromWhereId("taxprovince_id", "acc_location", stkLocId).toString();
    RB_String salesAreaId = custModel->getCurrentValue("area_id", RB2::RoleOrigData).toString();
    RB_String salesTypeId = custModel->getCurrentValue("salestype_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxProvinceId) || !RB_DEBUG->isValidId(salesAreaId)
            || !RB_DEBUG->isValidId(salesTypeId)) {
        return false;
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);

    RB_String dtParent = "";
    RB_String dtTransDocId = "";
    RB_String dtDescription = "";
    QModelIndex idx;

    // Create from sales line items the debtor transactions list
    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
        double qtyDispatched = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyDispatched <= 0.0 && completed == 1) continue;

        // Update quantities in sales order detail if posted
        if (isPost) {
            // quantity = order quantity
            // qtyinvoiced = already dispatched
            // qtydispatched = current dispatch
            // completed = completed
            idx = odModel->index(row, odModel->fieldIndex("quantity"));
            double quantity = idx.data(Qt::DisplayRole).toDouble();
            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
            double qtyInvoiced = idx.data(Qt::DisplayRole).toDouble();

            // Reset current dispatch and set total invoiced
            idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
            odModel->setData(idx, 0.0, Qt::EditRole);
            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
            odModel->setData(idx, qtyInvoiced + qtyDispatched, Qt::EditRole);

            // If applicable set order detail completed
            if (quantity <= qtyInvoiced + qtyDispatched) {
                idx = odModel->index(row, odModel->fieldIndex("completed"));
                odModel->setData(idx, 1, Qt::EditRole);
            }
        }

        // Prepare data for debtor transaction
        RB_ObjectBase* dt = NULL;
        dtParent = ACC_MODELFACTORY->getRootId();

        dtTransDocId = transDoc->getId();

        idx = odModel->index(row, odModel->fieldIndex("stkdescription"));
        dtDescription = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockCode = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterCode = idx.data(Qt::DisplayRole).toString();

        query.clear();
        // TODO: Stock movement booking
//        f.getCogsAccount(query, salesArea, stockId, salesType);
//        query.first();
//        RB_String dtChartMasterId = query.value(0).toString();
//        RB_String dtChartMasterName = "";
//        // If chartmaster is not found set to default
//        if (dtChartMasterId == "") dtChartMasterId = "0";
//        int dtActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", dtChartMasterId).toInt();
//        // TODO: discount account is query.value(1)

        idx = odModel->index(row, odModel->fieldIndex("id"));
        RB_String dtOrderDetailId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        idx = odModel->index(row, odModel->fieldIndex("taxcat_id"));
        RB_String taxCatId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        RB_String stockCatId = f.selectFromWhereId("parent", "acc_stockmaster", stockId).toString();
        f.getTaxAuthId(query, taxGroupId, taxProvinceId, taxCatId);
        if (!query.first()){
            RB_String str = " Tax Group ID = " + taxGroupId + "\n";
            str += " Tax Province ID = " + taxProvinceId + "\n";
            str += " Tax Category ID = " + taxCatId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Tax Authority ID in ACC_SoTransDoc:\n" + str);
            return false;
        }

        RB_String taxAuthId = query.value(0).toString();
        query.clear();
        f.getSalesGlAccount(query, taxAuthId, salesAreaId, stockCatId, salesTypeId);

        if (!query.first()){
            RB_String str = "Tax Authority ID = " + taxAuthId + "\n";
            str += " Sales Area ID = " + salesAreaId + "\n";
            str += " Stock Category ID = " + stockCatId + "\n";
            str += " Sales Type ID = " + salesTypeId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Sales GL Account in ACC_SoTransDoc:\n" + str);
            return false;
        }
        RB_String dtChartMasterId = query.value(0).toString();
        RB_String dtChartMasterName = query.value(1).toString();
        int dtActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", dtChartMasterId).toInt();
        // TODO: discount account query.value(2) and query.value(3)

        // Process tax data
        bool isFirstTax = true;
        RB_String prevTaxAuthId = "";
        RB_String prevTaxChartMasterId = "";
        RB_String prevTaxChartMasterName = "";
        double taxAmt = 0.0;

        // First debtor transaction
        dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                          dtList, "", true);
        dt->setValue("parent", dtParent); // parent is ACC_Project ID
        dt->setValue("transdoc_id", dtTransDocId);
        dt->setValue("description", dtDescription);
        RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(dtChartMasterId);
        if (obj) {
            dtChartMasterName = obj->getValue("accountcode").toString()
                    + " - " + obj->getValue("accountname").toString();
            dt->setValue("chartmaster_idx", dtChartMasterId);
            dt->setDValue("chartmaster_idx", dtChartMasterName);
        } else {
            RB_DEBUG->error("ACC_SoTransDoc::exec() ACC_ChartMaster object ERROR");
        }
        dt->setValue("accountcontrol", dtActCtrl);
        dt->setValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxHighId());
        dt->setDValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxHighName());
        dt->setValue("taxhighamt", 0.0);
        dt->setValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxLowId());
        dt->setDValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxLowName());
        dt->setValue("taxlowamt", 0.0);
        dt->setValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxOtherId());
        dt->setDValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxOtherName());
        dt->setValue("hold", 0);
        dt->setValue("orderdetail_id", dtOrderDetailId);
        dt->setValue("taxotheramt", 0.0);

        // Data mainly for invoice line details
        idx =  odModel->index(row, odModel->fieldIndex("id")); // delete, already done above
        RB_String str = idx.data(RB2::RoleOrigData).toString(); //
        dt->setValue("orderdetail_id", str);                   //
        dt->setValue("stk_idx", stockId);
        dt->setDValue("stk_idx", stockCode);
        dt->setValue("costcenter_idx", costCenterId);
        dt->setDValue("costcenter_idx", costCenterCode);
        idx =  odModel->index(row, odModel->fieldIndex("narrative"));
        str = idx.data(RB2::RoleOrigData).toString();
        dt->setValue("narrative", str);
        idx =  odModel->index(row, odModel->fieldIndex("unitprice"));
        double amt = idx.data(RB2::RoleOrigData).toDouble();
        dt->setValue("unitprice", amt);
        dt->setValue("qtydispatched", qtyDispatched);
        idx =  odModel->index(row, odModel->fieldIndex("stkuom"));
        str = idx.data(RB2::RoleOrigData).toString();
        dt->setValue("stkuom", str);
        idx =  odModel->index(row, odModel->fieldIndex("discountpercent"));
        double discount = idx.data(RB2::RoleOrigData).toDouble();
        dt->setValue("discountpercent", discount);

        amount = amt * qtyDispatched * (1 - discount / 100.0);

        // Prevent bookings with zero (0.00) amount
        if (amount > -0.005 && amount < 0.005) {
            dtList->remove(dt); // remove and delete
            dt = NULL;
            continue;
        }

        totalAmount += amount;
        dt->setValue("amount", amount);

        // Set taxes, already sorted on calculation order
        query.clear();
        f.getSalesTaxes(query, taxGroupId, taxProvinceId, taxCatId);

        // 0 = acc_taxgroupauth.calculationorder
        // 1 = acc_taxauthority.description
        // 2 = acc_taxgroupauth.taxauth_id
        // 3 = acc_taxgroupauth.taxontax
        // 4 = acc_taxauthrate.taxrate
        // 5 = acc_salestaxposting.salestaxacct_idx

        // Create a temporay container with the tax data
        RB_ObjectContainer* taxList = new RB_ObjectContainer();

        while (query.next()) {
            RB_ObjectBase* tax = new RB_ObjectAtomic("", taxList, "");
            taxList->addObject(tax);
            tax->addMember(tax, "taxauth_id", "-", query.value(2), RB2::MemberChar40);
            tax->addMember(tax, "taxrate", "-", query.value(4), RB2::MemberDouble);
            tax->addMember(tax, "taxontax", "-", query.value(3), RB2::MemberInteger);
            tax->addMember(tax, "taxcalculationorder", "-", query.value(0), RB2::MemberInteger);
            tax->addMember(tax, "taxacct_idx", "-", query.value(5), RB2::MemberChar40);

            // Set display value
            obj = ACC_QACHARTMASTER->getAcctObj(query.value(5).toString());
            if (obj) {
                tax->setDValue("taxacct_idx", obj->getValue("accountcode").toString() + " - "
                               + obj->getValue("accountname").toString());
            } else {
                RB_DEBUG->error("ACC_SoTransDoc::execute() tax account2 not found ERROR");
            }
        }

        query.clear();

        RB_ObjectIterator* iter = taxList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* tax = iter->currentObject();

            if (!isFirstTax && (prevTaxAuthId != tax->getValue("taxauth_id").toString()
                    || prevTaxChartMasterId != tax->getValue("taxacct_idx").toString()
                    || tax->getValue("taxauth_id").toString().isEmpty()/ *
                    || tax->getValue("taxglacct_idx").toString().isEmpty()* /)) {
                // Create new debtor transaction
                dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                  dtList, "", true);
                prevTaxAuthId = tax->getValue("taxauth_id").toString();
                prevTaxChartMasterId = tax->getValue("taxacct_idx").toString();
                prevTaxChartMasterName = tax->getDValue("taxacct_idx").toString();

                dt->setValue("parent", dtParent);
                dt->setValue("transdoc_id", dtTransDocId);
                dt->setValue("description", dtDescription);
                dt->setValue("chartmaster_idx", dtChartMasterId);
                dt->setDValue("chartmaster_idx", dtChartMasterName);
                dt->setValue("accountcontrol", dtActCtrl);
                if (isFirstTax) {
                    dt->setValue("amount", amount);
                    isFirstTax = false;
                } else {
                    dt->setValue("amount", 0.0);
                }
                dt->setValue("taxhighchartmaster_idx", "0");
                dt->setValue("taxhighamt", 0.0);
                dt->setValue("taxlowchartmaster_idx", "0");
                dt->setValue("taxlowamt", 0.0);
                dt->setValue("taxotherchartmaster_idx", prevTaxChartMasterId);
                dt->setDValue("taxotherchartmaster_idx", prevTaxChartMasterName);
                dt->setValue("hold", 0);
                dt->setValue("orderdetail_id", dtOrderDetailId);
                dt->setValue("taxauth_id", tax->getValue("taxauth_id"));

                // Set tax amount
                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt = (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt = amount * tax->getValue("taxrate").toDouble() / 100;
                }
            } else {
                dt->setValue("taxauth_id", tax->getValue("taxauth_id"));

                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt += (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt += amount * tax->getValue("taxrate").toDouble() / 100;
                }
            }

            // tax is booked on relevant account
            if (dtActCtrl == ACC2::ControlSalesHighTax) {              // 12
                dt->setValue("taxhighamt", taxAmt);
            } else if (dtActCtrl == ACC2::ControlSalesLowTax) {        // 11
                dt->setValue("taxlowamt", taxAmt);
            } else if (dtActCtrl == ACC2::ControlSalesOtherTax) {      // 13
                dt->setValue("taxotheramt", taxAmt);
            }
        }

        totalTax += taxAmt;
        delete taxList;
        taxList = NULL;
    }

    // Freigh tcost
    double freightCost = soModel->getCurrentValue("freightcost", Qt::DisplayRole).toDouble();

    if (freightCost >= 0.005 && totalAmount >= 0.005) {
        RB_ObjectBase* dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                         dtList, "", true);
        dt->setValue("parent", dtParent);
        dt->setValue("transdoc_id", dtTransDocId);
        dt->setValue("description", tr("Freight cost") + " " + dtDescription);
        RB_String freightActId = ACC_MODELFACTORY->getRoot()->getValue("freightact_idx").toString();
        RB_String freightActName = ACC_MODELFACTORY->getRoot()->getDValue("freightact_idx").toString();
        dt->setValue("chartmaster_idx", freightActId);
        dt->setDValue("chartmaster_idx", freightActName);
        int actCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", freightActId).toInt();
        dt->setValue("accountcontrol", actCtrl);
        dt->setValue("amount", freightCost);
        dt->setValue("taxhighchartmaster_idx", "0");
        dt->setValue("taxhighamt", 0.0);
        dt->setValue("taxlowchartmaster_idx", "0");
        dt->setValue("taxlowamt", 0.0);
        dt->setValue("taxotherchartmaster_idx", "0");
        dt->setValue("taxotheramt", 0.0);
        dt->setValue("hold", 0);
        dt->setValue("orderdetail_id", "freight"); // used in acc_salesinvoicehtml
    }

    totalAmount += totalTax + freightCost;

    transDoc->setValue("totalamountrec", totalAmount);
    transDoc->setValue("totalamountpay", 0);
    transDoc->setValue("totaltax", totalTax);

    // Save changes of qtydispatched, qtyinvoiced, completed
    odModel->submitAllAndSelect(); // NOTE: deselects the current order detail

    return true;
}
*/
/**
 * Prepare the order for preview of the sales order invoice,
 * without updating qtyinvoiced, qtydispatched, stock moves
 * and collecting the invoice system number
 * @param transdoc transaction document
 * @param dtList debtor list (transaction document items)
 */
bool ACC_SoTransDoc::preparePreview(RB_ObjectBase* transDoc, RB_ObjectContainer* dtList) {
    if (!dtList || !transDoc) return false;

    dtList->setValue("parent", ACC_MODELFACTORY->getRootId());

    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer);
    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);

    // The tax group id of the customer
    RB_String taxGroupId = custModel->getCurrentValue("taxgroup_id", RB2::RoleOrigData).toString();
    // Selected location for delivery to customer
    RB_String stkLocId = soModel->getCurrentValue("fromstkloc_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxGroupId) || !RB_DEBUG->isValidId(stkLocId)) {
        RB_String str = " Tax Group ID = " + taxGroupId + "\n";
        str += " Dispatch Location ID = " + stkLocId;
        ACC_DIALOGFACTORY->commandMessage(
                    "Tax Group or Receipt Location invalid, check:\n" + str);
        return false;
    }

    ACC_SqlCommonFunctions f;

    // set transaction document members
    RB_String whereStr = "accountcontrol=" + RB_String::number((int)ACC2::ControlDebtor);
    whereStr += " AND parent='" + ACC_MODELFACTORY->getRootId() + "'";
    RB_String parentId = f.selectFromWhere("id", "acc_chartmaster", whereStr).toString();
    transDoc->setValue("parent", parentId);
    RB_String actName = f.selectFromWhereId("accountname", "acc_chartmaster", parentId).toString();
    transDoc->setValue("doctype", (int)ACC2::TransSalesOrder);
    transDoc->setValue("docname", actName);
    transDoc->setValue("salestype_id", custModel->getCurrentValue("salestype_id", RB2::RoleOrigData));
    QDateTime today = QDateTime::currentDateTime();
    transDoc->setValue("transdate", today);
    RB_String paymentId = custModel->getCurrentValue("paymentterm_id", RB2::RoleOrigData).toString();
    QDateTime dueDate;
    f.setDueDate(paymentId, dueDate);
    transDoc->setValue("duedate", dueDate);
//    int number = isPost ? f.getNextTransNo((int)ACC2::TransDebtor, "none", "Debtor") : -1; // only at post
//    transDoc->setValue("transno", number);
    transDoc->setValue("transno", -1); // invalid
    transDoc->setValue("debtor_idx", custModel->getCurrentId());
    transDoc->setDValue("debtor_idx", custModel->getCurrentValue("mname").toString()); // display value
    transDoc->setValue("creditor_idx", "0");
    transDoc->setValue("refno", soModel->getCurrentValue("customerref", RB2::RoleOrigData));
    RB_String str = "SO";
    str +=  " " + soModel->getCurrentValue("orderno").toString();

    RB_String orderTitleStr = soModel->getCurrentValue("ordertitle").toString();

    if (!orderTitleStr.isEmpty()) {
        str += " " + orderTitleStr;
    }

    RB_String contactStr = soModel->getCurrentValue("contact_idx").toString();

    if (contactStr.size() > 38) {
        contactStr.remove(38);
        str += " " + contactStr;
    }

    transDoc->setValue("description", str);
    // duedate depending payment terms at invoice processing
    // TODO: rate exchange rate
    // TODO: diffonexch
    transDoc->setValue("alloc", 0.0);
    transDoc->setValue("settled", 0);
    transDoc->setValue("hold", 0);
    transDoc->setValue("edisent", 0);
    transDoc->setValue("consignment", 0);
    transDoc->setValue("order_id", soModel->getCurrentValue("id", RB2::RoleOrigData));
    transDoc->setValue("invoicecomment", soModel->getCurrentValue("invoicecomment", RB2::RoleOrigData));

    double totalAmount = 0.0;
    double totalTax = 0.0;
    double amount = 0.0;

    dtList->erase();

    // get dispatch taxprovince ID, sales area ID and sales type ID
    RB_String taxProvinceId = f.selectFromWhereId("taxprovince_id", "acc_location", stkLocId).toString();
    RB_String salesAreaId = custModel->getCurrentValue("area_id", RB2::RoleOrigData).toString();
    RB_String salesTypeId = custModel->getCurrentValue("salestype_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxProvinceId) || !RB_DEBUG->isValidId(salesAreaId)
            || !RB_DEBUG->isValidId(salesTypeId)) {
        ACC_DIALOGFACTORY->commandMessage(
                    "Not a valid:\n"
                    "- Tax Province ID,\n"
                    "- Sales Area ID or\n"
                    "- Sales Type ID");
        RB_String str = "Not a valid tax province found with:\n";
        str = " Dispatch Location ID = " + stkLocId + "\n";
        str = " or invalid Sales Area ID = " + salesAreaId + "\n";
        str = " or invalid Sales Type ID = " + salesTypeId;
        ACC_DIALOGFACTORY->commandMessage(str);

        return false;
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);

    RB_String dtParent = "";
    RB_String dtTransDocId = "";
    RB_String dtDescription = "";
    QModelIndex idx;

    // Create from sales line items the debtor transactions list
    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
        double qtyDispatched = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyDispatched <= 0.0 && completed == 1) continue;

//        // Update quantities in sales order detail if posted
//        if (isPost) {
//            // quantity = order quantity
//            // qtyinvoiced = already dispatched
//            // qtydispatched = current dispatch
//            // completed = completed
//            idx = odModel->index(row, odModel->fieldIndex("quantity"));
//            double quantity = idx.data(Qt::DisplayRole).toDouble();
//            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
//            double qtyInvoiced = idx.data(Qt::DisplayRole).toDouble();

//            // Reset current dispatch and set total invoiced
//            idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
//            odModel->setData(idx, 0.0, Qt::EditRole);
//            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
//            odModel->setData(idx, qtyInvoiced + qtyDispatched, Qt::EditRole);

//            // If applicable set order detail completed
//            if (quantity <= qtyInvoiced + qtyDispatched) {
//                idx = odModel->index(row, odModel->fieldIndex("completed"));
//                odModel->setData(idx, 1, Qt::EditRole);
//            }
//        }

        // Prepare data for debtor transaction
        RB_ObjectBase* dt = NULL;
        dtParent = ACC_MODELFACTORY->getRootId();

        dtTransDocId = transDoc->getId();

        idx = odModel->index(row, odModel->fieldIndex("stkdescription"));
        dtDescription = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockCode = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("serialno"));
        stockCode += " " + idx.data(Qt::DisplayRole).toString(); // add serial no
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterCode = idx.data(Qt::DisplayRole).toString();

        query.clear();

        idx = odModel->index(row, odModel->fieldIndex("id"));
        RB_String dtOrderDetailId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        idx = odModel->index(row, odModel->fieldIndex("taxcat_id"));
        RB_String taxCatId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        RB_String stockCatId = f.selectFromWhereId("parent", "acc_stockmaster", stockId).toString();
        f.getTaxAuthId(query, taxGroupId, taxProvinceId, taxCatId);
        if (!query.first()){
            RB_String str = " Tax Group ID = " + taxGroupId + "\n";
            str += " Tax Province ID = " + taxProvinceId + "\n";
            str += " Tax Category ID = " + taxCatId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Tax Authority ID in ACC_SoTransDoc:\n" + str);
            return false;
        }

        RB_String taxAuthId = query.value(0).toString();
        query.clear();
        f.getSalesGlAccount(query, taxAuthId, salesAreaId, stockCatId, salesTypeId);

        if (!query.first()){
            RB_String str = "Tax Authority ID = " + taxAuthId + "\n";
            str += " Sales Area ID = " + salesAreaId + "\n";
            str += " Stock Category ID = " + stockCatId + "\n";
            str += " Sales Type ID = " + salesTypeId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Sales GL Account in ACC_SoTransDoc:\n" + str);
            return false;
        }
        RB_String dtChartMasterId = query.value(0).toString();
        RB_String dtChartMasterName = query.value(1).toString();
        int dtActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", dtChartMasterId).toInt();
        // TODO: discount account query.value(2) and query.value(3)

        // Process tax data
        bool isFirstTax = true;
        RB_String prevTaxAuthId = "";
        RB_String prevTaxChartMasterId = "";
        RB_String prevTaxChartMasterName = "";
        double taxAmt = 0.0;

        // First debtor transaction
        dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                          dtList, "", true);
        dt->setValue("parent", dtParent); // parent is ACC_Project ID
        dt->setValue("transdoc_id", dtTransDocId);
        dt->setValue("description", dtDescription);
        RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(dtChartMasterId);
        if (obj) {
            dtChartMasterName = obj->getValue("accountcode").toString()
                    + " - " + obj->getValue("accountname").toString();
            dt->setValue("chartmaster_idx", dtChartMasterId);
            dt->setDValue("chartmaster_idx", dtChartMasterName);
        } else {
            RB_DEBUG->error("ACC_SoTransDoc::exec() ACC_ChartMaster object ERROR");
        }
        dt->setValue("accountcontrol", dtActCtrl);
        dt->setValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxHighId());
        dt->setDValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxHighName());
        dt->setValue("taxhighamt", 0.0);
        dt->setValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxLowId());
        dt->setDValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxLowName());
        dt->setValue("taxlowamt", 0.0);
        dt->setValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxOtherId());
        dt->setDValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccSalesTaxOtherName());
        dt->setValue("hold", 0);
        dt->setValue("orderdetail_id", dtOrderDetailId);
        dt->setValue("taxotheramt", 0.0);

        // Data mainly for invoice line details
        idx =  odModel->index(row, odModel->fieldIndex("id")); // delete, already done above
        RB_String str = idx.data(RB2::RoleOrigData).toString(); //
        dt->setValue("orderdetail_id", str);                   //
        dt->setValue("stk_idx", stockId);
        dt->setDValue("stk_idx", stockCode);
        dt->setValue("costcenter_idx", costCenterId);
        dt->setDValue("costcenter_idx", costCenterCode);
        idx =  odModel->index(row, odModel->fieldIndex("narrative"));
        str = idx.data(RB2::RoleOrigData).toString();
        dt->setValue("narrative", str);
        idx =  odModel->index(row, odModel->fieldIndex("unitprice"));
        double amt = idx.data(RB2::RoleOrigData).toDouble();
        dt->setValue("unitprice", amt);
        dt->setValue("qtydispatched", qtyDispatched);
        idx =  odModel->index(row, odModel->fieldIndex("stkuom"));
        str = idx.data(RB2::RoleOrigData).toString();
        dt->setValue("stkuom", str);
        idx =  odModel->index(row, odModel->fieldIndex("discountpercent"));
        double discount = idx.data(RB2::RoleOrigData).toDouble();
        dt->setValue("discountpercent", discount);

        amount = amt * qtyDispatched * (1 - discount / 100.0);

        // Prevent bookings with zero (0.00) amount
        if (amount > -0.005 && amount < 0.005) {
            dtList->remove(dt); // remove and delete
            dt = NULL;
            continue;
        }

        totalAmount += amount;
        dt->setValue("amount", amount);

        // Set taxes, already sorted on calculation order
        query.clear();
        f.getSalesTaxes(query, taxGroupId, taxProvinceId, taxCatId);

        // 0 = acc_taxgroupauth.calculationorder
        // 1 = acc_taxauthority.description
        // 2 = acc_taxgroupauth.taxauth_id
        // 3 = acc_taxgroupauth.taxontax
        // 4 = acc_taxauthrate.taxrate
        // 5 = acc_salestaxposting.salestaxacct_idx

        // Create a temporay container with the tax data
        RB_ObjectContainer* taxList = new RB_ObjectContainer();

        while (query.next()) {
            RB_ObjectBase* tax = new RB_ObjectAtomic("", taxList, "");
            taxList->addObject(tax);
            tax->addMember(tax, "taxauth_id", "-", query.value(2), RB2::MemberChar40);
            tax->addMember(tax, "taxrate", "-", query.value(4), RB2::MemberDouble);
            tax->addMember(tax, "taxontax", "-", query.value(3), RB2::MemberInteger);
            tax->addMember(tax, "taxcalculationorder", "-", query.value(0), RB2::MemberInteger);
            tax->addMember(tax, "taxacct_idx", "-", query.value(5), RB2::MemberChar40);

            // Set display value
            obj = ACC_QACHARTMASTER->getAcctObj(query.value(5).toString());
            if (obj) {
                tax->setDValue("taxacct_idx", obj->getValue("accountcode").toString() + " - "
                               + obj->getValue("accountname").toString());
            } else {
                RB_DEBUG->error("ACC_SoTransDoc::execute() tax account2 not found ERROR");
            }
        }

        query.clear();

        RB_ObjectIterator* iter = taxList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* tax = iter->currentObject();

            if (!isFirstTax && (prevTaxAuthId != tax->getValue("taxauth_id").toString()
                    || prevTaxChartMasterId != tax->getValue("taxacct_idx").toString()
                    || tax->getValue("taxauth_id").toString().isEmpty()/*
                    || tax->getValue("taxglacct_idx").toString().isEmpty()*/)) {
                // Create new debtor transaction
                dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                  dtList, "", true);
                prevTaxAuthId = tax->getValue("taxauth_id").toString();
                prevTaxChartMasterId = tax->getValue("taxacct_idx").toString();
                prevTaxChartMasterName = tax->getDValue("taxacct_idx").toString();

                dt->setValue("parent", dtParent);
                dt->setValue("transdoc_id", dtTransDocId);
                dt->setValue("description", dtDescription);
                dt->setValue("chartmaster_idx", dtChartMasterId);
                dt->setDValue("chartmaster_idx", dtChartMasterName);
                dt->setValue("accountcontrol", dtActCtrl);
                if (isFirstTax) {
                    dt->setValue("amount", amount);
                    isFirstTax = false;
                } else {
                    dt->setValue("amount", 0.0);
                }
                dt->setValue("taxhighchartmaster_idx", "0");
                dt->setValue("taxhighamt", 0.0);
                dt->setValue("taxlowchartmaster_idx", "0");
                dt->setValue("taxlowamt", 0.0);
                dt->setValue("taxotherchartmaster_idx", prevTaxChartMasterId);
                dt->setDValue("taxotherchartmaster_idx", prevTaxChartMasterName);
                dt->setValue("hold", 0);
                dt->setValue("orderdetail_id", dtOrderDetailId);
                dt->setValue("taxauth_id", tax->getValue("taxauth_id"));

                // Set tax amount
                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt = (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt = amount * tax->getValue("taxrate").toDouble() / 100;
                }
            } else {
                dt->setValue("taxauth_id", tax->getValue("taxauth_id"));

                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt += (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt += amount * tax->getValue("taxrate").toDouble() / 100;
                }
            }

            // tax is booked on relevant account
            if (dtActCtrl == ACC2::ControlSalesHighTax) {              // 12
                dt->setValue("taxhighamt", taxAmt);
            } else if (dtActCtrl == ACC2::ControlSalesLowTax) {        // 11
                dt->setValue("taxlowamt", taxAmt);
            } else if (dtActCtrl == ACC2::ControlSalesOtherTax) {      // 13
                dt->setValue("taxotheramt", taxAmt);
            }
        }

        totalTax += taxAmt;
        delete taxList;
        taxList = NULL;
    }

    // Freigh tcost
    double freightCost = soModel->getCurrentValue("freightcost", Qt::DisplayRole).toDouble();

    if (freightCost >= 0.005 && totalAmount >= 0.005) {
        RB_ObjectBase* dt = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                         dtList, "", true);
        dt->setValue("parent", dtParent);
        dt->setValue("transdoc_id", dtTransDocId);
        dt->setValue("description", tr("Freight cost") + " " + dtDescription);
        RB_String freightActId = ACC_MODELFACTORY->getRoot()->getValue("freightact_idx").toString();
        RB_String freightActName = ACC_MODELFACTORY->getRoot()->getDValue("freightact_idx").toString();
        dt->setValue("chartmaster_idx", freightActId);
        dt->setDValue("chartmaster_idx", freightActName);
        int actCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", freightActId).toInt();
        dt->setValue("accountcontrol", actCtrl);
        dt->setValue("amount", freightCost);
        dt->setValue("taxhighchartmaster_idx", "0");
        dt->setValue("taxhighamt", 0.0);
        dt->setValue("taxlowchartmaster_idx", "0");
        dt->setValue("taxlowamt", 0.0);
        dt->setValue("taxotherchartmaster_idx", "0");
        dt->setValue("taxotheramt", 0.0);
        dt->setValue("hold", 0);
        dt->setValue("orderdetail_id", "freight"); // used in acc_salesinvoicehtml
    }

    totalAmount += totalTax + freightCost;

    transDoc->setValue("totalamountrec", totalAmount);
    transDoc->setValue("totalamountpay", 0);
    transDoc->setValue("totaltax", totalTax);

//    // Save changes of qtydispatched, qtyinvoiced, completed
//    odModel->submitAllAndSelect(); // NOTE: deselects the current order detail

    return true;
}

/**
 * Prepare the order for the posting of the sales order invoice,
 * setting qtyinvoiced, qtydispatched, stock moves
 * and collecting the invoice system number
 * @param transdoc transaction document
 */
bool ACC_SoTransDoc::preparePost(RB_ObjectBase* transDoc) {
    if (!transDoc) return false;

    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);
    QModelIndex idx = soModel->getCurrentIndex();
    if (!idx.isValid()) return false;

    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetail);

    ACC_SqlCommonFunctions f;
    int number = f.getNextTransNo((int)ACC2::TransDebtor, "none", "Debtor");
    transDoc->setValue("transno", number);

    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
        double qtyDispatched = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyDispatched <= 0.0 && completed == 1) continue;

        // Update quantities in sales order detail if posted
        // quantity = order quantity
        // qtyinvoiced = already dispatched
        // qtydispatched = current dispatch
        // completed = completed
        idx = odModel->index(row, odModel->fieldIndex("quantity"));
        double quantity = idx.data(Qt::DisplayRole).toDouble();
        idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
        double qtyInvoiced = idx.data(Qt::DisplayRole).toDouble();

        // Reset current dispatch and set total invoiced
        idx = odModel->index(row, odModel->fieldIndex("qtydispatched"));
        odModel->setData(idx, 0.0, Qt::EditRole);
        idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
        odModel->setData(idx, qtyInvoiced + qtyDispatched, Qt::EditRole);

        // If applicable set order detail completed
        if (quantity <= qtyInvoiced + qtyDispatched) {
            idx = odModel->index(row, odModel->fieldIndex("completed"));
            odModel->setData(idx, 1, Qt::EditRole);
        }

        // TODO: Stock movement booking, refer also above
//        f.getCogsAccount(query, salesArea, stockId, salesType);
//        query.first();
//        RB_String dtChartMasterId = query.value(0).toString();
//        RB_String dtChartMasterName = "";
//        // If chartmaster is not found set to default
//        if (dtChartMasterId == "") dtChartMasterId = "0";
//        int dtActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", dtChartMasterId).toInt();
//        // TODO: discount account is query.value(1)
    }

    // Save changes of qtydispatched, qtyinvoiced, completed
    odModel->submitAllAndSelect(); // NOTE: deselects the current order detail

    return true;
}
