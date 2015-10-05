/*****************************************************************
 * $Id: acc_sqlcommonfunctions.h 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SQLCOMMONFUNCTIONS_H
#define ACC_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 * such as next salesorder number and invoice number
 */
class ACC_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    ACC_SqlCommonFunctions();
    virtual ~ACC_SqlCommonFunctions();

    virtual int getLastTransNo(int transType,
                               const RB_String& typeListId);
    virtual bool setLastTransNo(int transType,
                               const RB_String& typeListId,
                               int lastNumber);
    virtual int getNextTransNo(int transType,
                               const RB_String& typeListId,
                               const RB_String& typeListName);
    virtual void getStockGlCode(QSqlQuery& query, const RB_String& stockId);
    virtual void getTaxAuthId(QSqlQuery& query,  const RB_String& taxLocationId,
                               const RB_String& taxProvinceId,
                               const RB_String& taxCatId);
    virtual void getSalesTaxes(QSqlQuery& query, const RB_String& taxLocation,
                               const RB_String& dispTaxProvince,
                               const RB_String& taxCategoryId);
    virtual void getPurchaseTaxes(QSqlQuery& query, const RB_String& taxLocation,
                                  const RB_String& receiptTaxProvince,
                                  const RB_String& taxCategoryId);
    virtual void getCompanyTaxes(QSqlQuery& query,
                                 const RB_String& dispTaxProvince,
                                 const RB_String& taxLocationId);
    virtual void getCogsGlAccount(QSqlQuery& query,
                                       const RB_String& area,
                                       const RB_String& stockId/*,
                                       const RB_String& salesType*/);
//    virtual void getSalesGlAccount(QSqlQuery& query,
//                                   const RB_String& taxLocation,
//                                   const RB_String& dispTaxProvince,
//                                   const RB_String& taxCategory,
//                                   const RB_String& stockCatId);
    virtual void getSalesGlAccount(QSqlQuery& query,
                                   const RB_String& taxAuthId,
                                   const RB_String& areaId,
                                   const RB_String& stockId,
                                   const RB_String& salesTypeId);
    virtual void getPurchaseGlAccount(QSqlQuery& query,
                                      const RB_String& taxGroupId,
                                      const RB_String& stockCatId);
    virtual void getTrialBalance(QSqlQuery& query,
                                 int fromPeriod,
                                 int toPeriod);

    virtual void setDueDate(const RB_String& paymentTermsId,
                            QDateTime& dueDate);
    virtual bool isAccountUsed(const RB_String& accountId);
    virtual QDate lastBankTransactionDate(const RB_String& bankAccId);

    virtual void getDebtorAccrued(QSqlQuery& query, const QDate& startDate,
                                  const QDate& endDate);
    virtual void getCreditorAccrued(QSqlQuery& query, const QDate& startDate,
                                    const QDate& endDate);

    // Debug only:
    virtual bool isGlTransOrphanExisting();
    virtual bool isAllocNotConsistent();

private:
    bool execSysTypeSql(int transType,
                        const RB_String& typeListId,
                        const RB_String& typeListName);
    void getTaxQuery(RB_String& qStr,
                     const QString& taxLocationId,
                     const QString& dispReceiptTaxProvinceId,
                     const QString& taxCategoryId);
    void getInvoicesAccrued(QSqlQuery& query, const QDate &startDate,
                            const QDate& endDate,
                            int debtCredType1,
                            int debtCredType2);

};
#endif // ACC_SQLCOMMONFUNCTIONS_H
