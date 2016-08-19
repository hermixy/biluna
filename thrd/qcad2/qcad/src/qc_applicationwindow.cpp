/****************************************************************************
** $Id: qc_applicationwindow.cpp 10916 2008-08-04 20:34:06Z andrew $
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

#include "qc_applicationwindow.h"

//#include <QtAssistant/QAssistantClient>
#include <QDesktopServices>
#include <QPixmap>
#include <QStringList>
#include <QMouseEvent>
#include <QFrame>
#include <QKeyEvent>
#include <QMenu>
#include <QCloseEvent>
#include <QFileOpenEvent>
#include <QSplashScreen>

#include <fstream>

#include <qaction.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qnamespace.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qtimer.h>
#include <qtextstream.h>

#include <qdockwidget.h>
#include <qeventloop.h>
#include <qtextedit.h>

#include "rs_dialogfactory.h"
#include "rs_settings.h"
#include "rs_system.h"

#include "rs_actionblocksadd.h"
#include "rs_actionblocksattributes.h"
#include "rs_actionblockscreate.h"
#include "rs_actionblocksedit.h"
#include "rs_actionblockseditinsert.h"
#include "rs_actionblockshideall.h"
#include "rs_actionblocksshowall.h"
#include "rs_actionblocksinsert.h"
#include "rs_actionblocksremove.h"
#include "rs_actionblockstoggleview.h"
#include "rs_actiondebugentities.h"
#include "rs_actiondebugguiactions.h"
#include "rs_actiondimaligned.h"
#include "rs_actiondimangular.h"
#include "rs_actiondimdiametric.h"
#include "rs_actiondimleader.h"
#include "rs_actiondimlinear.h"
#include "rs_actiondimhorizontal.h"
#include "rs_actiondimvertical.h"
#include "rs_actiondimordinate.h"
#include "rs_actiondimradial.h"
#include "rs_actiontoolregeneratedimensions.h"
#include "rs_actiondrawarc3p.h"
#include "rs_actiondrawarc.h"
#include "rs_actiondrawarctangential.h"
#include "rs_actiondrawcircle2p.h"
#include "rs_actiondrawcircle3p.h"
#include "rs_actiondrawcirclecr.h"
#include "rs_actiondrawcircle.h"
#include "rs_actiondrawellipseaxis.h"
#include "rs_actiondrawellipsearcaxis.h"
#include "rs_actiondrawhatch.h"
#include "rs_actiondrawimage.h"
#include "rs_actiondrawlineangle.h"
#include "rs_actiondrawlinehorizontal.h"
#include "rs_actiondrawlinevertical.h"
#include "rs_actiondrawlinebisector.h"
#include "rs_actiondrawlinefree.h"
#include "rs_actiondrawline.h"
#include "rs_actiondrawlinehorvert.h"
#include "rs_actiondrawlineorthogonal.h"
#include "rs_actiondrawlineparallel.h"
#include "rs_actiondrawarcparallel.h"
#include "rs_actiondrawcircleparallel.h"
#include "rs_actiondrawlineparallelthrough.h"
#include "rs_actiondrawlinepolygon2.h"
#include "rs_actiondrawlinepolygon.h"
#include "rs_actiondrawlinerectangle.h"
#include "rs_actiondrawlinerelangle.h"
#include "rs_actiondrawlinetangent1.h"
#include "rs_actiondrawlinetangent2.h"
#include "rs_actiondrawpoint.h"
#include "rs_actiondrawspline.h"
#include "rs_actiondrawtext.h"
#include "rs_actioneditcopy.h"
#include "rs_actioneditcopyautoref.h"
#include "rs_actioneditcut.h"
#include "rs_actioneditcutautoref.h"
#include "rs_actioneditpaste.h"
#include "rs_actioneditredo.h"
#include "rs_actioneditundo.h"
#include "rs_actionfileclose.h"
#include "rs_actionfileexport.h"
#include "rs_actionfilenew.h"
#include "rs_actionfileopen.h"
#include "rs_actionfilepdf.h"
#include "rs_actionfilesvg.h"
#include "rs_actionfileprint.h"
#include "rs_actionfileprintpreview.h"
#include "rs_actionfilequit.h"
#include "rs_actionfilesaveas.h"
#include "rs_actionfilesave.h"
#include "rs_actioninfoangle.h"
#include "rs_actioninfoarea.h"
#include "rs_actioninfodist2.h"
#include "rs_actioninfodist.h"
#include "rs_actioninfoinside.h"
#include "rs_actioninfototallength.h"
#include "rs_actionlayersadd.h"
#include "rs_actionlayersshowall.h"
#include "rs_actionlayersedit.h"
#include "rs_actionlayershideall.h"
#include "rs_actionlayersremove.h"
#include "rs_actionlayerstogglelock.h"
#include "rs_actionlayerstoggleview.h"
#include "rs_actionlibraryinsert.h"
#include "rs_actionlockrelativezero.h"
#include "rs_actionmodifyattributes.h"
#include "rs_actionmodifytext.h"
#include "rs_actionmodifybevel.h"
#include "rs_actionmodifydelete.h"
#include "rs_actionmodifydivide2.h"
#include "rs_actionmodifydivide.h"
#include "rs_actionmodifyentity.h"
#include "rs_actionmodifyexplode.h"
#include "rs_actionmodifyexplodetext.h"
#include "rs_actionmodifyisoproject.h"
#include "rs_actionmodifymirror.h"
#include "rs_actionmodifymove.h"
#include "rs_actionmodifymoverotate.h"
#include "rs_actionmodifyrotate2.h"
#include "rs_actionmodifyrotate.h"
#include "rs_actionmodifyround.h"
#include "rs_actionmodifyscale.h"
#include "rs_actionmodifystretch.h"
#include "rs_actionmodifytoback.h"
#include "rs_actionmodifytofront.h"
#include "rs_actionmodifytrimamount.h"
#include "rs_actionmodifytrim.h"
#include "rs_actionmodifytrimboth.h"
#include "rs_actionoptionsdrawing.h"
#include "rs_actionoptionsgeneral.h"
#include "rs_actionreset.h"
#include "rs_actionscriptopenide.h"
#include "rs_actionselectall.h"
#include "rs_actiondeselectall.h"
#include "rs_actionselectcontour.h"
#include "rs_actionselectintersected.h"
#include "rs_actiondeselectintersected.h"
#include "rs_actionselectinvert.h"
#include "rs_actionselectlayer.h"
#include "rs_actionselectsingle.h"
#include "rs_actionselectwindow.h"
#include "rs_actiondeselectwindow.h"
#include "rs_actionsetrelativezero.h"
#include "rs_actionsetsnapmode.h"
#include "rs_actionsetsnaprestriction.h"
#include "rs_actionsnapintersectionmanual.h"
#include "rs_actiontoolregeneratedimensions.h"
#include "rs_actionviewdraft.h"
#include "rs_actionviewgrid.h"
#include "rs_actionviewstatusbar.h"
#include "rs_actionzoomauto.h"
#include "rs_actionzoomin.h"
#include "rs_actionzoomout.h"
#include "rs_actionzoompan.h"
#include "rs_actionzoomprevious.h"
#include "rs_actionzoomredraw.h"
#include "rs_actionzoomselection.h"
#include "rs_actionzoomwindow.h"
    
#include "rs_actionsetsnapmodefree.h"
#include "rs_actionsetsnapmodegrid.h"
#include "rs_actionsetsnapmodeendpoint.h"
#include "rs_actionsetsnapmodeonentity.h"
#include "rs_actionsetsnapmodecenter.h"
#include "rs_actionsetsnapmodemiddle.h"
#include "rs_actionsetsnapmodedist.h"
#include "rs_actionsetsnapmodeintersection.h"
#include "rs_actionsnapintersectionmanual.h"
#include "rs_actionsetsnapmodereference.h"
#include "rs_actionsetsnapmodeauto.h"
#include "rs_actionsetsnapmodecoordinate.h"
#include "rs_actionsetsnapmodecoordinatepolar.h"
#include "rs_actionsetsnaprestrictionnothing.h"
#include "rs_actionsetsnaprestrictionorthogonal.h"
#include "rs_actionsetsnaprestrictionhorizontal.h"
#include "rs_actionsetsnaprestrictionvertical.h"

#ifdef RS_ALGO
#include "rs_actionmodifytriangulate.h"
#include "rs_actionmodifydobooleanoperation.h"
#endif

#ifdef RS_PROF
#include "rs_actiondrawpolyline.h"
#include "rs_actionpolylineadd.h"
#include "rs_actionpolylineappend.h"
#include "rs_actionpolylinedel.h"
#include "rs_actionpolylinedelbetween.h"
#include "rs_actionpolylinetrim.h"
#include "rs_actionpolylineequidistant.h"
#include "rs_actionpolylinefromsegments.h"
#endif

#ifdef RS_CAM
#include "rs_camdialog.h"
#include "rs_simulationcontrols.h"
#include "rs_actioncamexportauto.h"
#include "rs_actioncamreorder.h"
#endif

#include "qg_blockwidget.h"
#include "qg_cadtoolbarmain.h"
#include "qg_dialogfactory.h"
#include "qg_dlgimageoptions.h"
#include "qg_filedialog.h"
#include "qg_layerwidget.h"
#include "qg_librarywidget.h"
#include "qg_pentoolbar.h"

#include "qg_aboutdialog.h"
#include "main.h"

QC_ApplicationWindow* QC_ApplicationWindow::appWindow = NULL;

#ifndef QC_APP_ICON
#define QC_APP_ICON "qcad.png"
#endif
#ifndef QC_APP_ICON16
#define QC_APP_ICON16 "qcad16.png"
#endif

extern QSplashScreen* splash;

/**
 * Constructor. Initializes the app.
 */
