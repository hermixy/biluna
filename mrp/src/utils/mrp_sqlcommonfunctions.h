/*****************************************************************
 * $Id: sail_sqlcommonfunctions.h 1746 2012-08-23 21:01:10Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SQLCOMMONFUNCTIONS_H
#define MRP_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_stringlist.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 */
class MRP_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    MRP_SqlCommonFunctions();
    virtual ~MRP_SqlCommonFunctions();

    virtual void selectLastNameData(QSqlQuery& query,
                                    const RB_String& fieldFrom,
                                    const RB_String& fieldTo);
    virtual void selectLicenseEndData(QSqlQuery& query,
                                      const RB_String& fieldFrom,
                                      const RB_String& fieldTo);
    virtual void selectModifiedData(QSqlQuery& query,
                                    const RB_String& fieldFrom,
                                    const RB_String& fieldTo);

private:
    void selectLicenseData(QSqlQuery& query,
                           const RB_String& fieldName,
                           const RB_String& fieldFrom,
                           const RB_String& fieldTo);

};
#endif // MRP_SQLCOMMONFUNCTIONS_H
