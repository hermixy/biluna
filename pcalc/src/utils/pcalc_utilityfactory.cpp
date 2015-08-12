/*****************************************************************
 * $Id: acc_utilityfactory.cpp 1621 2012-04-19 21:34:17Z rutger $
 * Created: Aug 11, 2015 5:34:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_utilityfactory.h"

#include "rb_debug.h"
#include "rb_utility.h"


PCALC_UtilityFactory* PCALC_UtilityFactory::mActiveFactory = 0;


/**
 * Constructor
 */
PCALC_UtilityFactory::PCALC_UtilityFactory() : RB_UtilityFactory() {
    RB_DEBUG->print("PCALC_UtilityFactory::PCALC_UtilityFactory()");
    DB_UTILITYFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
PCALC_UtilityFactory::~PCALC_UtilityFactory() {
    // Here and not in base class, otherwise mActiveFactory is already NULL
    while (!mUtilityVector.empty()) {
        RB_Utility* f = mUtilityVector.back();
        delete f; // deletes utility and removes, therefor no pop_back()
    }

    DB_UTILITYFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("PCALC_UtilityFactory::~PCALC_UtilityFactory() OK");
}

/**
 * Create instance of factory
 */
PCALC_UtilityFactory* PCALC_UtilityFactory::getInstance() {
    if (!mActiveFactory) {
        mActiveFactory = new PCALC_UtilityFactory();
    }
    return mActiveFactory;
}