QC_ApplicationWindow::QC_ApplicationWindow()
    : QG_MainWindow()
{
    setAttribute(Qt::WA_DeleteOnClose);
    RS_DEBUG->print("QC_ApplicationWindow::QC_ApplicationWindow");

    appWindow = this;
//    assistant = NULL;
    
    RS_DEBUG->print("QC_ApplicationWindow::QC_ApplicationWindow: OK");
    
    RS_DEBUG->print("QC_ApplicationWindow::init: setting icon");
    setWindowIcon(QIcon(":/qcad/images/" QC_APP_ICON));
    
#ifdef RS_SCRIPTING
    RS_DEBUG->print("QC_ApplicationWindow::init: creating scripter");
    scripter = new QS_Scripter(this, this);
    RS_DEBUG->print("QC_ApplicationWindow::init: creating scripter: OK");
#endif
    
    RS_DEBUG->print("QC_ApplicationWindow::init: init status bar");
    initStatusBar();
    
    RS_DEBUG->print("QC_ApplicationWindow::init: init toolbar");
    initToolBar();
    
    RS_DEBUG->print("QC_ApplicationWindow::init: init view");
    initView();

    // requires toolbar and view (dock widgets for toolbars submenu of view menu)
    RS_DEBUG->print("QC_ApplicationWindow::init: init actions");
    initActions();

    // requires actions
    RS_DEBUG->print("QC_ApplicationWindow::init: init menu bar");
    initMenuBar();

    // requires menubar
    RS_DEBUG->print("QC_ApplicationWindow::init: init settings");
    restoreDocks();

    // Disable menu and toolbar items
    emit windowsChanged(false);

    show();
#ifdef QC_DELAYED_SPLASH_SCREEN
    splash->raise();
    qApp->processEvents();
    splash->clearMessage();
    QTimer::singleShot(1000*2, this, SLOT(finishSplashScreen()));
#endif

    //initSettings();
    
    slotMessage(QC_APPNAME " Ready");
}



