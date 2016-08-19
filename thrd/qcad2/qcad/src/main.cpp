/****************************************************************************
** $Id: main.cpp 10889 2008-08-01 11:23:57Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the QCad project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid QCad Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "main.h"

#include <locale.h>

#ifdef __APPLE__
#include <QMacStyle>
#endif

#ifdef Q_OS_WIN32
// #include <QWindowsStyle>
#endif

#include <QTimer>
#include <qplugin.h>
#include <QSplashScreen>
#include <QPixmap>

#ifdef RS_SCRIPTING
#include <qsproject.h>
#endif

#include "rs_fontlist.h"
#include "rs_patternlist.h"
#include "rs_scriptlist.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_fileio.h"
#include "rs_filtercxf.h"
#include "rs_filterdxf.h"
#include "rs_filterdxf1.h"

#include "qg_dlginitial.h"

#include "qc_application.h"
#include "qc_applicationwindow.h"

#ifdef RS_CAM
# include "rs_filtercam.h"
# include "rs_camdialog.h"
#endif

#ifndef RS_NO_SVG
#include "rb_filtersvg.h"
#endif

#ifndef QC_SPLASH_TXTCOL
# define QC_SPLASH_TXTCOL Qt::black
#endif
    
// commented out by rutger
//Q_IMPORT_PLUGIN(QJpegPlugin)
//Q_IMPORT_PLUGIN(QGifPlugin)
//Q_IMPORT_PLUGIN(qcncodecs)
//Q_IMPORT_PLUGIN(qjpcodecs)
//Q_IMPORT_PLUGIN(qkrcodecs)
//Q_IMPORT_PLUGIN(qtwcodecs)

QSplashScreen* splash = NULL;


/**
 * Main. Creates Application window.
 *
 * Cleaning up #defines.
 */
