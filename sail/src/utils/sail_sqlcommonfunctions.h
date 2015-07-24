/*****************************************************************
 * $Id: sail_sqlcommonfunctions.h 1746 2012-08-23 21:01:10Z rutger $
 * Created: Aug 15, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SQLCOMMONFUNCTIONS_H
#define SAIL_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_stringlist.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 */
class SAIL_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    SAIL_SqlCommonFunctions();
    virtual ~SAIL_SqlCommonFunctions();

    // Move to RB_SqlCommonFunctions ?
    //
    virtual RB_Variant selectFromWhereId(const RB_String& field,
                                         const RB_String& table,
                                         const RB_String& id);
    virtual bool update(const RB_String& table, const RB_String& field,
                        const RB_String& value, const RB_String& id);
    virtual RB_Variant selectFromWhere(const RB_String& field,
                                       const RB_String& table,
                                       const RB_String& whereStr);
    virtual void selectAllFromWhere(QSqlQuery& query,
                                    const RB_String& table,
                                    const RB_String& whereStr);
    // End move

    virtual void selectRouteDistanceData(QSqlQuery& query, const RB_StringList& idList);
    virtual void selectTrackDistanceData(QSqlQuery& query);
    virtual void selectRouteCoordinates(QSqlQuery& query, RB_String& parentId);

private:

};
#endif // SAIL_SQLCOMMONFUNCTIONS_H
