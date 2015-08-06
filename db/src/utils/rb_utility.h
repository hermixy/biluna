#ifndef RB_UTILITY_H
#define RB_UTILITY_H

#include "db_global.h"

class RB_UtilityFactory;

class DB_EXPORT RB_Utility {

public:
    RB_Utility();
    virtual ~RB_Utility();

    virtual void refresh() { }

protected:
    RB_UtilityFactory* mUtilityFactory;
};

#endif // RB_UTILITY_H
