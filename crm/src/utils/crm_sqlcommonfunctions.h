/*****************************************************************
 * $Id: crm_sqlcommonfunctions.h 2199 2014-12-08 21:41:10Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Customer Relation Management (CRM) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SQLCOMMONFUNCTIONS_H
#define CRM_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"
#include "rb_variant.h"

class QSqlQuery;


/**
 * Class with common sql functions for getting and setting frequently used data
 * such as document ID
 */
class CRM_SqlCommonFunctions : public RB_SqlCommonFunctions {

public:
    CRM_SqlCommonFunctions();
    virtual ~CRM_SqlCommonFunctions();

    virtual void getTargetList(QSqlQuery& query, const RB_String& groupId);
    virtual void getTemplateId(QSqlQuery& query, const RB_String& targetId);

    using RB_SqlCommonFunctions::getLastUsedSystemSeqNumber;
    using RB_SqlCommonFunctions::setLastSystemSeqNumber;
    using RB_SqlCommonFunctions::getNextSystemSeqNumber;
    virtual int getLastUsedSystemSeqNumber(int modelType,
                                           const RB_String& modelSubTypeId = "");
    virtual bool setLastSystemSeqNumber(int modelType,
                                        int lastNumber,
                                        const RB_String& modelSubTypeId = "");
    virtual int getNextSystemSeqNumber(int modelType,
                                       const RB_String& seqNumberName,
                                       const RB_String& modelSubTypeId = "");
/*
    virtual RB_Variant selectFromWhere(const RB_String& field,
                                       const RB_String& table,
                                       const RB_String& whereStr);
*/
};
#endif // CRM_SQLCOMMONFUNCTIONS_H
