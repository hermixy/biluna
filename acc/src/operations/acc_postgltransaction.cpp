/*****************************************************************
 * $Id: acc_postgltransaction.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 7, 2010 10:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_postgltransaction.h"

#include <cmath>
#include "acc_costsum.h"
#include "acc_glsum.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "acc_qacostcenter.h"
#include "acc_sqlcommonfunctions.h"


/**
 * Constructor
 */
ACC_PostGlTransaction::ACC_PostGlTransaction() {
    mDebitAmt = 0.0;
    mCreditAmt = 0.0;
    mGlSumList = NULL;
    mCcSumList = NULL;
}

/**
 * Destructor
 */
ACC_PostGlTransaction::~ACC_PostGlTransaction() {
    if (mGlSumList) {
        delete mGlSumList;
    }
    if (mCcSumList) {
        delete mCcSumList;
    }
}

/**
 * Post or unpost the GL transaction from the ACC_GlSumList and ACC_CostSum.
 * Used by sales/purchase orders
 * @param glTrans GL transaction
 * @return true on success
 */
bool ACC_PostGlTransaction::execute(RB_ObjectContainer* glTransList) {
    if (!glTransList) return false;

    // Get the current cost center amount and if not create, now also the record is available
//    RB_String costCenterId = glTrans->getIdValue("costcenter_idx").toString();
//    if (costCenterId != "0" && costCenterId != "") {
//        setCcDebitCredit(glTrans);
//        postCc(glTrans);
//    }

//    // Get the current actual amount and if not create, now also the record is available
//    setDebitCredit(glTrans);
//    return postTransaction(glTrans);

    // new
    if (mCcSumList) {
        delete mCcSumList;
    }

    mCcSumList = new RB_ObjectContainer("", NULL, "ACC_CostSumList",
                                        ACC_OBJECTFACTORY);

    if (mGlSumList) {
        delete mGlSumList;
    }

    mGlSumList = new RB_ObjectContainer("", NULL, "ACC_GlSumList",
                                        ACC_OBJECTFACTORY);

    bool success = createGlSumList(glTransList);
    success = success && updateGlSumList(glTransList);
    success = success && createCcSumList(glTransList);
    success = success && updateCcSumList(glTransList);
    return success;
}

/**
 * Create summary list, used when for example when ACC_GlTrans is deleted
 * called from ACC_GlTransactionWidget
 * @param returns true upon success
 */
/*
bool ACC_PostGlTransaction::createSumList(RB_ObjectContainer* glTransList) {
    if (!glTransList) return false;

    if (mGlSumList) {
        delete mGlSumList;
    }

    mGlSumList = new RB_ObjectContainer("", NULL, "ACC_GlSumList",
                                        ACC_OBJECTFACTORY);

    RB_ObjectIterator* iter = glTransList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        setGlSumAmount(obj);
    }
    delete iter;

    // return true;
    return createCcSumList(glTransList);
}
*/
/**
 * Post summary list, updated with deleted and/or added ACC_GlTtrans
 * @returns true on success
 */
/*
bool ACC_PostGlTransaction::postSumList() {
    if (mGlSumList) {
        bool success = mGlSumList->dbUpdateList(ACC_MODELFACTORY->getDatabase(),
                                                RB2::ResolveOne);
        if (success) {
            return postCcSumList();
        }
    }

    return false;
}
*/
/**
 * @brief Create Gl summary list based on new
 * and existing (to be deleted) transactions
 * @param glTransList
 * @return true on success
 */
bool ACC_PostGlTransaction::createGlSumList(RB_ObjectContainer* glTransList) {
    if (!glTransList || !mGlSumList) {
        RB_DEBUG->error("ACC_PostGlTransaction::createGlSumList() "
                        "NULL ERROR");
        return false;
    }

    QString where = "";
    QString accountId = "";
    int periodNo = 0;
    bool isFirst = true;

    RB_ObjectIterator* iter = glTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        accountId = glTrans->getIdValue("chartmaster_idx").toString();
        periodNo = glTrans->getValue("periodno").toInt();

        if (!isFirst) {
            where += " OR ";
            isFirst = false;
        }

        where += "(parent='" + accountId + "' AND period="
                + QString::number(periodNo) + ")";
    }

    delete iter;
    mGlSumList->dbReadWhere(ACC_MODELFACTORY->getDatabase(), where);
    return true;
}