/**
 * Destructor.
 */
QC_ApplicationWindow::~QC_ApplicationWindow() {
    RS_DEBUG->print("QC_ApplicationWindow::~QC_ApplicationWindow");

#ifdef RS_SCRIPTING
    delete scripter;
#endif

//    if (assistant!=NULL) {
//        delete assistant;
//    }

    RS_DEBUG->print("QC_ApplicationWindow::~QC_ApplicationWindow: OK");
}

    

/**
 * Runs the start script if scripting is available.
 */
void QC_ApplicationWindow::slotRunStartScript() {
    slotRunScript("autostart.qs");
    restoreDocks();
}



/**
 * Runs a script. The action that triggers this slot has to carry the
 * name of the script file.
 */
void QC_ApplicationWindow::slotRunScript() {
    RS_DEBUG->print("QC_ApplicationWindow::slotRunScript");

    const QObject* s = sender();
    if (s!=NULL) {
        QString script = ((QAction*)s)->text();
        RS_DEBUG->print("QC_ApplicationWindow::slotRunScript: %s", 
            (const char*)script.toLatin1());
        slotRunScript(script);
    }
}



/**
 * Runs the script with the given name.
 */
#ifdef RS_SCRIPTING
void QC_ApplicationWindow::slotRunScript(const QString& name) {
    RS_DEBUG->print("QC_ApplicationWindow::slotRunScript");

    if (scripter==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "QC_ApplicationWindow::slotRunScript: "
            "scripter not initialized");
        return;
    }


    if (name.startsWith('/')) {
        slotMessage(tr("Running script '%1'").arg(name));
        scripter->runScript(name, "main");
    }
    else {
        QStringList scriptList = RS_SYSTEM->getScriptList();
        scriptList.append(RS_SYSTEM->getHomeDir() + "/.qcad/" + name);
        
        for (QStringList::Iterator it = scriptList.begin(); it!=scriptList.end(); ++it) {
            RS_DEBUG->print("QC_ApplicationWindow::slotRunScript: "
                "checking script '%s'", (const char*)((*it).toLatin1()));
            QFileInfo fi(*it);
            if (fi.exists() && fi.fileName()==name) {
                RS_DEBUG->print("QC_ApplicationWindow::slotRunScript: running '%s'", 
                    (const char*)((*it).toLatin1()));
                slotMessage(tr("Running script '%1'").arg(*it));
                scripter->runScript(*it, "main");
            }
        }
    }
}
#else
void QC_ApplicationWindow::slotRunScript(const QString&) {}
#endif



/**
 * Called from toolbar buttons that were added by scripts to 
 * insert blocks.
 */
void QC_ApplicationWindow::slotInsertBlock() {
    const QObject* s = sender();
    if (s!=NULL) {
        QString block = ((QAction*)s)->text();
        RS_DEBUG->print("QC_ApplicationWindow::slotInsertBlock: %s", 
            (const char*)block.toLatin1());
        slotInsertBlock(block);
    }
}



/**
 * Called to insert blocks.
 *
 * @todo: move to qcad libraries
 */
void QC_ApplicationWindow::slotInsertBlock(const QString& name) {
    RS_DEBUG->print("QC_ApplicationWindow::slotInsertBlock: '%s'", (const char*)name.toLatin1());

    slotMessage(tr("Inserting block '%1'").arg(name));

    RS_GraphicView* graphicView = getGraphicView();
    RS_Document* document = getDocument();
    if (graphicView!=NULL && document!=NULL) {
        RS_ActionLibraryInsert* action = 
            new RS_ActionLibraryInsert(*document, *graphicView);
        action->setFile(name);
        graphicView->setCurrentAction(action);
    }
}



/**
 * Called when the splash screen has to terminate.
 */
void QC_ApplicationWindow::finishSplashScreen() {
    if (splash) {
        splash->finish(this);
        delete splash;
        splash = 0;
    }
}



/**
 * Initializes all QActions of the application.
 */
