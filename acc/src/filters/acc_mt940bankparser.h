/*****************************************************************
 * $Id: acc_mt940bankparser.h 1801 2012-11-16 21:38:36Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MT940BANKPARSER_H
#define ACC_MT940BANKPARSER_H

#include "rb_objectcontainer.h"
#include "rb_string.h"
#include "rb_stringlist.h"


/**
 * Base class MT940 bank file format parser
 * Based on Engine_mt940_banking_parser, engine.php
 */
class ACC_MT940BankParser {

public:
    ACC_MT940BankParser();
    virtual ~ACC_MT940BankParser();

    virtual bool parse(const RB_String& rawData, RB_ObjectContainer *statementList);
    virtual void getMessage(RB_String& message) { message += mMessage; }

protected:
    virtual RB_StringList parseStatementData(const RB_String& rawData);
    virtual RB_StringList parseTransactionData(const RB_String& statementData);

    virtual RB_String parseStatementBank(const RB_String& statementData);
    virtual RB_String parseStatementAccount(const RB_String& statementData);
    virtual RB_String parseStatementStartDebitCredit(const RB_String& statementData);
    virtual double parseStatementStartAmount(const RB_String& statementData);
    virtual RB_String parseStatementEndDebitCredit(const RB_String& statementData);
    virtual double parseStatementEndAmount(const RB_String& statementData);
    virtual QDate parseStatementTimeStamp(const RB_String& statementData);
    virtual RB_String parseStatementNumber(const RB_String& statementData);
    virtual RB_String parseTransactionAccount(const RB_String& transactionData);
    virtual RB_String parseTransactionAccountName(const RB_String& transactionData);
    virtual RB_String parseTransactionDebitCredit(const RB_String& transactionData);
    virtual double parseTransactionAmount(const RB_String& transactionData);
    virtual RB_String parseTransactionDescription(const RB_String& transactionData);
    virtual QDate parseTransactionEntryTimeStamp(const RB_String& transactionData);
    virtual QDate parseTransactionValueTimeStamp(const RB_String& transactionData);
    virtual RB_String parseTransactionCode(const RB_String& transactionData);

    virtual RB_String sanitizeAccount(const RB_String& account);
    virtual RB_String sanitizeAccountName(const RB_String& accountName);
    virtual QDate sanitizeTimeStamp(const RB_String& timeStamp);
    virtual RB_String sanitizeDescription(const RB_String& description);
    virtual RB_String sanitizeDebitCredit(const RB_String& debitCredit);
    virtual double sanitizeAmount(const RB_String& amount);

    RB_String mMessage;
    RB_String mStatementData; // for RABO parseTransactionEntryStamp()
    RB_String mTransactionData;
    bool mIsDebitAmount;
};

#endif // ACC_MT940PARSER_H
