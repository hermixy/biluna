/*****************************************************************
 * $Id: db_transferdataaction.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: May 23, 2008 9:07:28 PM - Rutger Botermans
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_transferdataaction.h"

#include <QtSql>
#include "db_dialogfactory.h"
#include "db_objectfactory.h"
#include "rb_csvwriter.h"
#include "rb_debug.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_string.h"
#include "rb_stringlist.h"
#include "rb_xmlwriter.h"


DB_TransferDataAction::DB_TransferDataAction(){
	mDbRoot = NULL;
	mErrorMessage = "";
}

DB_TransferDataAction::~DB_TransferDataAction() {
    delete mDbRoot;
}

/**
 * Transfer (copy) entire source database to destination database,
 * tables are created if not exists, rows are inserted if id (primary key)
 * does not exists.
 * @param dbSource source database
 * @param dbDestination destination database
 */
bool DB_TransferDataAction::transferDatabase(QSqlDatabase dbSource,
                                             QSqlDatabase dbDestination) {
    RB_DEBUG->print("DB_TransferDataAction::transferDatabase");

    // Tables visible to the user
    RB_StringList tableList = dbSource.tables(QSql::Tables);
    bool success = false;

    for (int i = 0; i < tableList.size(); ++i) {
        success = transferDatabaseTable(dbSource, dbDestination, tableList.at(i));

        if (!success) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                    "DB_TransferDataAction::transferDatabase() ERROR");
            return false;
        }
    }

    return true;
}

/**
 * Transfer data from database table to other database. Except for the SQLite
 * database, the database should already exist
 * @param dbSource source database
 * @param dbDestination destination database
 * @param tableName table name
 */
bool DB_TransferDataAction::transferDatabaseTable(QSqlDatabase dbSource,
                                                  QSqlDatabase dbDestination,
                                                  const RB_String& tableName) {
    // create database table
    bool success = createDatabaseTable(dbSource, dbDestination, tableName);

    if (!success) {
        RB_DEBUG->print("DB_TransferDataAction::transferDatabaseTable() "
                "could not create table ERROR");
        return false;
    }

    success = fillDatabaseTable(dbSource, dbDestination, tableName);
    DB_DIALOGFACTORY->commandMessage("Transfer of table: " + tableName + " DONE");
    return success;
}

/**
 * Transfer (copy) entire database to SQLite database 
 * @param dbSource source database
 * @param dbDestination destination database
 */
//bool DB_TransferDataAction::transferDatabaseSqlite(QSqlDatabase dbSource,
//								QSqlDatabase dbDestination) {
//	RB_DEBUG->print("DB_TransferDataAction::transferDatabaseSqlite()");
//
//	// Tables visible to the user
//	RB_StringList tableList = dbSource.tables(QSql::Tables);
//	bool success = false;
//
//	for (int i = 0; i < tableList.size(); ++i) {
//		success = transferTableSqlite(dbSource, dbDestination, tableList.at(i));
//
//		if (!success) {
//			RB_DEBUG->print(RB_Debug::D_ERROR,
//					"DB_TransferDataAction::transferDatabaseSqlite() ERROR");
//			return false;
//		}
//	}
//
//	return true;
//}

/**
 * Transfer data from a database table to a new SQLite table
 * @param dbSource source database
 * @param dbDestination destination database
 * @param tableName table name with data to be transfered (copied)
 */
//bool DB_TransferDataAction::transferTableSqlite(QSqlDatabase dbSource,
//                      QSqlDatabase dbDestination,
//			const RB_String& tableName) {
//	RB_DEBUG->print("DB_TransferDataAction::transferTableSqlite()");
//
//    // create database table
//	bool success = createDatabaseTable(dbSource, dbDestination, tableName);
//
//	if (!success) {
//		RB_DEBUG->print("DB_TransferDataAction::transferTableSqlite() "
//				"could not create table");
//		return false;
//	}
//
//    // success = fillDatabaseTable(dbSource, dbDestination, tableName);
//	success = fillSqliteTable(dbSource, dbDestination, tableName);
//	return success;
//}