/**
 * @brief Update GL summary list
 * @param glTransList
 */
bool ACC_PostGlTransaction::updateGlSumList(RB_ObjectContainer* glTransList) {
    if (!glTransList || !mGlSumList) {
        RB_DEBUG->error("ACC_PostGlTransaction::updateGlSumList() "
                        "NULL ERROR");
        return false;
    }

    RB_ObjectIterator* iter = glTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        // Get relevant GL summary
        RB_ObjectBase* glTrans = iter->currentObject();
        RB_ObjectBase* targetGlSum = nullptr;
        RB_ObjectIterator* sumIter = mGlSumList->createIterator();

        for (sumIter->first(); !sumIter->isDone(); sumIter->next()) {
            RB_ObjectBase* glSum = iter->currentObject();

            if (glTrans->getIdValue("chartmaster_idx")
                    == glSum->getValue("parent")
                    && glTrans->getIdValue("periodno")
                    == glSum->getValue("periodno")) {
                targetGlSum = glSum;
                sumIter->setDone();
            }

        }

        delete iter;

        // Create GL summary if not already existing
        if (!targetGlSum) {
            // No chartdetail yet available, create new
            targetGlSum = mGlSumList->newObject();

            QString accountId = glTrans->getIdValue("chartmaster_idx").toString();
            int periodNo = glTrans->getValue("periodno").toInt();
            QString accountCode = "0";
            QString accountName = "Default";
            RB_ObjectBase* acctObj = ACC_QACHARTMASTER->getAcctObj(accountId);

            if (acctObj) {
                accountCode = acctObj->getValue("accountcode").toString();
                accountName = acctObj->getValue("accountname").toString();
            }

            targetGlSum->setValue("parent", accountId);
            targetGlSum->setValue("period", periodNo);
            targetGlSum->setValue("accountcode", accountCode);
            targetGlSum->setValue("accountname", accountName);
            targetGlSum->setValue("debit", 0.0);
            targetGlSum->setValue("credit", 0.0);
        }

        // Update the summary amount
        double debitAmt = targetGlSum->getValue("debit").toDouble();
        double creditAmt = targetGlSum->getValue("credit").toDouble();
        double amount = glTrans->getValue("amount").toDouble();

        // update amount
        if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
            if (amount > 0) {
                debitAmt += amount;
            } else {
                creditAmt -= amount;
            }
        } else {
            if (amount > 0) {
                debitAmt -= amount;
            } else {
                creditAmt += amount;
            }
        }

        targetGlSum->setValue("debit", debitAmt);
        targetGlSum->setValue("credit", creditAmt);
    }

    delete iter;
    return true;
}

/**
 * Set debit/credit amount of ACC_GlSum and,
 * if not exists, create ACC_GlSum record
 * @param glTrans GL transaction
 */
/*
void ACC_PostGlTransaction::setDebitCredit(RB_ObjectBase* glTrans) {
    RB_String accountId = glTrans->getIdValue("chartmaster_idx").toString();
    if (!ACC_MODELFACTORY->isValidId(accountId)) {
        RB_DEBUG->error("ACC_PostGlTransaction::setDebitCredit() id ERROR");
    }

    int periodNo = glTrans->getValue("periodno").toInt();
    mDebitAmt = 0.0;
    mCreditAmt = 0.0;

    // get existing amounts
    RB_String qStr = "";
    qStr = "SELECT debit, credit FROM acc_glsum WHERE parent='"
           + accountId + "' AND period=" + RB_String::number(periodNo) + ";";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) {
            int accountCode = 0;
            RB_String accountName = "Default";
            qStr = "SELECT accountcode, accountname FROM acc_chartmaster WHERE id='"
                   + accountId + "';";
            if (query.exec(qStr) && query.first()) {
                accountCode = query.value(0).toInt();
                accountName = query.value(1).toString();
            }

            // No chartdetail yet available, create new
            ACC_GlSum* glSum = new ACC_GlSum(RB_Uuid::createUuid().toString(),
                                             NULL, "ACC_GlSum");
            glSum->setValue("parent", accountId);
            glSum->setValue("period", periodNo);
            glSum->setValue("accountcode", accountCode);
            glSum->setValue("accountname", accountName);
            glSum->setValue("debit", 0.0);
            glSum->setValue("credit", 0.0);
            glSum->dbUpdate(ACC_MODELFACTORY->getDatabase());
            delete glSum;
        } else {
            mDebitAmt = query.value(0).toDouble();
            mCreditAmt = query.value(1).toDouble();
        }
    } else {
        RB_DEBUG->error("ACC_PostGlTransaction::setDebitCredit() "
                         + query.lastError().text() + " ERROR");
    }

    // update amount
    if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
        double amount = glTrans->getValue("amount").toDouble();
        if (amount > 0) {
            mDebitAmt += amount;
        } else {
            mCreditAmt -= amount;
        }
    } else {
        double amount = glTrans->getValue("amount").toDouble();
        if (amount > 0) {
            mDebitAmt -= amount;
        } else {
            mCreditAmt += amount;
        }
    }
}
*/
/**
 * Post (or unpost) the GL transaction to the GL Summary per month
 * @param glTrans GL transaction
 * @returns true on success
 */
