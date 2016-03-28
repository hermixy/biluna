/*****************************************************************
 * $Id: acc_sqlcommonfunctions.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_sqlcommonfunctions.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
ACC_SqlCommonFunctions::ACC_SqlCommonFunctions() {

}

/**
 * Destructor
 */
ACC_SqlCommonFunctions::~ACC_SqlCommonFunctions() {

}

/**
 * Get last transaction sequence number base on transaction type
 * and the list ID. These sequence numbers are maintained
 * in the table ACC_SysType.
 * Returns -1 if the record does not exist.
 * @param transType transaction type
 * @param typeListId transaction list ID for example ACC_ChartMaster ID
 * (to differentiate for example bank accounts)
 * @return last transaction number
 */
int ACC_SqlCommonFunctions::getLastTransNo(int transType,
                                           const RB_String& typeListId) {

    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::getLastTransNo()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    // Check if ACC_SysType have been set for this project
    qStr = "SELECT systypeno FROM acc_systype WHERE systypecode="
            + RB_String::number(transType)
            + " AND systypelist_id='" + typeListId + "';";
    if (query.exec(qStr)) {
        if (!query.first()) {
            return -1;
        } else {
            // return the value
            return query.value(0).toInt();
        }
    }

    RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getLastTransNo() "
                    + query.lastError().text() + " ERROR");
    return -1;

}

/**
 * Set last transaction number. Used when importing bank statements
 * @param transType transaction type
 * @param typeListId type list id, to differentiate between bank accounts
 * @param lastNumber last sequence number used
 * @returns true if successful, false if for example record does not yet exist
 */
bool ACC_SqlCommonFunctions::setLastTransNo(int transType,
                                           const RB_String& typeListId,
                                           int lastNumber) {
    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    qStr = "UPDATE acc_systype SET systypeno=" + RB_String::number(lastNumber)
            + ", changed='" + QDateTime::currentDateTime().toString(Qt::ISODate)
            + "', muser='" + username
            + "' WHERE systypecode=" + RB_String::number(transType)
            + " AND systypelist_id='" + typeListId
            + "' AND parent='" + ACC_MODELFACTORY->getRootId() + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::setLastTransNo() "
                        + query.lastError().text() + " ERROR");

        return false;
    }

    return true;
}

/**
 * Get next transaction sequence number base on transaction type
 * and the list ID. These sequence numbers are maintained
 * in the table ACC_SysType. Also increments the transaction number.
 * The type list name is used to create the record if it does not
 * exists and for reference in case of editing by the user.
 * @param transType transaction type
 * @param typeListId transaction list ID for example ACC_ChartMaster ID
 * (to differentiate for example bank accounts)
 * @param typeListName type list name for example ACC_ChartMaster name
 * @return next transaction number
 */
int ACC_SqlCommonFunctions::getNextTransNo(int transType,
                                           const RB_String& typeListId,
                                           const RB_String& typeListName) {

    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::getNextTransNo()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    // Check if ACC_SysType have been set for this project
    qStr = "SELECT systypeno FROM acc_systype WHERE systypecode="
            + RB_String::number(transType)
            + " AND systypelist_id='" + typeListId
            + "' AND parent='" + ACC_MODELFACTORY->getRootId() + "'";
    if (query.exec(qStr)) {
        if (!query.first()) {
            if (!execSysTypeSql(transType, typeListId, typeListName)) {
                return -1;
            }
        }
    } else {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getNextTransNo() "
                        + query.lastError().text() + " ERROR");
        return -1;
    }

    // Set new transaction number
    ACC_MODELFACTORY->getDatabase().transaction();
    query.clear();
    query.exec(qStr); // rerun query

    if (!query.first()) {
        ACC_MODELFACTORY->getDatabase().commit();
        return -1; // ERROR no ACC_SysType
    }

    int nextTransNo = query.value(0).toInt();
    ++nextTransNo;
    query.clear();

    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    qStr = "UPDATE acc_systype SET systypeno=" + RB_String::number(nextTransNo)
            + ", changed='" + QDateTime::currentDateTime().toString(Qt::ISODate)
            + "', muser='" + username
            + "' WHERE systypecode=" + RB_String::number(transType)
            + " AND systypelist_id='" + typeListId
            + "' AND parent='" + ACC_MODELFACTORY->getRootId() + "'";

    if (!query.exec(qStr)) {
        ACC_MODELFACTORY->getDatabase().rollback();
        return -1;
    }

    // Commit transaction
    ACC_MODELFACTORY->getDatabase().commit();
    query.clear();
    return nextTransNo;
}

/**
 * Gets the GL Codes relevant to the stock item account from
 * the stock category record
 * 0 is stock category Gl account
 * 1 is stock category adjustment GL account, for example after inventory check
 * 2 is stock category sales or purchase price variances/discounts GL account
 * 3 is stock category material usage variance GL account
 * 4 is stock category work in progress GL account
 * @param glCode general ledger code to be set
 * @param stockId id of stock item
 */
void ACC_SqlCommonFunctions::getStockGlCode(QSqlQuery& query,
                                            const RB_String& stockId) {
/*
$QuerySQL = "SELECT stockact,
            adjglact,
            purchpricevaract,
            materialuseagevarac,
            wipact
        FROM stockmaster,
            stockcategory
        WHERE stockmaster.categoryid=stockcategory.categoryid
        AND stockmaster.stockid = '" . $StockID . "'";
*/
    RB_String qStr = "SELECT stockact, adjglact, purchpricevaract, "
                       "materialuseagevarac, wipact "
                     "FROM acc_stockmaster, acc_stockcategory "
                     "WHERE acc_stockmaster.parent=acc_stockcategory.id "
                     "AND acc_stockmaster.id = '" + stockId + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getSalesTaxes() "
                        + query.lastError().text() + " ERROR");
    }

    // Get defaults if above not found
    if (!query.first()) {
        query.clear();
        qStr = "SELECT stockact, adjglact, purchpricevaract, "
                      "materialuseagevarac, wipact "
               "FROM acc_stockmaster, acc_stockcategory "
               "WHERE acc_stockmaster.parent=acc_stockcategory.id "
               "AND acc_stockmaster.id = '0'";

        if (!query.exec(qStr)) {
            RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getStockGlCode() 2 "
                            + query.lastError().text() + " ERROR");
        }
    }
}

void ACC_SqlCommonFunctions::getTaxAuthId(QSqlQuery& query,
                                          const RB_String& taxGroupId,
                                          const RB_String& taxProvinceId,
                                          const RB_String& taxCatId) {
    RB_String qStr = "";
    qStr = "SELECT acc_taxgroupauth.taxauth_id "
            "FROM acc_taxauthrate INNER JOIN acc_taxgroupauth ON "
                "acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id "
                "INNER JOIN acc_taxauthority ON "
                "acc_taxauthrate.parent=acc_taxauthority.id " // only to include acc_taxauthority
            "WHERE acc_taxgroupauth.parent='" + taxGroupId
          + "' AND acc_taxauthrate.dispatchtaxprovince_id='" + taxProvinceId
          + "' AND acc_taxauthrate.taxcat_id='" + taxCatId
          + "' ORDER BY acc_taxgroupauth.calculationorder";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getTaxAuthId() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " Tax Group ID = " + taxGroupId
                + "\n Dispatch Tax Provice ID = " + taxProvinceId
                + "\n Tax Category ID = " + taxCatId;
        ACC_DIALOGFACTORY->commandMessage("ACC_SqlCommonFunctions::getTaxAutId() "
                                          + query.lastError().text() + "\n" + str);
    }

}

