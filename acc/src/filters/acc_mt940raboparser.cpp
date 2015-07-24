/*****************************************************************
 * $Id: acc_mt940raboparser.cpp 1801 2012-11-16 21:38:36Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_mt940raboparser.h"


ACC_MT940RaboParser::ACC_MT940RaboParser() : ACC_MT940BankParser() {
    // nothing
}


ACC_MT940RaboParser::~ACC_MT940RaboParser() {
    // nothing
}

/**
 * Overloaded for Rabo
 * @returns 'RABO'
 */
RB_String ACC_MT940RaboParser::parseStatementBank(const RB_String& /*statementData*/) {
    return "RABO";
}

/**
 * Overloaded for Rabo: Rabo has different way of storing account info
 * and 61 instead of 86
 * @returns from :61: account number of the transaction
 */
RB_String ACC_MT940RaboParser::parseTransactionAccount(const RB_String& transactionData) {
    QRegExp rx(":61:.{26}(.{16})");
    rx.setMinimal(true);
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeAccount(rx.cap(1));
    }

    return RB_String();
}

/**
 * Overloaded for Rabo: Rabo has different way of storing account name
 * and 61 instead of 86
 * @returns from :61: accountname of the transaction
 */
RB_String ACC_MT940RaboParser::parseTransactionAccountName(const RB_String& transactionData) {
    QRegExp rx(":61:.{42}(.{34})");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeAccountName(rx.cap(1));
    }

    return RB_String();
}

/**
 * Overloaded for Rabo: Rabo has different way of storing transaction
 * value timestamps (ymd) and 60F from statementData instead of 61
 * @returns from :61: the entry time stamp
 */
QDate ACC_MT940RaboParser::parseTransactionEntryTimeStamp(const RB_String& /*transactionData*/) {
    QRegExp rx(":60F:[CD]([\\d]{6})");
    rx.setMinimal(true);
    int pos = rx.indexIn(mStatementData);

    if (pos > -1) {
        return sanitizeTimeStamp(rx.cap(1));
    }

    return QDate();
}

/**
 * Overloaded for Rabo: Rabo has different way of storing transaction
 * value timestamps (ymd)
 * @returns from :61: the value time stamp.
 */
QDate ACC_MT940RaboParser::parseTransactionValueTimeStamp(const RB_String& transactionData) {
    QRegExp rx(":61:([\\d]{6})[CD]");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeTimeStamp(rx.cap(1));
    }

    return QDate();
}

/**
 * Overloaded: Rabo uses longer strings for accountnumbers
 * @returns account number as only a number
 */
RB_String ACC_MT940RaboParser::sanitizeAccount(const RB_String& account) {
    RB_String str = ACC_MT940BankParser::sanitizeAccount(account);

    if (str.count() > 20 && str.indexOf("80000") == 0) {
        return str.remove(4);
    }

    return str;
}

