/****************************************************************************
** $Id: qc_applicationwindow.h 8340 2008-01-20 17:28:31Z andrew $
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

#ifndef QC_APPLICATIONWINDOW_H
#define QC_APPLICATIONWINDOW_H


#include <QAction>
#include <QCloseEvent>
#include <qdockwidget.h>
#include <QKeyEvent>
#include <qlayout.h>
#include <qmenubar.h>
#include <qmenu.h>
#include <QMouseEvent>
#include <QSignalMapper>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>

#include "rs_document.h"
//#include "rs_actionfactory.h"

#include "main.h"

#include "qg_mainwindow.h"
#include "qg_mdiwindow.h"
#include "qg_cadtoolbar.h"
#ifdef RS_PROF
#include "qg_commandwidget.h"
#else
class QG_CommandWidget;
#endif

#ifdef RS_SCRIPTING
#include "qs_scripter.h"
#include <qsproject.h>
#endif

#ifdef RS_CAM
#include "rs_simulationcontrols.h"
#endif

//class QAssistantClient;

class QG_DialogFactory;
class QG_CoordinateWidget;
class QG_MouseWidget;
class QG_SelectionWidget;


/**
 * Main application window. Hold together document, view and controls.
 *
 * @author Andrew Mustun
 */
class QC_ApplicationWindow: public QG_MainWindow {
    Q_OBJECT

public:
    QC_ApplicationWindow();
    ~QC_ApplicationWindow();

    void initActions();
    void initMenuBar();
    void initToolBar();
    void initStatusBar();

    void initView();
    
    virtual QString getApplicationName() {
        return QC_APPNAME;
    }

public slots:
    void finishSplashScreen();

    void slotViewStatusBar(bool toggle);

    void slotOptionsGeneral();
    
    void slotScriptOpenIDE();
    void slotScriptRun();
    
    void slotRunStartScript();
    void slotRunScript();
    void slotRunScript(const QString& name);
    
    void slotInsertBlock();
    void slotInsertBlock(const QString& name);

    void slotHelpAbout();
    void slotHelpManual();
    void slotHelpWebsite();
    void slotHelpWebsiteOrder();

public:
    /**
     * Implementation from QG_MainWindow.
     */
#ifdef RS_CAM
    virtual void showSimulationControls() {
        simulationDockWindow->show();
    }
#endif

    /**
     * @return Pointer to the qsa object.
     */
#ifdef RS_SCRIPTING
    QSProject* getQSAProject() {
        if (scripter!=NULL) {
            return scripter->getQSAProject();
        }
        else {
            return NULL;
        }
    }
#endif

private:
    /** Pointer to the application window (this). */
    static QC_ApplicationWindow* appWindow;

    /** Layer list dock widget */
    //QDockWidget* layerDockWindow;
    /** Block list dock widget */
    //QDockWidget* blockDockWindow;
    /** Library list dock widget */
    //QDockWidget* libraryDockWindow;
#ifdef RS_CAM
    /** Simulation controls dock widget */
    QDockWidget* simulationDockWindow;
#endif

#ifdef RS_SCRIPTING
    /** Scripting interface. */
    QS_Scripter* scripter;
#endif

    QMenu* scriptMenu;
    QMenu* helpMenu;

//    QAssistantClient* assistant;

    QAction* scriptOpenIDE;
    QAction* scriptRun;
    
    QAction* helpAboutApp;
    QAction* helpManual;
    QAction* helpWebsite;
    QAction* helpWebsiteOrder;
};


#endif