/**
 * Get (sales) tax values relevant to taxgroup taxprovince
 * and stock taxcategory. The query contains at:
 * 0 calculationorder
 * 1 description
 * 2 taxauth_id
 * 3 taxontax
 * 4 taxrate
 * 5 salestaxacct
 * @param query to be filled and executed, used by caller as return value
 * @param taxGroupId ID tax location of authorities
 * @param dispTaxProvinceId ID taxprovince of dispatching location
 * @param taxCategoryId ID taxcategory of stockitem
 */
void ACC_SqlCommonFunctions::getSalesTaxes(QSqlQuery& query,
                                      const RB_String& taxGroupId,
                                      const RB_String& dispTaxProvinceId,
                                      const RB_String& taxCategoryId) {
    RB_String qStr = "";
    getTaxQuery(qStr, taxGroupId, dispTaxProvinceId, taxCategoryId);

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getSalesTaxes() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " Tax Group ID = " + taxGroupId
                + "\n Dispatch Tax Provice ID = " + dispTaxProvinceId
                + "\n Tax Category ID = " + taxCategoryId;
        ACC_DIALOGFACTORY->commandMessage("ACC_SqlCommonFunctions::getSalesTaxes() "
                                          + query.lastError().text() + "\n" + str);
    }

    // TODO: Get defaults if above not found
}

/**
 * Get purchase tax values relevant to taxgroup taxprovince
 * and stock taxcategory. The query contains at:
 * 0 calculationorder
 * 1 description
 * 2 taxauth_id
 * 3 taxontax
 * 4 taxrate
 * 5 purchasetaxacct
 * @param query to be filled and executed, used by caller as return value
 * @param taxGroupId ID tax location of authorities
 * @param receiptTaxProvinceId ID taxprovince of receiving location
 * @param taxCategoryId ID taxcategory of stockitem
 */
void ACC_SqlCommonFunctions::getPurchaseTaxes(QSqlQuery& query,
                                              const QString& taxGroupId,
                                              const QString& receiptTaxProvinceId,
                                              const QString& taxCategoryId) {
    RB_String qStr = "";
    getTaxQuery(qStr, taxGroupId, receiptTaxProvinceId, taxCategoryId);

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getPurchaseTaxes() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " Tax Group ID = " + taxGroupId
                + "\n Receipt Tax Provice ID = " + receiptTaxProvinceId
                + "\n Tax Category ID = " + taxCategoryId;
        ACC_DIALOGFACTORY->commandMessage("ACC_SqlCommonFunctions::getPurchaseTaxes() "
                                          + query.lastError().text() + "\n" + str);
    }

    // TODO: Get defaults if above not found
}

/**
 * Get sales tax query text
 * NOTE: acc_salestaxposting is also used as purchase tax posting
 * @param query text to be filled and executed
 * @param taxGroupId ID tax location of authorities
 * @param dispReceiptTaxProvinceId ID taxprovince of dispatching location
 * @param taxCategoryId ID taxcategory of stockitem
 */
void ACC_SqlCommonFunctions::getTaxQuery(QString &qStr,
                                         const QString& taxGroupId,
                                         const QString& dispReceiptTaxProvinceId,
                                         const QString& taxCategoryId) {
    qStr = "SELECT acc_taxgroupauth.calculationorder, "
                "acc_taxauthority.description, "
                "acc_taxgroupauth.taxauth_id, "
                "acc_taxgroupauth.taxontax, "
                "acc_taxauthrate.taxrate, ";
    substrIdxId("acc_salestaxposting.salestaxacct_idx", qStr);
    qStr += " FROM acc_taxauthrate INNER JOIN acc_taxgroupauth ON "
                "acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id "
                "INNER JOIN acc_taxauthority ON "
                "acc_taxauthrate.parent=acc_taxauthority.id " // only to include acc_taxauthority
                "INNER JOIN acc_salestaxposting ON "
                "acc_taxauthrate.parent=acc_salestaxposting.taxauth_id "; // only to include acc_salestaxposting
    qStr += "WHERE acc_taxgroupauth.parent='" + taxGroupId
          + "' AND acc_taxauthrate.dispatchtaxprovince_id='" + dispReceiptTaxProvinceId
          + "' AND acc_taxauthrate.taxcat_id='" + taxCategoryId
          + "' AND acc_salestaxposting.taxcat_id='" + taxCategoryId
          + "' AND acc_salestaxposting.parent='" + ACC_MODELFACTORY->getRootId()
            + "' ORDER BY acc_taxgroupauth.calculationorder";
}

/**
 * Get company (sales) tax values relevant to taxprovince and taxlocation,
 * The query contains at:
 * 0 accountcontrol sales tax account ID corresponding to taxrate
 * 1 salestaxacct
 * 2 taxrate
 * @param query to be filled and executed, used by caller as return value
 * @param dispTaxProvinceId taxprovince ID of dispatching location
 * @param taxGroupId default (delivery) tax location ID
 */
void ACC_SqlCommonFunctions::getCompanyTaxes(QSqlQuery& query,
                                             const RB_String& dispTaxProvinceId,
                                             const RB_String& taxGroupId) {
    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String qStr = "";
    qStr = "SELECT acc_chartmaster.accountcontrol, ";
    substrIdxId("acc_salestaxposting.salestaxacct_idx", qStr);
    qStr +=     ", acc_taxauthrate.taxrate "
            "FROM acc_salestaxposting "
                "INNER JOIN acc_taxauthrate ON acc_taxauthrate.parent=acc_salestaxposting.taxauth_id "
                "INNER JOIN acc_taxgroupauth ON acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id "
                "INNER JOIN acc_chartmaster ON ";
    substrIdxId("acc_salestaxposting.salestaxacct_idx", qStr);
    qStr +=     "=acc_chartmaster.id "
            "WHERE acc_taxauthrate.dispatchtaxprovince_id='" + dispTaxProvinceId + "' "
                "AND acc_taxgroupauth.parent='" + taxGroupId + "' "
                "AND acc_salestaxposting.taxcat_id=acc_taxauthrate.taxcat_id "
                "AND acc_salestaxposting.parent='" + rootId + "';";

    /*
    ### one parent
    SELECT DISTINCT acc_taxauthrate.parent
    FROM acc_taxauthrate
    INNER JOIN acc_taxgroupauth ON acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id
    WHERE acc_taxauthrate.dispatchtaxprovince_id='{7c6d6bb5-9308-4440-930a-86ff36d90e21}'
    AND acc_taxgroupauth.parent='{f1b1dc5b-7989-4639-9fc5-b3433992bd72}';

    ###
    SELECT acc_chartmaster.accountcontrol, acc_taxauthrate.taxrate, SUBSTR(acc_salestaxposting.salestaxacct_idx,1,38)  as idx
    FROM acc_salestaxposting
    INNER JOIN acc_taxauthrate ON acc_taxauthrate.parent=acc_salestaxposting.taxauth_id
    INNER JOIN acc_taxgroupauth ON acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id
    INNER JOIN acc_chartmaster ON SUBSTR(acc_salestaxposting.salestaxacct_idx,1,38)=acc_chartmaster.id
    WHERE acc_taxauthrate.dispatchtaxprovince_id='{7c6d6bb5-9308-4440-930a-86ff36d90e21}'
    AND acc_taxgroupauth.parent='{f1b1dc5b-7989-4639-9fc5-b3433992bd72}';

    ###
    SELECT acc_taxauthrate.taxrate, acc_taxauthrate.parent
    FROM acc_taxauthrate
    INNER JOIN acc_taxgroupauth ON acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id
    WHERE acc_taxauthrate.dispatchtaxprovince_id='{7c6d6bb5-9308-4440-930a-86ff36d90e21}'
    AND acc_taxgroupauth.parent='{f1b1dc5b-7989-4639-9fc5-b3433992bd72}';
    */



    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getCompanyTaxes() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " Dispatch Tax Provice ID = " + dispTaxProvinceId;
        ACC_DIALOGFACTORY->commandMessage("ACC_SqlCommonFunctions::getCompanyTaxes() "
                                          + query.lastError().text() + "\n" + str);
    }
}