/**
 * Transfer data from a database or database table to a XML file
 * @param db source database
 * @param fileName XML file
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::transferDbXmlFile(QSqlDatabase db,
                                            const RB_String& fileName,
                                            const RB_String& tableName) {
    RB_DEBUG->print("DB_TransferDataAction::transferTableXmlFile()");
	
    // create root
    if (!mDbRoot) {
        mDbRoot = new RB_ObjectContainer("", NULL, db.databaseName(), NULL, false);
    }
    
    RB_String rootName = "DATABASE_" + db.databaseName() + "_TABLE_" + tableName;
    QRegExp rx("[^a-zA-Z0-9]");
    rootName.replace(rx, "_");
    mDbRoot->setName(rootName);

    if (tableName != "") {
        createTableModel(db, tableName);
    } else {
        // create tables in model
        QStringList tables = db.tables();
        for (int i = 0; i < tables.count(); ++i) {
            createTableModel(db, tables.at(i));
        }
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        RB_DEBUG->warning("DB_TransferDataAction::transferTableXmlFile() "
                          "cannot write file");
        return false;
    }

    RB_XmlWriter* xw = new RB_XmlWriter(&file);
    xw->setUomIsColumnDefinition(true);
    mDbRoot->acceptVisitor(*xw);

    delete xw;
    delete mDbRoot;
    mDbRoot = NULL;

    return true;
}

/**
 * Transfer data from a database table to a CSV (,) file
 * @param db source database
 * @param fileName CSV file
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::transferDbXmlFileHierarchy(
                            QSqlDatabase db, const QString& fileName,
                            const QString& tableName, const QString& id) {
    RB_DEBUG->print("DB_TransferDataAction::transferDbXmlFileHierarchy()");

    RB_StringList strL = tableName.split("_");
    RB_ObjectFactory* f = DB_OBJECTFACTORY->getFactory(strL.at(0).toUpper());

    if (!f) {
        return false;
    }


    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        return false;
    }

    RB_XmlWriter* xfw = new RB_XmlWriter(&file);

    RB_ObjectBase* obj = f->newSingleObject(tableName);
    obj->setId(id);
    obj->dbRead(db, RB2::ResolveAll);
    obj->acceptVisitor(*xfw);
    delete xfw;
    delete obj;

    return true;
}

/**
 * Transfer data from a database table to a CSV (,) file
 * @param db source database
 * @param fileName CSV file
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::transferDbCsvCommaFile(QSqlDatabase db,
                                const RB_String& fileName,
                                const RB_String& tableName) {
    RB_DEBUG->print("DB_TransferDataAction::transferTableCsvSemicolonFile");
	
    // create root
    if (!mDbRoot) {
    	mDbRoot = new RB_ObjectContainer("", NULL, db.databaseName());
    }
    
    if (tableName != "") {
        createTableModel(db, tableName);
    } else {
        // create tables in model
        QStringList tables = db.tables();
        for (int i = 0; i < tables.count(); ++i) {
            createTableModel(db, tables.at(i));
        }
    }

    // export file
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        RB_DEBUG->warning("DB_TransferDataAction::transferTableXmlFile() "
                          "cannot write file");
        return false;
    }

    RB_CsvWriter* xw = new RB_CsvWriter(&file);
    xw->setSeparator(RB2::CsvComma);
    mDbRoot->acceptVisitor(*xw);

    delete xw;
    delete mDbRoot;
    mDbRoot = NULL;

    return true;
}

/**
 * Transfer data from a database or database table one CSV (;) separated file
 * @param db source database
 * @param fileName CSV file
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::transferDbCsvSemicolonFile(QSqlDatabase db, 
                                const RB_String& fileName,
                                const RB_String& tableName) {
    // create root
    if (!mDbRoot) {
        mDbRoot = new RB_ObjectContainer("", NULL, db.databaseName());
    }

    if (tableName != "") {
        createTableModel(db, tableName);
    } else {
        // create tables in model
        QStringList tables = db.tables();
        for (int i = 0; i < tables.count(); ++i) {
            createTableModel(db, tables.at(i));
        }
    }
	
    // export file
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        RB_DEBUG->warning("DB_TransferDataAction::transferTableXmlFile() "
                          "cannot write file");
        return false;
    }

    RB_CsvWriter* xw = new RB_CsvWriter(&file);
    xw->setSeparator(RB2::CsvSemicolon);
    mDbRoot->acceptVisitor(*xw);

    delete xw;
    delete mDbRoot;
    mDbRoot = NULL;

    return true;
}

/**
 * Transfer data from a database or database table to a CSV (TAB) file
 * @param db source database
 * @param tableName table name with data to be transfered (copied)
 * @param fileName CSV file
 */
