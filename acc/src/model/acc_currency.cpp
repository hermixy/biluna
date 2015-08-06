/*****************************************************************
 * $Id: acc_currency.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_currency.h"

ACC_Currency::ACC_Currency (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Currency::ACC_Currency(ACC_Currency* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Currency::~ACC_Currency() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - currency currency name
 * - currcode ISO4217 currency code, such as USD and EUR
 * - country country where currency is used
 * - hundredsname name of one hundredth of the currency
 * - rate exchange rate against company default currency
 */
void ACC_Currency::createMembers() {
    addMember("currency", "-", "", RB2::MemberChar20);
    //! ISO4217 Code
    addMember("currcode", "-", "", RB2::MemberChar10);
    addMember("country", "-", "", RB2::MemberChar125);
    addMember("hundredsname", "-", "Cents", RB2::MemberChar125);
    addMember("rate", "-", 0.0, RB2::MemberDouble);
}