/**
 * Get the relavent GL Code to post COGS entries to
 * Refer to WebERP includes/GetSalesTransGLCodes.inc
 * TODO: Combine into one SQL statement
 * @param query to be created and executed, further used by function caller
 * @param area sales area
 * @param stockId stock ID
 * @param salesType sales type or price list
 */
void ACC_SqlCommonFunctions::getCogsGlAccount(QSqlQuery& query,
                                              const RB_String& area,
                                              const RB_String& stockId/*,
                                              const RB_String& salesType*/) {

    /*
    Get the StockCategory for this item
        $SQL = "SELECT categoryid FROM stockmaster WHERE stockid='" . $StockID . "'";
        $Result=DB_query($SQL,$db);
        $myrow = DB_fetch_row($Result);
        $StockCategory = $myrow[0];

    Gets the GL Code for the COGS for a specific area and stock category.
        $SQL = "SELECT glcode FROM cogsglposting
                WHERE area = '" . $Area . "'
                AND stkcat = '" . $StockCategory . "'
                AND salestype='" . $SalesType . "'";

    -> not implemented
    If no match for all fields area, sales type, stock category then the rules for choosing the nearest match
    are
    - goes for gold a match for salestype stock category and area then -
    - matching Area, stock category and AN Sales type
    - see if matching Area, stock category - AN sales type
    - see if matching Area, saletype and ANY StockCategory
    - see if mathcing Area, ANY stock category and AN salestype
    - see if matching stockcategory, AN area and AN salestype
    - if still no record is found then the GL Code for the default area, sales type and default stock category is used
    */

    RB_String stockCategoryId = "";
    RB_String qStr = "";
    qStr = "SELECT parent FROM acc_stockmaster WHERE id='" + stockId + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getCogsGlAccount() 1A "
                        + query.lastError().text() + " ERROR");
        return;
    } else {
        if (!query.first()) {
            RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getCogsGlAccount() 1B "
                            + query.lastError().text() + " ERROR");
            return;
        }
        stockCategoryId = query.value(0).toString();
    }

    query.clear();
    qStr = "SELECT glcode FROM acc_cogsglposting WHERE area='" + area
           + "' AND stkcat='" + stockCategoryId + "'";
//           + "' AND salestype='" + salesType + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getCogsGlAccount() 2 "
                        + query.lastError().text() + " ERROR");
    }

    // Get default GL account if above not found
    if (!(query.size() > 0)) { // returns only one value
        query.clear();
        qStr = "SELECT glcode FROM acc_cogsglposting "
               "WHERE area='0' AND stkcat='0'"; // AND salestype='0'";

        if (!query.exec(qStr)) {
            RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getCogsGlAccount() 3 "
                            + query.lastError().text() + " ERROR");
        }
    }
}

/**
 * Get the Sales GL Code for a specific location and tax level
 * @param query to be filled and executed, used by caller as return value
 * @param taxLocation tax location of authorities
 * @param dispTaxProvince taxprovince of dispatching location
 * @param taxCategory taxcategory of stockitem
 */
//void ACC_SqlCommonFunctions::getSalesGlAccount(QSqlQuery& query,
//                                               const RB_String& taxLocation,
//                                               const RB_String& dispTaxProvince,
//                                               const RB_String& taxCategory,
//                                               const RB_String& stockCatId) {
//    RB_String qStr = "";
//    qStr = "SELECT SUBSTR(`acc_taxauthrate`.`glacct_idx`,1,38), "
//                "SUBSTR(`acc_taxauthrate`.`glacct_idx`,39) "
//            "FROM acc_taxauthrate "
//            "INNER JOIN acc_taxgroupauth "
//                "ON acc_taxauthrate.parent=acc_taxgroupauth.taxauth_id "
//            "WHERE acc_taxgroupauth.parent='" + taxLocation
//          + "' AND acc_taxauthrate.dispatchtaxprovince_id='" + dispTaxProvince
//          + "' AND acc_taxauthrate.taxcat_id='" + taxCategory
//          + "' AND SUBSTR(`acc_taxauthrate`.`stockcat_idx`,1,38)='" + stockCatId
//          + "' ORDER BY acc_taxgroupauth.calculationorder";

//    if (!query.exec(qStr)) {
//        RB_DEBUG->error(
//                        "ACC_SqlCommonFunctionsFunction::getSalesGlAccount() 1 ",
//                        + query.lastError().text() + " ERROR");
//    }

////    // Get defaults if above not found
////    if (!(query.size() > 0)) {
////        RB_DEBUG->warning("ACC_SqlCommonFunctionsFunction::getSalesGlAccount() 2 "
////                          "query.size() == 0 WARNING");
////    }
//}

/**
 * Get the  Sales GL Code for a specific area,
 * sales type and stock category
 * Refer to WebERP includes/GetSalesTransGLCodes.inc
 * Query will be set with at:
 * 0 salesglcode
 * 1 discountglcode
 * @param query to be created and executed, further used by function caller
 * @param taxAuthId tax authority ID
 * @param areaId sales area ID
 * @param stockId stock ID
 * @param salesTypeId sales type or price list ID
 */
