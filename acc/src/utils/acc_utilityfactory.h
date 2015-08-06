/*****************************************************************
 * $Id: acc_utilityfactory.h 1621 2012-04-19 21:34:17Z rutger $
 * Created: Apr 18, 2012 5:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_UTILITYFACTORY_H
#define ACC_UTILITYFACTORY_H

#include <vector>
#include "db_utilityfactory.h"


#define ACC_UTILITYFACTORY ACC_UtilityFactory::getInstance()


/**
 * Static utility factory class, container for all static utilities
 * in the ACC accounting perspective The utility classes are for
 * frequently used access to data of question/answer purposes instead
 * of querying the database continuously
 */
class ACC_UtilityFactory : public RB_UtilityFactory {

public:
    virtual ~ACC_UtilityFactory();
    static ACC_UtilityFactory* getInstance();

private:
    ACC_UtilityFactory();

    //! Unique factory instance
    static ACC_UtilityFactory* mActiveFactory;
    //! List of all currently created object factories
    std::vector<RB_UtilityFactory*> mFactoryList;

};

#endif // ACC_UTILITYFACTORY_H
