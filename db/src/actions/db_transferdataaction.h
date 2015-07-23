/*****************************************************************
 * $Id: db_transferdataaction.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: May 23, 2008 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TRANSFERDATAACTION_H
#define DB_TRANSFERDATAACTION_H

#include <QSqlField>
#include "qsqldatabase.h"
#include "rb_string.h"
#include "rb_variant.h"

class RB_ObjectBase;
class RB_ObjectContainer;


/**
 * Data transfer action from source:
 * - entire database 
 * - selected table 
 * - selected rows
 * - SQL query selection 
 * to destination:
 * - MySQL
 * - SQLite
 * - XML
 * - CSV
 */
class DB_EXPORT DB_TransferDataAction {
	
public:
    DB_TransferDataAction();
    virtual ~DB_TransferDataAction();
	
    virtual bool transferDatabase(QSqlDatabase dbSource,
                                  QSqlDatabase dbDestination);
    virtual bool transferDatabaseTable(QSqlDatabase dbSource,
                                       QSqlDatabase dbDestination,
                                       const RB_String& tableName);
//    virtual bool transferDatabaseSqlite(QSqlDatabase dbSource,
//                                    QSqlDatabase dbDestination);
//    bool transferTableSqlite(QSqlDatabase dbSource,
//                                    QSqlDatabase dbDestination,
//                                    const RB_String& tableName);
    virtual bool transferDbXmlFile(QSqlDatabase db,
                                    const RB_String& fileName,
                                    const RB_String& tableName = "");
    virtual bool transferDbXmlFileHierarchy(QSqlDatabase db,
                                            const RB_String& fileName,
                                            const RB_String& tableName,
                                            const RB_String& id);
    virtual bool transferDbCsvCommaFile(QSqlDatabase db,
                                    const RB_String& fileName,
                                    const RB_String& tableName = "");
    virtual bool transferDbCsvSemicolonFile(QSqlDatabase db,
                                    const RB_String& fileName,
                                    const RB_String& tableName = "");
    virtual bool transferDbCsvTabFile(QSqlDatabase db,
                                    const RB_String& fileName,
                                    const RB_String& tableName = "");
    virtual RB_String errorMessage() { return mErrorMessage; }

private:
    void createTableModel(QSqlDatabase db,
                          const RB_String& tableName);
    bool createDatabaseTable(QSqlDatabase dbSource, QSqlDatabase dbDestination,
                             const RB_String& tableName);
    bool createFromMysqlTable(QSqlDatabase dbSource, QSqlDatabase dbDestination,
                              const RB_String& tableName);
    bool fillDatabaseTable(QSqlDatabase dbSource, QSqlDatabase dbDestination,
                           const RB_String& tableName);
    bool fillSqliteTable(QSqlDatabase dbSource, QSqlDatabase dbDestination,
                         const RB_String& tableName);

    void setFieldProperties(const QSqlField& fld, RB_String& fldStr);
    void setDataTypeToSqlType(QVariant::Type qtType, int length,
                             RB_String& strType);

    RB_ObjectContainer* mDbRoot;
    RB_String mErrorMessage;
};

#endif /*DB_TRANSFERDATAACTION_H*/