void ACC_SqlCommonFunctions::getSalesGlAccount(QSqlQuery& query,
                                               const RB_String& taxAuthId,
                                               const RB_String& areaId,
                                               const RB_String& stockCatId,
                                               const RB_String& salesTypeId) {

    /*
    Get the StockCategory for this item
        $SQL = "SELECT categoryid FROM stockmaster WHERE stockid='" . $StockID . "'";
        $Result=DB_query($SQL,$db);
        $myrow = DB_fetch_row($Result);
        $StockCategory = $myrow[0];


    Need to determine if Sales GL codes set up for the stk cat, area and
    sales type of the item/customer branch and 	use the most appropriate GL Code.
        $SQL = "SELECT salesglcode,
                discountglcode
            FROM salesglposting
            WHERE area = '" . $Area . "'
            AND stkcat = '" . $StockCategory . "'
            AND salestype = '". $SalesType . "'";

    -> not implemented
    If no match for all fields area, sales type, stock category then the rules for choosing the nearest match
    are

    - goes for gold a match for salestype stock category and area then -
    - matching Area, stock category and AN Sales type
    - see if matching Area, stock category - AN sales type
    - see if matching Area, saletype and ANY StockCategory
    - see if mathcing Area, ANY stock category and AN salestype
    - see if matching stockcategory, AN area and AN salestype
    - if still no record is found then the GL Code for the default area,
        sales type and default stock category is used
    */

//    RB_String stockCategoryId = "";
    RB_String qStr = "";
//    qStr = "SELECT parent FROM acc_stockmaster WHERE id='" + stockId + "'";

//    if (!query.exec(qStr)) {
//        RB_DEBUG->error(
//                        "ACC_SqlCommonFunctionsFunction::getSalesGlAccount() 1 ",
//                        + query.lastError().text() + " ERROR");
//        return;
//    } else {
//        if (!query.first()) return;
//        stockCategoryId = query.value(0).toString();
//    }

    qStr = "SELECT ";
    substrIdxId("salesglact_idx", qStr);
    qStr += ", ";
    substrIdxContent("salesglact_idx", qStr);
    qStr += ", ";
    substrIdxId("discountglact_idx", qStr);
    qStr += ", ";
    substrIdxContent("discountglact_idx", qStr);
    qStr += " FROM `acc_salesglposting` "
           "WHERE `area_id`='" + areaId
            + "' AND `taxauth_id`='" + taxAuthId + "' AND ";
    substrIdxId("stkcat_idx", qStr);
    qStr += "='" + stockCatId + "' AND `salestype_id`='" + salesTypeId + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getSalesGlAccount() "
                        + query.lastError().text() + " ERROR");
    }

    // Get default GL accounts if above not found
//    if (!(query.size() > 0)) {
//        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getSalesGlAccount() 3 "
//                        "query.size() == 0 ERROR");
//        query.clear();
//        qStr = "SELECT salesglcode, discountglcode FROM acc_salesglposting "
//               "WHERE area='0' AND stkcat='0' AND salestype='0'";

//        if (!query.exec(qStr)) {
//            RB_DEBUG->error(
//                            "ACC_SqlCommonFunctionsFunction::getSalesGlAccount() 3 ",
//                            + query.lastError().text() + " ERROR");
//        }
//    }
}


/**
 * Get the Purchase GL Code for a specific stock category, location and tax level
 * @param query to be filled and executed, used by caller as return value
 * @param taxGroupId tax group ID of supplier
 * @param taxCategory taxcategory of stockitem
 */
void ACC_SqlCommonFunctions::getPurchaseGlAccount(QSqlQuery& query,
                                                  const RB_String& taxGroupId,
                                                  const RB_String& stockCatId) {
    RB_String qStr = "SELECT ";
    substrIdxId("purchaseglact_idx", qStr);
    qStr += ", ";
    substrIdxContent("purchaseglact_idx", qStr);
    qStr += " FROM `acc_purchaseglposting` "
            "WHERE `taxgroup_id`='" + taxGroupId + "' AND ";
    substrIdxId("stkcat_idx", qStr);
    qStr += "='" + stockCatId + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getPurchaseGlAccount() "
                        + query.lastError().text() + " ERROR");
    }

    // Get defaults if above not found
//    if (!(query.size() > 0)) {
//        RB_DEBUG->warning("ACC_SqlCommonFunctionsFunction::getPurchaseGlAccount() 2 "
//                          "query.size() == 0 WARNING");
//    }
}

/**
 * Get trial balance to recreate the ACC_GlSum table.
 * During recreation the fromPeriod and toPeriod are the same, usually a month.
 * @param query query object
 * @param fromPeriod period from which to start
 * @param toPeriod period to which to end
 */
void ACC_SqlCommonFunctions::getTrialBalance(QSqlQuery& query,
                                             int fromPeriod,
                                             int toPeriod) {
/*
SELECT acc_accountgroup.sequenceintb,
acc_accountgroup.groupname,
acc_accountgroup.pandl,
acc_chartmaster.id as parent,
acc_chartmaster.accountcode,
acc_chartmaster.accountname,
SUM(CASE WHEN acc_gltrans.periodno>=201501 AND acc_gltrans.periodno<=201512 AND acc_gltrans.amount > 0 THEN acc_gltrans.amount ELSE 0 END) AS debit,
SUM(CASE WHEN acc_gltrans.periodno>=201501 AND acc_gltrans.periodno<=201512 AND acc_gltrans.amount < 0 THEN -acc_gltrans.amount ELSE 0 END) AS credit
FROM acc_chartmaster
INNER JOIN acc_accountgroup ON acc_chartmaster.accountgroup_id = acc_accountgroup.id
INNER JOIN acc_gltrans ON acc_chartmaster.id = SUBSTR(acc_gltrans.chartmaster_idx,1,38)
WHERE acc_chartmaster.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}'
GROUP BY acc_chartmaster.accountcode
ORDER BY acc_accountgroup.pandl ASC,
acc_accountgroup.sequenceintb,
acc_chartmaster.accountcode;
*/
    RB_String fromPrd = RB_String::number(fromPeriod);
    RB_String toPrd = RB_String::number(toPeriod);
    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String qStr = "";
    qStr = "SELECT acc_accountgroup.sequenceintb, "
           "acc_accountgroup.groupname, "
           "acc_accountgroup.pandl, "
           "acc_chartmaster.id as parent, "
           "acc_chartmaster.accountcode, "
           "acc_chartmaster.accountname, "
           "SUM(CASE WHEN acc_gltrans.periodno>=" + fromPrd + " AND acc_gltrans.periodno<="
            + toPrd + " AND acc_gltrans.amount > 0 THEN acc_gltrans.amount ELSE 0 END) AS debit, "
           "SUM(CASE WHEN acc_gltrans.periodno>=" + fromPrd + " AND acc_gltrans.periodno<="
            + toPrd + " AND acc_gltrans.amount < 0 THEN -acc_gltrans.amount ELSE 0 END) AS credit "
           "FROM acc_chartmaster "
           "INNER JOIN acc_accountgroup ON acc_chartmaster.accountgroup_id = acc_accountgroup.id "
           "INNER JOIN acc_gltrans ON acc_chartmaster.id = SUBSTR(acc_gltrans.chartmaster_idx,1,38) "
           "WHERE acc_chartmaster.parent='" + rootId + "' "
           "GROUP BY acc_chartmaster.accountcode "
           "ORDER BY acc_accountgroup.pandl ASC, "
           "acc_accountgroup.sequenceintb, "
           "acc_chartmaster.accountcode;";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getTrialBalance() "
                        + query.lastError().text() + " ERROR");
    }
}

/**
 * Clear trial balance (GL summary) for selected period
 * @param fromPeriod
 * @param toPeriod
 */
