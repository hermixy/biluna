/*****************************************************************
 * $Id: rb_dbvisitor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 7, 2011 3:49:10 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DBVISITOR_H
#define RB_DBVISITOR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "rb_visitor.h"


class RB_ObjectBase;

/**
 * Visitor class for synchronizing the objects with the database
 */
class DB_EXPORT RB_DbVisitor : public RB_Visitor {
	
public:
    RB_DbVisitor();
    virtual ~RB_DbVisitor() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    virtual void setDbRead();
    virtual void setDbUpdate();
    virtual void setDbDelete();

    virtual void setDatabase(QSqlDatabase db);
    virtual void setCalledFromList(bool isFromList);
    virtual void setUseParentId(bool useParentId);
    virtual void setIdFieldName(const QString& idFieldName);
    virtual void setUpdateNewId(bool isNewId);
    virtual void setWhereStatement(const QString& whereStatement);

protected:
    virtual bool dbRead();
    virtual bool dbUpdate();
    virtual bool dbBeforeDelete();
    virtual bool dbDelete();
    virtual bool dbReadList();
    virtual bool dbUpdateList();
    virtual bool dbDeleteList();

    void setReadQuery(QSqlQuery& q, RB_ObjectBase* obj,
                      const QString& fieldName, const QString& id);
    void prepareInsertQuery(QSqlQuery& q, RB_ObjectBase* obj);
    void prepareUpdateQuery(QSqlQuery& q, RB_ObjectBase* obj);
    void setDeleteQuery(QSqlQuery& q, RB_ObjectBase* obj,
                        const QString& fieldName, const QString& id);
    RB_ObjectBase* mObject;
    RB_String mDatabaseConnection;
    bool mCalledFromList;
    bool mUseParentId;
    RB_String mIdFieldName;
    bool mIsUpdateNewId;
    RB_String mWhereStatement;

    enum DbAction {
        DbNone,
        DbRead,
        DbUpdate, // create if not exists
        DbDelete,
        DbDeleteList
    };
    
    DbAction mAction;

private:

};

#endif /*RB_DBVISITOR_H*/