void QC_ApplicationWindow::initActions() {
    RS_DEBUG->print("QC_ApplicationWindow::initActions()");

    //RS_ActionFactory actionFactory(&actionHandler, this);
    QAction* action;
    //QAction* explodeAction;
    QMenu* menu;
    QToolBar* tb;
    QMenu* subMenu;
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): file");
    
    // Reset:
    //
    tb = new QToolBar(this);
    tb->setIconSize(QSize(16,16));
    tb->setFixedHeight(30);
    //tb->setMinimumHeight(20);
    //tb->setMaximumHeight(20);
    tb->setObjectName("toolbar_reset");
    tb->setWindowTitle(tr("Reset"));
    
    action = RS_ActionReset::createGuiAction();
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    addToolBar(Qt::TopToolBarArea, tb);

    // File actions:
    //
    menu = new QMenu(this);
    tb = new QToolBar(this);
    tb->setFixedHeight(30);
    tb->setIconSize(QSize(16,16));
    //tb->setFixedHeight(32);
    tb->setObjectName("toolbar_file");
    tb->setWindowTitle(tr("File"));

    action = RS_ActionFileNew::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileNew()));
    tb->addAction(action);
    menu->addAction(action);

    action = RS_ActionFileOpen::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileOpen()));
    menu->addAction(action);
    tb->addAction(action);

    action = RS_ActionFileSave::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileSave()));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionFileSaveAs::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileSaveAs()));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionFileExport::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileExport()));
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionFileSvg::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileSvg()));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionFilePdf::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFilePdf()));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->addSeparator();

    action = RS_ActionFileClose::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileClose()));
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addSeparator();

    action = RS_ActionFilePrint::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFilePrint()));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionFilePrintPreview::createGuiAction();
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotFilePrintPreview(bool)));
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(printPreviewChanged(bool)), action, SLOT(setChecked(bool)));
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    insertRecentFileActions(menu);

    menu->addSeparator();
    
    action = RS_ActionFileQuit::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotFileQuit()));
    menu->addAction(action);
    menu->addSeparator();
    menu->setTitle(tr("&File"));
    menuBar()->addMenu(menu);
    addToolBar(Qt::TopToolBarArea, tb);
    //fileMenu = menu;
    //updateRecentFileActions();
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): edit");

    // Editing actions:
    //
    menu=new QMenu(this);
    tb = new QToolBar(this);
    tb->setFixedHeight(30);
    tb->setIconSize(QSize(16,16));
    //tb->setMaximumHeight(20);
    tb->setObjectName("toolbar_edit");
    tb->setWindowTitle(tr("Edit"));

    action = RS_ActionEditUndo::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionEditRedo::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    tb->addSeparator();
    menu->addSeparator();

    action = RS_ActionEditCutAutoRef::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionEditCopyAutoRef::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionEditPaste::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    tb->addSeparator();
    menu->addSeparator();

    action = RS_ActionEditCut::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionEditCopy::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->addSeparator();

    action = RS_ActionOptionsGeneral::createGuiAction();
    connect(action, SIGNAL(triggered()), this, SLOT(slotOptionsGeneral()));
    menu->addAction(action);
    action = RS_ActionOptionsDrawing::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->setTitle(tr("&Edit"));
    menuBar()->addMenu(menu);
    addToolBar(Qt::TopToolBarArea, tb);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): view");

    // Viewing / Zooming actions:
    //
    menu = new QMenu(this);
    tb = new QToolBar(this);
    tb->setFixedHeight(30);
    tb->setIconSize(QSize(16,16));
    //tb->setMaximumHeight(20);
    tb->setObjectName("toolbar_view");
    tb->setWindowTitle(tr("View"));

    action = RS_ActionViewGrid::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    //action->setChecked(true);
    connect(this, SIGNAL(gridChanged(bool)), action, SLOT(setChecked(bool)));
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    RS_SETTINGS->beginGroup("/Appearance");
    RS_SETTINGS->writeEntry("/DraftMode", 0);
    RS_SETTINGS->endGroup();

    action = RS_ActionViewDraft::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionZoomRedraw::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionZoomIn::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    action = RS_ActionZoomOut::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionZoomAuto::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionZoomSelection::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionZoomPrevious::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionZoomWindow::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
    action = RS_ActionZoomPan::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->addSeparator();

    action = RS_ActionViewStatusBar::createGuiAction();
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotViewStatusBar(bool)));
    menu->addAction(action);
    action->setChecked(true);

    subMenu = createPopupMenu();
    subMenu->setTitle(tr("&Toolbars"));
    menu->addMenu(subMenu);

#ifdef RS_PROF
    action = new QAction(QIcon(":/qcad/images/editclear.png"),
                         tr("Focus on &Command Line"), this);
#ifndef Q_OS_MACX
    action->setShortcut(Qt::Key_Space);
#else
    action->setShortcut(Qt::CTRL + Qt::Key_M);
#endif
    connect(action, SIGNAL(triggered()),
            this, SLOT(slotFocusCommandLine()));
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
#endif

    menu->setTitle(tr("&View"));
    menuBar()->addMenu(menu);
    addToolBar(Qt::TopToolBarArea, tb);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): selecting");

    // Selecting actions:
    //
    menu=new QMenu(this);
    action = RS_ActionDeselectAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectSingle::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectContour::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDeselectWindow::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectWindow::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectInvert::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectIntersected::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDeselectIntersected::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSelectLayer::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&Select"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): drawing");

    // Drawing actions:
    //
    menu=new QMenu(this);

    // Points:
    subMenu=new QMenu(this);
    action = RS_ActionDrawPoint::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    subMenu->setTitle(tr("&Point"));
    menu->addMenu(subMenu);

    // Lines:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Line"));
    action = RS_ActionDrawLine::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineAngle::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineHorizontal::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineVertical::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineRectangle::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineBisector::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineParallel::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineParallelThrough::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineTangent1::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineTangent2::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineOrthogonal::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineRelAngle::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLinePolygon::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLinePolygon2::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawLineFree::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->addMenu(subMenu);

    // Arcs:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Arc"));
    action = RS_ActionDrawArc::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawArc3P::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawArcParallel::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawArcTangential::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addMenu(subMenu);
    // Circles:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Circle"));
    action = RS_ActionDrawCircle::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawCircleCR::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawCircle2P::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawCircle3P::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawCircleParallel::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addMenu(subMenu);
    
    // Ellipses:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Ellipse"));
    action = RS_ActionDrawEllipseAxis::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDrawEllipseArcAxis::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addMenu(subMenu);

    // Splines:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Spline"));
    action = RS_ActionDrawSpline::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addMenu(subMenu);
    
