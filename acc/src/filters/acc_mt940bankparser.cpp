/*****************************************************************
 * $Id: acc_mt940bankparser.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_mt940bankparser.h"

#include "acc_bankimport.h"


ACC_MT940BankParser::ACC_MT940BankParser () {
    mMessage = "";
    mStatementData = "";
    mTransactionData = "";
    mIsDebitAmount = false;

}


ACC_MT940BankParser::~ACC_MT940BankParser() {
    // nothing
}

/**
 * Parse raw data to object model
 * @param rawData raw data from bank file import
 * @param statementList final statement list with transaction lists (ACC_TransDocList)
 */
bool ACC_MT940BankParser::parse(const RB_String& rawData,
                                RB_ObjectContainer* statementList) {
    RB_StringList statementItems = parseStatementData(rawData);
    RB_StringList transactionItems;
    int sCount = statementItems.count();

    RB_String statbankname;
    RB_String statactnumber;
    double statstartamount;
    double statendamount;
    QDate stattimestamp;
    RB_String statnumber;
    RB_String dummyOnly;

    for (int i = 0; i < sCount; ++i) {
        mStatementData = statementItems.at(i);

        // set the statement data
        statbankname = parseStatementBank(mStatementData);
        statactnumber = parseStatementAccount(mStatementData);
        dummyOnly = parseStatementStartDebitCredit(mStatementData);
        statstartamount = parseStatementStartAmount(mStatementData);
        dummyOnly = parseStatementEndDebitCredit(mStatementData);
        statendamount = parseStatementEndAmount(mStatementData);
        stattimestamp = parseStatementTimeStamp(mStatementData);
        statnumber = parseStatementNumber(mStatementData);

        transactionItems.clear();
        transactionItems = parseTransactionData(mStatementData);
        int tCount = transactionItems.count();

        for (int j = 0; j < tCount; ++j) {
            mTransactionData = transactionItems.at(j);

            ACC_BankImport* obj = new ACC_BankImport("", statementList, "ACC_BankImport");

            obj->setValue("importmessage", "-"); // at import check
            obj->setValue("statbankname", statbankname);
            obj->setValue("statactnumber", statactnumber);
            obj->setValue("statstartamount", statstartamount);
            obj->setValue("statendamount", statendamount);
            obj->setValue("stattimestamp", stattimestamp);
            obj->setValue("statnumber", statnumber);
            obj->setValue("transaccount", parseTransactionAccountName(mTransactionData));
            obj->setValue("transactnumber", parseTransactionAccount(mTransactionData)); // account actual number
            obj->setValue("transamount", parseTransactionAmount(mTransactionData));
            obj->setValue("transdebitcredit", parseTransactionDebitCredit(mTransactionData));
            obj->setValue("transdescription", parseTransactionDescription(mTransactionData));
            obj->setValue("transentrytimestamp", parseTransactionEntryTimeStamp(mTransactionData));
            obj->setValue("transvaluetimestamp", parseTransactionValueTimeStamp(mTransactionData));
            obj->setValue("transcode", parseTransactionCode(mTransactionData));

            statementList->addObject(obj);
        }
    }

    return true;
}

/**
 * @returns a list of statement lines
 */
RB_StringList ACC_MT940BankParser::parseStatementData(const RB_String& rawData) {
    QRegExp rx(":20:(.*)(:62F:)(.*)(?=\\n)"); // catch one :62F: before new line
    rx.setMinimal(true);
    RB_StringList strL;
    int pos = 0;

    while ((pos = rx.indexIn(rawData, pos)) != -1) {
        strL << (":20:" + rx.cap(1) + rx.cap(2) + rx.cap(3)); // no cap(4) due to ?=
        pos += rx.matchedLength();
    }

    return strL;
}

/**
 * @returns list of transaction lines
 */
RB_StringList ACC_MT940BankParser::parseTransactionData(const RB_String& statementData) {
    QRegExp rx(":61:(.*:86:)+(.*)(?=:61:|:62F:)"); // catch one or more :86: before :61: or :62F:
    rx.setMinimal(true);
    RB_StringList strL;
    int pos = 0;

    while ((pos = rx.indexIn(statementData, pos)) != -1) {
        strL << (":61:" + rx.cap(1) + rx.cap(2));
        pos += rx.matchedLength();
    }

    return strL;
}

/**
 * @returns actual raw data string, returns 'Unkown' here
 */
RB_String ACC_MT940BankParser::parseStatementBank(const RB_String& /*statementData*/) {
    return "Unkown";
}

/**
 * @returns from :25: account number
 */
