/*****************************************************************
 * $Id: main.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: 14-Nov-06 7:25:57 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "main.h"

#include <QtGui>

//#include "browser.h"
#include "db_testmainwindow.h"
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_system.h"


int main(int argc, char *argv[]) {
	
    RB_DEBUG->setLevel(RB_Debug::D_WARNING);
    
    // set locale to default environments to avoid . / , decimal confusion
    setlocale(LC_ALL, "C");
    
    QApplication app(argc, argv);

    for (int i = 0; i < argc; i++) {
        if (QString("--debug") == argv[i]) {
			RB_DEBUG->setLevel(RB_Debug::D_DEBUGGING);
		}
        RB_DEBUG->print("param " + QString::number(i) + QString(argv[i]));
    }
	
    // Set organization particularities used by QSettings default constructor initialize settings
    QCoreApplication::setOrganizationName(RB_COMPANYKEY);
    QCoreApplication::setOrganizationDomain(RB_COMPANYDOMAIN);
    QCoreApplication::setApplicationName(RB_APPNAME);

    // Initialize settings in RB_Settings, TODO: can be changed refer QCad3?
    RB_SETTINGS->init(RB_COMPANYKEY, RB_APPKEY);
                    
	// set application name, version. directory name and path, initialize system
    QFileInfo prgInfo(QFile::decodeName(argv[0]));
    QString prgDir(prgInfo.path());
    RB_SYSTEM->init(RB_APPNAME, RB_VERSION, RB_APPDIR, prgDir);

    DB_TestMainWindow w;
    w.show();

    return app.exec();
}

