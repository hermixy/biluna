/*****************************************************************
 * $Id: peng_sqlcommonfunctions.h 1836 2012-12-17 16:29:14Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Plant Engineering (PENG) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_SQLCOMMONFUNCTIONS_H
#define PENG_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_stringlist.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 * such as document ID
 */
class PENG_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    PENG_SqlCommonFunctions();
    virtual ~PENG_SqlCommonFunctions();

    virtual void getPedEqList(QSqlQuery& query, const QStringList& eqIdList);
    virtual void getPedLineList(QSqlQuery& query, const QStringList& lineIdList);

    virtual bool isDefaultRow();

    virtual RB_Variant selectFromWhere(const RB_String& field,
                                       const RB_String& table,
                                       const RB_String& whereStr);


};
#endif // PENG_SQLCOMMONFUNCTIONS_H