/*
bool ACC_PostGlTransaction::postTransaction(RB_ObjectBase* glTrans) {
    RB_String accountId = glTrans->getIdValue("chartmaster_idx").toString();
    if (!ACC_MODELFACTORY->isValidId(accountId)) {
        RB_DEBUG->error("ACC_PostGlTransaction::setDebitCredit() id ERROR");
    }

    int periodNo = glTrans->getValue("periodno").toInt();

    RB_String qStr = "";
    qStr = "UPDATE acc_glsum SET debit=" + RB_String::number(mDebitAmt, 'f', 6)
           + ", credit=" + RB_String::number(mCreditAmt, 'f', 6)
           + " WHERE period=" + RB_String::number(periodNo)
           + " AND  parent='" + accountId + "';";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_PostGlTransaction::postTransaction() "
                         + query.lastError().text() + " ERROR");
        return false;
    }

    return true;
}
*/
/**
 * Set amount of ACC_GlSum and, if not exists, create ACC_GlSum record
 * helper function of createSumList()
 * @param glTrans GL transaction
 */
/*
void ACC_PostGlTransaction::setGlSumAmount(RB_ObjectBase* glTrans) {
    RB_String accountId = glTrans->getIdValue("chartmaster_idx").toString();
    int periodNo = glTrans->getValue("periodno").toInt();
    RB_ObjectBase* glSum = NULL;

    RB_String qStr = "";
    qStr = "SELECT id FROM acc_glsum WHERE parent='"
           + accountId + "' AND period=" + RB_String::number(periodNo)+ ";";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) {
            int accountCode = 0;
            RB_String accountName = "Default";
            qStr = "SELECT accountcode, accountname FROM acc_chartmaster WHERE id='"
                   + accountId + "';";
            if (query.exec(qStr) && query.first()) {
                accountCode = query.value(0).toInt();
                accountName = query.value(1).toString();
            }

            // Find glSum if already created
            RB_ObjectIterator* iter = mGlSumList->createIterator();
            for (iter->first(); !iter->isDone() && !glSum; iter->next()) {
                RB_ObjectBase* obj = iter->currentObject();

                if (obj->getValue("parent").toString() == accountId
                        && obj->getValue("period").toInt() == periodNo) {
                    glSum = obj;
                }
            }
            delete iter;

            // glSum not found and to be created
            if (!glSum) {
                glSum = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                     mGlSumList, "ACC_GlSum");
                glSum->setValue("parent", accountId);
                glSum->setValue("period", periodNo);
                glSum->setValue("accountcode", accountCode);
                glSum->setValue("accountname", accountName);
                glSum->setValue("debit", 0.0);
                glSum->setValue("credit", 0.0);
            }
        } else {
            RB_String id = query.value(0).toString();
            glSum = mGlSumList->getObject(id);

            if (!glSum) {
                glSum = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                     mGlSumList, "ACC_GlSum");
                glSum->setId(id);
                glSum->dbRead(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            }
        }
    } else {
        RB_DEBUG->error("ACC_PostGlTransaction::setGlSumAmount() "
                         + query.lastError().text() + " ERROR");
        return;
    }

    if (!glSum){
        RB_DEBUG->error("ACC_PostGlTransaction::setGlSumAmount() ERROR");
        return;
    }

    double debitAmt = glSum->getValue("debit").toDouble();
    double creditAmt = glSum->getValue("credit").toDouble();
    double amount = glTrans->getValue("amount").toDouble();

    // update amount
    if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
        if (amount > 0) {
            debitAmt += amount;
        } else {
            creditAmt -= amount;
        }
    } else {
        if (amount > 0) {
            debitAmt -= amount;
        } else {
            creditAmt += amount;
        }
    }

    glSum->setValue("debit", debitAmt);
    glSum->setValue("credit", creditAmt);
}
*/
/**
 * Create cost center summary list, used when for example
 * when ACC_GlTrans is deleted
 * called from ACC_GlTransactionWidget
 * @param returns true upon success
 */

