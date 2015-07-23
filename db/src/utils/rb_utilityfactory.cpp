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

// HIDDEN

/**
 * Constructor
 */
RB_UtilityFactory::RB_UtilityFactory() {
    RB_DEBUG->print("RB_UtilityFactory::RB_UtilityFactory()");

    mIsEncryption = false;

	// HIDDEN
	
    mAes.init(RB_String("Replace this string with your own salt string"));
    mIsEncryption = true;
}

/**
 * Destructor
 */
RB_UtilityFactory::~RB_UtilityFactory() {
    // nothing
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