bool DB_TransferDataAction::transferDbCsvTabFile(QSqlDatabase db,
                                const RB_String& fileName,
                                const RB_String& tableName) {
    RB_DEBUG->print("DB_TransferDataAction::transferDbCsvTabFile");
	
	// create root
    if (!mDbRoot) {
    	mDbRoot = new RB_ObjectContainer("", NULL, db.databaseName());
    }
    
    if (tableName != "") {
        createTableModel(db, tableName);
    } else {
        // create tables in model
        QStringList tables = db.tables();
        for (int i = 0; i < tables.count(); ++i) {
            createTableModel(db, tables.at(i));
        }
    }

    // export file
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        RB_DEBUG->warning("DB_TransferDataAction::transferTableXmlFile() "
                          "cannot write file");
        return false;
    }

    RB_CsvWriter* xw = new RB_CsvWriter(&file);
    xw->setSeparator(RB2::CsvTab);
    mDbRoot->acceptVisitor(*xw);

    delete xw;
    delete mDbRoot;
    mDbRoot = NULL;

    return true;
}

/**
 * Create table in in-memory model
 * @param db source database
 * @param tableName table name with data to be transfered (copied)
 */
void DB_TransferDataAction::createTableModel(QSqlDatabase db,
                                             const RB_String& tableName) {
	
    RB_String strData = "";
    QSqlTableModel* model = new QSqlTableModel(NULL, db);
    model->setTable(tableName);
    model->select();
    
//    if (model->lastError().type() != QSqlError::NoError)
//        emit statusMessage(model->lastError().text());

    // create table container and add to root
    RB_String tmpStr = tableName + "List";
    RB_ObjectContainer* tableContainer;
    tableContainer = new RB_ObjectContainer("", mDbRoot, tmpStr, NULL, false);
    mDbRoot->addObject(tableContainer);
    
    RB_ObjectAtomic* obj;
    
    // get field (column) names
    QStringList colNameList;
    QStringList colDefList;
    QSqlRecord rec = db.record(tableName);
    QSqlField fld;
    
    for (int i = 0; i < rec.count(); ++i) {
        fld = rec.field(i);
        colNameList << fld.name();
        
        if (fld.typeID() == -1) {
                colDefList << QString(QVariant::typeToName(fld.type()));
        } else {
                colDefList << QString("%1 (%2)")
        			.arg(QVariant::typeToName(fld.type())).arg(fld.typeID());
        }
    }
    
    while (model->canFetchMore()) {
        model->fetchMore();
    }

    // fill object model
    for (int row = 0; row < model->rowCount(); ++row) {
        obj = new RB_ObjectAtomic("", tableContainer, tableName, NULL, false);
        tableContainer->addObject(obj);
		
        for (int col = 0; col < model->columnCount(); ++col) {
            strData = model->data(model->index(row,col,QModelIndex())).toString();
            obj->addMember(colNameList.at(col), colDefList.at(col), strData);
        }
    }
	
    delete model;
    model = NULL;
}

