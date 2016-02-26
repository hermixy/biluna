/*****************************************************************
 * $Id: rb_tcpsocket.cpp 2178 2014-09-25 13:50:56Z rutger $
 * Created: Sep 24, 2014 11:34:00 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_tcpsocket.h"

#include "rb_database.h"
#include "rb_debug.h"


RB_TcpSocket::RB_TcpSocket(QObject *parent) : QTcpSocket(parent) {
    RB_DEBUG->print("RB_TcpSocket::RB_TcpSocket()");

    mHostName = "";
    mPort = -1;
    mTimerId = -1;
    mTimer = NULL;
}

RB_TcpSocket::~RB_TcpSocket() {
    killTimer(mTimerId);
    disconnectFromHost();

    if (mTimer) {
        delete mTimer;
        mTimer = NULL;
    }

    RB_DEBUG->print("->RB_TcpSocket::~RB_TcpSocket() OK");
}

void RB_TcpSocket::keepConnectionAlive(const RB_String& hostName,
                                       int port, int mSec) {
    mHostName = hostName;
    mPort = port;
    mTimerId = startTimer(mSec); // implicit connection to timerEvent
}

// not used?
void RB_TcpSocket::slotConnectToHost() {
    RB_DEBUG->print("RB_TcpSocket::slotConnectToHost()");
    connectToHost(mHostName, mPort, ReadWrite);
}

void RB_TcpSocket::timerEvent(QTimerEvent* /* e */) {
    RB_DEBUG->print("RB_TcpSocket::timerEvent()");
    // connectToHost(mHostName, mPort, ReadWrite); not used

    RB_DATABASE->dummyQuery();
}