RB_String ACC_MT940BankParser::parseStatementAccount(const RB_String& statementData) {
    QRegExp rx(":25:([\\d\\.]+)");
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeAccount(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :60F: determin debit or credit
 */
RB_String ACC_MT940BankParser::parseStatementStartDebitCredit(const RB_String& statementData) {
    QRegExp rx(":60F:([CD])", Qt::CaseInsensitive);
    rx.setMinimal(true);
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeDebitCredit(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :60F: statement start amount
 */
double ACC_MT940BankParser::parseStatementStartAmount(const RB_String& statementData) {
    QRegExp rx(":60F:[CD]\\d+[A-Z]{3}(\\d*,\\d{0,3})"); // [A-Z]{3} is EUR
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeAmount(rx.cap(1));
    }

    return 0.0;
}

/**
 * Only used to set isDebit flag
 * @returns from :62F: determin debit or credit
 */
RB_String ACC_MT940BankParser::parseStatementEndDebitCredit(const RB_String& statementData) {
    QRegExp rx(":62F:([CD])", Qt::CaseInsensitive);
    rx.setMinimal(true);
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeDebitCredit(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :62F: statement end amount
 */
double ACC_MT940BankParser::parseStatementEndAmount(const RB_String& statementData) {
    QRegExp rx(":62F:[CD]\\d+[A-Z]{3}(\\d*,\\d{0,3})"); // [A-Z]{3} is EUR
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeAmount(rx.cap(1));
    }

    return 0.0;
}

/**
 * @returns from :60F: statement date
 */
QDate ACC_MT940BankParser::parseStatementTimeStamp(const RB_String& statementData) {
    QRegExp rx(":60F:[CD](\\d{6})");
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return sanitizeTimeStamp(rx.cap(1));
    }

    return QDate();
}

/**
 * @returns from :28C: statement number
 */
RB_String ACC_MT940BankParser::parseStatementNumber(const RB_String& statementData) {
    QRegExp rx(":28C?:(.*)(:)", Qt::CaseInsensitive);
    rx.setMinimal(true);
    int pos = rx.indexIn(statementData);

    if (pos > -1) {
        return rx.cap(1).trimmed();
    }

    return RB_String();
}

/**
 * @returns from :86: account number of the transaction
 */
RB_String ACC_MT940BankParser::parseTransactionAccount(const RB_String& transactionData) {
    QRegExp rx(":86:([\\d\\.]+)\\s", Qt::CaseInsensitive);
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeAccount(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :86: accountname of the transaction
 */
RB_String ACC_MT940BankParser::parseTransactionAccountName(const RB_String& transactionData) {
    QRegExp rx("[\\n]:86:[\\d\\.]+ (.+)");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeAccountName(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :61: determin debit or credit
 */
RB_String ACC_MT940BankParser::parseTransactionDebitCredit(const RB_String& transactionData) {
    QRegExp rx(":61:.*([CD])", Qt::CaseInsensitive);
    rx.setMinimal(true);
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeDebitCredit(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :61: amount
 */
double ACC_MT940BankParser::parseTransactionAmount(const RB_String& transactionData){
    QRegExp rx(":61:.*[CD]([\\d,\\.]+)N", Qt::CaseInsensitive);
    rx.setMinimal(true);
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeAmount(rx.cap(1));
    }

    return 0.0;
}

/**
 * @returns from :86: the description
 */
RB_String ACC_MT940BankParser::parseTransactionDescription(const RB_String& transactionData) {
    QRegExp rx("[\\n]:86:(.*)(?=\\n:|$)");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeDescription(rx.cap(1));
    }

    return RB_String();
}

/**
 * @returns from :61: the entry time stamp
 */
QDate ACC_MT940BankParser::parseTransactionEntryTimeStamp(const RB_String& transactionData) {
    QRegExp rx(":61:(\\d{6})");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeTimeStamp(rx.cap(1));
    }

    return QDate();
}

/**
 * @returns from :61: the value time stamp. Here the same as the entry time stamp
 */
QDate ACC_MT940BankParser::parseTransactionValueTimeStamp(const RB_String& transactionData) {
    QRegExp rx(":61:(\\d{6})");
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return sanitizeTimeStamp(rx.cap(1));
    }

    return QDate();
}

/**
 * @returns from :61: the bank specific transaction code
 */
RB_String ACC_MT940BankParser::parseTransactionCode(const RB_String& transactionData) {
    QRegExp rx(":61:.*N(.{3}).*");
    rx.setMinimal(true);
    int pos = rx.indexIn(transactionData);

    if (pos > -1) {
        return rx.cap(1); // 0 is whole string
    }

    return RB_String();
}

/**
 * @returns account number as only number and giro account with a 'P' in front
 */
RB_String ACC_MT940BankParser::sanitizeAccount(const RB_String& account) {
    RB_String str = account;
    str = str.replace(" ", "");
    str = str.replace(".", "");
    str = str.replace("GIRO", "");

    if (str.count() < 9 && str != "NONREF") {
        str = "P" + str;
    }

    return str;
}

/**
 * @returns account name without white space characters such as \t, \n, etc.
 */
RB_String ACC_MT940BankParser::sanitizeAccountName(const RB_String& accountName) {
    RB_String str = accountName;
    return str.simplified();
}

/**
 * @returns date from string with numbers
 */
QDate ACC_MT940BankParser::sanitizeTimeStamp(const RB_String& timeStamp) {
    // timeStamp is yyMMdd
    RB_String century = RB_String::number((int)QDate::currentDate().year() / 100);
    return QDate::fromString(century + timeStamp, "yyyyMMdd");
}

/**
 * @returns description without white space characters such as \t, \n, etc.
 */
RB_String ACC_MT940BankParser::sanitizeDescription(const RB_String& description) {
    RB_String str = description;
    str = str.remove(":86:");
    return str.simplified();
}

/**
 * Sanitize Debit D and Credit C letters
 */
RB_String ACC_MT940BankParser::sanitizeDebitCredit(const RB_String& debitCredit) {
    RB_String str = debitCredit;
    str = str.toUpper();
    str = str.left(1);
    mIsDebitAmount = (str.compare("D", Qt::CaseInsensitive) == 0);

    if (str.compare("C", Qt::CaseInsensitive) != 0
            && !mIsDebitAmount) {
        mMessage += "Error debit credit identifier";
        return "";
    }


    return str;
}

/**
 * Sanize amount from HTML markup and replace the comma by a decimal point
 * @return amount as double
 */
double ACC_MT940BankParser::sanitizeAmount(const RB_String& amount) {
    RB_String str = amount.trimmed();
    str = str.replace(QRegExp("@<[^>]*>"), ""); // remove any html markup
    str = str.replace(",", ".");
    return mIsDebitAmount ? -1.0 * str.toDouble() : str.toDouble();
}



