/*****************************************************************
 * $Id: rb_sqlcommonfunctions.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jun 23, 2012 14:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SQLCOMMONFUNCTIONS_H
#define RB_SQLCOMMONFUNCTIONS_H

#include <QSqlQuery>
#include "db_global.h"
#include "rb_string.h"
#include "rb_variant.h"


/**
 * Base class with common sql statements
 */
class DB_EXPORT RB_SqlCommonFunctions  {

public:
    RB_SqlCommonFunctions();
    virtual ~RB_SqlCommonFunctions();

    virtual void substrIdxId(const RB_String& field, RB_String& sql);
    virtual void substrIdxContent(const RB_String& field, RB_String& sql);

    bool isPerspectiveVersionExisting(const RB_String& persp, int major,
                                      int medior, int minor, int build = -1);
    void insertPerspectiveVersion(const RB_String& persp, int major,
                                  int medior, int minor, int build = -1,
                                  const RB_String& comment = "");

    virtual int getLastUsedSystemSeqNumber(const RB_String& perspectiveCode,
                                           const RB_String& parentId,
                                           int modelType,
                                           const RB_String& modelSubTypeId);
    virtual bool setLastSystemSeqNumber(const RB_String& perspectiveCode,
                                        const RB_String& parentId,
                                        int modelType,
                                        int lastNumber,
                                        const RB_String& modelSubTypeId);
    virtual int getNextSystemSeqNumber(const RB_String& perspectiveCode,
                                       const RB_String& parentId,
                                       int modelType,
                                       const RB_String& seqNumberName,
                                       const RB_String& modelSubTypeId);

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

private:
    bool createSystemSeqNumber(const RB_String& perspectiveCode,
                               const RB_String& parentId,
                               int modelType,
                               const RB_String& seqNumberName,
                               const RB_String& modelSubTypeId);

};
#endif // RB_SQLCOMMONFUNCTIONS_H
