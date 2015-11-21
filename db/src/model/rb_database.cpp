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
    RB_DEBUG->print("->RB_DATABASE->~RB_Database() OK");
}

RB_Database *RB_Database::getInstance() {
    if (!mActiveDatabase) {
        mActiveDatabase = new RB_Database();
    }

    return mActiveDatabase;
}

/**
 * @brief RB_Database::keepConnectionAlive
 * @param db
 * @param mSec should be larger 5000 milliseconds
 */
void RB_Database::keepConnectionAlive(const QSqlDatabase& db, int mSec) {
    if (db.driverName() == "QMYSQL" && mSec >= 5000) {
        mServerDatabase = db;

        if (!mTcpSocket) {
            mTcpSocket = new RB_TcpSocket();
        }

        mTcpSocket->keepConnectionAlive(mServerDatabase.hostName(),
                                        mServerDatabase.port(), mSec);
    }
}


/**
 * Local database name
 */
RB_String RB_Database::localDbName() const {
    return this->mLocalDbName;
}

/**
 * Set local database name
 * @param name local database name
 */
void RB_Database::setLocalDbName(const RB_String& name) {
    this->mLocalDbName = name;
}

void RB_Database::dummyQuery() {
    if (!mServerDatabase.isOpen()) {
        RB_DEBUG->error("RB_Database::dummyQuery() ERROR");
        RB_DEBUG->error(mServerDatabase.lastError().text());
        return;
    }

    mServerDatabase.tables(QSql::AllTables);

    RB_StringList strL = mServerDatabase.tables(QSql::AllTables);
    int count = strL.count();
    RB_DEBUG->print("RB_Database::dummyQuery() table count: " + QString::number(count));
}

