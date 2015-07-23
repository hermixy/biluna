/*****************************************************************
 * $Id: rb_hardware.cpp 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 4, 2008 7:57:26 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_hardware.h"

#include <QProcess>


RB_Hardware::RB_Hardware() {
	
}

RB_Hardware::~RB_Hardware() {
	
}

/**
 * Get Ethernet (Eth0) card number
 * TODO: (Linux) assumes Eth0 number is on first line, to be tested first
 */
RB_String RB_Hardware::getEthernetCardNumber() {
	RB_String ethStr = "";
	getIpConfig(ethStr);

#ifdef Q_OS_UNIX
	RB_StringList ethStrList = ethStr.split("\n");
	ethStr = ethStrList.at(0).trimmed();
	ethStr = ethStr.right(17);
#endif
	
#ifdef Q_OS_WIN
	RB_StringList ethStrList = ethStr.split("\n");
	bool found = false;

	for (int i = 0; i < ethStrList.count() || !found; ++i) {
		if (ethStrList.at(i).contains("Physical Address")) {
			ethStr = ethStrList.at(i).trimmed();
			found = true;
		}
	}
	
	ethStr = ethStr.right(17);
#endif
	
	return ethStr;
}

/**
 * Get hardware and software IP configuration
 */
const char* RB_Hardware::getIpConfig(RB_String& ipStr) {
	QProcess extProcess;

#ifdef Q_OS_UNIX
	extProcess.start("/sbin/ifconfig");
#endif
	
#ifdef Q_OS_WIN
	extProcess.start("ipconfig", QStringList() << "/all");
//	extProcess.start("ping www.red-bag.com -n 4");
#endif
	
	if (!extProcess.waitForStarted())
		return "wait for started ERROR";
	
	if (!extProcess.waitForFinished())
		return "wait for finished ERROR";
	
	QByteArray byteArray = extProcess.readAll();
	ipStr = byteArray.constData();
}
