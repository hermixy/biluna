/*****************************************************************
 * $Id: acc_mt940ingparser.cpp 1795 2012-10-24 17:53:54Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_mt940ingparser.h"


ACC_MT940IngParser::ACC_MT940IngParser() : ACC_MT940BankParser() {
    // nothing
}


ACC_MT940IngParser::~ACC_MT940IngParser() {
    // nothing
}

/**
 * Overloaded for ING
 * @returns 'ING'
 */
RB_String ACC_MT940IngParser::parseStatementBank(const RB_String& /*statementData*/) {
    return "ING";
}
