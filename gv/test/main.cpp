/*****************************************************************
 * $Id: main.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Mar 19, 2008 9:29:09 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "main.h"

#include <iostream>
#include <QApplication>
#include <QFileInfo>
// #include <QMessageBox>

#include "gv_mainwindow.h"
#include "rb_settings.h"
#include "rb_system.h"


using namespace std;

int main(int argc, char ** argv) {
    
    RB_DEBUG->setLevel(RB_Debug::D_WARNING);
    
    // set locale to default environments to avoid . / , decimal confusion
    setlocale(LC_ALL, "C"); // TODO: still required?
    
    QApplication app(argc, argv);
    
    for (int i = 0; i < argc; i++) {
        if (QString("--debug") == argv[i]) {
			RB_DEBUG->setLevel(RB_Debug::D_DEBUGGING);
		}
		RB_DEBUG->print("param %i: %s", i, argv[i]);
    }
    
 	// set organization particularities, initialize settings
    QCoreApplication::setOrganizationName(RB_COMPANYKEY);
    QCoreApplication::setOrganizationDomain(RB_COMPANYDOMAIN);
    QCoreApplication::setApplicationName(RB_APPNAME);
    RB_SETTINGS->init(RB_COMPANYKEY, RB_APPKEY);
                    
	// set application name, version. directory name and path, initialize system
    QFileInfo prgInfo(QFile::decodeName(argv[0]));
    QString prgDir(prgInfo.path());
    RB_SYSTEM->init(RB_APPNAME, RB_VERSION, RB_APPDIR, prgDir);
    
    // create and show mainwindow
    RB_DEBUG->print("main: creating main window..");
    GV_MainWindow* appWin = new GV_MainWindow();
    //    app.setAppWin(appWin);
    RB_DEBUG->print("main: setting caption");
    appWin->setWindowTitle(RB_APPNAME);
    RB_DEBUG->print("main: show main window");
    appWin->show();
    RB_DEBUG->print("main: creating main window: OK");

    // return QApplication values
    RB_DEBUG->print("main: app.exec()");
    int r = 0;
    r = app.exec();

    RB_DEBUG->print("main: finished: %d", r);
    RB_DEBUG->print("main: locale is: %s", getenv("LC_ALL"));
    RB_DEBUG->print("main: num locale is: %s", getenv("LC_NUMERIC"));

    return r;
}
