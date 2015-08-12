/*****************************************************************
 * $Id: acc_utilityfactory.h 1621 2012-04-19 21:34:17Z rutger $
 * Created: Aug 11, 2015 5:34:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_UTILITYFACTORY_H
#define PCALC_UTILITYFACTORY_H

#include <vector>
#include "db_utilityfactory.h"


#define PCALC_UTILITYFACTORY PCALC_UtilityFactory::getInstance()


/**
 * Static utility factory class, container for all static utilities
 * in the PCALC accounting perspective The utility classes are for
 * frequently used access to data of question/answer purposes instead
 * of querying the database continuously
 */
class PCALC_UtilityFactory : public RB_UtilityFactory {

public:
    virtual ~PCALC_UtilityFactory();
    static PCALC_UtilityFactory* getInstance();

private:
    PCALC_UtilityFactory();

    //! Unique factory instance
    static PCALC_UtilityFactory* mActiveFactory;
    //! List of all currently created object factories
    std::vector<RB_UtilityFactory*> mFactoryList;

};

#endif // PCALC_UTILITYFACTORY_H
