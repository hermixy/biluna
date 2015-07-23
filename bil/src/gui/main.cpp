/*****************************************************************
 * $Id: main.cpp 2195 2014-11-10 12:26:20Z rutger $
 * Created: 16-Dec-2010 7:25:57 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna BIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "main.h"

#include <QtWidgets>
#include "qtsingleapplication.h"

#include "bil_mainwindow.h"
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_system.h"



int main(int argc, char *argv[]) {

    RB_DEBUG->setLevel(RB_Debug::D_WARNING);

    // set locale to default environments to avoid . / , decimal confusion
    setlocale(LC_ALL, "C");

//    QApplication app(argc, argv);
    // Single application start
    QtSingleApplication app(argc, argv);
    if (app.isRunning()) {
        return !app.sendMessage("Biluna already running ...");
    }

    for (int i = 0; i < argc; i++) {
        if (QString("--debug") == argv[i]) {
            RB_DEBUG->setLevel(RB_Debug::D_DEBUGGING);
        }
        RB_DEBUG->print("param %i: %s", i, argv[i]);
    }

#ifdef Q_OS_MAC
    //qt_mac_set_menubar_icons(false);
    QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

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

    // initialize resource in static lib DB
    Q_INIT_RESOURCE(db);

    BIL_MainWindow w;
    w.show();
    app.setActivationWindow(&w); // for single

#ifdef BILUNA_DEMO
    QTimer::singleShot(10 * 60 * 1000, Qt::VeryCoarseTimer, &w, SLOT(slotFileDemoQuit()));
#endif

    int ret = app.exec();
    return ret;
}