void ACC_SqlCommonFunctions::clearTrialBalance(int fromPeriod, int toPeriod) {
/*
MySQL:
DELETE acc_glsum FROM acc_glsum
INNER JOIN acc_costcenter ON acc_costcenter.id = acc_glsum.parent
WHERE acc_costcenter.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}'
AND acc_glsum.period >= 201601
AND acc_glsum.period <= 201601;
SQLite:
DELETE FROM acc_glsum WHERE id IN
(SELECT sel.id FROM acc_glsum AS sel
INNER JOIN acc_chartmaster ON acc_chartmaster.id = acc_glsum.parent
WHERE acc_chartmaster.parent='{82cce3e9-b337-4fe2-b31c-93c2ba1424bc}'
AND acc_glsum.period >= 201601
AND acc_glsum.period <= 201601);
*/
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::clearTrialBalance()"
                        " database not open ERROR");
        return;
    }

    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String fromPrd = RB_String::number(fromPeriod);
    RB_String toPrd = RB_String::number(toPeriod);
    RB_String qStr = "";

    if (ACC_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        qStr = "DELETE acc_glsum FROM acc_glsum "
                "INNER JOIN acc_chartmaster ON acc_chartmaster.id = acc_glsum.parent "
                "WHERE acc_chartmaster.parent='" + rootId + "' "
                "AND acc_glsum.period >= " + fromPrd + " "
                "AND acc_glsum.period <= " + toPrd + ";";
    } else if (ACC_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        qStr = "DELETE FROM acc_glsum WHERE id IN "
                "(SELECT sel.id FROM acc_glsum AS sel "
                "INNER JOIN acc_chartmaster ON acc_chartmaster.id = acc_glsum.parent "
                "WHERE acc_chartmaster.parent='" + rootId + "' "
                "AND acc_glsum.period >= " + fromPrd + " "
                "AND acc_glsum.period <= " + toPrd + ");";
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::clearTrialBalance() "
                        + query.lastError().text() + " ERROR");
    }
}

/**
 * Get Cost Center summary to recreate the ACC_CostSum table.
 * During recreation the fromPeriod and toPeriod are the same, usually a month.
 * @param query query object
 * @param fromPeriod period from which to start
 * @param toPeriod period to which to end
 */
void ACC_SqlCommonFunctions::getCostCenterSum(QSqlQuery& query,
                                              int fromPeriod,
                                              int toPeriod) {
/*
SELECT acc_costgroup.sequenceno,
acc_costgroup.groupname,
acc_costgroup.costprofit,
acc_costcenter.id as parent,
acc_costcenter.centercode,
acc_costcenter.centername,
SUM(CASE WHEN acc_gltrans.periodno>=201501 AND acc_gltrans.periodno<=201512 AND acc_gltrans.amount > 0 THEN acc_gltrans.amount ELSE 0 END) AS debit,
SUM(CASE WHEN acc_gltrans.periodno>=201501 AND acc_gltrans.periodno<=201512 AND acc_gltrans.amount < 0 THEN -acc_gltrans.amount ELSE 0 END) AS credit
FROM acc_costcenter
INNER JOIN acc_costgroup ON SUBSTR(acc_costcenter.centergroup_idx,1,38) = acc_costgroup.id
INNER JOIN acc_gltrans ON acc_costcenter.id= SUBSTR(acc_gltrans.costcenter_idx,1,38)
WHERE acc_costcenter.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}'
GROUP BY acc_costcenter.centercode
ORDER BY acc_costgroup.costprofit ASC,
acc_costgroup.sequenceno,
acc_costcenter.centercode;
*/
    RB_String fromPrd = RB_String::number(fromPeriod);
    RB_String toPrd = RB_String::number(toPeriod);
    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String qStr = "";
    qStr = "SELECT acc_costgroup.sequenceno, "
            "acc_costgroup.groupname, "
            "acc_costgroup.costprofit, "
            "acc_costcenter.id as parent, "
            "acc_costcenter.centercode, "
            "acc_costcenter.centername, "
            "SUM(CASE WHEN acc_gltrans.periodno>=" + fromPrd + " AND acc_gltrans.periodno<="
            + toPrd + " AND acc_gltrans.amount > 0 THEN acc_gltrans.amount ELSE 0 END) AS debit, "
            "SUM(CASE WHEN acc_gltrans.periodno>=" + fromPrd + " AND acc_gltrans.periodno<="
            + toPrd + " AND acc_gltrans.amount < 0 THEN -acc_gltrans.amount ELSE 0 END) AS credit "
            "FROM acc_costcenter "
            "INNER JOIN acc_costgroup ON SUBSTR(acc_costcenter.centergroup_idx,1,38) = acc_costgroup.id "
            "INNER JOIN acc_gltrans ON acc_costcenter.id=SUBSTR(acc_gltrans.costcenter_idx,1,38) "
            "WHERE acc_costcenter.parent='" + rootId + "' "
            "GROUP BY acc_costcenter.centercode "
            "ORDER BY acc_costgroup.costprofit ASC, "
            "acc_costgroup.sequenceno, "
            "acc_costcenter.centercode; ";


    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::getCostCenterSum() "
                        + query.lastError().text() + " ERROR");
    }
}

/**
 * Clear cost center summary for selected period
 * @param fromPeriod
 * @param toPeriod
 */
void ACC_SqlCommonFunctions::clearCostCenterSum(int fromPeriod, int toPeriod) {
/*
MySQL:
DELETE acc_costsum FROM acc_costsum
INNER JOIN acc_costcenter ON acc_costcenter.id = acc_costsum.parent
WHERE acc_costcenter.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}'
AND acc_costsum.period >= 201601
AND acc_costsum.period <= 201601;
SQLite:
DELETE FROM acc_costsum WHERE id IN
(SELECT sel.id FROM acc_costsum AS sel
INNER JOIN acc_costcenter ON acc_costcenter.id = acc_costsum.parent
WHERE acc_costcenter.parent='{82cce3e9-b337-4fe2-b31c-93c2ba1424bc}'
AND acc_costsum.period >= 201601
AND acc_costsum.period <= 201601);
*/
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::clearCostCenterSum()"
                        " database not open ERROR");
        return;
    }

    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String fromPrd = RB_String::number(fromPeriod);
    RB_String toPrd = RB_String::number(toPeriod);
    RB_String qStr = "";

    if (ACC_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        qStr = "DELETE acc_costsum FROM acc_costsum "
                "INNER JOIN acc_costcenter ON acc_costcenter.id = acc_costsum.parent "
                "WHERE acc_costcenter.parent='" + rootId + "' "
                "AND acc_costsum.period >= " + fromPrd + " "
                "AND acc_costsum.period <= " + toPrd + ";";
    } else if (ACC_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        qStr = "DELETE FROM acc_costsum WHERE id IN "
                "(SELECT sel.id FROM acc_costsum AS sel "
                "INNER JOIN acc_costcenter ON acc_costcenter.id = acc_costsum.parent "
                "WHERE acc_costcenter.parent='" + rootId + "' "
                "AND acc_costsum.period >= " + fromPrd + " "
                "AND acc_costsum.period <= " + toPrd + ");";
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::clearCostCenterSum() "
                        + query.lastError().text() + " ERROR");
    }
}

