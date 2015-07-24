/*****************************************************************
 * $Id: acc_createinvoiceaged.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Dec 8, 2010 12:29:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createinvoiceaged.h"

#include "acc.h"
#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_CreateInvoiceAged::ACC_CreateInvoiceAged() {
    mOverDueDays1 = 30;
    mOverDueDays2 = 60;
    mFromCustSupp = "0";
    mToCustSupp = "zzzzzz";
    mDocType = -1;
    mToDaysOpen = "";
    mNowDateTime = "";
    mCustSuppName = "";
    mCustSupp = "";
    mTotalAmountRecPay = "";
    mDocTypeSql = "";
}

/**
 * Destructor
 */
ACC_CreateInvoiceAged::~ACC_CreateInvoiceAged() {

}

/**
 * Create the GL transactions from the ACC_GlTrans table
 * @param glTransList container which will contain the GL transaction objects
 * @return true on success
 */
bool ACC_CreateInvoiceAged::execute(RB_ObjectBase* invoiceList) {
    if (!invoiceList) return false;

    RB_String qStr = "";

/* webERP, summary:
$SQL = "SELECT supplier.supplierid, supplier.suppname, currency.currency, paymentterm.terms,

SUM(supptrans.ovamount + supptrans.ovgst  - supptrans.alloc) as balance,

SUM(CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(supptrans.trandate)) >= paymentterm.daysbeforedue
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1', 'MONTH') . "), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= 0
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END) AS due,


SUM(CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(supptrans.trandate) > paymentterm.daysbeforedue AND TO_DAYS(Now())
        - TO_DAYS(supptrans.trandate) >= (paymentterm.daysbeforedue + " . $_SESSION['PastDueDays1'] . ")
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1', 'MONTH') ."), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= "
        . $_SESSION['PastDueDays1'] . ") THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END) AS overdue1,


SUM(CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(supptrans.trandate) > paymentterm.daysbeforedue	AND TO_DAYS(Now())
        - TO_DAYS(supptrans.trandate) >= (paymentterm.daysbeforedue + " . $_SESSION['PastDueDays2'] . ")
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1', 'MONTH') . "), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= "
        . $_SESSION['PastDueDays2'] . ") THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END) AS overdue2


FROM supplier, paymentterm, currency, supptrans WHERE supplier.paymentterm = paymentterm.termsindicator
    AND supplier.currcode = currency.currabrev
    AND supplier.supplierid = supptrans.supplierno
    AND supplier.supplierid >= '" . $_POST['FromCriteria'] . "'
    AND supplier.supplierid <= '" . $_POST['ToCriteria'] . "'
    AND  supplier.currcode ='" . $_POST['Currency'] . "'
GROUP BY supplier.supplierid,
    supplier.suppname,
    currency.currency,
    paymentterm.terms,
    paymentterm.daysbeforedue,
    paymentterm.dayinfollowingmonth
HAVING SUM(supptrans.ovamount + supptrans.ovgst - supptrans.alloc) <>0";



- webERP detailed:
$sql = "SELECT systype.systypename, supptrans.suppreference, supptrans.trandate,
    (supptrans.ovamount + supptrans.ovgst - supptrans.alloc) as balance,
CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(supptrans.trandate)) >= paymentterm.daysbeforedue
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1', 'MONTH') . "), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= 0
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END AS due,

CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(supptrans.trandate) > paymentterm.daysbeforedue	AND TO_DAYS(Now())
        - TO_DAYS(supptrans.trandate) >= (paymentterm.daysbeforedue + " . $_SESSION['PastDueDays1'] . ")
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1','MONTH') . "), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= "
        . $_SESSION['PastDueDays1'] . ") THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END AS overdue1,

CASE WHEN paymentterm.daysbeforedue > 0 THEN
    CASE WHEN TO_DAYS(Now()) - TO_DAYS(supptrans.trandate) > paymentterm.daysbeforedue AND TO_DAYS(Now())
        - TO_DAYS(supptrans.trandate) >= (paymentterm.daysbeforedue + " . $_SESSION['PastDueDays2'] . ")
    THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
ELSE
    CASE WHEN (TO_DAYS(Now()) - TO_DAYS(DATE_ADD(DATE_ADD(supptrans.trandate, " . INTERVAL('1','MONTH') . "), "
        . INTERVAL('(paymentterm.dayinfollowingmonth - DAYOFMONTH(supptrans.trandate))', 'DAY') . ")) >= "
        . $_SESSION['PastDueDays2'] . ") THEN supptrans.ovamount + supptrans.ovgst - supptrans.alloc ELSE 0 END
END AS overdue2

FROM supplier, paymentterm, supptrans, systype
WHERE systype.typeid = supptrans.type
    AND supplier.paymentterm = paymentterm.termsindicator
    AND supplier.supplierid = supptrans.supplierno
    AND ABS(supptrans.ovamount + supptrans.ovgst - supptrans.alloc) >0.009
    AND supptrans.settled = 0
    AND supptrans.supplierno = '" . $AgedAnalysis["suppliercode"] . "'";


    - acc_supplier:
    supplierid
    suppname
    currcode is ISO code
    paymentterm  is ID

    - acc_transdoc:
    totalamountrec
    totalamountpay
    totaltax
    duedate
    rate
    alloc
    settled


- MySQL
SELECT acc_supplier.supplierid, acc_supplier.suppname, acc_currency.currency, acc_paymentterm.terms,

SUM(acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc) as balance,

SUM(CASE WHEN (TO_DAYS(Now()) - TO_DAYS(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS due,

SUM(CASE WHEN (TO_DAYS(Now()) - TO_DAYS(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue + 30
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS overdue1,

SUM(CASE WHEN (TO_DAYS(Now()) - TO_DAYS(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue + 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS overdue2

FROM acc_supplier, acc_paymentterm, acc_currency, acc_transdoc
WHERE acc_supplier.paymentterm = acc_paymentterm.termsindicator
    AND acc_supplier.currcode = acc_currency.currabrev
    AND acc_supplier.id = acc_transdoc.creditor_idx
    AND acc_supplier.supplierid >= '0'
    AND acc_supplier.supplierid <= 'ZZZZ'
    AND acc_supplier.currcode = 'EUR'
GROUP BY acc_supplier.supplierid,
    acc_supplier.suppname,
    acc_currency.currency,
    acc_paymentterm.terms,
    acc_paymentterm.daysbeforedue,
    acc_paymentterm.dayinfollowingmonth
HAVING SUM(acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc) <>0;



- Sqlite TO_DATE(Now()) replaced by julianday(" + mNowDateTime + "), ODBC is DATEDIFF()?
SELECT acc_supplier.supplierid, acc_supplier.suppname, acc_currency.currency, acc_paymentterm.terms,

SUM(acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc) as balance,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS due,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue + 30
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS overdue1,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.transdate)) >= acc_paymentterm.daysbeforedue + 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc ELSE 0 END)
AS overdue2

FROM acc_supplier, acc_paymentterm, acc_currency, acc_transdoc
WHERE acc_supplier.paymentterm = acc_paymentterm.termsindicator
    AND acc_supplier.currcode = acc_currency.currabrev
    AND acc_supplier.id = acc_transdoc.creditor_idx
    AND acc_supplier.supplierid >= '0'
    AND acc_supplier.supplierid <= 'ZZZZ'
    AND acc_supplier.currcode = 'EUR'
GROUP BY acc_supplier.supplierid,
    acc_supplier.suppname,
    acc_currency.currency,
    acc_paymentterm.terms,
    acc_paymentterm.daysbeforedue,
    acc_paymentterm.dayinfollowingmonth
HAVING SUM(acc_transdoc.totalamountpay + acc_transdoc.totaltax  - acc_transdoc.alloc) <>0;


// test 1
//
SELECT acc_supplier.supplierid, acc_supplier.suppname, acc_currency.currency, acc_paymentterm.terms, acc_transdoc.transdate, acc_transdoc.duedate,

SUM(acc_transdoc.totalamountpay - acc_transdoc.alloc) as balance,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 0
    THEN acc_transdoc.totalamountpay  - acc_transdoc.alloc ELSE 0 END)
AS due,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 30
    THEN acc_transdoc.totalamountpay  - acc_transdoc.alloc ELSE 0 END)
AS overdue1,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 60
    THEN acc_transdoc.totalamountpay  - acc_transdoc.alloc ELSE 0 END)
AS overdue2

FROM acc_supplier, acc_paymentterm, acc_currency, acc_transdoc
WHERE acc_supplier.supplierid >= '0'
    AND acc_supplier.supplierid <= 'ZZZZ'

GROUP BY acc_supplier.suppname
HAVING SUM(acc_transdoc.totalamountpay - acc_transdoc.alloc) <>0;


// final 2 details:
//
SELECT acc_supplier.suppname, acc_transdoc.transno, acc_transdoc.transdate, acc_transdoc.duedate, acc_currency.currabrev, acc_transdoc.totalamountpay, acc_transdoc.alloc,

CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 0 AND  (julianday('now') - julianday(acc_transdoc.duedate)) < 30
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END
AS due,

CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 30 AND  (julianday('now') - julianday(acc_transdoc.duedate)) < 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END
AS overdue1,

CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END
AS overdue2

FROM acc_supplier INNER JOIN acc_transdoc ON acc_supplier.id=acc_transdoc.creditor_idx
    INNER JOIN acc_currency ON acc_supplier.currcode=acc_currency.id
    INNER Join acc_chartmaster ON acc_transdoc.parent=acc_chartmaster.id

WHERE acc_supplier.supplierid >= '0'
    AND acc_supplier.supplierid <= 'ZZZZ'
    AND acc_chartmaster.parent='projectId'
    AND acc_transdoc.doctype = 20
    AND (acc_transdoc.totalamountpay + acc_transdoc.alloc) <>0;
== NOTE + here are - for customer!


// final 2 summary:
//
SELECT acc_supplier.suppname, acc_currency.currabrev,

SUM(acc_transdoc.totalamountpay) as totalamountpay,

SUM( acc_transdoc.alloc) as alloc,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 0 AND  (julianday('now') - julianday(acc_transdoc.duedate)) < 30
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END)
AS due,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 30 AND  (julianday('now') - julianday(acc_transdoc.duedate)) < 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END)
AS overdue1,

SUM(CASE WHEN (julianday('now') - julianday(acc_transdoc.duedate)) >= 60
    THEN acc_transdoc.totalamountpay + acc_transdoc.alloc ELSE 0 END)
AS overdue2

FROM acc_supplier INNER JOIN acc_transdoc ON acc_supplier.id=acc_transdoc.creditor_idx
    INNER JOIN acc_currency ON acc_supplier.currcode=acc_currency.id

WHERE acc_supplier.supplierid >= '0'
    AND acc_supplier.supplierid <= 'ZZZZ'
    AND acc_transdoc.parent='projectId'
    AND acc_transdoc.doctype = 20
    AND (acc_transdoc.totalamountpay + acc_transdoc.alloc) <>0;

*/


    if (ACC_MODELFACTORY->getDatabase().driverName()
            == RB2::enumToDriverName(RB2::DatabaseSqlite)) {
        mToDaysOpen = "julianday(";
        mNowDateTime = "'now'";
    } else if (ACC_MODELFACTORY->getDatabase().driverName()
               == RB2::enumToDriverName(RB2::DatabaseMysql)) {
        mToDaysOpen = "TO_DAYS(";
        mNowDateTime = "NOW()";
    } else if (ACC_MODELFACTORY->getDatabase().driverName()
               == RB2::enumToDriverName(RB2::DatabaseOdbc)) {
        // TODO: DATEDIFF?
    }
    ACC_SqlCommonFunctions util;

    if (mDocType == ACC2::TransCreditor) {
        mCustSuppName = "acc_supplier.suppname";
        mCustSupp = "acc_supplier";
        mCustSuppIdName = "";
        util.substrIdxId("acc_transdoc.creditor_idx", mCustSuppIdName);
        mTotalAmountRecPay = "acc_transdoc.totalamountpay";
        mTotalAmountRecPayPlMin = "acc_transdoc.totalamountpay +";
        mDocTypeSql = "(`acc_transdoc`.`doctype` = "
                + RB_String::number(ACC2::TransCreditor)
                + " OR `acc_transdoc`.`doctype` = "
                + RB_String::number(ACC2::TransPurchOrder) + ")";
    } else if (mDocType == ACC2::TransDebtor) {
        mCustSuppName = "acc_customer.mname";
        mCustSupp = "acc_customer";
        mCustSuppIdName = "";
        util.substrIdxId("acc_transdoc.debtor_idx", mCustSuppIdName);
        mTotalAmountRecPay = "acc_transdoc.totalamountrec";
        mTotalAmountRecPayPlMin = "acc_transdoc.totalamountrec -";
        mDocTypeSql = "(`acc_transdoc`.`doctype` = "
                + RB_String::number(ACC2::TransDebtor)
                + " OR `acc_transdoc`.`doctype` = "
                + RB_String::number(ACC2::TransSalesOrder) + ")";
    }

    setQuery(qStr);
//    RB_DEBUG->print(qStr);
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateInvoiceAged::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    /*
    0 suppname
    1 transno
    2 description
    3 transdate
    4 duedate
    5 currabrev
    6 totalamountpay
    7 alloc
    8 due
    9 overdue1
    10 overdue2
    */

    while (query.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", invoiceList, "ACC_Invoice");
        invoiceList->addObject(obj);
        obj->addMember(obj, "suppname", "-", query.value(0), RB2::MemberChar125);
        obj->addMember(obj, "transno", "-", query.value(1), RB2::MemberInteger);
        obj->addMember(obj, "description", "-", query.value(2), RB2::MemberChar125);
        obj->addMember(obj, "transdate", "-", query.value(3), RB2::MemberChar40);
        obj->addMember(obj, "duedate", "-", query.value(4), RB2::MemberChar40);
        obj->addMember(obj, "currency_id", "-", query.value(5), RB2::MemberChar10);
        obj->addMember(obj, "amount", "-", query.value(6), RB2::MemberDouble);
        obj->addMember(obj, "alloc", "-", query.value(7), RB2::MemberDouble);
        obj->addMember(obj, "due", "-", query.value(8), RB2::MemberDouble);
        obj->addMember(obj, "overdue1", "-", query.value(9), RB2::MemberDouble);
        obj->addMember(obj, "overdue2", "-", query.value(10), RB2::MemberDouble);
    }

    return true;
}