#ifdef RS_PROF
    // Polylines:
    subMenu=new QMenu(this);
    subMenu->setTitle(tr("&Polyline"));
    action = RS_ActionDrawPolyline::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineFromSegments::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineAdd::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineAppend::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineDel::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineDelBetween::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineTrim::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionPolylineEquidistant::createGuiAction();
    subMenu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addMenu(subMenu);
#endif
    
    // Text:
    action = RS_ActionDrawText::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    // Hatch:
    action = RS_ActionDrawHatch::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    // Image:
    action = RS_ActionDrawImage::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&Draw"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): dimension");

    // Dimensioning actions:
    //
#ifdef __APPLE__
    QMenu* m = menu;
#endif

    menu=new QMenu(this);
    menu->setTitle(tr("&Dimension"));
    action = RS_ActionDimAligned::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimLinear::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimHorizontal::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimVertical::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimOrdinate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimLeader::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimRadial::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimDiametric::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionDimAngular::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    RS_ActionToolRegenerateDimensions::createGuiAction();

#ifdef __APPLE__
    // under OS X, dimensions is a submenu of the create menu:
    m->addMenu(menu);
#else
    // under other OSs it's a separate menu
    menuBar()->addMenu(menu);
#endif
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): modify");

    // Modification actions:
    //
    menu=new QMenu(this);
    RS_DEBUG->print("RS2::ActionModifyMove");
    action = RS_ActionModifyMove::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyRotate");
    action = RS_ActionModifyRotate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyScale");
    action = RS_ActionModifyScale::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyMirror");
    action = RS_ActionModifyMirror::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyMoveRotate");
    action = RS_ActionModifyMoveRotate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyRotate2");
    action = RS_ActionModifyRotate2::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyTrim");
    action = RS_ActionModifyTrim::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyTrim2");
    action = RS_ActionModifyTrimBoth::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyTrimAmount");
    action = RS_ActionModifyTrimAmount::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyStretch");
    action = RS_ActionModifyStretch::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyBevel");
    action = RS_ActionModifyBevel::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyRound");
    action = RS_ActionModifyRound::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyDivide");
    action = RS_ActionModifyDivide::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyDivide2");
    action = RS_ActionModifyDivide2::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyEntity");
    action = RS_ActionModifyEntity::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyAttributes");
    action = RS_ActionModifyAttributes::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyDelete");
    action = RS_ActionModifyDelete::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionModifyExplode::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyExplodeText");
    action = RS_ActionModifyExplodeText::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyText");
    action = RS_ActionModifyText::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyToFront");
    action = RS_ActionModifyToFront::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyToBack");
    action = RS_ActionModifyToBack::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    RS_DEBUG->print("RS2::ActionModifyIsoProject");
    action = RS_ActionModifyIsoProject::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    
#ifdef RS_ALGO
    /*
    RS_DEBUG->print("RS2::ActionModifyTriangulate");
    action = RS_ActionModifyTriangulate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    */
    
    RS_DEBUG->print("RS2::ActionModifyDoBooleanOperation");
    action = RS_ActionModifyDoBooleanOperation::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
#endif
    menu->setTitle(tr("&Modify"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): snapping");

    // Snapping actions:
    //
    menu=new QMenu(this);
    action = RS_ActionSetSnapModeFree::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    //action->setChecked(true);
    action = RS_ActionSetSnapModeGrid::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeEndpoint::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeOnEntity::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeCenter::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeMiddle::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeDist::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeIntersection::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSnapIntersectionManual::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeReference::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeAuto::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action->setChecked(true);
    menu->addSeparator();
    action = RS_ActionSetSnapModeCoordinate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapModeCoordinatePolar::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addSeparator();
    action = RS_ActionSetSnapRestrictionNothing::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapRestrictionOrthogonal::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapRestrictionHorizontal::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionSetSnapRestrictionVertical::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->addSeparator();
    action = RS_ActionSetRelativeZero::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLockRelativeZero::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&Snap"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): info");

    // Info actions:
    //
    menu=new QMenu(this);
    action = RS_ActionInfoDist::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionInfoDist2::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionInfoAngle::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionInfoTotalLength::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionInfoArea::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&Info"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): layer");

    // Layer actions:
    //
    menu=new QMenu(this);
    action = RS_ActionLayersShowAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersHideAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersAdd::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersRemove::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersEdit::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersToggleView::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionLayersToggleLock::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&Layer"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): block");

    // Block actions:
    //
    menu=new QMenu(this);
    action = RS_ActionBlocksShowAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksHideAll::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksAdd::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksRemove::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksAttributes::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksEdit::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksEditInsert::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksInsert::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksToggleView::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionBlocksCreate::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionModifyExplode::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->setTitle(tr("&Block"));
    menuBar()->addMenu(menu);
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): toolbars: pen, tool options");

    addToolBarBreak();
    assert(QG_DIALOGFACTORY->getPenToolBar()!=NULL);
    addToolBar(Qt::TopToolBarArea, QG_DIALOGFACTORY->getPenToolBar());
    if (QG_DIALOGFACTORY->getOptionWidget()!=NULL) {
        addToolBar(Qt::TopToolBarArea, QG_DIALOGFACTORY->getOptionWidget());
    }
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): toolbars: pen, tool options: OK");

