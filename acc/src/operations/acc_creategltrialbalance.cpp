/*****************************************************************
 * $Id: acc_creategltrialbalance.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 7, 2010 10:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_creategltrialbalance.h"

#include <cmath>
#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_CreateGlTrialBalance::ACC_CreateGlTrialBalance() {

}

/**
 * Destructor
 */
ACC_CreateGlTrialBalance::~ACC_CreateGlTrialBalance() {

}

/**
 * Create the trial balance from the ACC_GlSum table
 * @param trialBalanceList container which will contain the trial balance objects
 * @return true on success
 */
bool ACC_CreateGlTrialBalance::execute(RB_ObjectBase* trialBalanceList) {
    if (!trialBalanceList) return false;

    /* weberp:
    $SQL = 'SELECT accountgroup.groupname,
            accountgroup.parentgroupname,
            accountgroup.pandl,
            chartdetails.accountcode,
            chartmaster.accountname,
            Sum(CASE WHEN chartdetails.period=' . $_POST['FromPeriod'] . ' THEN chartdetails.bfwd ELSE 0 END) AS firstprdbfwd,
            Sum(CASE WHEN chartdetails.period=' . $_POST['FromPeriod'] . ' THEN chartdetails.bfwdbudget ELSE 0 END) AS firstprdbudgetbfwd,
            Sum(CASE WHEN chartdetails.period=' . $_POST['ToPeriod'] . ' THEN chartdetails.bfwd + chartdetails.actual ELSE 0 END) AS lastprdcfwd,
            Sum(CASE WHEN chartdetails.period=' . $_POST['ToPeriod'] . ' THEN chartdetails.actual ELSE 0 END) AS monthactual,
            Sum(CASE WHEN chartdetails.period=' . $_POST['ToPeriod'] . ' THEN chartdetails.budget ELSE 0 END) AS monthbudget,
            Sum(CASE WHEN chartdetails.period=' . $_POST['ToPeriod'] . ' THEN chartdetails.bfwdbudget + chartdetails.budget ELSE 0 END) AS lastprdbudgetcfwd
        FROM chartmaster INNER JOIN accountgroup ON chartmaster.group_ = accountgroup.groupname
            INNER JOIN chartdetails ON chartmaster.accountcode= chartdetails.accountcode
        GROUP BY accountgroup.groupname,
                accountgroup.parentgroupname,
                accountgroup.pandl,
                accountgroup.sequenceintb,
                chartdetails.accountcode,
                chartmaster.accountname
        ORDER BY accountgroup.pandl desc,
            accountgroup.sequenceintb,
            accountgroup.groupname,
            chartdetails.accountcode';
     */

    /* Working sql with not required account section
    SELECT acc_accountsection.sectionid, acc_accountsection.sectionname,
        acc_accountgroup.groupname, acc_accountgroup.pandl,
        acc_glsum.accountcode, acc_chartmaster.accountname,
        Sum(CASE WHEN acc_glsum.period>201008 AND acc_glsum.period<201011 THEN acc_glsum.debit ELSE 0 END) AS debit,
        Sum(CASE WHEN acc_glsum.period>201008 AND acc_glsum.period<201011 THEN acc_glsum.credit ELSE 0 END) AS credit
    FROM acc_accountsection INNER JOIN acc_accountgroup ON acc_accountsection.id=acc_accountgroup.sectioninaccounts
        INNER JOIN acc_chartmaster ON acc_chartmaster.accountgroup=acc_accountgroup.id
        INNER JOIN acc_glsum ON acc_chartmaster.id=acc_glsum.parent
    GROUP BY acc_accountgroup.id, acc_accountgroup.parent,
        acc_accountgroup.pandl, acc_accountgroup.sequenceintb,
        acc_glsum.accountcode, acc_chartmaster.accountname
    ORDER BY acc_accountgroup.pandl desc, acc_accountgroup.sequenceintb,
        acc_accountgroup.groupname, acc_glsum.accountcode;
    */

    /* Working sql for trialbalance without budget or forecast USED:
    SELECT acc_accountgroup.groupname, acc_accountgroup.pandl,
        acc_glsum.accountcode, acc_chartmaster.accountname,
        Sum(CASE WHEN acc_glsum.period>201008 AND acc_glsum.period<201011 THEN acc_glsum.debit ELSE 0 END) AS debit,
        Sum(CASE WHEN acc_glsum.period>201008 AND acc_glsum.period<201011 THEN acc_glsum.credit ELSE 0 END) AS credit
    FROM acc_accountgroup INNER JOIN acc_chartmaster ON acc_chartmaster.accountgroup=acc_accountgroup.id
        INNER JOIN acc_glsum ON acc_chartmaster.id=acc_glsum.parent
    GROUP BY acc_accountgroup.id, acc_accountgroup.parent,
        acc_accountgroup.pandl, acc_accountgroup.sequenceintb,
        acc_glsum.accountcode, acc_chartmaster.accountname
    ORDER BY acc_accountgroup.pandl, acc_accountgroup.sequenceintb,
        acc_accountgroup.groupname, acc_glsum.accountcode;
    */

    /* Working sql for gltrans balance
    SELECT Sum(CASE WHEN acc_gltrans.amount>0 THEN acc_gltrans.amount ELSE 0 END) AS debit,
        Sum(CASE WHEN acc_gltrans.amount<0 THEN -acc_gltrans.amount ELSE 0 END) AS credit
    FROM acc_gltrans;
    */

    /* Working sql for chartdetails balance
    SELECT Sum(acc_glsum.debit) AS chartdetails_debit,
        Sum(acc_glsum.credit) AS chartdetails_credit
    FROM acc_glsum;
    */

    RB_String qStr = "";
    qStr = "SELECT acc_accountgroup.groupname, acc_accountgroup.pandl, "
                "acc_glsum.accountcode, acc_chartmaster.accountname, "
                "SUM(CASE WHEN acc_glsum.period>=" + RB_String::number(mFromPrd)
                + " AND acc_glsum.period<=" + RB_String::number(mToPrd) + " THEN acc_glsum.debit ELSE 0 END) AS debit, "
                "SUM(CASE WHEN acc_glsum.period>=" + RB_String::number(mFromPrd)
                + " AND acc_glsum.period<=" + RB_String::number(mToPrd) + " THEN acc_glsum.credit ELSE 0 END) AS credit "
            "FROM acc_accountgroup INNER JOIN acc_chartmaster ON acc_chartmaster.accountgroup_id=acc_accountgroup.id "
                "INNER JOIN acc_glsum ON acc_chartmaster.id=acc_glsum.parent "
            "WHERE acc_chartmaster.parent='" + ACC_MODELFACTORY->getRootId() + "' "
            "GROUP BY acc_accountgroup.id, acc_accountgroup.parent, "
                "acc_accountgroup.pandl, acc_accountgroup.sequenceintb, "
                "acc_glsum.accountcode, acc_chartmaster.accountname "
            "ORDER BY acc_accountgroup.pandl, acc_accountgroup.sequenceintb, "
                "acc_accountgroup.groupname, acc_glsum.accountcode;";



    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateGlTrialBalance::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());

    }

    //  0 groupname
    //  1 pandl
    //  2 accountcode
    //  3 accountname
    //  4 sumdebit
    //  5 sumcredit
    // trialdebit
    // trialcredit
    // pandldebit
    // pandlcredit
    // finaldebit
    // finalcredit
    while (query.next()) {
        if (std::fabs(query.value(4).toDouble()) < 0.005
                && std::fabs(query.value(5).toDouble()) < 0.005) {
            // do nothing
        } else {
            RB_ObjectBase* obj = new RB_ObjectAtomic("", trialBalanceList, "ACC_TrialBalanceAccount");
            trialBalanceList->addObject(obj);
            obj->addMember(obj, "groupname", "-", query.value(0), RB2::MemberDouble);
            obj->addMember(obj, "pandl", "-", query.value(1), RB2::MemberDouble);
            obj->addMember(obj, "accountcode", "-", query.value(2), RB2::MemberDouble);
            obj->addMember(obj, "accountname", "-", query.value(3), RB2::MemberDouble);
            obj->addMember(obj, "sumdebit", "-", query.value(4), RB2::MemberDouble);
            obj->addMember(obj, "sumcredit", "-", query.value(5), RB2::MemberDouble);

            double balance = query.value(4).toDouble() - query.value(5).toDouble();

            if (balance >= 0.0) {
                obj->addMember(obj, "trialdebit", "-", balance, RB2::MemberDouble);
                obj->addMember(obj, "trialcredit", "-", 0.0, RB2::MemberDouble);
            } else {
                obj->addMember(obj, "trialdebit", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "trialcredit", "-", -balance, RB2::MemberDouble);
            }

            // Profit and loss
            if (query.value(1).toInt() > 0) {
                if (balance >= 0.0) {
                    obj->addMember(obj, "pandldebit", "-", balance, RB2::MemberDouble);
                    obj->addMember(obj, "pandlcredit", "-", 0.0, RB2::MemberDouble);
                } else {
                    obj->addMember(obj, "pandldebit", "-", 0.0, RB2::MemberDouble);
                    obj->addMember(obj, "pandlcredit", "-", -balance, RB2::MemberDouble);
                }
            } else {
                obj->addMember(obj, "pandldebit", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "pandlcredit", "-", 0.0, RB2::MemberDouble);
            }

            // Final balance
            if (query.value(1).toInt() < 1) {
                if (balance >= 0.0) {
                    obj->addMember(obj, "baldebit", "-", balance, RB2::MemberDouble);
                    obj->addMember(obj, "balcredit", "-", 0.0, RB2::MemberDouble);
                } else {
                    obj->addMember(obj, "baldebit", "-", 0.0, RB2::MemberDouble);
                    obj->addMember(obj, "balcredit", "-", -balance, RB2::MemberDouble);
                }
            } else {
                obj->addMember(obj, "baldebit", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "balcredit", "-", 0.0, RB2::MemberDouble);
            }
        }
    }

    return true;
}

/**
 * Set period
 * @param fromPrd starting period and including (period = year * 100 * month)
 * @param toPrd ending period and including (period = year * 100 * month)
 */
void ACC_CreateGlTrialBalance::setPeriod(int fromPrd, int toPrd) {
    mFromPrd = fromPrd;
    mToPrd = toPrd;
}
