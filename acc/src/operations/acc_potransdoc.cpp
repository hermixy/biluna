/*****************************************************************
 * $Id: acc_potransdoc.cpp 2156 2014-08-13 10:20:33Z rutger $
 * Created: Sep 26, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_potransdoc.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_PoTransDoc::ACC_PoTransDoc() {

}

/**
 * Destructor
 */
ACC_PoTransDoc::~ACC_PoTransDoc() {

}


/**
 * Execute the creation of line items and setting their taxes. Creates a
 * temporary debtor transactions with taxes
 * @param transDoc transaction document (or sales invoice)
 * @param ctList creditor transaction list, temporary container for the results
 * @param isPost true if posting of invoice is done, false if only preview of invoice
 * @return true on success
 */ /*
bool ACC_PoTransDoc::execute(RB_ObjectBase* transDoc, RB_ObjectContainer* ctList, bool isPost) {
    if (!ctList || !transDoc) return false;

    ctList->setValue("parent", ACC_MODELFACTORY->getRootId());

    RB_MmProxy* suppModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    RB_MmProxy* poModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrder);

    // The tax group id of the supplier
    RB_String taxGroupId = suppModel->getCurrentValue("taxgroup_id", RB2::RoleOrigData).toString();
    // Warehouse location for supplier delivery
    RB_String receiptLocId = poModel->getCurrentValue("receiptat_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxGroupId) || !RB_DEBUG->isValidId(receiptLocId)) {
        return false;
    }

    ACC_SqlCommonFunctions f;

    // set transaction document members
    RB_String whereStr = "accountcontrol=" + RB_String::number((int)ACC2::ControlCreditor);
    whereStr += " AND parent='" + ACC_MODELFACTORY->getRootId() + "'";
    RB_String parentId = f.selectFromWhere("id", "acc_chartmaster", whereStr).toString();
    transDoc->setValue("parent", parentId);
    RB_String actName = f.selectFromWhereId("accountname", "acc_chartmaster", parentId).toString();
    transDoc->setValue("doctype", (int)ACC2::TransPurchOrder);
    transDoc->setValue("docname", actName);
    transDoc->setValue("salestype_id", 0 / *suppModel->getCurrentValue("salestype_id", RB2::RoleOrigData)* /);
    QDateTime invoiceDate = poModel->getCurrentValue("suppinvoicedate").toDateTime();
    transDoc->setValue("transdate", invoiceDate);
    RB_String paymentId = suppModel->getCurrentValue("paymentterm_id", RB2::RoleOrigData).toString();
    QDateTime dueDate;
    f.setDueDate(paymentId, dueDate);
    transDoc->setValue("duedate", dueDate);
//    int number = isPost ? f.getNextTransNo((int)ACC2::TransCreditor, "none", "Creditor") : -1; // only at post
    transDoc->setValue("transno", -1);
    transDoc->setValue("debtor_idx", "0");
    transDoc->setValue("creditor_idx", suppModel->getCurrentId());
    transDoc->setDValue("creditor_idx", suppModel->getCurrentValue("suppname").toString()); // display value
    transDoc->setValue("refno", poModel->getCurrentValue("supplierref", RB2::RoleOrigData));
    RB_String str = "PO";
    str +=  " " + poModel->getCurrentValue("orderno").toString();
    transDoc->setValue("description", str);
    // duedate depending payment terms at invoice processing
    // TODO: rate exchange rate
    // TODO: diffonexch
    transDoc->setValue("alloc", 0.0);
    transDoc->setValue("settled", 0);
    transDoc->setValue("hold", 0);
    transDoc->setValue("edisent", 0);
    transDoc->setValue("consignment", 0);
    transDoc->setValue("order_id", poModel->getCurrentValue("id", RB2::RoleOrigData));
//    transDoc->setValue("invoicecomment", poModel->getCurrentValue("invoicecomment", RB2::RoleOrigData));

    double totalAmount = 0.0;
    double totalTax = 0.0;
    double amount = 0.0;

    ctList->erase();

    // get dispatch taxprovince id
    RB_String taxProvinceId = f.selectFromWhereId("taxprovince_id", "acc_location", receiptLocId).toString();
    // RB_String salesAreaId = suppModel->getCurrentValue("area_id", RB2::RoleOrigData).toString();
    // RB_String salesTypeId = "0";

    if (!RB_DEBUG->isValidId(taxProvinceId)) {
        // Error
        return false;
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrderDetail);

    RB_String ctParent = "";
    RB_String ctTransDocId = "";
    RB_String ctDescription = "";
    QModelIndex idx;

    // Create from purchase line items the creditor transactions list
    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtyreceived"));
        double qtyReceived = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyReceived <= 0.0 && completed == 1) continue;

        // Update quantities in purchase order detail if posted
//        if (isPost) {
//            // quantity = order quantity
//            // qtyinvoiced = already received
//            // qtyreceived = current receipt
//            // completed = completed
//            idx = odModel->index(row, odModel->fieldIndex("quantity"));
//            double quantity = idx.data(Qt::DisplayRole).toDouble();
//            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
//            double qtyInvoiced = idx.data(Qt::DisplayRole).toDouble();

//            // Reset current receipt and set total invoiced
//            idx = odModel->index(row, odModel->fieldIndex("qtyreceived"));
//            odModel->setData(idx, 0.0, Qt::EditRole);
//            idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
//            odModel->setData(idx, qtyInvoiced + qtyReceived, Qt::EditRole);

//            // If applicable set order detail completed
//            if (quantity <= qtyInvoiced + qtyReceived) {
//                idx = odModel->index(row, odModel->fieldIndex("completed"));
//                odModel->setData(idx, 1, Qt::EditRole);
//            }
//        }

        // Prepare data for creditor transaction
        RB_ObjectBase* ct = NULL;
        ctParent = ACC_MODELFACTORY->getRootId();

        ctTransDocId = transDoc->getId();

        idx = odModel->index(row, odModel->fieldIndex("stkdescription"));
        ctDescription = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("stk_idx"));
        RB_String stockCode = idx.data(Qt::DisplayRole).toString();
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterId = idx.data(RB2::RoleOrigData).toString();
        idx = odModel->index(row, odModel->fieldIndex("costcenter_idx"));
        RB_String costCenterCode = idx.data(Qt::DisplayRole).toString();

        query.clear();

        idx = odModel->index(row, odModel->fieldIndex("id"));
        RB_String ctOrderDetailId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        idx = odModel->index(row, odModel->fieldIndex("taxcat_id"));
        RB_String taxCatId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        RB_String stockCatId = f.selectFromWhereId("parent", "acc_stockmaster", stockId).toString();
//        f.getTaxAuthId(query, taxGroupId, taxProvinceId, taxCatId);
//        if (!query.first()){
//            RB_String str = " Tax Location ID = " + taxGroupId + "\n";
//            str += " Tax Province ID = " + taxProvinceId + "\n";
//            str += " Tax Category ID = " + taxCatId;
//            ACC_DIALOGFACTORY->commandMessage(
//                        "Could not find Tax Authority ID in ACC_PoTransDoc:\n" + str);
//            return false;
//        }

//        RB_String taxAuthId = query.value(0).toString();
        query.clear();
        f.getPurchaseGlAccount(query, taxGroupId, stockCatId);

        if (!query.first()){
            RB_String str = "Tax Group ID = " + taxGroupId + "\n";
            str += " Stock Category ID = " + stockCatId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Purchase GL Account in ACC_PoTransDoc:\n" + str);
            return false;
        }


        RB_String ctChartMasterId = query.value(0).toString();
        RB_String ctChartMasterName = query.value(1).toString();
        int ctActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", ctChartMasterId).toInt();
        // TODO: discount account

        // Process tax data
        bool isFirstTax = true;
        RB_String prevTaxAuthId = "";
        RB_String prevTaxChartMasterId = "";
        RB_String prevTaxChartMasterName = "";
        double taxAmt = 0.0;

        // First debtor transaction
        ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                          ctList, "", true);
        ct->setValue("parent", ctParent); // parent is ACC_Project ID
        ct->setValue("transdoc_id", ctTransDocId);
        ct->setValue("description", ctDescription);
        RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(ctChartMasterId);
        if (obj) {
            ctChartMasterName = obj->getValue("accountcode").toString()
                    + " - " + obj->getValue("accountname").toString();
            ct->setValue("chartmaster_idx", ctChartMasterId);
            ct->setDValue("chartmaster_idx", ctChartMasterName);
        } else {
            RB_DEBUG->error("ACC_PoTransDoc::exec() ACC_ChartMaster object ERROR");
        }
        ct->setValue("accountcontrol", ctActCtrl);
        ct->setValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxHighId());
        ct->setDValue("taxhighchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxHighName());
        ct->setValue("taxhighamt", 0.0);
        ct->setValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxLowId());
        ct->setDValue("taxlowchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxLowName());
        ct->setValue("taxlowamt", 0.0);
        ct->setValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxOtherId());
        ct->setDValue("taxotherchartmaster_idx", ACC_QACHARTMASTER->getAccPurchaseTaxOtherName());
        ct->setValue("hold", 0);
        ct->setValue("orderdetail_id", ctOrderDetailId);
        ct->setValue("taxotheramt", 0.0);

        // Data mainly for invoice line details
        idx =  odModel->index(row, odModel->fieldIndex("id"));
        RB_String str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("orderdetail_id", str);
        ct->setValue("stk_idx", stockId);
        ct->setDValue("stk_idx", stockCode);
        ct->setValue("costcenter_idx", costCenterId);
        ct->setDValue("costcenter_idx", costCenterCode);
        idx =  odModel->index(row, odModel->fieldIndex("narrative"));
        str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("narrative", str);
        idx =  odModel->index(row, odModel->fieldIndex("unitprice"));
        double amt = idx.data(RB2::RoleOrigData).toDouble();
        ct->setValue("unitprice", amt);
        ct->setValue("qtyreceived", qtyReceived);
        idx =  odModel->index(row, odModel->fieldIndex("stkuom"));
        str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("stkuom", str);
        idx =  odModel->index(row, odModel->fieldIndex("discountpercent"));
        double discount = idx.data(RB2::RoleOrigData).toDouble();
        ct->setValue("discountpercent", discount);

        amount = amt * qtyReceived * (1 - discount / 100.0);

        // Prevent bookings with zero (0.00) amount
        if (amount > -0.005 && amount < 0.005) {
            ctList->remove(ct); // remove and delete
            ct = NULL;
            continue;
        }

        totalAmount += amount;
        ct->setValue("amount", amount);

        // Set taxes, already sorted on calculation order
        query.clear();
        f.getPurchaseTaxes(query, taxGroupId, taxProvinceId, taxCatId);

        // 0 = acc_taxgroupauth.calculationorder
        // 1 = acc_taxauthority.description
        // 2 = acc_taxgroupauth.taxauth_id
        // 3 = acc_taxgroupauth.taxontax
        // 4 = acc_taxauthrate.taxrate
        // 5 = acc_purchasetaxposting.salestaxacct_idx

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
                RB_DEBUG->error("ACC_PoTransDoc::execute() tax account2 not found ERROR");
            }
        }

        query.clear();

        RB_ObjectIterator* iter = taxList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* tax = iter->currentObject();

            if (!isFirstTax && (prevTaxAuthId != tax->getValue("taxauth_id").toString()
                    || prevTaxChartMasterId != tax->getIdValue("taxacct_idx").toString()
                    || tax->getValue("taxauth_id").toString().isEmpty()
                    || tax->getIdValue("taxglacct_idx").toString().isEmpty())) {
                // Create new debtor transaction
                ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                  ctList, "", true);
                prevTaxAuthId = tax->getValue("taxauth_id").toString();
                prevTaxChartMasterId = tax->getIdValue("taxacct_idx").toString();
                prevTaxChartMasterName = tax->getDValue("taxacct_idx").toString();

                ct->setValue("parent", ctParent);
                ct->setValue("transdoc_id", ctTransDocId);
                ct->setValue("description", ctDescription);
                ct->setValue("chartmaster_idx", ctChartMasterId);
                ct->setDValue("chartmaster_idx", ctChartMasterName);
                ct->setValue("accountcontrol", ctActCtrl);
                if (isFirstTax) {
                    ct->setValue("amount", amount);
                    isFirstTax = false;
                } else {
                    ct->setValue("amount", 0.0);
                }
                ct->setValue("taxhighchartmaster_idx", "0");
                ct->setValue("taxhighamt", 0.0);
                ct->setValue("taxlowchartmaster_idx", "0");
                ct->setValue("taxlowamt", 0.0);
                ct->setValue("taxotherchartmaster_idx", prevTaxChartMasterId);
                ct->setDValue("taxotherchartmaster_idx", prevTaxChartMasterName);
                ct->setValue("hold", 0);
                ct->setValue("orderdetail_id", ctOrderDetailId);
                ct->setValue("taxauth_id", tax->getValue("taxauth_id"));

                // Set tax amount
                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt = (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt = amount * tax->getValue("taxrate").toDouble() / 100;
                }
            } else {
                ct->setValue("taxauth_id", tax->getValue("taxauth_id"));

                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt += (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt += amount * tax->getValue("taxrate").toDouble() / 100;
                }
            }

            // tax is booked on relevant account
            // Note: not ACC2::ControlPurchaseAllTax
            if (ctActCtrl == ACC2::ControlPurchaseHighTax) {           // 32
                ct->setValue("taxhighamt", taxAmt);
            } else if (ctActCtrl == ACC2::ControlPurchaseLowTax) {     // 31
                ct->setValue("taxlowamt", taxAmt);
            } else if (ctActCtrl == ACC2::ControlPurchaseOtherTax) {   // 33
                ct->setValue("taxotheramt", taxAmt);
            }
        }

        totalTax += taxAmt;
        delete taxList;
        taxList = NULL;
    }

    // Freightcost part of Sales Order only, for later?
//    double freightCost = poModel->getCurrentValue("freightcost", Qt::DisplayRole).toDouble();

//    if (freightCost >= 0.005 && totalAmount >= 0.005) {
//        RB_ObjectBase* ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
//                                                         ctList, "", true);
//        ct->setValue("parent", ctParent);
//        ct->setValue("transdoc_id", ctTransDocId);
//        ct->setValue("description", tr("Freight cost") + " " + ctDescription);
//        RB_String freightActId = ACC_MODELFACTORY->getRoot()->getIdValue("freightact_idx").toString();
//        ct->setValue("chartmaster_id", freightActId);
//        int actCtrl = f.selectFromWhereId("accountcontrol", "acc_charmaster", freightActId).toInt();
//        ct->setValue("accountcontrol", actCtrl);
//        ct->setValue("amount", freightCost);
//        ct->setValue("taxhighchartmaster_idx", "0");
//        ct->setValue("taxhighamt", 0.0);
//        ct->setValue("taxlowchartmaster_idx", "0");
//        ct->setValue("taxlowamt", 0.0);
//        ct->setValue("taxotherchartmaster_idx", "0");
//        ct->setValue("taxotheramt", 0.0);
//        ct->setValue("hold", 0);
//        ct->setValue("orderdetail_id", "0");
//    }

    totalAmount += totalTax / *+ freightCost* /;

    transDoc->setValue("totalamountrec", 0);
    transDoc->setValue("totalamountpay", totalAmount);
    transDoc->setValue("totaltax", totalTax);

    // Save changes of qtyreceived, qtyinvoiced, completed
//    odModel->submitAllAndSelect(); // NOTE: deselects the current order detail

    return true;
}
*/
/**
 * Prepare the order for preview of the purchase order invoice,
 * without updating qtyinvoiced, qtyreceived, stock moves
 * and collecting the invoice system number
 * @param transdoc transaction document
 * @param ctList creditor list (transaction document items)
 */
