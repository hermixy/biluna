/*****************************************************************
 * $Id: db_utilityfactory.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 27, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_utilityfactory.h"

#include "rb_debug.h"


DB_UtilityFactory* DB_UtilityFactory::mActiveFactory = NULL;


/**
 * Constructor
 */
DB_UtilityFactory::DB_UtilityFactory() : RB_UtilityFactory() {
    RB_DEBUG->print("DB_UtilityFactory::DB_UtilityFactory()");
    // nothing
}

/**
 * Destructor
 */
DB_UtilityFactory::~DB_UtilityFactory() {
    closeAllFactories();
    mActiveFactory = NULL;
    RB_DEBUG->print("DB_UtilityFactory::~DB_UtilityFactory() OK");
}

/**
 * Create instance of factory
 */
DB_UtilityFactory* DB_UtilityFactory::getInstance() {
    if (!mActiveFactory) {
        mActiveFactory = new DB_UtilityFactory();
    }
    return mActiveFactory;
}

/**
 * @return factory list
 */
std::vector<RB_UtilityFactory*> DB_UtilityFactory::getFactoryList() {
    return mFactoryList;
}

/**
 * Register factory
 * @param f Objectfactory
 */
void DB_UtilityFactory::registerFactory(RB_UtilityFactory* f) {
    bool exists = false;

    std::vector<RB_UtilityFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (iter != mFactoryList.end() && !exists) {
        if(f == *iter) {
            exists = true;
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_UtilityFactory::registerFactory() "
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
void DB_UtilityFactory::unregisterFactory(RB_UtilityFactory* f) {
    bool exists = false;

    std::vector<RB_UtilityFactory*>::iterator iter;
    iter = mFactoryList.begin();
    while (!exists && iter != mFactoryList.end()) {
        if (f == *iter) {
            mFactoryList.erase(iter);
            exists = true;
        } else {
            ++iter;
        }
    }

    if (!exists) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_UtilityFactory::unregisterFactory() "
                        "objectfactory was not registered ERROR");
    }
}

/**
 * Close and delete all factories except this (DB) factory
 */
void DB_UtilityFactory::closeAllFactories() {
    while (!mFactoryList.empty()) {
        RB_UtilityFactory* f
                = dynamic_cast<RB_UtilityFactory*>(mFactoryList.back());
        delete f; // unregisters factory and removes, therefor no pop_back()
    }
}