/**
 * Set due date for invoice receipt or payment of debtor or creditor
 * @param paymentTermsId payment terms ID of debtor or creditor
 * @param dueDate due to be set
 */
void ACC_SqlCommonFunctions::setDueDate(const RB_String& paymentTermsId,
                                             QDate& dueDate) {
    /*
    SELECT daysbeforedue, dayinfollowingmonth
    FROM acc_paymentterm INNER JOIN acc_customer
        ON acc_paymentterm.id=acc_customer.paymentterm
    WHERE acc_customer.id='{b304f1da-1789-4ad0-9180-2b86d0c57f96}';
    */
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::setDueDate()"
                        " database not open ERROR");
        return;
    }

    RB_String qStr = "";
    qStr = "SELECT daysbeforedue, dayinfollowingmonth "
           "FROM acc_paymentterm "
           "WHERE acc_paymentterm.id='" + paymentTermsId + "';";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::setDueDate() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    if (!query.first()) {
        RB_DEBUG->debug("ACC_SqlCommonFunctionsFunction::getDueDate() "
                        "no paymentterm found DEBUG");
        return;
    }

    dueDate = QDate::currentDate();

    if (query.value(1).toInt() == 0) {
        dueDate = dueDate.addDays(query.value(0).toInt());
    } else {
        // following month
        int dayInFollowingMonth = query.value(0).toInt();
        dueDate = dueDate.addMonths(1);
        dueDate.setDate(dueDate.year(),
                        dueDate.month(),
                        dayInFollowingMonth);
    }
}

/**
 * Is the account used in field of a table in and after the (current) year
 * @param accountId account ID to be found
 * @returns true in case ID is used in field of table
 */
bool ACC_SqlCommonFunctions::isAccountUsed(const RB_String& accountId) {
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::isAccountUsed()"
                        " database not open ERROR");
        return true;
    }


    RB_String qStr = "";
    qStr = "SELECT * FROM `acc_gltrans` WHERE ";
    substrIdxId("chartmaster_idx", qStr);
    qStr += " = '" + accountId + "' LIMIT 0, 2;";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::isAccountUsed() "
                        + query.lastError().text() + " ERROR");
        return true;
    }

    if (!query.first()) {
        // a record exists
        return false;
    }

    return true;
}

/**
 * @returns the last bank transaction date for import of bank statements
 * or invalid date if the bank account is not part of this ACC_Project
 */
QDate ACC_SqlCommonFunctions::lastBankTransactionDate(const RB_String& bankAccId) {
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error("ACC_SqlCommonFunctions::lastBankTransactionDate()"
                        " database not open ERROR");
        return QDate();
    }

    RB_String qStr = "SELECT MAX(acc_banktrans.transdate) FROM acc_transdoc "
            "INNER JOIN acc_banktrans ON acc_banktrans.transdoc_id=acc_transdoc.id "
            "WHERE acc_transdoc.parent='" + bankAccId + "';";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) {
            RB_DEBUG->info("ACC_SqlCommonFunctions::lastBankTransactionDate() 1 "
                            " MAX(transdate) not found INFORMATION");
            return QDate(1970, 1, 1);
        }

        QDate date = query.value(0).toDate();

        if (!date.isValid()) {
            // first time if no bank transactions have been made
            RB_DEBUG->info("ACC_SqlCommonFunctions::lastBankTransactionDate() 2 "
                           " MAX(transdate) not valid INFORMATION");
            return QDate(1970, 1, 1);
        }

        return date;
    }

    RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::lastBankTransactionDate() 3 "
                    + query.lastError().text() + " ERROR");
    return QDate();
}

void ACC_SqlCommonFunctions::getDebtorAccrued(QSqlQuery& query,
                                              const QDate& startDate,
                                              const QDate& endDate) {
    // enum TransType
    // TransDebtor = 10,           /**< Debtor, sales invoice or customer transaction type */
    // TransSalesOrder = 30,       /**< Sales order (Note: not sales invoice) */

    getInvoicesAccrued(query, startDate, endDate, 10, 30);
}

void ACC_SqlCommonFunctions::getCreditorAccrued(QSqlQuery& query,
                                                const QDate& startDate,
                                                const QDate& endDate) {
    // enum TransType
    // TransCreditor = 20,         /**< Creditor, purchase invoice or supplier transaction type */
    // TransPurchOrder = 40,       /**< Purchase order (Note: not purchase invoice) */

    getInvoicesAccrued(query, startDate, endDate, 20, 40);
}

void ACC_SqlCommonFunctions::getBankPaymentList(
        QSqlQuery& query, const RB_String& projectId, const RB_String& dateFrom,
        const RB_String& dateTo, const RB_String& bankAccountNumber) {
    // 'Z' in SQL are for including the date even if there is time behind date

    RB_String qStr = "SELECT  description, SUBSTR(chartmaster_idx, 39) as GL, "
                     "amount, transno, amountcleared as cleared, "
                     "SUBSTR(transdate, 1, 10) as transdate "
                     "FROM acc_banktrans WHERE parent='"
            + projectId + "' AND transdate>='"
            + dateFrom + "' AND transdate<='"
            + dateTo + "Z' ";
    if (!bankAccountNumber.isEmpty()) {
        qStr += "AND bankaccountnumber='" + bankAccountNumber + "' ";
    }
    qStr += "ORDER BY transdate;";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getBankPaymentList() "
                        + query.lastError().text() + " ERROR");
        return;
    }
}