/**
 * Create database table. If first field name contains id, ID, Id or iD,
 * this first field will be set as primary key
 * @param dbSource source database
 * @param dbDestination destination database
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::createDatabaseTable(QSqlDatabase dbSource,
                                                QSqlDatabase dbDestination,
                                                const RB_String& tableName) {
    if (dbSource.driverName() == "QMYSQL") {
        return createFromMysqlTable(dbSource, dbDestination, tableName);
    }

    RB_String sqlStr = "CREATE TABLE IF NOT EXISTS ";
    sqlStr += "`" + tableName + "` (";
	
    QSqlRecord rec = dbSource.record(tableName);
    QSqlField fld;
    RB_String fldStr;
    
    for (int i = 0; i < rec.count(); ++i) {
        if (i > 0) {
            sqlStr += ", ";
        }

        fld = rec.field(i);
        sqlStr += "`" + fld.name() + "`";
        setFieldProperties(fld, fldStr);
        sqlStr += fldStr;
    }

    RB_String firstField = rec.fieldName(0);

    if (firstField.contains("id", Qt::CaseInsensitive)) {
        if (dbDestination.driverName() == "QMYSQL") {
            sqlStr += ", PRIMARY KEY (`" + firstField + "`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
        } else {
            sqlStr += ", PRIMARY KEY (`" + firstField + "`));";
        }
    } else {
        sqlStr += ");";
    }

	QSqlQuery query = dbDestination.exec(sqlStr);
	
	if (query.lastError().type() != QSqlError::NoError) {
		RB_DEBUG->print(RB_Debug::D_ERROR, 
				"DB_TransferDataAction::createDatabaseTable() ERROR");
		mErrorMessage = query.lastError().driverText();
		return false;
	}
	
    return true;
}

/**
    => MSAccess

    (no information yet)

    => MySQL

    SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH,
      NUMERIC_PRECISION, IS_NULLABLE, EXTRA, COLUMN_DEFAULT,
      COLUMN_KEY, NUMERIC_SCALE, COLUMN_COMMENT
    FROM INFORMATION_SCHEMA.COLUMNS
    WHERE table_name = 'acc_customers'
    AND TABLE_SCHEMA = 'biluna'  <- not optional!

    or

    SHOW COLUMNS FROM acc_customers; (shorter information)

    => SQL Server, to be tested:

    SELECT * FROM INFORMATION_SCHEMA.Columns WHERE TABLE_NAME = 'PccCap';

    => SQLite

    SELECT * FROM sqlite_master WHERE type = 'table' AND name ='myTable';

    If you are running the sqlite3 command-line access program you can type ".tables" to get a list of all tables. Or you can type ".schema" to see the complete database schema including all tables and indices. Either of these commands can be followed by a LIKE pattern that will restrict the tables that are displayed.

    From within a C/C++ program (or a script using Tcl/Ruby/Perl/Python bindings) you can get access to table and index names by doing a SELECT on a special table named "SQLITE_MASTER". Every SQLite database has an SQLITE_MASTER table that defines the schema for the database. The SQLITE_MASTER table looks like this:

        CREATE TABLE sqlite_master (
          type TEXT,
          name TEXT,
          tbl_name TEXT,
          rootpage INTEGER,
          sql TEXT
        );

    For tables, the type field will always be 'table' and the name field will be the name of the table. So to get a list of all tables in the database, use the following SELECT command:

        SELECT name FROM sqlite_master
        WHERE type='table'
        ORDER BY name;

    For indices, type is equal to 'index', name is the name of the index and tbl_name is the name of the table to which the index belongs. For both tables and indices, the sql field is the text of the original CREATE TABLE or CREATE INDEX statement that created the table or index. For automatically created indices (used to implement the PRIMARY KEY or UNIQUE constraints) the sql field is NULL.

    The SQLITE_MASTER table is read-only. You cannot change this table using UPDATE, INSERT, or DELETE. The table is automatically updated by CREATE TABLE, CREATE INDEX, DROP TABLE, and DROP INDEX commands.

    Temporary tables do not appear in the SQLITE_MASTER table. Temporary tables and their indices and triggers occur in another special table named SQLITE_TEMP_MASTER. SQLITE_TEMP_MASTER works just like SQLITE_MASTER except that it is only visible to the application that created the temporary tables. To get a list of all tables, both permanent and temporary, one can use a command similar to the following:

        SELECT name FROM
           (SELECT * FROM sqlite_master UNION ALL
            SELECT * FROM sqlite_temp_master)
        WHERE type='table'
        ORDER BY name


 */