#ifdef RS_SCRIPTING
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): script");
    
    // Scripts menu:
    //
    scriptMenu = new QMenu(this);
    scriptOpenIDE = RS_ActionScriptOpenIde::createGuiAction();
    connect(scriptOpenIDE, SIGNAL(triggered()),
        this, SLOT(slotScriptOpenIDE()));
    scriptMenu->addAction(scriptOpenIDE);
    //scriptRun = actionFactory.createAction(RS2::ActionScriptRun, this);
    //scriptMenu->addAction(scriptRun);
#else
    scriptMenu = 0;
    scriptOpenIDE = 0;
    scriptRun = 0;
#endif

#ifdef RS_CAM
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): cam");
    // CAM menu:
    menu=new QMenu(this);
    action = RS_ActionCamExportAuto::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    action = RS_ActionCamReorder::createGuiAction();
    menu->addAction(action);
    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
    menu->setTitle(tr("&CAM"));
    menuBar()->addMenu(menu);
#endif

    RS_DEBUG->print("QC_ApplicationWindow::initActions(): help");
    
    // Help menu:
    //
    helpAboutApp = new QAction(QIcon(":/qcad/images/" QC_APP_ICON16), 
                               tr("&About %1").arg(QC_APPNAME), 
                               this);
    helpAboutApp->setStatusTip(tr("About the application"));
    connect(helpAboutApp, SIGNAL(triggered()),
            this, SLOT(slotHelpAbout()));

    helpManual = new QAction(QIcon(":/qcad/images/contents.png"), tr("&Manual"), this);
    helpManual->setShortcut(Qt::Key_F1);
    helpManual->setStatusTip(tr("Launch the online manual"));
    connect(helpManual, SIGNAL(triggered()),
            this, SLOT(slotHelpManual()));
    
    helpWebsite = new QAction(QIcon(":/qcad/images/web.png"), 
                               tr("Visit &Website"), 
                               this);
    helpWebsite->setStatusTip(tr("Visit the Biluna website"));
    connect(helpWebsite, SIGNAL(triggered()),
            this, SLOT(slotHelpWebsite()));
    
#ifdef RS_DEMO
    helpWebsiteOrder = new QAction(QIcon(":/qcad/images/web.png"), 
                               tr("&Order QCad Professional Now"), 
                               this);
    helpWebsiteOrder->setStatusTip(tr("Order QCad Professional from the RibbonSoft web site"));
    connect(helpWebsiteOrder, SIGNAL(triggered()),
            this, SLOT(slotHelpWebsiteOrder()));
#endif

    RS_ActionDebugEntities::createGuiAction();
    RS_ActionDebugGuiActions::createGuiAction();
    
    RS_DEBUG->print("QC_ApplicationWindow::initActions(): OK");
}


/**
 * Initializes the menu bar.
 */
void QC_ApplicationWindow::initMenuBar() {
    RS_DEBUG->print("QC_ApplicationWindow::initMenuBar()");

    insertWindowsMenu();

    // menuBar entry helpMenu
    helpMenu=new QMenu();
    helpMenu->addAction(helpManual);
    helpMenu->addSeparator();
    helpMenu->addAction(helpAboutApp);
    helpMenu->addAction(helpWebsite);
#ifdef RS_DEMO
    helpMenu->addAction(helpWebsiteOrder);
#endif

    // menuBar configuration
#ifdef RS_SCRIPTING
    scriptMenu->setTitle(tr("&Scripts"));
    menuBar()->addMenu(scriptMenu);
#endif    
    menuBar()->addSeparator();
    helpMenu->setTitle(tr("&Help"));
    menuBar()->addMenu(helpMenu);

    RS_DEBUG->print("QC_ApplicationWindow::initMenuBar(): OK");
}



/**
 * Initializes special tool bars (pen tool bar, tool options tool bar).
 */
void QC_ApplicationWindow::initToolBar() {
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar");
    
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 000");

    addToolBar(QG_DIALOGFACTORY->createPenToolBar(this));
    
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 001");

    addToolBar(QG_DIALOGFACTORY->createOptionWidget(this));
    
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 002");
    
    // CAD toolbar left:
    QToolBar* t = new QToolBar(this);
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 003");
    t->setObjectName("toolbar_cad");
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 004");
    t->setWindowTitle(tr("CAD Tools"));
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 005");
    addToolBar(Qt::LeftToolBarArea, t);
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: 006");

    t->addWidget(QG_DIALOGFACTORY->createCadToolBar(t));
    
    RS_DEBUG->print("QC_ApplicationWindow::initToolBar: OK");
}



/**
 * Initializes the status bar at the bottom.
 */
