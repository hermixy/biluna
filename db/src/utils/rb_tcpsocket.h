/*****************************************************************
 * $Id: rb_tcpsocket.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 24, 2014 11:34:00 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TCPSOCKET_H
#define RB_TCPSOCKET_H

#include <QTcpSocket>
#include <QTimer>
#include <QTimerEvent>

#include "db_global.h"
#include "rb_string.h"


class DB_EXPORT RB_TcpSocket : public QTcpSocket {

    Q_OBJECT

public:
    explicit RB_TcpSocket(QObject *parent = 0);
    virtual ~RB_TcpSocket();

    void keepConnectionAlive(const RB_String& hostName,
                             int port, int mSec = 60000);
//    void setDatabaseInstance(RB_Database* db) {
//        mDatabaseInstance = db;
//    }

public slots:
    void slotConnectToHost();

protected:
    void timerEvent(QTimerEvent* e);

private:
    RB_String mHostName;
    int mPort;
    int mTimerId;

    QTimer* mTimer;
//    RB_Database* mDatabaseInstance;
};

#endif // RB_TCPSOCKET_H
