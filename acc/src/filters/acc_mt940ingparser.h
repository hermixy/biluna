/*****************************************************************
 * $Id: acc_mt940ingparser.h 1795 2012-10-24 17:53:54Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MT940INGPARSER_H
#define ACC_MT940INGPARSER_H

#include "acc_mt940bankparser.h"
#include "rb_string.h"
#include "rb_stringlist.h"
class RB_ObjectContainer;

/**
 * MT940 file format parser for ING
 * Based on Ing_Engine_mt940_banking_parser, ing.php
 */
class ACC_MT940IngParser : public ACC_MT940BankParser {

public:
    ACC_MT940IngParser();
    virtual ~ACC_MT940IngParser();

protected:
    RB_String parseStatementBank(const RB_String& statementData);

};

#endif // ACC_MT940INGPARSER_H
