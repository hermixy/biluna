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
    closeAllFactories();
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

/**
 * Refresh the utilities, for example after changing the project/root
 */
void ACC_UtilityFactory::refresh() {
    std::vector<RB_UtilityFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end()) {
        RB_UtilityFactory* f = *iter;
        f->refresh();
        ++iter;
    }
}

/**
 * @return factory list
 */
std::vector<RB_UtilityFactory*> ACC_UtilityFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * Register factory
 * @param f Objectfactory
 */
void ACC_UtilityFactory::registerFactory(RB_UtilityFactory* f) {
    bool exists = false;

    std::vector<RB_UtilityFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "ACC_UtilityFactory::registerFactory() "
                            "objectfactory already registered ERROR");
        }
        ++iter;
    }

    if (!exists) {
        mFactoryList.push_back(f);
    }
}

/**
 * Unregister factory
 * @param f Objectfactory
 */
void ACC_UtilityFactory::unregisterFactory(RB_UtilityFactory* f) {
    bool exists = false;

    std::vector<RB_UtilityFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        }
        ++iter;
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_UtilityFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}

/**
 * Close and delete all factories except this (DB) factory
 */
void ACC_UtilityFactory::closeAllFactories() {
    while (!mFactoryList.empty()) {
        RB_UtilityFactory* f
                = dynamic_cast<RB_UtilityFactory*>(mFactoryList.back());
        delete f; // unregisters factory and removes, therefor no pop_back()
    }
}