void ACC_SqlCommonFunctions::getInvoicesAccrued(QSqlQuery& query,
                                                const QDate& startDate,
                                                const QDate& endDate,
                                                int debtCredType1,
                                                int debtCredType2) {

    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_String start = startDate.toString(Qt::ISODate);
    RB_String end = endDate.toString(Qt::ISODate);
    RB_String docType1 = RB_String::number(debtCredType1);
    RB_String docType2 = RB_String::number(debtCredType2);

    /*
    RB_String qStr = "SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, acc_transdoc.transno, "
    "acc_transdoc.description, acc_transdoc.totalamountrec, acc_transdoc.totalamountpay, "
    "SUBSTR(t2.transdate,1,10) AS dateallocdoc, t2.transno AS transnoallocdoc, acc_transallocn.amount as "
    "allocamount "
    "FROM acc_transdoc "
    "LEFT OUTER JOIN acc_transallocn ON acc_transallocn.docallocto_id = acc_transdoc.id "
    "INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent "
    "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
    "LEFT OUTER JOIN acc_transdoc as t2 ON t2.id=acc_transallocn.docfrom_id "
    "WHERE (acc_transdoc.settled=0 OR t2.transdate>'" + end + "' OR t2.transdate<'0') AND "
    "acc_transdoc.transdate>='" + start + "' AND acc_transdoc.transdate<='" + end + "' AND "
    "(acc_transdoc.doctype=" + docType1+ " OR acc_transdoc.doctype=" + docType2+ ") AND "
    "acc_project.id='" + rootId + "' "
    "ORDER BY acc_transdoc.transno; ";
    */
/*
    RB_String qSubStr1 = "(SELECT SUM(acc_transallocn.amount) FROM acc_transdoc as t2 "
        "LEFT OUTER JOIN acc_transallocn ON acc_transallocn.docallocto_id = t2.id "
        "INNER JOIN acc_chartmaster ON acc_chartmaster.id=t2.parent "
        "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
        "WHERE acc_transallocn.docallocto_id = acc_transdoc.id "
        "AND acc_transallocn.datealloc<='" + end + "' "
        "AND (t2.doctype=" + docType1 + " "
        "OR t2.doctype=" + docType2 + ") "
        "AND acc_project.id='" + rootId + "' "
        "GROUP BY t2.id)";

    RB_String qSubStr2 = "";

    if (debtCredType1 == 10) {
        qSubStr2 = "amountallocated < acc_transdoc.totalamountrec - 0.005";
    } else {
        // amountallocated is negative
        qSubStr2 = "amountallocated > -acc_transdoc.totalamountpay + 0.005";
    }

    RB_String qStr = "SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, acc_transdoc.transno, "
        "acc_transdoc.description, acc_transdoc.totalamountrec, acc_transdoc.totalamountpay, "
        "CASE WHEN " + qSubStr1 + " THEN " + qSubStr1 + " ELSE 0 END as amountallocated "
        "FROM acc_transdoc "
        "INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent "
        "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
        "WHERE   acc_transdoc.transdate>='" + start + "' "
        "AND acc_transdoc.transdate<='" + end + "' "
        "AND (acc_transdoc.doctype=" + docType1 + " OR acc_transdoc.doctype=" + docType2 + ") "
        "AND acc_project.id='" + rootId + "' "
        "AND " + qSubStr2 + " ORDER BY acc_transdoc.transno";

    RB_String qStr = "SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, "
            "acc_transdoc.transno, acc_transdoc.description, acc_transdoc.totalamountrec, "
            "acc_transdoc.totalamountpay, (SELECT SUM(acc_transallocn.amount) "
            "FROM acc_transallocn WHERE acc_transallocn.docallocto_id = acc_transdoc.id "
            "AND SUBSTR(acc_transallocn.datealloc,1,10) <='" + end + "') as allocated "
            "FROM acc_transdoc "
            "LEFT OUTER JOIN acc_transallocn ON acc_transallocn.docallocto_id = acc_transdoc.id "
            "INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent "
            "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
            "LEFT OUTER JOIN acc_transdoc as t2 ON t2.id=acc_transallocn.docfrom_id "
            "WHERE (acc_transdoc.settled=0 OR SUBSTR(t2.transdate,1,10)>'" + end + "' OR t2.transdate<'0') "
            "AND SUBSTR(acc_transdoc.transdate,1,10)<='" + end + "' "
            "AND (acc_transdoc.doctype=" + docType1 + " OR acc_transdoc.doctype=" + docType2 + ") "
            "AND acc_project.id='" + rootId + "' "
            "GROUP BY acc_transdoc.transno "
            "ORDER BY acc_transdoc.transno;";
*/

    // 'Z' in SQL are for including the date even if there is time behind date

//    RB_String qStr = "SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, "
//                     "acc_transdoc.transno, acc_transdoc.description, "
//                     "acc_transdoc.totalamountrec, acc_transdoc.totalamountpay, "
//                     "SUBSTR(t2.transdate,1,10) AS dateallocdoc, t2.transno AS transnoallocdoc, "
//                     "acc_transallocn.amount as allocamount "
//                     "FROM acc_transdoc "
//                     "LEFT OUTER JOIN acc_transallocn ON acc_transallocn.docallocto_id = acc_transdoc.id "
//                     "INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent "
//                     "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
//                     "LEFT OUTER JOIN acc_transdoc as t2 ON t2.id=acc_transallocn.docfrom_id "
//                     "WHERE (acc_transdoc.settled=0 OR t2.transdate>'" + end
//            + "Z' OR t2.transdate<'0') AND acc_transdoc.transdate>='" + start
//            + "' AND acc_transdoc.transdate<='" + end
//            + "Z' AND (acc_transdoc.doctype=" + docType1
//            + " OR acc_transdoc.doctype=" + docType2
//            + ") AND acc_project.id='" + rootId
//            + "' ORDER BY acc_transdoc.transno;";
    /*
SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, acc_transdoc.transno, acc_transdoc.description, acc_transdoc.totalamountrec, acc_transdoc.totalamountpay,
(CASE WHEN td3.transdate<='{$valueto}Z' THEN SUBSTR(td3.transdate,1,10) ELSE 0 END) AS dateallocdoc,
(CASE WHEN td3.transdate<='{$valueto}Z' THEN td3.transno ELSE 0 END) AS transnoallocdoc,
(CASE WHEN td3.transdate<='{$valueto}Z' THEN ta3.amount ELSE 0 END) AS allocamount
FROM acc_transdoc
LEFT OUTER JOIN acc_transallocn as ta3 ON ta3.docallocto_id = acc_transdoc.id
LEFT OUTER JOIN acc_transdoc as td3 ON td3.id=ta3.docfrom_id
WHERE acc_transdoc.id IN
(
SELECT DISTINCT acc_transdoc.id
FROM acc_transdoc
LEFT OUTER JOIN acc_transallocn as ta2 ON ta2.docallocto_id = acc_transdoc.id
INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent
INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent
LEFT OUTER JOIN acc_transdoc as td2 ON td2.id=ta2.docfrom_id
WHERE (acc_transdoc.settled=0 OR td2.transdate>'{$valueto}Z' OR td2.transdate<'0') AND acc_transdoc.transdate>='{$valuefrom}' AND acc_transdoc.transdate<='{$valueto}Z' AND (acc_transdoc.doctype=10 OR acc_transdoc.doctype=30) AND
acc_project.company='InIPED'
)
ORDER BY acc_transdoc.transno;
-- set {$valuefrom} to date for example 2013-01-01, set {$valueto} to date for example 2013-12-31. Note: if part payments the original invoice amount is duplicated, doctype=30 Sales Invoice, 10 Sales Order, Z after {$valueto} to include possibly added time
    */

    RB_String qStr =
            "SELECT SUBSTR(acc_transdoc.transdate,1,10) as transdocdate, "
            "acc_transdoc.transno, acc_transdoc.description, "
            "acc_transdoc.totalamountrec, acc_transdoc.totalamountpay, "
            "SUBSTR(td3.transdate,1,10) AS dateallocdoc, "
            "td3.transno AS transnoallocdoc, "
            "(CASE WHEN td3.transdate<='" + end + "Z' THEN ta3.amount ELSE 0 END) AS allocamount, "
            "(CASE WHEN td3.transdate<='" + end + "Z' THEN 0 ELSE ta3.amount END) AS nextperiod "
            "FROM acc_transdoc "
            "LEFT OUTER JOIN acc_transallocn as ta3 ON ta3.docallocto_id = acc_transdoc.id "
            "LEFT OUTER JOIN acc_transdoc as td3 ON td3.id=ta3.docfrom_id "
            "WHERE acc_transdoc.id IN "
            "("
            "SELECT DISTINCT acc_transdoc.id "
            "FROM acc_transdoc "
            "LEFT OUTER JOIN acc_transallocn as ta2 ON ta2.docallocto_id = acc_transdoc.id "
            "INNER JOIN acc_chartmaster ON acc_chartmaster.id=acc_transdoc.parent "
            "INNER JOIN acc_project ON acc_project.id=acc_chartmaster.parent "
            "LEFT OUTER JOIN acc_transdoc as td2 ON td2.id=ta2.docfrom_id "
            "WHERE (acc_transdoc.settled=0 OR td2.transdate>'" + end
            + "Z' OR td2.transdate<'0') AND acc_transdoc.transdate>='" + start
            + "' AND acc_transdoc.transdate<='" + end
            + "Z' AND (acc_transdoc.doctype=" + docType1
            + " OR acc_transdoc.doctype=" + docType2
            + ") AND acc_project.id='" + rootId
            + "') "
            + "ORDER BY acc_transdoc.transno;";

// RB_DEBUG->print(qStr);

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::getInvoicesAccrued() "
                        + query.lastError().text() + " ERROR");
        return;
    }
}