bool ACC_PoTransDoc::preparePreview(RB_ObjectBase* transDoc, RB_ObjectContainer* ctList) {
    if (!ctList || !transDoc) return false;

    ctList->setValue("parent", ACC_MODELFACTORY->getRootId());

    RB_MmProxy* suppModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    RB_MmProxy* poModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrder);

    // The tax group id of the supplier
    RB_String taxGroupId = suppModel->getCurrentValue("taxgroup_id", RB2::RoleOrigData).toString();
    // Warehouse location for supplier delivery
    RB_String receiptLocId = poModel->getCurrentValue("receiptat_id", RB2::RoleOrigData).toString();

    if (!RB_DEBUG->isValidId(taxGroupId) || !RB_DEBUG->isValidId(receiptLocId)) {
        RB_String str = " Tax Group ID = " + taxGroupId + "\n";
        str += " Receipt Location ID = " + receiptLocId;
        ACC_DIALOGFACTORY->commandMessage(
                    "Tax Group or Receipt Location invalid, check:\n" + str);
        return false;
    }

    ACC_SqlCommonFunctions f;

    // set transaction document members
    RB_String whereStr = "accountcontrol=" + RB_String::number((int)ACC2::ControlCreditor);
    whereStr += " AND parent='" + ACC_MODELFACTORY->getRootId() + "'";
    RB_String parentId = f.selectFromWhere("id", "acc_chartmaster", whereStr).toString();
    transDoc->setValue("parent", parentId);
    RB_String actName = f.selectFromWhereId("accountname", "acc_chartmaster", parentId).toString();
    transDoc->setValue("doctype", (int)ACC2::TransPurchOrder);
    transDoc->setValue("docname", actName);
    transDoc->setValue("salestype_id", 0 /*suppModel->getCurrentValue("salestype_id", RB2::RoleOrigData)*/);
    QDateTime invoiceDate = poModel->getCurrentValue("suppinvoicedate").toDateTime();
    transDoc->setValue("transdate", invoiceDate);
    RB_String paymentId = suppModel->getCurrentValue("paymentterm_id", RB2::RoleOrigData).toString();
    QDateTime dueDate;
    f.setDueDate(paymentId, dueDate);
    transDoc->setValue("duedate", dueDate);
    transDoc->setValue("transno", -1);
    transDoc->setValue("debtor_idx", "0");
    transDoc->setValue("creditor_idx", suppModel->getCurrentId()
                       + suppModel->getCurrentValue("suppname").toString());
    transDoc->setValue("refno", poModel->getCurrentValue("supplierref", RB2::RoleOrigData));
    RB_String str = "PO";
    str +=  " " + poModel->getCurrentValue("orderno").toString();

    RB_String orderTitleStr = poModel->getCurrentValue("ordertitle").toString();

    if (!orderTitleStr.isEmpty()) {
        str += " " + orderTitleStr;
    }

    RB_String contactStr = poModel->getCurrentValue("contact_idx").toString();

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
    transDoc->setValue("order_id", poModel->getCurrentValue("id", RB2::RoleOrigData));

    double totalAmount = 0.0;
    double totalTax = 0.0;
    double amount = 0.0;

    ctList->erase();

    // get dispatch taxprovince id
    RB_String taxProvinceId = f.selectFromWhereId("taxprovince_id", "acc_location", receiptLocId).toString();

    if (!RB_DEBUG->isValidId(taxProvinceId)) {
        // Error
        RB_String str = "Not a valid tax province found with:\n";
        str = " Receipt Location ID = " + receiptLocId;
        ACC_DIALOGFACTORY->commandMessage(str);
        return false;
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrderDetail);

    RB_String ctParent = "";
    RB_String ctTransDocId = "";
    RB_String ctDescription = "";
    QModelIndex idx;

    // Create from purchase line items the creditor transactions list
    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtyreceived"));
        double qtyReceived = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyReceived <= 0.0 && completed == 1) continue;

        // Prepare data for creditor transaction
        RB_ObjectBase* ct = NULL;
        ctParent = ACC_MODELFACTORY->getRootId();

        ctTransDocId = transDoc->getId();

        idx = odModel->index(row, odModel->fieldIndex("stkdescription"));
        ctDescription = idx.data(Qt::DisplayRole).toString();
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
        RB_String ctOrderDetailId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        idx = odModel->index(row, odModel->fieldIndex("taxcat_id"));
        RB_String taxCatId = odModel->hiddenData(idx, RB2::RoleOrigData).toString();

        RB_String stockCatId = f.selectFromWhereId("parent", "acc_stockmaster", stockId).toString();

        query.clear();

        f.getPurchaseGlAccount(query, taxGroupId, stockCatId);

        if (!query.first()){
            RB_String str = "Tax Group ID = " + taxGroupId + "\n";
            str += " Stock Category ID = " + stockCatId;
            ACC_DIALOGFACTORY->commandMessage(
                        "Could not find Purchase GL Account in ACC_PoTransDoc:\n" + str);
            return false;
        }


        RB_String ctChartMasterId = query.value(0).toString();
        RB_String ctChartMasterName = query.value(1).toString();
        int ctActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", ctChartMasterId).toInt();
        // TODO: discount account

        // Process tax data
        bool isFirstTax = true;
        RB_String prevTaxAuthId = "";
        RB_String prevTaxChartMasterId = "";
        RB_String prevTaxChartMasterName = "";
        double taxAmt = 0.0;

        // First debtor transaction
        ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                          ctList, "", true);
        ct->setValue("parent", ctParent); // parent is ACC_Project ID
        ct->setValue("transdoc_id", ctTransDocId);
        ct->setValue("description", ctDescription);
        RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(ctChartMasterId);
        if (obj) {
            ctChartMasterName = obj->getValue("accountcode").toString()
                    + " - " + obj->getValue("accountname").toString();
            ct->setValue("chartmaster_idx", ctChartMasterId + ctChartMasterName);
        } else {
            RB_DEBUG->error("ACC_PoTransDoc::exec() ACC_ChartMaster object ERROR");
        }
        ct->setValue("accountcontrol", ctActCtrl);
        ct->setValue("taxhighchartmaster_idx",
                     ACC_QACHARTMASTER->getAccPurchaseTaxHighId()
                     + ACC_QACHARTMASTER->getAccPurchaseTaxHighName());
        ct->setValue("taxhighamt", 0.0);
        ct->setValue("taxlowchartmaster_idx",
                     ACC_QACHARTMASTER->getAccPurchaseTaxLowId()
                     + ACC_QACHARTMASTER->getAccPurchaseTaxLowName());
        ct->setValue("taxlowamt", 0.0);
        ct->setValue("taxotherchartmaster_idx",
                     ACC_QACHARTMASTER->getAccPurchaseTaxOtherId()
                     + ACC_QACHARTMASTER->getAccPurchaseTaxOtherName());
        ct->setValue("hold", 0);
        ct->setValue("orderdetail_id", ctOrderDetailId);
        ct->setValue("taxotheramt", 0.0);

        // Data mainly for invoice line details
        idx =  odModel->index(row, odModel->fieldIndex("id"));
        RB_String str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("orderdetail_id", str);
        ct->setValue("stk_idx", stockId + stockCode);
        ct->setValue("costcenter_idx", costCenterId + costCenterCode);
        idx =  odModel->index(row, odModel->fieldIndex("narrative"));
        str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("narrative", str);
        idx =  odModel->index(row, odModel->fieldIndex("unitprice"));
        double amt = idx.data(RB2::RoleOrigData).toDouble();
        ct->setValue("unitprice", amt);
        ct->setValue("qtyreceived", qtyReceived);
        idx =  odModel->index(row, odModel->fieldIndex("stkuom"));
        str = idx.data(RB2::RoleOrigData).toString();
        ct->setValue("stkuom", str);
        idx =  odModel->index(row, odModel->fieldIndex("discountpercent"));
        double discount = idx.data(RB2::RoleOrigData).toDouble();
        ct->setValue("discountpercent", discount);

        amount = amt * qtyReceived * (1 - discount / 100.0);

        // Prevent bookings with zero (0.00) amount
        if (amount > -0.005 && amount < 0.005) {
            ctList->remove(ct); // remove and delete
            ct = NULL;
            continue;
        }

        totalAmount += amount;
        ct->setValue("amount", amount);

        // Set taxes, already sorted on calculation order
        query.clear();
        f.getPurchaseTaxes(query, taxGroupId, taxProvinceId, taxCatId);

        // 0 = acc_taxgroupauth.calculationorder
        // 1 = acc_taxauthority.description
        // 2 = acc_taxgroupauth.taxauth_id
        // 3 = acc_taxgroupauth.taxontax
        // 4 = acc_taxauthrate.taxrate
        // 5 = acc_purchasetaxposting.salestaxacct_idx

        // Create a temporay container with the tax data
        RB_ObjectContainer* taxList = new RB_ObjectContainer();

        while (query.next()) {
            RB_ObjectBase* tax = new RB_ObjectAtomic("", taxList, "");
            taxList->addObject(tax);
            tax->addMember("taxauth_id", "-", query.value(2), RB2::MemberChar40);
            tax->addMember("taxrate", "-", query.value(4), RB2::MemberDouble);
            tax->addMember("taxontax", "-", query.value(3), RB2::MemberInteger);
            tax->addMember("taxcalculationorder", "-", query.value(0), RB2::MemberInteger);

            RB_String taxAcctId = query.value(5).toString();
            obj = ACC_QACHARTMASTER->getAcctObj(taxAcctId);

            if (obj) {
                RB_String str = taxAcctId
                        + obj->getValue("accountcode").toString() + " - "
                        + obj->getValue("accountname").toString();
                tax->addMember("taxacct_idx", "-", str, RB2::MemberChar40);
            } else {
                RB_DEBUG->error("ACC_PoTransDoc::execute() tax account2 not found ERROR");
            }
        }

        query.clear();

        RB_ObjectIterator* iter = taxList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* tax = iter->currentObject();

            if (!isFirstTax && (prevTaxAuthId != tax->getValue("taxauth_id").toString()
                    || prevTaxChartMasterId != tax->getIdValue("taxacct_idx").toString()
                    || tax->getValue("taxauth_id").toString().isEmpty()
                    || tax->getIdValue("taxglacct_idx").toString().isEmpty())) {
                // Create new debtor transaction
                ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                  ctList, "", true);
                prevTaxAuthId = tax->getValue("taxauth_id").toString();
                prevTaxChartMasterId = tax->getIdValue("taxacct_idx").toString();
                prevTaxChartMasterName = tax->getDValue("taxacct_idx").toString();

                ct->setValue("parent", ctParent);
                ct->setValue("transdoc_id", ctTransDocId);
                ct->setValue("description", ctDescription);
                ct->setValue("chartmaster_idx", ctChartMasterId
                             + ctChartMasterName);
                ct->setValue("accountcontrol", ctActCtrl);
                if (isFirstTax) {
                    ct->setValue("amount", amount);
                    isFirstTax = false;
                } else {
                    ct->setValue("amount", 0.0);
                }
                ct->setValue("taxhighchartmaster_idx", "0");
                ct->setValue("taxhighamt", 0.0);
                ct->setValue("taxlowchartmaster_idx", "0");
                ct->setValue("taxlowamt", 0.0);
                ct->setValue("taxotherchartmaster_idx", prevTaxChartMasterId
                             + prevTaxChartMasterName);
                ct->setValue("hold", 0);
                ct->setValue("orderdetail_id", ctOrderDetailId);
                ct->setValue("taxauth_id", tax->getValue("taxauth_id"));

                // Set tax amount
                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt = (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt = amount * tax->getValue("taxrate").toDouble() / 100;
                }
            } else {
                ct->setValue("taxauth_id", tax->getValue("taxauth_id"));

                if (tax->getValue("taxontax").toInt() != 0) {
                    taxAmt += (amount + taxAmt) * tax->getValue("taxrate").toDouble() / 100;
                } else {
                    taxAmt += amount * tax->getValue("taxrate").toDouble() / 100;
                }
            }

            // tax is booked on relevant account
            // Note: not ACC2::ControlPurchaseAllTax
            if (ctActCtrl == ACC2::ControlPurchaseHighTax) {           // 32
                ct->setValue("taxhighamt", taxAmt);
            } else if (ctActCtrl == ACC2::ControlPurchaseLowTax) {     // 31
                ct->setValue("taxlowamt", taxAmt);
            } else if (ctActCtrl == ACC2::ControlPurchaseOtherTax) {   // 33
                ct->setValue("taxotheramt", taxAmt);
            }
        }

        totalTax += taxAmt;
        delete taxList;
        taxList = NULL;
    }

    // Freightcost part of Sales Order only, for later?
