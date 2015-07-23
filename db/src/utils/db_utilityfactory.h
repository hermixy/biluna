/*****************************************************************
 * $Id: db_utilityfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 27, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_UTILITYFACTORY_H
#define DB_UTILITYFACTORY_H

#include <vector>
#include "rb_utilityfactory.h"

#define DB_UTILITYFACTORY DB_UtilityFactory::getInstance()

/**
 * DB and base utility factory
 */
class DB_EXPORT DB_UtilityFactory : public RB_UtilityFactory {

public:
    virtual ~DB_UtilityFactory();
    static DB_UtilityFactory* getInstance();

    virtual std::vector<RB_UtilityFactory*> getFactoryList();
    virtual void registerFactory(RB_UtilityFactory* factory);
    virtual void unregisterFactory(RB_UtilityFactory* factory);
    virtual void closeAllFactories();

private:
    DB_UtilityFactory();

    //! Unique factory instance
    static DB_UtilityFactory* mActiveFactory;
    //! List of all currently created object factories
    std::vector<RB_UtilityFactory*> mFactoryList;
};

#endif // DB_UTILITYFACTORY_H