bool ACC_PostGlTransaction::createCcSumList(RB_ObjectContainer* glTransList) {
    if (!glTransList || !mCcSumList) {
        RB_DEBUG->error("ACC_PostGlTransaction::createCcSumList() "
                        "NULL ERROR");
        return false;
    }

    QString where = "";
    QString costCenterId = "";
    int periodNo = 0;
    bool isFirst = true;

    RB_ObjectIterator* iter = glTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        costCenterId = glTrans->getIdValue("costcenter_idx").toString();
        periodNo = glTrans->getValue("periodno").toInt();

        if (!isFirst) {
            where += " OR ";
            isFirst = false;
        }

        where += "(parent='" + costCenterId + "' AND period="
                + QString::number(periodNo) + ")";
    }

    delete iter;
    mCcSumList->dbReadWhere(ACC_MODELFACTORY->getDatabase(), where);
    return true;
}

/**
 * @brief Update cost center summary list
 * @param glTransList
 */
bool ACC_PostGlTransaction::updateCcSumList(RB_ObjectContainer* glTransList) {
    if (!glTransList || !mCcSumList) {
        RB_DEBUG->error("ACC_PostGlTransaction::updateCcSumList() "
                        "NULL ERROR");
        return false;
    }

    RB_ObjectIterator* iter = glTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        // Get relevant GL summary
        RB_ObjectBase* glTrans = iter->currentObject();
        RB_ObjectBase* targetCcSum = nullptr;
        RB_ObjectIterator* sumIter = mCcSumList->createIterator();

        for (sumIter->first(); !sumIter->isDone(); sumIter->next()) {
            RB_ObjectBase* ccSum = iter->currentObject();

            if (glTrans->getIdValue("chartmaster_idx")
                    == ccSum->getValue("parent")
                    && glTrans->getIdValue("periodno")
                    == ccSum->getValue("periodno")) {
                targetCcSum = ccSum;
                sumIter->setDone();
            }

        }

        delete iter;

        // Create GL summary if not already existing
        if (!targetCcSum) {
            // No chartdetail yet available, create new
            targetCcSum = mCcSumList->newObject();

            QString centerId = glTrans->getIdValue("costcenter_idx").toString();
            int periodNo = glTrans->getValue("periodno").toInt();
            QString centerCode = "0";
            QString centerName = "Default";
            RB_ObjectBase* ccObj = ACC_QACOSTCENTER->getCostCenterObj(centerId);

            if (ccObj) {
                centerCode = ccObj->getValue("centercode").toString();
                centerName = ccObj->getValue("centername").toString();
            }

            targetCcSum->setValue("parent", centerId);
            targetCcSum->setValue("period", periodNo);
            targetCcSum->setValue("centercode", centerCode);
            targetCcSum->setValue("centername", centerName);
            targetCcSum->setValue("debit", 0.0);
            targetCcSum->setValue("credit", 0.0);
        }

        // Update the summary amount
        double debitAmt = targetCcSum->getValue("debit").toDouble();
        double creditAmt = targetCcSum->getValue("credit").toDouble();
        double amount = glTrans->getValue("amount").toDouble();

        // update amount
        if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
            if (amount > 0) {
                debitAmt += amount;
            } else {
                creditAmt -= amount;
            }
        } else {
            if (amount > 0) {
                debitAmt -= amount;
            } else {
                creditAmt += amount;
            }
        }

        targetCcSum->setValue("debit", debitAmt);
        targetCcSum->setValue("credit", creditAmt);
    }

    delete iter;
    return true;
}

