/*****************************************************************
 * $Id: rb_database.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Nov 15, 2009 1:00:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_database.h"
#include "rb_debug.h"
#include "rb_stringlist.h"
#include "rb_tcpsocket.h"

RB_Database* RB_Database::mActiveDatabase = NULL;


RB_Database::RB_Database() : QSqlDatabase() {
    RB_DEBUG->print("RB_Database::RB_Database()");
    mTcpSocket = NULL;
}

RB_Database::~RB_Database() {
    if (mTcpSocket) {
        delete mTcpSocket;
    }

    mActiveDatabase = NULL;
    RB_DEBUG->print("->RB_DATABASE::~RB_Database() OK");
}

RB_Database *RB_Database::getInstance() {
    if (!mActiveDatabase) {
        mActiveDatabase = new RB_Database();
    }

    return mActiveDatabase;
}

/**
 * Get database with standards
 * @return QSqlDatabase database with standards
 */
QSqlDatabase RB_Database::standardDatabase() {
    QSqlDatabase db = QSqlDatabase::database(STANDARD_CONNECTION);

    if (!db.isValid()) {
        // default connection
        return QSqlDatabase::database();
    }

    return db;
}

/**
 * Get database with custom standard data from user
 * @return QSqlDatabase database with custom standard data
 */
QSqlDatabase RB_Database::customDatabase() {
    QSqlDatabase db = QSqlDatabase::database(CUSTOM_CONNECTION);

    if (!db.isValid()) {
        // default connection
        return QSqlDatabase::database();
    }

    return db;
}

/**
 * @brief RB_Database::keepConnectionAlive
 * @param db
 * @param mSec should be larger 5000 milliseconds
 */
void RB_Database::keepConnectionAlive(const QSqlDatabase& db, int mSec) {
    if (db.driverName() == "QMYSQL" && mSec >= 5000) {

        if (!mTcpSocket) {
            mTcpSocket = new RB_TcpSocket();
        }

        mTcpSocket->keepConnectionAlive(db.hostName(), db.port(), mSec);
    }
}

void RB_Database::dummyQuery() {
    QStringList nameList = connectionNames();

    foreach (const QString& connName, nameList) {
        QSqlDatabase db = database(connName);

        if (!db.isOpen()) {
            RB_DEBUG->error("RB_Database::dummyQuery() ERROR");
            RB_DEBUG->error(db.lastError().text());
        } else if (db.driverName() != "QSQLITE" && db.driverName() != "QODBC") {
            db.tables(QSql::AllTables); // does query
            QStringList strL = db.tables(QSql::AllTables);
            int count = strL.count();
            RB_DEBUG->debug("RB_Database::dummyQuery() table count: "
                            + QString::number(count));
        }
    }
}

