/*****************************************************************
 * $Id: rb_utilityfactory.cpp 2215 2015-01-31 14:37:26Z rutger $
 * Created: Jan 27, 2011 6:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_utilityfactory.h"

#include "rb_debug.h"
#include "rb_utility.h"

//(From: http://etutorials.org/Programming/secure+programming/Chapter+12.+Anti-Tampering/12.11+Hiding+Strings/)
#define A(c)            (c) - 0x17
#define UNHIDE_STR(str) do { char *p = str;  while (*p) *p++ += 0x17; } while (0)
#define HIDE_STR(str)   do { char *p = str;  while (*p) *p++ -= 0x17; } while (0)

/**
 * Constructor
 */
RB_UtilityFactory::RB_UtilityFactory() {
    RB_DEBUG->print("RB_UtilityFactory::RB_UtilityFactory()");

    mIsEncryption = false;

    char charStr[] = { // ;lksdohe039hsl*
      A(';'), A('l'), A('k'), A('s'), A('d'), A('o'), A('h'), A('e'),
      A('0'), A('3'), A('9'), A('h'), A('s'), A('l'), A('*'), 0
    };

    UNHIDE_STR(charStr);
    if(mAes.init(RB_String(charStr))) {
        mIsEncryption = true;
    }
    HIDE_STR(charStr);
}

/**
 * Destructor
 */
RB_UtilityFactory::~RB_UtilityFactory() {
    RB_DEBUG->print("RB_UtilityFactory::~RB_UtilityFactory() OK");
}

QString RB_UtilityFactory::encrypt(const QString& str) {
    if (mIsEncryption) {
        return mAes.encrypt(str);
    } else {
        RB_DEBUG->error("RB_UtilityFactory::encrypt() no encryption ERROR");
    }

    return str;
}

QString RB_UtilityFactory::decrypt(const QVariant& var) {
    if (mIsEncryption) {
        return mAes.decrypt(var.toByteArray());
    } else {
        RB_DEBUG->error("RB_UtilityFactory::decrypt() no encryption ERROR");
    }

    return var.toString();
}

void RB_UtilityFactory::addUtility(RB_Utility* util) {
    mUtilityVector.push_back(util);
}

void RB_UtilityFactory::removeUtility(RB_Utility* util) {
    std::vector<RB_Utility*>::iterator iter
            = std::remove(mUtilityVector.begin(), mUtilityVector.end(), util);
    mUtilityVector.erase(iter);
}

void RB_UtilityFactory::refresh() {
    std::vector<RB_Utility*>::iterator iter;
    iter = mUtilityVector.begin();

    while (iter != mUtilityVector.end()) {
        RB_Utility* util = *iter;
        util->refresh();
        ++iter;
    }
}