/**
 * Set amount of ACC_CostSum and, if not exists, create ACC_CostSum record
 * helper function of createCcSumList()
 * @param glTrans GL transaction
 */
/*
void ACC_PostGlTransaction::setCcSumAmount(RB_ObjectBase* glTrans) {
    RB_String costCenterId = glTrans->getIdValue("costcenter_idx").toString();
    int periodNo = glTrans->getValue("periodno").toInt();
    RB_ObjectBase* costSum = NULL;

    RB_String qStr = "";
    qStr = "SELECT id FROM acc_costsum WHERE parent='"
           + costCenterId + "' AND period=" + RB_String::number(periodNo)+ ";";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) {
            RB_String centerCode = "0";
            RB_String centerName = "Default";
            qStr = "SELECT centercode, centername FROM acc_costcenter WHERE id='"
                   + costCenterId + "';";
            if (query.exec(qStr) && query.first()) {
                centerCode = query.value(0).toString();
                centerName = query.value(1).toString();
            }

            // Find costSum if already created
            RB_ObjectIterator* iter = mCcSumList->createIterator();
            for (iter->first(); !iter->isDone() && !costSum; iter->next()) {
                RB_ObjectBase* obj = iter->currentObject();

                if (obj->getValue("parent").toString() == costCenterId
                        && obj->getValue("period").toInt() == periodNo) {
                    costSum = obj;
                }
            }
            delete iter;

            // costSum not found and to be created
            if (!costSum) {
                costSum = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                     mCcSumList, "ACC_CostSum");
                costSum->setValue("parent", costCenterId);
                costSum->setValue("period", periodNo);
                costSum->setValue("centercode", centerCode);
                costSum->setValue("centername", centerName);
                costSum->setValue("debit", 0.0);
                costSum->setValue("credit", 0.0);
            }
        } else {
            RB_String id = query.value(0).toString();
            costSum = mCcSumList->getObject(id);

            if (!costSum) {
                costSum = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(),
                                                       mCcSumList, "ACC_CostSum");
                costSum->setId(id);
                costSum->dbRead(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            }
        }
    } else {
        RB_DEBUG->error("ACC_PostGlTransaction::setCcSumAmount() "
                         + query.lastError().text() + " ERROR");
        return;
    }

    if (!costSum){
        RB_DEBUG->error("ACC_PostGlTransaction::setCcSumAmount() ERROR");
        return;
    }

    double debitAmt = costSum->getValue("debit").toDouble();
    double creditAmt = costSum->getValue("credit").toDouble();
    double amount = glTrans->getValue("amount").toDouble();

    // update amount
    if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
        if (amount > 0) {
            debitAmt += amount;
        } else {
            creditAmt -= amount;
        }
    } else {
        if (amount > 0) {
            debitAmt -= amount;
        } else {
            creditAmt += amount;
        }
    }

    costSum->setValue("debit", debitAmt);
    costSum->setValue("credit", creditAmt);
}
*/
/**
 * Post cost center summary list, updated with deleted
 * and/or added ACC_GlTtrans
 * @returns true on success
 */
/*
bool ACC_PostGlTransaction::postCcSumList() {
    if (mCcSumList) {
        return mCcSumList->dbUpdateList(ACC_MODELFACTORY->getDatabase(),
                                                RB2::ResolveOne);
    }

    return false;
}
*/

/**
 * Set debit/credit amount of ACC_CostSum and,
 * if not exists, create ACC_CostSum record
 * @param glTrans GL transaction
 */
