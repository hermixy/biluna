/*****************************************************************
 * $Id: acc_mt940abnparser.cpp 1803 2012-11-22 21:43:16Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_mt940abnparser.h"


ACC_MT940AbnParser::ACC_MT940AbnParser() : ACC_MT940BankParser() {
    // nothing
}


ACC_MT940AbnParser::~ACC_MT940AbnParser() {
    // nothing
}

/**
 * Overloaded for ABN
 * @returns 'ABN'
 */
RB_String ACC_MT940AbnParser::parseStatementBank(const RB_String& /*statementData*/) {
    return "ABN";
}

/**
 * Overloaded for ABN, ABN Amro shows GIRO
 * includes fix for 'for GIRO 1234567 TEST 201009063689 CLIEOP 21-9' and translates that into 1234567
 * @returns from :86: account number of the transaction
 */
RB_String ACC_MT940AbnParser::parseTransactionAccount(const RB_String& transactionData) {
    RB_String str = ACC_MT940BankParser::parseTransactionAccount(transactionData);

    if (str.isEmpty() && transactionData.contains(":86:GIRO")) {
        QRegExp rx(":86:(.{13})", Qt::CaseInsensitive);
        int pos = rx.indexIn(transactionData);

        if (pos > -1) {
            return sanitizeAccount(rx.cap(1));
        }
    }

    return str;
}

/**
 * Overloaded for ABN, ABN Amro uses the :61: date-part of the field
 * for two values: Valuetimestamp (yyMMdd) and Entry date (book date) (MMdd)
 * The entry date is equal or larger than the value date.
 * @returns from :86: account number of the transaction
 */
QDate ACC_MT940AbnParser::parseTransactionEntryTimeStamp(const RB_String& transactionData) {
    QRegExp rx(":61:(\\d{6})(\\d{4})[CD]");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        QDate valueDate = sanitizeTimeStamp(rx.cap(1));
        RB_String century = RB_String::number((int)valueDate.year()); // 2012
        RB_String entryStr = century + rx.cap(2); // actual entry date
        QDate entryDate = QDate::fromString(entryStr, "yyyyMMdd");

        if (entryDate >= valueDate) {
            return entryDate;
        } else {
            // for example entry should be:(2013)-01-01 and value: 2012-12-31
            return entryDate.addYears(+1);
        }
    }

    return QDate();
}


