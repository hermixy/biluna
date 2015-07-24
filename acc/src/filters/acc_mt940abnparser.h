/*****************************************************************
 * $Id: acc_mt940abnparser.h 1795 2012-10-24 17:53:54Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MT940ABNPARSER_H
#define ACC_MT940ABNPARSER_H

#include "acc_mt940bankparser.h"
#include "rb_string.h"
#include "rb_stringlist.h"
class RB_ObjectContainer;

/**
 * MT940 file format parser for ABN Amro
 * Based on Abn_Engine_mt940_banking_parser, abn.php
 */
class ACC_MT940AbnParser : public ACC_MT940BankParser {

public:
    ACC_MT940AbnParser();
    virtual ~ACC_MT940AbnParser();

protected:
    RB_String parseStatementBank(const RB_String& statementData);
    RB_String parseTransactionAccount(const RB_String& transactionData);
    QDate parseTransactionEntryTimeStamp(const RB_String& transactionData);

};

#endif // ACC_MT940ABNPARSER_H