//    double freightCost = poModel->getCurrentValue("freightcost", Qt::DisplayRole).toDouble();

//    if (freightCost >= 0.005 && totalAmount >= 0.005) {
//        RB_ObjectBase* ct = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
//                                                         ctList, "", true);
//        ct->setValue("parent", ctParent);
//        ct->setValue("transdoc_id", ctTransDocId);
//        ct->setValue("description", tr("Freight cost") + " " + ctDescription);
//        RB_String freightActId = ACC_MODELFACTORY->getRoot()->getValue("freightact_idx").toString();
//        ct->setValue("chartmaster_id", freightActId);
//        int actCtrl = f.selectFromWhereId("accountcontrol", "acc_charmaster", freightActId).toInt();
//        ct->setValue("accountcontrol", actCtrl);
//        ct->setValue("amount", freightCost);
//        ct->setValue("taxhighchartmaster_idx", "0");
//        ct->setValue("taxhighamt", 0.0);
//        ct->setValue("taxlowchartmaster_idx", "0");
//        ct->setValue("taxlowamt", 0.0);
//        ct->setValue("taxotherchartmaster_idx", "0");
//        ct->setValue("taxotheramt", 0.0);
//        ct->setValue("hold", 0);
//        ct->setValue("orderdetail_id", "0");
//    }

    totalAmount += totalTax /*+ freightCost*/;

    transDoc->setValue("totalamountrec", 0);
    transDoc->setValue("totalamountpay", totalAmount);
    transDoc->setValue("totaltax", totalTax);

    return true;
}

