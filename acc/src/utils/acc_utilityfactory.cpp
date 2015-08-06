/*****************************************************************
 * $Id: acc_utilityfactory.cpp 1621 2012-04-19 21:34:17Z rutger $
 * Created: Jan 26, 2011 5:34:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_utilityfactory.h"

#include "rb_debug.h"
#include "rb_utility.h"


ACC_UtilityFactory* ACC_UtilityFactory::mActiveFactory = 0;


/**
 * Constructor
 */
ACC_UtilityFactory::ACC_UtilityFactory() : RB_UtilityFactory() {
    RB_DEBUG->print("ACC_UtilityFactory::ACC_UtilityFactory()");
    DB_UTILITYFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_UtilityFactory::~ACC_UtilityFactory() {
    // Here and not in base class, otherwise mActiveFactory is already NULL
    while (!mUtilityVector.empty()) {
        RB_Utility* f = mUtilityVector.back();
        delete f; // deletes utility and removes, therefor no pop_back()
    }

    DB_UTILITYFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
    RB_DEBUG->print("ACC_UtilityFactory::~ACC_UtilityFactory() OK");
}

/**
 * Create instance of factory
 */
ACC_UtilityFactory* ACC_UtilityFactory::getInstance() {
    if (!mActiveFactory) {
        mActiveFactory = new ACC_UtilityFactory();
    }
    return mActiveFactory;
}
