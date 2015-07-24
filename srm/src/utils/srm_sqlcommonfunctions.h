/*****************************************************************
 * $Id: srm_sqlcommonfunctions.h 2073 2014-01-25 17:07:56Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Customer Relation Management (CRM) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SQLCOMMONFUNCTIONS_H
#define SRM_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 * such as document ID
 */
class SRM_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    SRM_SqlCommonFunctions();
    virtual ~SRM_SqlCommonFunctions();

    virtual void getTargetList(QSqlQuery& query, const RB_String& groupId);
    virtual void getTemplateId(QSqlQuery& query, const RB_String& targetId);

    virtual int getLastUsedSystemSeqNumber(int modelType,
                                           const RB_String& modelSubTypeId = "");
    virtual bool setLastSystemSeqNumber(int modelType,
                                        int lastNumber,
                                        const RB_String& modelSubTypeId = "");
    virtual int getNextSystemSeqNumber(int modelType,
                                       const RB_String& seqNumberName,
                                       const RB_String& modelSubTypeId = "");

    virtual RB_Variant selectFromWhere(const RB_String& field,
                                       const RB_String& table,
                                       const RB_String& whereStr);

};
#endif // SRM_SQLCOMMONFUNCTIONS_H