/*
void ACC_PostGlTransaction::setCcDebitCredit(RB_ObjectBase *glTrans) {
    RB_String costCenterId = glTrans->getIdValue("costcenter_idx").toString();
    int periodNo = glTrans->getValue("periodno").toInt();
    mCcDebitAmt = 0.0;
    mCcCreditAmt = 0.0;

    // get existing amounts
    RB_String qStr = "";
    qStr = "SELECT debit, credit FROM acc_costsum WHERE parent='"
           + costCenterId + "' AND period=" + RB_String::number(periodNo)+ ";";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) {
            RB_String centerCode = "0";
            RB_String centerName = "Default";
            qStr = "SELECT centercode, centername FROM acc_costcenter WHERE id='"
                   + costCenterId + "';";
            if (query.exec(qStr) && query.first()) {
                centerCode = query.value(0).toString();
                centerName = query.value(1).toString();
            }

            // No chartdetail yet available, create new
            ACC_CostSum* costSum = new ACC_CostSum(RB_Uuid::createUuid().toString(),
                                                   NULL, "ACC_CostSum");
            costSum->setValue("parent", costCenterId);
            costSum->setValue("period", periodNo);
            costSum->setValue("centercode", centerCode);
            costSum->setValue("centername", centerName);
            costSum->setValue("debit", 0.0);
            costSum->setValue("credit", 0.0);
            costSum->dbUpdate(ACC_MODELFACTORY->getDatabase());
            delete costSum;
        } else {
            mCcDebitAmt = query.value(0).toDouble();
            mCcCreditAmt = query.value(1).toDouble();
        }
    } else {
        RB_DEBUG->error("ACC_PostGlTransaction::setCcDebitCredit() "
                         + query.lastError().text() + " ERROR");
    }

    // update amount
    if (!glTrans->getFlag(RB2::FlagIsDeleted)) {
        double amount = glTrans->getValue("amount").toDouble();
        if (amount > 0) {
            mCcDebitAmt += amount;
        } else {
            mCcCreditAmt -= amount;
        }
    } else {
        double amount = glTrans->getValue("amount").toDouble();
        if (amount > 0) {
            mCcDebitAmt -= amount;
        } else {
            mCcCreditAmt += amount;
        }
    }

}
*/
/**
 * Post (or unpost) the GL transaction to the cost center summary per month
 * @param glTrans GL transaction
 * @returns true on success
 */
/*
bool ACC_PostGlTransaction::postCc(RB_ObjectBase* glTrans) {
    RB_String qStr = "";
    qStr = "UPDATE acc_costsum SET debit=" + RB_String::number(mCcDebitAmt, 'f', 6)
           + ", credit=" + RB_String::number(mCcCreditAmt, 'f', 6)
           + " WHERE period=" + glTrans->getValue("periodno").toString()
           + " AND  parent='" + glTrans->getIdValue("costcenter_idx").toString() + "';";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_PostGlTransaction::postCc() "
                         + query.lastError().text() + " ERROR");
        return false;
    }

    return true;

}
*/
/**
 * Recreate GL summary and cost center summary in case of emergency,
 * @param fromPrd from period
 * @param toPrd up to and including period
 * @return true on success
 */
bool ACC_PostGlTransaction::recreate(int fromPrd, int toPrd) {
    bool success = recreateGlSum(fromPrd, toPrd);
    return success && recreateCcSum(fromPrd, toPrd);
}

/**
 * Recreate GL summary in case of emergency,
 * TODO: handle in inner join queries instead of individual per account
 * @param fromPrd from period
 * @param toPrd up to and including period
 * @return true on success
 */
