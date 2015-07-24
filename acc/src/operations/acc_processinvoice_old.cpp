/*****************************************************************
 * $Id: acc_processinvoice.cpp 1447 2011-08-09 17:57:46Z rutger $
 * Created: Mar 22, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_processinvoice.h"

#include "acc.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 * NOTE: class is not used anymore for reference only
 */
ACC_ProcessInvoice::ACC_ProcessInvoice() {
    mStockMovesList = NULL;
    mDebtorTrans = NULL;
    mGlTransList = NULL;
}

/**
 * Destructor
 */
ACC_ProcessInvoice::~ACC_ProcessInvoice() {
    delete mGlTransList;
}

/**
 * Execute the posting of the invoice to the General Ledger
 * @param td transaction document (sales invoice data)
 * @param dtList debtor transactions or detail items on the sales invoice
 * @return true on success
 */
bool ACC_ProcessInvoice::execute(RB_ObjectBase* td,
                                 RB_ObjectContainer* dtList) {
    if (!td || !dtList) return false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //
    // START TRANSACTION
    //
    ACC_MODELFACTORY->getDatabase().transaction();
    ACC_SqlCommonFunctions f;

    // invoice number
    int invoiceNo = f.getNextTransNo(ACC2::TransDebtor);
    td->setValue("transno", invoiceNo);

    // date
    QDateTime today = QDateTime::currentDateTime();
    td->setValue("transdate", today);

    // due date
    RB_String debtorId =  td->getValue("debtor_id").toString();
    QDateTime duedate = f.getDueDate(debtorId, "", today);
    td->setValue("duedate", duedate);




    ACC_MODELFACTORY->getDatabase().commit();
    //
    // END TRANSACTION
    //
    QApplication::restoreOverrideCursor();

    return true;

}

/**
 * @returns true if quantity dispatched of the current order is positive
 * @param mStockMovesList
 * @return true if dispatched quantities are >=0
 */
bool ACC_ProcessInvoice::isQuantityDispatchedPositive() {
    if (!mStockMovesList) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_Operation::isQuantityDispatchedPositive() ERROR");
        return false;
    }

    bool success = true;
    RB_ObjectIterator* iterSm = mStockMovesList->createIterator();

    for (iterSm->first(); !iterSm->isDone() || !success; iterSm->next()) {
        RB_ObjectBase* stockMoves = iterSm->currentObject();

        if (stockMoves->getValue("qty").toDouble() < 0) {
            success = false;
        }
    }

    delete iterSm;
    return success;
}

/**
 * Execute debtor transaction
 * @return true on success
 */
bool ACC_ProcessInvoice::debtorTransaction() {
    if (!mStockMovesList || !mDebtorTrans) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_Operation::initDebtorTransaction() ERROR");
        return false;
    }

//    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(
//            ACC_ModelFactory::ModelCustomers);

    int invoiceNo = -1;

    // debtorsmaster.currcode = currencies.currabrev
    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(
            ACC_ModelFactory::ModelCustomers);
    double rate = 1.0;
    RB_String currId = custModel->getCurrentValue("currcode", (Qt::UserRole + 1)).toString();
    ACC_SqlCommonFunctions* sql = new ACC_SqlCommonFunctions();
    RB_Variant val = sql->selectFromWhereId("rate", "ACC_Currencies", currId);
    delete sql;
    if (val.isValid()) {
        rate = val.toDouble();
        mDebtorTrans->setValue("rate", val);
    }

    // ACC_DebtorTrans fields, not all are set, settled, ovdiscount, diffonexch,
    // alloc and edisent
    mDebtorTrans->setValue("transno", invoiceNo);
    mDebtorTrans->setValue("type", (int)ACC2::TransDebtor); // 10 is sales invoice
    mDebtorTrans->setValue("branchcode", custModel->getCurrentId());

    QDateTime dt = QDateTime::currentDateTime();
    mDebtorTrans->setValue("transdate", dt);

    // for period just take the year and month as a number 2010 and 03, becomes 201003
    int period = dt.date().year() *100 + dt.date().month();
    mDebtorTrans->setValue("prd", period);
    mDebtorTrans->setValue("reference", "");
    mDebtorTrans->setValue("tpe", custModel->getCurrentValue("salestype", (Qt::UserRole + 1)));

    RB_MmProxy* soModel = ACC_MODELFACTORY->getModel(
            ACC_ModelFactory::ModelSalesOrders);
    mDebtorTrans->setValue("salesorder", soModel->getCurrentId());
    mDebtorTrans->setValue("ovamount", mStockMovesList->getValue("totalnetprice"));
    mDebtorTrans->setValue("ovgst", mStockMovesList->getValue("totaltax"));
    mDebtorTrans->setValue("ovfreight", mStockMovesList->getValue("freightcost"));
    mDebtorTrans->setValue("invtext", soModel->getCurrentValue("invoicecomment", Qt::DisplayRole));
    mDebtorTrans->setValue("shipvia", custModel->getCurrentValue("defaultshipvia", Qt::EditRole));
    mDebtorTrans->setValue("consignment", soModel->getCurrentValue("consignment", Qt::DisplayRole));

    return true;
}