/**
 * Prepare the order for the posting of the purchase order invoice,
 * setting updating qtyinvoiced, qtyreceived, stock moves
 * and collecting the invoice system number
 * @param transdoc transaction document
 */
bool ACC_PoTransDoc::preparePost(RB_ObjectBase* transDoc) {
    if (!transDoc) return false;

    RB_MmProxy* poModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrder);
    QModelIndex idx = poModel->getCurrentIndex();
    if (!idx.isValid()) return false;

    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrderDetail);

    ACC_SqlCommonFunctions f;
    int number = f.getNextTransNo((int)ACC2::TransCreditor, "none", "Creditor");
    transDoc->setValue("transno", number);

    // Create from purchase line items the creditor transactions list
    for (int row = 0; row < odModel->rowCount(); row++) {
        idx = odModel->index(row, odModel->fieldIndex("qtyreceived"));
        double qtyReceived = idx.data(Qt::DisplayRole).toDouble();
        idx =  odModel->index(row, odModel->fieldIndex("completed"));
        int completed = idx.data(Qt::DisplayRole).toInt();

        // Check if a relevant quantity is dispatched (>0) and not completed (=0)
        if (qtyReceived <= 0.0 && completed == 1) continue;

        // quantity = order quantity
        // qtyinvoiced = already received
        // qtyreceived = current receipt
        // completed = completed
        idx = odModel->index(row, odModel->fieldIndex("quantity"));
        double quantity = idx.data(Qt::DisplayRole).toDouble();
        idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
        double qtyInvoiced = idx.data(Qt::DisplayRole).toDouble();

        // Reset current receipt and set total invoiced
        idx = odModel->index(row, odModel->fieldIndex("qtyreceived"));
        odModel->setData(idx, 0.0, Qt::EditRole);
        idx = odModel->index(row, odModel->fieldIndex("qtyinvoiced"));
        odModel->setData(idx, qtyInvoiced + qtyReceived, Qt::EditRole);

        // If applicable set order detail completed
        if (quantity <= qtyInvoiced + qtyReceived) {
            idx = odModel->index(row, odModel->fieldIndex("completed"));
            odModel->setData(idx, 1, Qt::EditRole);
        }

        // TODO: Stock movement booking
//        f.getCogsGlAccount(query, salesArea, stockId/*, salesType*/);
//        query.first();
//        RB_String ctChartMasterId = query.value(0).toString();
//        RB_String ctChartMasterName = "";
//        // If chartmaster is not found set to default
//        if (ctChartMasterId == "") ctChartMasterId = "0";
//        int ctActCtrl = f.selectFromWhereId("accountcontrol", "acc_chartmaster", ctChartMasterId).toInt();
        // TODO: discount account is query.value(1)
    }

    // Save changes of qtyreceived, qtyinvoiced, completed
    odModel->submitAllAndSelect(); // NOTE: deselects the current order detail

    return true;
}