int main(int argc, char** argv) {
    
    /*
    RS_DEBUG->setLevel(RS_Debug::D_WARNING);
    RS_DEBUG->print("param 0: %s", argv[0]);
    */

    bool styleGiven = false;
    for (int i=0; i<argc; i++) {
        if (QString("-style") == argv[i]) {
            styleGiven = true;
        }
    }

    // avoid . / , confusion on German environments
    //setlocale(LC_ALL, "C");

    QC_Application app(argc, argv);

    // don't close app when init dialog closes:
    app.setQuitOnLastWindowClosed(false);
    
    RS_DEBUG->setLevel(RS_Debug::D_WARNING);

#ifdef RS_SCRIPTING
    Q_INIT_RESOURCE(qcadscripting);
    Q_INIT_RESOURCE(ide);
#endif
#ifdef RS_PROF
    //Q_INIT_RESOURCE(qcadprof);
#endif
    Q_INIT_RESOURCE(qcadactions);
    Q_INIT_RESOURCE(qcadguiqt);
#ifdef RS_CAM
    Q_INIT_RESOURCE(qcadcam);
#endif
    
    for (int i=0; i<app.argc(); i++) {
        if (QString("--debug") == app.argv()[i]) {
            RS_DEBUG->setLevel(RS_Debug::D_DEBUGGING);
        }
    }
    
    QFileInfo prgInfo( QFile::decodeName(argv[0]) );
    QString prgDir(prgInfo.absolutePath());
    
    QCoreApplication::setOrganizationName(QC_COMPANYKEY);
    QCoreApplication::setOrganizationDomain(QC_COMPANYDOMAIN);
    QCoreApplication::setApplicationName(QC_APPNAME);
    RS_SETTINGS->init(QC_COMPANYKEY, QC_APPKEY);
                    
    RS_SYSTEM->init(QC_APPNAME, QC_VERSION, QC_APPDIR, prgDir);
    
    RS_FILEIO->registerFilter(new RS_FilterCxf());
    RS_FILEIO->registerFilter(new RS_FilterDxf());
    RS_FILEIO->registerFilter(new RS_FilterDxf1());
#ifdef RS_OPT_CAM 
    RS_FILEIO->registerFilter(new RS_FilterCam());
#endif
#ifndef RS_NO_SVG
    RS_FILEIO->registerFilter(new RB_FilterSvg());
#endif

    // parse command line arguments that might not need a launched program:
    QStringList fileList = handleArgs(argc, argv);
    QStringList scriptList = handleScriptArgs(argc, argv);

    RS_String lang;
    //RS_String langCmd;
    RS_String unit;
    
    RS_SETTINGS->beginGroup("/Defaults");
#ifndef QC_PREDEFINED_UNIT
    unit = RS_SETTINGS->readEntry("/Unit", "Invalid");
#else
    unit = RS_SETTINGS->readEntry("/Unit", QC_PREDEFINED_UNIT);
#endif
    RS_SETTINGS->endGroup();

    bool firstLaunch = (unit=="Invalid");

    // show initial config dialog:
    if (firstLaunch) {
        RS_DEBUG->print("main: show initial config dialog..");
        QG_DlgInitial di(NULL);
        di.setText(QString("<font size=\"+1\"><b>Welcome to %1</b></font>"
        "<br>"
        "Please choose the unit you want to use for new drawings and your "
        "preferred language.<br>"
        "You can change these settings later in the "
        "Options Dialog of %2.").arg(QC_APPNAME).arg(QC_APPNAME));
#ifdef RS_CAM
        QPixmap pxm(":/qcad/images/intro_camexpert.png");
#else
        QPixmap pxm(":/qcad/images/intro_qcad.png");
#endif
        di.setPixmap(pxm);
        if (di.exec()) {
            RS_SETTINGS->beginGroup("/Defaults");
            unit = RS_SETTINGS->readEntry("/Unit", "None");
            RS_SETTINGS->endGroup();
        }
        RS_DEBUG->print("main: show initial config dialog: OK");
    }

    RS_DEBUG->print("main: init fontlist..");
    RS_FONTLIST->init();
    RS_DEBUG->print("main: init fontlist: OK");
    
    RS_DEBUG->print("main: init patternlist..");
    RS_PATTERNLIST->init();
    RS_DEBUG->print("main: init patternlist: OK");

    //RS_DEBUG->print("main: init scriptlist..");
    //RS_SCRIPTLIST->init();
    //RS_DEBUG->print("main: init scriptlist: OK");

    RS_DEBUG->print("main: loading translation..");
    RS_SETTINGS->beginGroup("/Appearance");
    
#ifdef QC_PREDEFINED_LOCALE
    lang = RS_SETTINGS->readEntry("/Language", "");
    if (lang.isEmpty()) {
        lang=QC_PREDEFINED_LOCALE;
        RS_SETTINGS->writeEntry("/Language", lang);
    }
    /*
    langCmd = RS_SETTINGS->readEntry("/LanguageCmd", "");
    if (langCmd.isEmpty()) {
        langCmd=QC_PREDEFINED_LOCALE;
        RS_SETTINGS->writeEntry("/LanguageCmd", langCmd);
    }
    */
#else
    lang = RS_SETTINGS->readEntry("/Language", "en");
    //langCmd = RS_SETTINGS->readEntry("/LanguageCmd", "en");
#endif

    RS_SETTINGS->endGroup();
    
    RS_SYSTEM->loadTranslation(lang/*, langCmd*/);
    RS_DEBUG->print("main: loading translation: OK");

    RS_DEBUG->print("main: splashscreen..");
#ifdef QC_CUSTOM_SPLASH
    QPixmap* pixmap = new QPixmap(QC_CUSTOM_SPLASH); 
#else
#ifdef RS_CAM
    QPixmap* pixmap = new QPixmap(":/qcad/images/splash_camexpert.png");
#else
    QPixmap* pixmap = new QPixmap(":/qcad/images/splash_qcad.png");
#endif
#endif
    splash = new QSplashScreen(*pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Loading.."), 
        Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
    RS_DEBUG->print("main: splashscreen: OK");

#ifdef __APPLE__
    if (!styleGiven) {
        QApplication::setStyle(new QMacStyle());
    }
#endif

#ifdef Q_OS_WIN32
    if (!styleGiven) {
        QApplication::setStyle(new QWindowsStyle());
    }
#endif

#ifdef QC_BUILTIN_STYLE //js:
    RS_DEBUG->print("main: applying built in style..");
    applyBuiltinStyle();
#endif

    RS_DEBUG->print("main: creating main window..");
    QC_ApplicationWindow* appWin = new QC_ApplicationWindow();
    app.setAppWin(appWin);
    RS_DEBUG->print("main: setting caption");
    appWin->setWindowTitle(QC_APPNAME);
    RS_DEBUG->print("main: show main window");
    appWin->show();
    RS_DEBUG->print("main: creating main window: OK");

    if (splash) {
        RS_DEBUG->print("main: updating splash..");
        splash->showMessage(QObject::tr("Loading Library Paths.."), 
            Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
        RS_DEBUG->print("main: processing events");
        qApp->processEvents();
        RS_DEBUG->print("main: updating splash: OK");
    }

    // loading files:
    RS_DEBUG->print("main: loading files..");
    bool filesLoaded = appWin->getMdiWindow()!=NULL;
    for (QStringList::Iterator it = fileList.begin(); it != fileList.end(); 
        ++it ) {
        
        if (splash) {
            splash->showMessage(QObject::tr("Loading File %1..")
                .arg(QFileInfo(*it).fileName()), 
            Qt::AlignRight|Qt::AlignBottom, QC_SPLASH_TXTCOL);
            qApp->processEvents();
        }
        appWin->slotFileOpen(*it, RS2::FormatUnknown);
        filesLoaded = true;
    }
    RS_DEBUG->print("main: loading files: OK");
    
#ifdef RS_DEMO
    QTimer::singleShot(10 * 60 * 1000, appWin, SLOT(slotFileDemoQuit()));
#endif
        
# ifndef QC_DELAYED_SPLASH_SCREEN
    if (splash) {
        splash->finish(appWin);
        delete splash;
        splash = 0;
    }
# endif
    delete pixmap;
    
    RS_DEBUG->print("main: set focus");
    //appWin->setFocus();

    //app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    // renew: avoid . / , confusion on German environments
    //setlocale(LC_ALL, "C");
    
    // loading scripts:
    RS_DEBUG->print("main: loading scripts..");
    for (QStringList::Iterator it = scriptList.begin(); it != scriptList.end(); 
        ++it ) {
        
        RS_DEBUG->print("main: running script: %s", (const char*)(*it).toLatin1());
        appWin->slotRunScript(*it);
    }
    RS_DEBUG->print("main: loading scripts: OK");
    
    if (!filesLoaded) {
        RS_DEBUG->print("main: creating new empty drawing");
        appWin->slotFileNew();
        RS_DEBUG->print("main: creating new empty drawing: OK");
    }

    RS_DEBUG->print("main: running startup script");
    appWin->slotRunStartScript();
    RS_DEBUG->print("main: running startup script: OK");

#ifdef RS_DEMO
    RS_SETTINGS->beginGroup("/Demo");
    int numDemo = RS_SETTINGS->readNumEntry("/n", 0);
    RS_SETTINGS->writeEntry("/n", numDemo+1);
    RS_SETTINGS->endGroup();

    if (numDemo>600) {
        appWin->slotFileDemoQuit();
    }
#endif
    
    RS_DEBUG->print("main: app.exec()");
    int r = 0;
    r = app.exec();

    //delete appWin;
    RS_FileIO::deleteInstance();
    RS_FONTLIST->clearFonts();

    RS_DEBUG->print("main: finished: %d", r);
    RS_DEBUG->print("main: locale is: %s", getenv("LC_ALL"));
    RS_DEBUG->print("main: num locale is: %s", getenv("LC_NUMERIC"));

    return r;
}



/**
 * Handles command line arguments that might not require a GUI.
 *
 * @return list of files to load on startup.
 */
QStringList handleArgs(int argc, char** argv) {
    RS_DEBUG->print("main: handling args..");
    QStringList ret;

    bool doexit = false;
    QString machine;
    QString input;
    QString output;
    
    for (int i=1; i<argc; i++) {
        if (QString(argv[i]).startsWith("-")==false) {
            QString fname = QDir::convertSeparators( 
                QFileInfo(QFile::decodeName(argv[i])).absoluteFilePath() );
            ret.append(fname);
        }
        else if (QString(argv[i])=="--exit") {
            doexit = true;
        }
        else if (QString(argv[i])=="--script") {
            ++i;
        }
#ifdef RS_CAM
        else if (QString(argv[i])=="--convert") {
            ++i;
            if (i<argc) {
                machine = QString(argv[i]);
            }
            else {
                RS_DEBUG->print(RS_Debug::D_WARNING,
                    "No machine configuration given after --convert. " 
                    "Aborting..");
                exit(1);
            }
            ++i;
            if (i<argc) {
                input = QString(argv[i]);
            }
            else {
                RS_DEBUG->print(RS_Debug::D_WARNING,
                    "No input given after --convert. " 
                    "Aborting..");
                exit(1);
            }
            ++i;
            if (i<argc) {
                output = QString(argv[i]);
            }
            else {
                RS_DEBUG->print(RS_Debug::D_WARNING,
                    "No output given after --convert. " 
                    "Aborting..");
                exit(1);
            }
        }
#endif
    }

#ifdef RS_CAM
    // auto cam convert
    if (machine.isEmpty()==false && input.isEmpty()==false && 
        output.isEmpty()==false) {

        RS_FilterCam fc;
        RS_Graphic gr;
        RS_FILEIO->fileImport(gr, input, RS2::FormatUnknown);
        RS_CamDialog dlg(gr, NULL);
        dlg.activateMachineGenerator(machine);
        dlg.externalOK();
        fc.sort(gr);
        fc.fileExport(gr, output, RS2::FormatCam);
    }
#endif

    if (doexit) {
        exit(0);
    }
    
    RS_DEBUG->print("main: handling args: OK");
    return ret;
}



/**
 * Handles script related command line arguments.
 *
 * @return list of scripts to load on startup.
 */
QStringList handleScriptArgs(int argc, char** argv) {
    RS_DEBUG->print("main: handling script args..");
    QStringList ret;

    for (int i=1; i<argc; i++) {
#ifdef RS_SCRIPTING
        if (QString(argv[i])=="--script") {
            ++i;
            QString script;
            if (i<argc) {
                script = QString(argv[i]);
            }
            else {
                RS_DEBUG->print(RS_Debug::D_WARNING,
                    "No script given after --script. " 
                    "Aborting..");
            }
            ret.append(script);
        }
#endif
    }

    RS_DEBUG->print("main: handling script args: OK");
    return ret;
}