/**
 * Create and set debtor transaction taxes per authority id
 * @param debtorTrans debtor transation object
 * @param taxAuthId tax authority id
 * @param taxAmount tax amount
 * @param currencyRate currency rate
 * @return true on success
 */
bool ACC_ProcessInvoice::debtorTransTaxes(RB_ObjectBase* debtorTrans,
                                          const RB_String& taxAuthId,
                                          double taxAmount,
                                          double currencyRate) {

    if (!debtorTrans || taxAuthId.isEmpty() || taxAmount < 0.0
            || currencyRate <= 0.0) {
        return false;
    }

    RB_ObjectContainer* debtorTransTaxesList = debtorTrans->getContainer("ACC_DebtorTransTaxesList");
    RB_ObjectBase* debtorTransTaxes = NULL;
    RB_ObjectIterator* iter = debtorTransTaxesList->createIterator();

    for (iter->first(); !iter->isDone() && !debtorTransTaxes; iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getValue("taxauth_id").toString() == taxAuthId) {
            debtorTransTaxes = obj;
        }
    }

    delete iter;

    if (!debtorTransTaxes) {
        debtorTransTaxes = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid(),
                                                debtorTransTaxesList, "", true);
        debtorTransTaxes->setValue("taxauth_id", taxAuthId);
    }

    double tax = debtorTransTaxes->getValue("taxamount").toDouble();
    tax += taxAmount / currencyRate;
    debtorTransTaxes->setValue("taxamount", tax);

    return true;
}

/**
 * Execute debtor, stock and General Ledger transactions
 *
 * -> Salesorder, DONE
 * 1-Update sales order comment (in execute())
 *
 * -> TODO: Stockmoves and sales
 * 21-Update salesorderdetails quantities
 * 22-Insert orderdeliverydifferenceslog
 * 23-Update salesorderdetails for quantity invoiced and actual dispatch dates
 * 24-Update location stock records if not a dummy stock item M, B (MBflag Make, Buy)
 *    else A (Assembly)
 * 25-Insert stockmoves, different for M, B flag and A flag
 * 26-Insert stockmovestaxes
 * 27-Insert stockserialmovements
 * 28-Insert and update salesanalysis records
 *
 * -> General Ledger, DONE
 * 31-Cost of sales
 * 32-Stock
 * 33-Net sales
 * 34-Discount
 * 35-Gross sales
 * 36-Freight
 * 37-Tax
 *
 * @returns true on success
 */