bool ACC_PostGlTransaction::recreateGlSum(int fromPrd, int toPrd) {
    RB_ObjectContainer* chartMasterList = ACC_QACHARTMASTER->getAcctList();

    ACC_SqlCommonFunctions util;
    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    // Recreate glSum in this period
    //
    ACC_GlSum* glSum = new ACC_GlSum("", NULL, "ACC_GlSum");
    RB_ObjectIterator* iter = chartMasterList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* chartMaster = iter->currentObject();

        for (int i = fromPrd; i < toPrd + 1; ++i) {
            qStr = "SELECT SUM(amount) FROM acc_gltrans WHERE ";
            util.substrIdxId("chartmaster_idx", qStr);
            qStr += "='"
                   + chartMaster->getId() + "' AND periodno=" + RB_String::number(i)
                   + " AND amount>=0;";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreate() 21 ERROR");
               return false;
            }

            query.first();
            double debit = query.value(0).toDouble();

            qStr = "SELECT SUM(amount) FROM acc_gltrans WHERE ";
            util.substrIdxId("chartmaster_idx", qStr);
            qStr += "='"
                   + chartMaster->getId() + "' AND periodno=" + RB_String::number(i)
                   + " AND amount<0;";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreate() 3 ERROR");
               return false;
            }

            query.first();
            double credit = -query.value(0).toDouble();

            // Set new glSum
            glSum->setValue("id", RB_Uuid::createUuid().toString());
            glSum->setValue("parent", chartMaster->getId());
            glSum->setValue("period", i);
            glSum->setValue("accountcode", chartMaster->getValue("accountcode").toInt());
            glSum->setValue("accountname", chartMaster->getValue("accountname").toString());
            glSum->setValue("debit", debit);
            glSum->setValue("credit", credit);
            glSum->deleteFlag(RB2::FlagFromDatabase);

            qStr = "DELETE FROM acc_glsum WHERE parent='"
                  + chartMaster->getId()
                  + "' AND period=" + RB_String::number(i)+ ";";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreate() 4 ERROR");
               return false;
            }

            // Update new glSum, if relevant
            if (std::fabs(debit) > 0.005 || std::fabs(credit) > 0.005) {
                glSum->dbUpdate(ACC_MODELFACTORY->getDatabase());
            }

            if (i % 100 == 12) {
                i += 87; // only first twelve (months)
            }
        }
    }

    delete glSum;
    delete iter;
    // Note: do not delete chartMasterList
    return true;
}

/**
 * Recreate cost center sum in case of emergency,
 * TODO: handle in inner join queries instead of individual per account
 * @param fromPrd from period
 * @param toPrd up to and including period
 * @return true on success
 */
bool ACC_PostGlTransaction::recreateCcSum(int fromPrd, int toPrd) {
    RB_ObjectContainer* costCenterList =
           new RB_ObjectContainer("", NULL, "ACC_CcList", ACC_OBJECTFACTORY);
    costCenterList->setValue("parent", ACC_MODELFACTORY->getRootId());
    costCenterList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne);

    ACC_SqlCommonFunctions util;
    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());


    // Recreate costSum in this period
    //
    ACC_CostSum* costSum = new ACC_CostSum("", NULL, "ACC_CostSum");
    RB_ObjectIterator* iter = costCenterList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* costCenter = iter->currentObject();

        for (int i = fromPrd; i < toPrd + 1; ++i) {
            qStr = "SELECT SUM(amount) FROM acc_gltrans WHERE ";
            util.substrIdxId("costcenter_idx", qStr);
            qStr += "='"
                   + costCenter->getId() + "' AND periodno=" + RB_String::number(i)
                   + " AND amount>=0;";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreateCcSum() 21 ERROR");
               return false;
            }

            query.first();
            double debit = query.value(0).toDouble();

            qStr = "SELECT SUM(amount) FROM acc_gltrans WHERE ";
            util.substrIdxId("costcenter_idx", qStr);
            qStr += "='"
                   + costCenter->getId() + "' AND periodno=" + RB_String::number(i)
                   + " AND amount<0;";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreateCcSum() 3 ERROR");
               return false;
            }

            query.first();
            double credit = -query.value(0).toDouble();

            // Set new costSum
            costSum->setValue("id", RB_Uuid::createUuid().toString());
            costSum->setValue("parent", costCenter->getId());
            costSum->setValue("period", i);
            costSum->setValue("centercode", costCenter->getValue("centercode").toString());
            costSum->setValue("centername", costCenter->getValue("centername").toString());
            costSum->setValue("debit", debit);
            costSum->setValue("credit", credit);
            costSum->deleteFlag(RB2::FlagFromDatabase);

            qStr = "DELETE FROM acc_costsum WHERE parent='"
                  + costCenter->getId()
                  + "' AND period=" + RB_String::number(i)+ ";";

            if (!query.exec(qStr)) {
               RB_DEBUG->error("ACC_PostGlTransaction::recreateCcSum() 4 ERROR");
               return false;
            }

            // Update new costSum, if relevant
            if (std::fabs(debit) > 0.005 || std::fabs(credit) > 0.005) {
                costSum->dbUpdate(ACC_MODELFACTORY->getDatabase());
            }

            if (i % 100 == 12) {
                i += 87; // only first twelve (months)
            }
        }
    }

    delete costSum;
    delete iter;
    delete costCenterList;

    return true;
}