void QC_ApplicationWindow::initStatusBar() {
    RS_DEBUG->print("QC_ApplicationWindow::initStatusBar()");

    statusBar()->setFixedHeight(34);
    statusBar()->addWidget(
        QG_DIALOGFACTORY->createCoordinateWidget(statusBar()));
    statusBar()->addWidget(QG_DIALOGFACTORY->createMouseWidget(statusBar()));
    statusBar()->addWidget(QG_DIALOGFACTORY->createSelectionWidget(statusBar()));
    
    RS_DEBUG->print("QC_ApplicationWindow::initStatusBar(): OK");
}



/**
 * Initializes the view.
 */
void QC_ApplicationWindow::initView() {
    RS_DEBUG->print("QC_ApplicationWindow::initView");

    RS_DEBUG->print("init view..");
    QDockWidget* dw;

#ifdef RS_CAM
    RS_DEBUG->print("  simulation widget..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createSimulationControls(dw));
    dw->resize(240, 80);
    dw->setFixedHeight(80);
    dw->setWindowTitle(tr("Simulation Controls"));
    dw->setObjectName("dockwidget_simulation");
    addDockWidget(Qt::RightDockWidgetArea, dw);
    simulationDockWindow = dw;
#endif

    RS_DEBUG->print("  layer widget..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createLayerWidget(dw));
    dw->setWindowTitle(tr("Layer List"));
    dw->setObjectName("dockwidget_layerlist");
    addDockWidget(Qt::RightDockWidgetArea, dw);
    //layerDockWindow = dw;

    RS_DEBUG->print("  block widget..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createBlockWidget(dw));
    dw->setWindowTitle(tr("Block List"));
    dw->setObjectName("dockwidget_blocklist");
    dw->hide();
    addDockWidget(Qt::RightDockWidgetArea, dw);
    //blockDockWindow = dw;

    RS_DEBUG->print("  library widget..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createLibraryWidget(dw));
    dw->setWindowTitle(tr("Library Browser"));
    dw->setObjectName("dockwidget_librarybrowser");
    dw->hide();
    addDockWidget(Qt::RightDockWidgetArea, dw);
    //libraryDockWindow = dw;
    
#ifdef RS_PROPERTYEDITOR
    RS_DEBUG->print("  property editor..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createPropertyEditor(dw));
    dw->setWindowTitle(tr("Property Editor"));
    dw->setObjectName("dockwidget_propertyeditor");
    dw->hide();
    addDockWidget(Qt::RightDockWidgetArea, dw);
    //propertyEditorDockWindow = dw;
#endif

#ifdef RS_PROF
    RS_DEBUG->print("  command widget..");
    dw = new QDockWidget(this);
    dw->setWidget(QG_DIALOGFACTORY->createCommandWidget(dw));
    connect(this, SIGNAL(windowsChanged(bool)),
            QG_DIALOGFACTORY->getCommandWidget(), SLOT(setEnabled(bool)));
    dw->setMinimumHeight(30);
    dw->setWindowTitle(tr("Command Line"));
    dw->setObjectName("dockwidget_commandline");
    addDockWidget(Qt::BottomDockWidgetArea, dw);
#endif

    RS_DEBUG->print("QC_ApplicationWindow::initView: OK");
}

    

/**
 * Shows / hides the status bar.
 *
 * @param toggle true: show, false: hide.
 */
void QC_ApplicationWindow::slotViewStatusBar(bool toggle) {
    RS_DEBUG->print("QC_ApplicationWindow::slotViewStatusBar");

    if (toggle==false) {
        statusBar()->hide();
    } else {
        statusBar()->show();
    }
    RS_DEBUG->print("QC_ApplicationWindow::slotViewStatusBar: OK");
}



/**
 * Shows the dialog for general application preferences.
 */
void QC_ApplicationWindow::slotOptionsGeneral() {
    RS_DIALOGFACTORY->requestOptionsGeneralDialog();

    // update background color of all open drawings:
    RS_SETTINGS->beginGroup("/Appearance");
    QColor color(RS_SETTINGS->readEntry("/BackgroundColor", "#000000"));
    QColor gridColor(RS_SETTINGS->readEntry("/GridColor", "Gray"));
    QColor metaGridColor(RS_SETTINGS->readEntry("/MetaGridColor", "Darkgray"));
    QColor selectedColor(RS_SETTINGS->readEntry("/SelectedColor", "#A54747"));
    QColor highlightedColor(RS_SETTINGS->readEntry("/HighlightedColor",
                            "#739373"));
    bool showRulers = (RS_SETTINGS->readEntry("/ShowRulers", "1")=="1");
    RS_SETTINGS->endGroup();

    QWidgetList windows = workspace->windowList();
    for (int i = 0; i < int(windows.count()); ++i) {
        QG_MdiWindow* m = (QG_MdiWindow*)windows.at(i);
        if (m!=NULL) {
            QG_GraphicView* gv = m->getGraphicView();
            if (gv!=NULL) {
                gv->setBackground(color);
                gv->setGridColor(gridColor);
                gv->setMetaGridColor(metaGridColor);
                gv->setSelectedColor(selectedColor);
                gv->setHighlightedColor(highlightedColor);
                gv->updateGrid();
                gv->redraw();
                gv->showRulers(showRulers);
            }
        }
    }

}



/**
 * Menu script -> show ide
 */
void QC_ApplicationWindow::slotScriptOpenIDE() {
#ifdef RS_SCRIPTING
    scripter->openIDE();
#endif
}



/**
 * Menu script -> run
 */
void QC_ApplicationWindow::slotScriptRun() {
#ifdef RS_SCRIPTING
    scripter->runScript();
#endif
}



/**
 * Menu help -> about.
 */
void QC_ApplicationWindow::slotHelpAbout() {
    RS_DEBUG->print("QC_ApplicationWindow::slotHelpAbout()");

    QString edition;

#ifdef RS_DEMO
    //demo; // = " [Demo]";
#else
#ifdef RS_PROF
    edition = " [Professional]";
#else
    edition = " [Community Edition]";
#endif
#endif

    QStringList modules;

#ifdef RS_CAM
    modules += "CAM";
#endif

#ifdef RS_SCRIPTING
    modules += "Scripting";
#endif

#ifdef RS_ALGO
    modules += "Triangulation";
#endif

#ifdef RS_ARCH
    modules += "Architecture";
#endif

    QString modulesString;

    if (modules.empty()==false) {
        modulesString = modules.join(", ");
    } else {
        modulesString = tr("None");
    }

    QG_AboutDialog dlg(this);
    dlg.setWindowTitle(tr("About..."));
    QString dlgText(   QString("<qt>") +  // no center for main stream QCad
#ifdef QC_ABOUT_HEADER
        QString("<center>") + 
        QString(QC_ABOUT_HEADER) +
#else
        "<h2>"+ QC_APPNAME+ "</h2>" +
#endif
        tr("<b>Version:</b> %1").arg(QC_VERSION) + "<br>" +
        QString("%1").arg(edition) + "<br>" +
        tr("<b>Date:</b> %1").arg(__DATE__) + 
        // QDate::fromString(__DATE__, "MMM d yyyy").toString(Qt::ISODate)) +
        // "<br>" +
        // QString("(c) 2001-2008 by RibbonSoft GmbH,<br> Andrew Mustun") +
        "<br>" +
        QString("<a href=\"http://www.biluna.com\">http://www.biluna.com</a>") +
        "<br>" + 
        tr("<b>Modules:</b> <i>%1</i><br>").arg(modulesString)
#ifndef QC_ABOUT_NO_QT_VER
        + tr("<b>Qt Version:</b> %1").arg(qVersion())
#endif
#ifdef QC_ABOUT_ADD_COMPANY
        + QC_ABOUT_ADD_COMPANY
#endif
#ifdef QC_ABOUT_HEADER
        + QString("</center>")
#endif
    );
    RS_DEBUG->print("dlgText: %s", (const char*)dlgText.toLatin1());
    dlg.setText( dlgText );
#ifndef QC_ABOUT_HEADER
    dlg.setWindowIcon( QIcon(":/qcad/images/" QC_APP_ICON) );
#endif
    dlg.exec();
}



/**
 * Menu help -> help.
 */
void QC_ApplicationWindow::slotHelpManual() {
    RS_DEBUG->print("QC_ApplicationWindow::slotHelpManual()");
    // Temporary solution
    QDesktopServices::openUrl(QUrl("http://www.biluna.com/bilunacad"));

//    if (assistant==NULL) {
//        RS_DEBUG->print("QC_ApplicationWindow::slotHelpManual(): appdir: %s",
//                        (const char*)RS_SYSTEM->getAppDir().toLatin1());
//        RS_DEBUG->print("QC_ApplicationWindow::slotHelpManual(): appdir: %s",
//                        (const char*)RS_SYSTEM->getAppDir().toLatin1());
//#ifdef Q_OS_WIN32
//        assistant = new QAssistantClient(RS_SYSTEM->getAppDir()+"/bin/assistant.exe", this);
//#else
//        assistant = new QAssistantClient(RS_SYSTEM->getAppDir()+"/bin/assistant", this);
//#endif
//        connect(assistant, SIGNAL(error(const QString&)),
//            this, SLOT(slotError(const QString&)));
//        QStringList args;
//        args << "-profile";
//        args << QDir::convertSeparators(RS_SYSTEM->getDocPath() + "/index.adp");
//        args << "-resourceDir";
//        args << QDir::convertSeparators(RS_SYSTEM->getAppDir() + "/qm");
//
//        assistant->setArguments(args);
//    }
//
//    assistant->openAssistant();
//
//    // no effect:
//    //assistant->showPage("index.html");
//
//    /*
//    QProcess* process = new QProcess(this);
//    process->start(RS_SYSTEM->getAppDir()+"/bin/assistant", QStringList() << QLatin1String("-enableRemoteControl"));
//    if (!process->waitForStarted()) {
//        slotError("Could not start the Help browser.");
//        return;
//    }
//    */
//
//    // show index page
//    /*
//    QTextStream str(process);
//    str << QLatin1String("SetSource qthelp://mycompany.com/doc/index.html")
//        << QLatin1Char('\0') << endl;
//    */
}




/**
 * Menu help -> web site.
 */
void QC_ApplicationWindow::slotHelpWebsite() {
    RS_DEBUG->print("QC_ApplicationWindow::slotHelpWebsite");
#ifdef RS_CAM
    QDesktopServices::openUrl(QUrl("http://www.biluna.com/bilunacad"));
#else
    QDesktopServices::openUrl(QUrl("http://www.biluna.com/bilunacad"));
#endif
}


/**
 * Menu help -> order.
 */
void QC_ApplicationWindow::slotHelpWebsiteOrder() {
    RS_DEBUG->print("QC_ApplicationWindow::slotHelpWebsiteOrder");
#ifdef RS_CAM
    QDesktopServices::openUrl(QUrl("http://www.biluna.com"));
#else
    QDesktopServices::openUrl(QUrl("http://www.biluna.com"));
#endif
}