bool DB_TransferDataAction::createFromMysqlTable(QSqlDatabase dbSource,
                                                 QSqlDatabase dbDestination,
                                                 const QString& tableName) {
    RB_String sqlStr = "";
    sqlStr = "SELECT COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, "
                "NUMERIC_PRECISION, IS_NULLABLE, EXTRA, COLUMN_DEFAULT, "
                "COLUMN_KEY, NUMERIC_SCALE, COLUMN_COMMENT "
            "FROM INFORMATION_SCHEMA.COLUMNS "
            "WHERE   table_name = '" + tableName + "' "
            "AND TABLE_SCHEMA = '" + dbSource.databaseName() + "';";
    QSqlQuery qSource(dbSource);

    if (!qSource.exec(sqlStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_BrowserDialog::showMysqlMetaData() "
                        "%s ERROR", qSource.lastError().text().toStdString().c_str());
        return false;
    }

    QSqlRecord rec; // = dbSource.record(tableName); for column names

    sqlStr.clear();
    sqlStr = "CREATE TABLE IF NOT EXISTS ";
    sqlStr += "`" + tableName + "` (";

    RB_String str;
    RB_String primaryKeyField = "";
    RB_String fldType;
    int i = 0;

    while (qSource.next()) {
        if (i > 0) {
            sqlStr += ", ";
        }

        rec = qSource.record();
        sqlStr += "`" + rec.value(0).toString() + "`";

        fldType = rec.value(1).toString();

        if (fldType.contains("varchar", Qt::CaseInsensitive)) {
            sqlStr += " VARCHAR(" + rec.value(2).toString() + ")";
        } else if (fldType.contains("int", Qt::CaseInsensitive)) {
            sqlStr += " INTEGER";
        } else {
            sqlStr += " " + fldType.toUpper();
        }

        // rec.value(3); precision not used

        if (rec.value(4).toString().contains("NO", Qt::CaseInsensitive)) {
            sqlStr += " NOT NULL";
        }

        // rec.value(5); auto value not used
        str = rec.value(6).toString();

        if (!str.isEmpty()) {
            if (fldType.contains("varchar", Qt::CaseInsensitive)
                    || fldType.contains("string", Qt::CaseInsensitive)) {
                sqlStr += " DEFAULT '" + str + "'";
            } else {
                sqlStr += " DEFAULT " + str;
            }
        }

        str = rec.value(7).toString();

        if (str.contains("pri", Qt::CaseInsensitive)) {
            primaryKeyField = rec.value(0).toString();
        }

        // rec.value(8); numeric scale not used
        // rec.value(9); comment not used

        ++i;
    }

    if (!primaryKeyField.isEmpty()) {
        if (dbDestination.driverName() == "QMYSQL") {
            sqlStr += ", PRIMARY KEY (`" + primaryKeyField + "`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
        } else {
            sqlStr += ", PRIMARY KEY (`" + primaryKeyField + "`));";
        }
    } else {
        sqlStr += ");";
    }

    QSqlQuery qDest = dbDestination.exec(sqlStr);

    if (qDest.lastError().type() != QSqlError::NoError) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "DB_TransferDataAction::createFromMysqlTable() ERROR");
        mErrorMessage = qDest.lastError().driverText();
        return false;
    }

    return true;
}