/**
 * Use following statement to find acc_gltrans orphans:
 * SELECT acc_gltrans.* FROM acc_transdoc RIGHT OUTER JOIN acc_gltrans
 * ON acc_gltrans.transdoc_id=acc_transdoc.id WHERE acc_transdoc.id IS NULL;
 * @returns true when orphans are found
 */
bool ACC_SqlCommonFunctions::isGlTransOrphanExisting() {
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::isGlTransOrphanExisting()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr = "";
    qStr = "SELECT `acc_gltrans`.* FROM `acc_transdoc` RIGHT OUTER JOIN `acc_gltrans` "
            "ON `acc_gltrans`.`transdoc_id`=`acc_transdoc`.`id` WHERE `acc_transdoc`.`id` IS NULL;";

    if (ACC_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        qStr = "SELECT * FROM acc_gltrans WHERE transdoc_id NOT IN (SELECT id FROM acc_transdoc);";
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::isGlTransOrphanExisting() "
                        + query.lastError().text() + " ERROR");
        return false;
    }

    while (query.next()) {
        // a record exists
        ACC_DIALOGFACTORY->commandMessage("Orphaned ACC_GlTrans "
                                          + query.value("transdocno").toString()
                                          + " " + query.value("amount").toString());
    }

    return query.first();
}

/**
 * Use following statement to check whether the allocations are consistent
 * with the allocation to the debtor/creditor documents
 * @brief ACC_SqlCommonFunctions::isAllocNotConsistent
 * @return
 */
bool ACC_SqlCommonFunctions::isAllocNotConsistent() {
    if (!ACC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error(
                        "ACC_SqlCommonFunctions::isGlTransOrphanExisting()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr =
            "SELECT acc_transdoc.id, acc_transdoc.transno, acc_transdoc.settled, acc_transdoc.alloc as docalloc, "
            "(SELECT SUM(acc_transallocn.amount) FROM acc_transallocn WHERE acc_transdoc.id=acc_transallocn.docallocto_id) as allocs "
            "FROM acc_transdoc "
            "WHERE (acc_transdoc.alloc - (SELECT SUM(acc_transallocn.amount) "
            "       FROM acc_transallocn WHERE acc_transdoc.id=acc_transallocn.docallocto_id) < -0.05 "
            "  OR acc_transdoc.alloc - (SELECT SUM(acc_transallocn.amount) "
            "       FROM acc_transallocn where acc_transdoc.id=acc_transallocn.docallocto_id) > 0.05) "
            "  OR (acc_transdoc.alloc <> 0 AND acc_transdoc.id NOT IN (SELECT docallocto_id FROM acc_transallocn));";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_SqlCommonFunctionsFunction::isAllocNotConsistent() "
                        + query.lastError().text() + " ERROR");
        return false;
    }

    while (query.next()) {
        // a record exists
        ACC_DIALOGFACTORY->commandMessage("Inconsistent ACC_TransAllocn "
                                          + query.value(0).toString()
                                          + " " + query.value(1).toString()
                                          + " " + query.value(2).toString()
                                          + " " + query.value(3).toString());
    }

    return query.first();
}

/**
 * Run the SQL to create the ACC_SysType (System Types) row if not exists.
 * The purpose of the ACC_SysType is to record the last used
 * document number and to create the new document number.
 * The systypecode corresponds with the ACC2::TransType
 * The default sequende number is build up as:
 * first four digits = PerspectiveType (2 digits) + TransType (2 digits) times
 * a hundredthousand (100000), example the first (sales) invoice will
 * look like: 111000000 (11 10 00000).
 * @param transType transaction type
 * @param typeListId transaction type list ID (to differentiate
 * for example bank accounts)
 * @param typeListName transaction type name
 * @returns true on success
 */
bool ACC_SqlCommonFunctions::execSysTypeSql(int transType,
                                            const RB_String& typeListId,
                                            const RB_String& typeListName) {

    /*
      Refer to ACC2::TransType
      From webERP for example:
        0,Journal - GL,2
        1,Payment - GL,2
        2,Receipt - GL,0
        3,Standing Journal,0
        10,Sales Invoice,10800049
        11,Credit Note,3
        12,Receipt,3
        13,Bank Cash,0
        15,Journal - Debtors,0
        16,Location Transfer,22
        17,Stock Adjustment,17
        18,Purchase Order,2
        20,Purchase Invoice,17
        21,Debit Note,3
        22,Creditors Payment,4
        23,Creditors Journal,0
        25,Purchase Order Delivery,31
        26,Work Order Receipt,7
        28,Work Order Issue,7
        29,Work Order Variance,1
        30,Sales Order,10700002
        31,Shipment Close,26
        35,Cost Update,19
        36,Exchange Difference,1
        40,Work Order,14
        50,Opening Balance,0
        500,Auto Debtor Number,0
    */

    // Create new record because record does not exists
    int typeNo = (int) RB2::PerspectiveACC; // 11
    typeNo *= 100;
    typeNo += (int) transType; // 10 for ACC2::TransDebtor
    typeNo *= 100000;

    RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    RB_ObjectBase* obj = ACC_OBJECTFACTORY->newSingleObject("ACC_SysType");
    obj->setValue("parent", ACC_MODELFACTORY->getRootId());
    obj->setValue("created", dt);
    obj->setValue("changed", dt);
    obj->setValue("muser", username);
    obj->setValue("systypecode", transType);
    obj->setValue("systypename", typeListName);
    obj->setValue("systypeno", typeNo);
    obj->setValue("systypelist_id", typeListId);
    obj->setFlag(RB2::FlagIsDirty);

    if (!obj->dbUpdate(ACC_MODELFACTORY->getDatabase())) {
        RB_DEBUG->debug("ACC_SqlCommonFunctions::execSysTypeSql() sql ERROR:");
        RB_DEBUG->print(" " + ACC_MODELFACTORY->getDatabase().lastError().text());
        delete obj;
        return false;
    }

    delete obj;
    return true;
}

