/*****************************************************************
 * $Id: acc_mt940raboparser.h 1795 2012-10-24 17:53:54Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MT940RABOPARSER_H
#define ACC_MT940RABOPARSER_H

#include "acc_mt940bankparser.h"
#include "rb_string.h"
#include "rb_stringlist.h"
class RB_ObjectContainer;

/**
 * MT940 file format parser for Rabo
 * Based on Rabo_Engine_mt940_banking_parser, rabo.php
 */
class ACC_MT940RaboParser : public ACC_MT940BankParser {

public:
    ACC_MT940RaboParser();
    virtual ~ACC_MT940RaboParser();

protected:
    virtual RB_String parseStatementBank(const RB_String& statementData);
    virtual RB_String parseTransactionAccount(const RB_String& transactionData);
    virtual RB_String parseTransactionAccountName(const RB_String& transactionData);
    virtual QDate parseTransactionEntryTimeStamp(const RB_String& transactionData);
    virtual QDate parseTransactionValueTimeStamp(const RB_String& transactionData);

    RB_String sanitizeAccount(const RB_String& account);

};

#endif // ACC_MT940RABOPARSER_H