/**
 * Fill destination database with source database table with table name

=> Update or insert row example:

IF (EXISTS (SELECT * FROM AA_TestTable AS t1
  WHERE t1.ord_num = 'FFF'))
begin
  UPDATE AA_TestTable
  SET ord_qty = 999
  WHERE ord_num = 'FFF'
end
else
begin
  INSERT INTO AA_TestTable (ord_num, top_assy, ord_qty)
  VALUES('FFF', 'XYZ', 999)
end

You should be VERY CAREFUL with things like this. If you can't afford to set
the transaction isolation level to SERIALIZABLE, some other transaction could
add the row with ord_num='FFF' after you've tested for its existence,
but before you've inserted it.
This way you end up with violated primary constraint and error in one of
these transactions.

=> Insert if not exists

This is a variation that works for tables with multiple primary keys.
If you have a users table with columns of username, dept, and age,
and primary keys of username and dept, then this will only insert a user
if it doesn't exist already.

INSERT INTO users (username, dept, age)
  SELECT username='mp', dept='tax', age=5
  WHERE (SELECT COUNT (*) FROM users WHERE username='mp' AND dept='tax')=0;

I initially found this syntax confusing, but here's how I 'parsed' it.
This part returns 0 if it doesn't exist:

SELECT COUNT (*) FROM users WHERE username='mp' AND dept='tax'

This part creates a record-like row with static values and
represents the record to insert:

SELECT username='mp', dept='tax', age=5

The WHERE-clause following the above part will only return the value to insert
if the count is 0.

Insert if not exists is used in this 'fillDatabaseTable()' function.

 * @param dbSource source database
 * @param dbDestination destination database
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::fillDatabaseTable(QSqlDatabase dbSource, 
												QSqlDatabase dbDestination,
												const RB_String& tableName) {

    // SQLite does not support insertion of multiple rows, TODO: however use transaction
    if (dbDestination.driverName() == "QSQLITE") {
        return fillSqliteTable(dbSource, dbDestination, tableName);
    }

    // TODO: test below with ODBC and MySQL
/*
        RB_String sqlInsert = "";
        RB_String sqlSelect = "";
        RB_String sqlWhere = "";

        while (query.next()) {
            sqlInsert = "INSERT INTO " + tableName + " (";
            sqlSelect = "SELECT ";
            sqlWhere = " WHERE (SELECT COUNT (*) FROM " + tableName + " WHERE id='";

            for (int col = 0; col < colCount; ++col) {
                if (!isFirstCol) {
                    sqlInsert += ", ";
                    sqlSelect += ", ";
                }

                if (rec.field(col).type() == QVariant::Double
                        || rec.field(col).type() == QVariant::Int) {
                    sqlInsert += rec.fieldName(col);
                    sqlSelect += rec.fieldName(col) + "=";
                    sqlSelect += rec.fieldName(col) + "=" + query.value(col).toString();
                } else {
                    sqlInsert += rec.fieldName(col);
                    sqlSelect += rec.fieldName(col) + "='" + query.value(col).toString() + "'";
                }

                if (rec.fieldName(col) == "id") {
                    sqlWhere += query.value(col).toString() + "'";
                }

                isFirstCol = false;
            }

            sqlInsert += ") ";
            sqlSelect += " ";
            sqlWhere += ")=0;";
            sqlStr = sqlInsert + sqlSelect + sqlWhere;

            isFirstCol = true;
            QSqlQuery query1 = dbDestination.exec(sqlStr);

            if (query1.lastError().type() != QSqlError::NoError) {
                RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_TransferDataAction::fillDatabaseTable() ERROR");
                mErrorMessage = query.lastError().driverText();
                return false;
            }
        }
    */

    RB_String sqlStr = "SELECT * FROM `" + tableName + "`;";
	QSqlQuery query(dbSource);
	query.setForwardOnly(true); // performance only
	query.exec(sqlStr);
	
	QSqlRecord rec = query.record();
	int colCount = rec.count();
    sqlStr = "INSERT INTO `" + tableName + "` VALUES ";
	bool isFirstRow = true;
	bool isFirstCol = true;
    RB_String str = "";
	
	while (query.next()) {
		if (!isFirstRow) sqlStr += ",";
		sqlStr += "(";
		
		for (int col = 0; col < colCount; ++col) {
			if (!isFirstCol) sqlStr += ",";

            if (!query.value(col).toString().data()->isNull()) { // NULL or \000
                str = query.value(col).toString();

                if (rec.field(col).type() == QVariant::Double
                        || rec.field(col).type() == QVariant::Int) {
                    sqlStr += str;
                } else {
                    str.replace("'", "''");
                    sqlStr += "'" + str + "'";
                }
            } else {
                sqlStr += "''"; // TODO: check if works in case of NULL or \000
            }
			
			isFirstCol = false;
		}
		
		sqlStr += ")";
		isFirstRow = false;
		isFirstCol = true;
	}

	sqlStr += ";";
	QSqlQuery query1 = dbDestination.exec(sqlStr);

	if (query1.lastError().type() != QSqlError::NoError) {
		RB_DEBUG->print(RB_Debug::D_ERROR, 
				"DB_TransferDataAction::fillDatabaseTable() ERROR");
        mErrorMessage = query1.lastError().driverText();
		return false;
	}
	
	return true;
}

/**
 * Fill destination SQLite database with source database table with table name,
 * this is a slower execution because SQLite does not support inserting 
 * multiple rows.
 *
 * INSERT OR IGNORE INTO tableName (id, parent, ...) VALUES ('001', 'none', ...);"
 *
 * @param dbSource source database
 * @param dbDestination destination database
 * @param tableName table name with data to be transfered (copied)
 */