/**
 * Set over due days
 * @param overdue days 1, usually 30 days
 * @param overdue days 2, usually 60 days
 */
void ACC_CreateInvoiceAged::setOverDue(int overDueDays1,
                                       int overDueDays2) {
    mOverDueDays1 = overDueDays1;
    mOverDueDays2 = overDueDays2;
}

/**
 * Set account range
 * @param fromCustSupp starting customer or supplier name
 * @param toCustSupp ending customer or supplier name
 */
void ACC_CreateInvoiceAged::setCustSuppRange(const RB_String& fromCustSupp,
                                             const RB_String& toCustSupp) {
    mFromCustSupp = fromCustSupp;
    mToCustSupp = toCustSupp + "zzzzzz";
}

/**
 * Set query for customer or supplier
 * and per different database type such as SQLite, MySQL
 * @param qStr query string
 */
void ACC_CreateInvoiceAged::setQuery(RB_String& qStr) {
    /*
    0 suppname
    1 transno
    2 description
    3 transdate
    4 duedate
    5 currcode
    6 totalamountpay
    7 alloc
    8 due
    9 overdue1
    10 overdue2
    */

    qStr = "SELECT " + mCustSuppName + ", acc_transdoc.transno, acc_transdoc.description, acc_transdoc.transdate, "
            "acc_transdoc.duedate, acc_currency.currcode, "+ mTotalAmountRecPay + ", acc_transdoc.alloc, "
    "CASE WHEN (" + mToDaysOpen + mNowDateTime + ") - " + mToDaysOpen + "acc_transdoc.duedate)) >= 0 "
        "AND  (" + mToDaysOpen + mNowDateTime + ") - " + mToDaysOpen + "acc_transdoc.duedate)) < " + RB_String::number(mOverDueDays1) + " "
        "THEN " + mTotalAmountRecPayPlMin + " acc_transdoc.alloc ELSE 0 END "
    "AS due, "
    "CASE WHEN (" + mToDaysOpen + mNowDateTime + ") - " + mToDaysOpen + "acc_transdoc.duedate)) >= " + RB_String::number(mOverDueDays1) + " "
        "AND  (" + mToDaysOpen + mNowDateTime + ") - " + mToDaysOpen + "acc_transdoc.duedate)) < " + RB_String::number(mOverDueDays2) + " "
        "THEN " + mTotalAmountRecPayPlMin + " acc_transdoc.alloc ELSE 0 END "
    "AS overdue1, "
    "CASE WHEN (" + mToDaysOpen + mNowDateTime + ") - " + mToDaysOpen + "acc_transdoc.duedate)) >= " + RB_String::number(mOverDueDays2) + " "
        "THEN " + mTotalAmountRecPayPlMin + " acc_transdoc.alloc ELSE 0 END "
    "AS overdue2 "
    "FROM " + mCustSupp + " INNER JOIN acc_transdoc ON " + mCustSupp + ".id=" + mCustSuppIdName + " "
        "INNER JOIN acc_currency ON " + mCustSupp + ".currency_id=acc_currency.id "
        "INNER JOIN acc_chartmaster ON acc_transdoc.parent=acc_chartmaster.id "
    "WHERE " + mCustSuppName + " >= '" + mFromCustSupp + "' "
        "AND " + mCustSuppName + " <= '" + mToCustSupp + "' "
        "AND acc_chartmaster.parent='" + ACC_MODELFACTORY->getRootId() + "' "
        "AND " + mDocTypeSql + " "
        "AND ABS(" + mTotalAmountRecPayPlMin + " acc_transdoc.alloc) > 0.005 "
    "ORDER BY " + mCustSuppName +" ASC;";
}