bool ACC_ProcessInvoice::transaction() {
    if (!mDebtorTrans || !mStockMovesList) return false;

    bool success = debtorTransaction();

    RB_ObjectBase* project = ACC_MODELFACTORY->getRoot();
    RB_MmProxy* custModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomers);
    RB_String salesType = custModel->getCurrentValue("salestype", Qt::UserRole+1).toString();
    RB_String area = custModel->getCurrentValue("area", Qt::UserRole+1).toString();
    RB_MmProxy* odModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrderDetails);


    ACC_SqlCommonFunctions f;
    RB_String currId = custModel->getCurrentValue("currcode", Qt::UserRole + 1).toString();
    double currencyRate = f.selectFromWhereId("rate", "acc_currencies", currId).toDouble();

    if (mGlTransList) delete mGlTransList;
    mGlTransList = new RB_ObjectContainer("", NULL, "ACC_GlTransList", ACC_OBJECTFACTORY);

    // Not implemented according WebERP: Update default shipper-from-location if changed

    RB_ObjectIterator* iterSm = mStockMovesList->createIterator();

    for (iterSm->first(); !iterSm->isDone(); iterSm->next()) {
        RB_ObjectBase* stockMoves = iterSm->currentObject();

        RB_String salesOrderLineId = stockMoves->getValue("solineid").toString();
        double qtyOrdered = odModel->getValue(salesOrderLineId, "quantity").toDouble();
        double qtyInvoiced = odModel->getValue(salesOrderLineId, "qtyinvoiced").toDouble();
        double qtyDispatched = odModel->getValue(salesOrderLineId, "qtydispatched").toDouble();


        // 21- Update salesorderdetails (order) quantities
        //

        // ...

        // 22- Insert orderdeliverydifferenceslog, if short delivered
        //

        // ...


        if (qtyDispatched >= 0.0) {

            // 23- Update salesorderdetails for quantity invoiced and actual dispatch dates
            //
            odModel->setValue(salesOrderLineId, "qtyinvoiced",
                              qtyInvoiced+qtyDispatched, Qt::EditRole);
            odModel->setValue(salesOrderLineId, "actualdispatchdate",
                              QDateTime::currentDateTime().date(), Qt::EditRole);
            if (qtyDispatched >= (qtyOrdered - qtyInvoiced) /* || (BOPolicy == CAN) */) {
                odModel->setValue(salesOrderLineId, "completed",
                                  1, Qt::EditRole);
            }

            // 24- Update location stock records
            //

            // ...
        }



        RB_String salesGlCode = "0";
        RB_String discountGlCode = "0";

        QSqlQuery query(ACC_MODELFACTORY->getDatabase());
        f.getSalesGlAccount(query, area, stockMoves->getValue("parent").toString(), salesType);
        if (query.first()) {
            salesGlCode = query.value(0).toString();
            discountGlCode = query.value(1).toString();
        }

        if (project->getValue("gllink_stock").toInt() == 1
                && stockMoves->getValue("standardcost").toDouble() != 0.0) {

            // 31- Debit cost of sales at standard cost
            //
            RB_String cogsGlCode = "0";
            query.clear();
            f.getCogsGlAccount(query, area, stockMoves->getValue("parent").toString(), salesType);
            if (query.first()) {
                cogsGlCode = query.value(0).toString();
            }
            RB_String narrative = stockMoves->getValue("debtorno").toString() + " - "
                            + stockMoves->getValue("parent").toString() + " x "
                            + RB_String::number(stockMoves->getValue("qty").toDouble()) + " @ "
                            + RB_String::number(stockMoves->getValue("standardcost").toDouble());
            double totalStandardCost = stockMoves->getValue("qty").toDouble()
                                       * stockMoves->getValue("standardcost").toDouble();
            addGlTrans(cogsGlCode, narrative, +totalStandardCost);

            // 32- Credit stock
            //
            RB_String stockAct = "0";
            query.clear();
            f.getStockGlCode(query, stockMoves->getValue("parent").toString());
            if (query.first()) {
                stockAct = query.value(0).toString(); // stockact
            }

            addGlTrans(stockAct, narrative, -totalStandardCost);
        }

        if (project->getValue("gllink_debtors").toInt() == 1
                && (stockMoves->getValue("qty").toDouble()
                    * stockMoves->getValue("price").toDouble()) != 0.0) {

            // 33- Credit sales (Net sales)
            //
            RB_String narrative = stockMoves->getValue("debtorno").toString() + " - "
                            + stockMoves->getValue("parent").toString() + " x "
                            + RB_String::number(stockMoves->getValue("qty").toDouble()) + " @ "
                            + RB_String::number(stockMoves->getValue("price").toDouble());
            double salesPrice = stockMoves->getValue("qty").toDouble()
                                       * stockMoves->getValue("price").toDouble();
            addGlTrans(salesGlCode, narrative, -salesPrice/currencyRate);

            if (stockMoves->getValue("discountpercent") != 0.0) {

                // 34- Debit discount
                //
                narrative = stockMoves->getValue("debtorno").toString() + " - "
                                + stockMoves->getValue("parent").toString() + " @ "
                                + RB_String::number(stockMoves->getValue("discountpercent").toDouble()) + "%";
                double discount = stockMoves->getValue("qty").toDouble()
                                           * stockMoves->getValue("price").toDouble()
                                           * stockMoves->getValue("discountpercent").toDouble()/100.0;
                addGlTrans(discountGlCode, narrative, discount/currencyRate);
            }
        }

    }

    // freightcost, totalNetPrice and totalTax are members of stockMovesList
    // refer ACC_SetLineItemTax::exec() and ACC_TaxConfirmationDialog::init()
    iterSm->first();
    RB_ObjectBase* stockMoves = iterSm->currentObject();

    if (project->getValue("gllink_debtors").toInt() == 1) {

        // 35- Debit debtors
        //
        addGlTrans(project->getValue("debtorsact").toString(),
                   stockMoves->getValue("debtorno").toString(),
                   +(mStockMovesList->getValue("totalnetprice").toDouble()
                     + mStockMovesList->getValue("freightcost").toDouble()
                     + mStockMovesList->getValue("totaltax").toDouble()) / currencyRate);

        // 36- Freight
        //
        addGlTrans(project->getValue("freightact").toString(),
                   stockMoves->getValue("debtorno").toString(),
                   -mStockMovesList->getValue("freightcost").toDouble() / currencyRate);

        // 37- Tax
        //

        /*
        NOTE: the stockMovesList is iterated twice, here and in ACC_SetLineItemTax
        for the same purpose, calculating the tax per lineItem. Due to the different
        output requirements debtortrans and gltrans here, totalNetPrice and percent
        per lineItem in the ACC_SetLineItemTax this is still separate but a duplication.
        TODO: handle tax calculation at one central location.
        */

        RB_String taxGlCode = ""; // authority sales Gl account

        for (iterSm->first(); !iterSm->isDone(); iterSm->next()) {
            RB_ObjectBase* stockMoves = iterSm->currentObject();
            RB_ObjectContainer* taxList = stockMoves->getContainer("ACC_StockMovesTaxesList");

            double totalLineTax = 0.0; // total lineitem tax
            double qtyDisp = stockMoves->getValue("qty").toDouble();
            double price = stockMoves->getValue("price").toDouble();
            double discount = stockMoves->getValue("discountpercent").toDouble();
            double totalExcl = qtyDisp * price * (1-discount/100.0);

            if (taxList->countObject() <= 0) {
                double taxPercent = 0.0;
                double tax = totalExcl * taxPercent / 100.0;
                totalLineTax += tax;
            } else {
                double totalIncl = 0.0;

                // Iterate through all taxes for this item
                RB_ObjectIterator* iterTax = taxList->createIterator();

                for (iterTax->first(); !iterTax->isDone(); iterTax->next()) {
                    RB_ObjectBase* stockTax = iterTax->currentObject();
                    double taxPercent = stockTax->getValue("taxrate").toDouble();
                    taxGlCode = stockTax->getValue("salestaxglacct_id").toString();

                    if (stockTax->getValue("taxontax").toInt()) {
                        if (totalIncl <= 0.0) totalIncl = totalExcl;
                        double tax = totalIncl * taxPercent / 100.0;
                        totalIncl = totalIncl + tax;
                        totalLineTax += tax;
                        debtorTransTaxes(mDebtorTrans, stockTax->getValue("taxauth_id").toString(),
                                         tax, currencyRate);
                    } else {
                        double tax = totalExcl * taxPercent / 100.0;
                        totalIncl = totalExcl + tax;
                        totalLineTax += tax;
                        debtorTransTaxes(mDebtorTrans, stockTax->getValue("taxauth_id").toString(),
                                         tax, currencyRate);
                    }

                }

                delete iterTax;
            }

            addGlTrans(taxGlCode, stockMoves->getValue("debtorno").toString(),
                       -totalLineTax / currencyRate);
        }

        // Add total freight cost and tax separately,
        // re-use first lineitem to retrieve the taxes
        iterSm->first();

        if (!iterSm->isDone()) {
            double totalLineTax = 0.0; // total lineitem tax
            double freightCost = mStockMovesList->getValue("freightcost").toDouble();
            RB_ObjectBase* stockMoves = iterSm->currentObject();
            RB_ObjectContainer* taxList = stockMoves->getContainer("ACC_StockMovesTaxesList");

            if (taxList->countObject() <= 0) {
                double qtyDisp = 1.0;
                double price = freightCost;
                double discount = 0.0;
                double taxPercent = 0.0;
                double totalExcl = qtyDisp * price * (1-discount/100.0);
                double tax = totalExcl * taxPercent / 100.0;
                totalLineTax += tax;
            } else {
                double totalIncl = 0.0;

                // Iterate through all taxes for this item
                RB_ObjectIterator* iterTax = taxList->createIterator();

                for (iterTax->first(); !iterTax->isDone(); iterTax->next()) {
                    RB_ObjectBase* stockTax = iterTax->currentObject();
                    double qtyDisp = 1.0;
                    double price = freightCost;
                    double discount = 0.0;
                    double taxPercent = stockTax->getValue("taxrate").toDouble();

                    if (stockTax->getValue("taxontax").toInt()) {
                        double totalExcl = qtyDisp * price * (1-discount/100.0);
                        if (totalIncl <= 0.0) totalIncl = totalExcl;
                        double tax = totalIncl * taxPercent / 100.0;
                        // Cascades only if chain in authority of tax-on-tax
                        totalIncl = totalIncl + tax;
                        totalLineTax += tax;
                        debtorTransTaxes(mDebtorTrans, stockTax->getValue("taxauth_id").toString(),
                                         tax, currencyRate);
                    } else {
                        double totalExcl = qtyDisp * price * (1-discount/100.0);
                        double tax = totalExcl * taxPercent / 100.0;
                        totalIncl = totalExcl + tax;
                        totalLineTax += tax;
                        debtorTransTaxes(mDebtorTrans, stockTax->getValue("taxauth_id").toString(),
                                         tax, currencyRate);
                    }
                }

                delete iterTax;
            }

            addGlTrans(taxGlCode, stockMoves->getValue("debtorno").toString(),
                       -totalLineTax / currencyRate);
        }

    }

    delete iterSm;
    return success;
}

/**
 * Add General Ledger transaction with fixed: parent=modelRootId, type=10,
 * typeno(invoiceNo)=0, transdate="0000-00-00T00:00:00" and periodno=0. Other
 * values are set with parameters
 * @param accountId GL account
 * @param narrative narrative with this post
 * @param amount financial amount in Companies currency
 */
void ACC_ProcessInvoice::addGlTrans(const RB_String& accountId, const RB_String& narrative,
                                    double amount) {
    if (!mGlTransList) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_ProcessInvoice::addGlTrans() ERROR");
        return;
    }

    RB_ObjectBase* glTrans = NULL;
    glTrans = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                           mGlTransList, "ACC_GlTrans", true);
    glTrans->setValue("parent", ACC_MODELFACTORY->getRootId());
    glTrans->setValue("type", 10);
    glTrans->setValue("typeno", 0);                         // invoice number set at exec()
    glTrans->setValue("transdate", "0000-00-00T00:00:00");   // date set at exec()
    glTrans->setValue("periodno", 0);                       // period number set at exec()
    glTrans->setValue("account", accountId);
    glTrans->setValue("narrative", narrative);
    glTrans->setValue("amount", amount);
}