bool DB_TransferDataAction::fillSqliteTable(QSqlDatabase dbSource, 
												QSqlDatabase dbDestination,
												const RB_String& tableName) {
	RB_String sqlStr = "SELECT * FROM " + tableName + ";";
	QSqlQuery query(dbSource);
	query.setForwardOnly(true); // performance only
	query.exec(sqlStr);
	
	QSqlRecord rec = query.record();
	int colCount = rec.count();
	bool isFirstCol = true;

	// only for SQLite in order to write the journal only once
	sqlStr = "BEGIN";
	dbDestination.exec(sqlStr);

    RB_String sqlInsert = "";
    RB_String sqlValues = "";
    RB_String str = "";

	while (query.next()) {
        // OR IGNORE is specific for SQLite
        sqlInsert = "INSERT OR IGNORE INTO `" + tableName + "` (";
        sqlValues = "VALUES (";
		
		for (int col = 0; col < colCount; ++col) {
            if (!isFirstCol) {
                sqlInsert += ", ";
                sqlValues += ", ";
            }
			
            sqlInsert += "`" + rec.fieldName(col) + "`";
            str = query.value(col).toString();

            // Test for NULL "\000", var.isNull() and var.toString().isNull() do not work
            if (str.data()->isNull()) {
                sqlValues += "''";
            } else {
                // also numeric values between single quotes
                str.replace("'", "''");
                sqlValues += "'" + str + "'";
            }

			isFirstCol = false;
		}
		
        sqlInsert += ") ";
        sqlValues += ");";
        sqlStr = sqlInsert + sqlValues;
		isFirstCol = true;
        QSqlQuery query1 = dbDestination.exec(sqlStr);

        if (query1.lastError().type() != QSqlError::NoError) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                    "DB_TransferDataAction::fillSqliteTable() ERROR");
            mErrorMessage = query1.lastError().driverText();
            return false;
        }
	}

	// only for SQLite in order to write the journal only once
	sqlStr = "END";
	dbDestination.exec(sqlStr);

    return true;
}

/**
 * Set field  properties
 * @param fld field of record
 * @param fldStr SQL string for field properties to be set
 */
void DB_TransferDataAction::setFieldProperties(const QSqlField& fld,
                                               QString& fldStr) {
    setDataTypeToSqlType(fld.type(), fld.length(), fldStr);

    if (fld.requiredStatus() == QSqlField::Required) {
        fldStr += " NOT NULL";
    } else if (fld.isNull()){ // allows NULL
        fldStr += " NULL";
    }

    if (!fld.defaultValue().toString().isEmpty()) {
        RB_String defaultStr = fld.defaultValue().toString();

        if (fld.type() != QVariant::String) {
            fldStr += " DEFAULT " + defaultStr;
        } else {
            if ((defaultStr.startsWith("'") && defaultStr.endsWith("'"))
                    || (defaultStr.startsWith("\"") && defaultStr.endsWith("\""))) {
                fldStr += " DEFAULT " + defaultStr;
            } else {
                fldStr += " DEFAULT '" + fld.defaultValue().toString() + "'";
            }
        }
    }
}

/**
 * Convert Qt data type to SQL type
 * -Qt										-MySQL
 * --------------------                     ---------------------  
 * QVariant::BitArray						BLOB
 * QVariant::DateTime						DATETIME
 * QVariant::Double (double)				DOUBLE
 * QVariant::Int (int)						INTEGER
 * QVariant::String length n <=255			VARCHAR(n)
 * QVariant::String length n > 255 or <= 0	TEXT
 * @param qtType Qt data type (enum)
 * @param strType SQL data type as string
 * @param length length of the string for the VARCHAR or otherwise STRING
 */
void DB_TransferDataAction::setDataTypeToSqlType(QVariant::Type qtType,
											int length, RB_String& strType) {
    strType = " ";

	switch (qtType) {
		case QVariant::ByteArray:
            strType += "BLOB";
			break;
		case QVariant::DateTime:
            strType += "DATETIME";
			break;
		case QVariant::Double:
            strType += "DOUBLE";
			break;
		case QVariant::Int:
            strType += "INTEGER";
			break;
		case QVariant::String:
            if (length > 0 && length/3 <= 255) {
                strType += "VARCHAR(" + RB_String::number((int)(length/3)) + ")";
			} else {
                strType += "STRING";
			}
			break;
		default:
            strType += "BLOB";
			break;
	}
}
