/****************************************************************************
** $Id: qc_mdiwindow.cpp 3254 2006-05-11 00:09:29Z andrew $
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

#include "qg_mainwindow.h"

#include "QAction"
#include "QImageWriter"
#include "QMenu"
#include "QMenuBar"
#include "QMessageBox"
#include "QPrintDialog"
#include "QPrintEngine"
#include "QSignalMapper"
#include "QStatusBar"
#include "QTime"

#include "rs_actionfileprintpreview.h"
#include "rs_application.h"
#include "rs_settings.h"
#include "rs_staticgraphicview.h"
#include "rs_system.h"
#include "rs_filtersvg.h"

#include "rs_actionsetsnapmode.h"
#include "rs_actionsetsnaprestriction.h"
#include "rs_actionlockrelativezero.h"

#include "qg_blockwidget.h"
#include "qg_dialogfactory.h"
#include "qg_filedialog.h"
#include "qg_layerwidget.h"
#include "qg_pentoolbar.h"
#include "qg_dlgimageoptions.h"

#ifdef RS_CAM
#include "rs_simulationcontrols.h"
#endif

/**
 * Constructor.
 */
QG_MainWindow::QG_MainWindow() 
    : /*actionHandler(this), */
    workspace(NULL),
    windowsMenu(NULL),
    separatorActRecentFiles(NULL) {

    RS_DEBUG->print("QG_MainWindow::QG_MainWindow");
    
    for (int i=0; i<QG_MAX_RECENT_FILES; ++i) {
        recentFileActions[i] = NULL;
    }
    
    QG_DialogFactory::init(this);
    QG_DIALOGFACTORY->setMainWindow(this);

    initMdi();
    restoreSettings();
    
    RS_DEBUG->print("QG_MainWindow::QG_MainWindow: OK");
}



/**
 * Inserts a menu for MDI Windows handling to the menu bar.
 */
void QG_MainWindow::insertWindowsMenu() {
    windowsMenu = menuBar()->addMenu(tr("&Window"));
    connect(windowsMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotWindowsMenuAboutToShow()));
}
    


/**
 * Creates the workspace and actions for handling MDI windows.
 */
void QG_MainWindow::initMdi() {
    RS_DEBUG->print("QG_MainWindow::initMdi");

    workspace = new QMdiArea;
    workspace->setScrollBarsEnabled(true);
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            workspace, SLOT(setActiveWindow(QWidget*)));
                            
    //workspace->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, 
    //    QSizePolicy::MinimumExpanding));
    workspace->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, 
        QSizePolicy::Preferred));
    setCentralWidget(workspace);

    connect(workspace, SIGNAL(windowActivated(QWidget*)),
            this, SLOT(slotWindowActivated(QWidget*)));
    
    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            workspace, SLOT(closeAllWindows()));
    connect(this, SIGNAL(windowsChanged(bool)),
            closeAllAct, SLOT(setEnabled(bool)));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), workspace, SLOT(tile()));
    connect(this, SIGNAL(windowsChanged(bool)),
            tileAct, SLOT(setEnabled(bool)));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), workspace, SLOT(cascade()));
    connect(this, SIGNAL(windowsChanged(bool)),
            cascadeAct, SLOT(setEnabled(bool)));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),
            workspace, SLOT(activateNextWindow()));
    connect(this, SIGNAL(windowsChanged(bool)),
            nextAct, SLOT(setEnabled(bool)));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            workspace, SLOT(activatePreviousWindow()));
    connect(this, SIGNAL(windowsChanged(bool)),
            previousAct, SLOT(setEnabled(bool)));

    separatorActWin = new QAction(this);
    separatorActWin->setSeparator(true);

    RS_DEBUG->print("QG_MainWindow::initMdi: OK");
}



/**
 * Stores the global application settings to file or registry.
 */
void QG_MainWindow::storeSettings() {
    RS_DEBUG->print("QG_MainWindow::storeSettings()");

    RS_SETTINGS->beginGroup("/Geometry");
    RS_SETTINGS->writeEntry("/WindowWidth", width());
    RS_SETTINGS->writeEntry("/WindowHeight", height());
    RS_SETTINGS->writeEntry("/WindowX", x());
    RS_SETTINGS->writeEntry("/WindowY", y());
    RS_SETTINGS->setValue("/DockWindows" QT_VERSION_STR, saveState());
    RS_SETTINGS->endGroup();

    RS_DEBUG->print("QG_MainWindow::storeSettings(): OK");
}



/**
 * Restores the global application settings from the 
 * config file (unix, mac) or registry (windows).
 */
void QG_MainWindow::restoreSettings() {
    RS_SETTINGS->beginGroup("/Geometry");
    int windowWidth = RS_SETTINGS->readNumEntry("/WindowWidth", 950);
    int windowHeight = RS_SETTINGS->readNumEntry("/WindowHeight", 700);
    int windowX = RS_SETTINGS->readNumEntry("/WindowX", 0);
    int windowY = RS_SETTINGS->readNumEntry("/WindowY", 30);
    RS_SETTINGS->endGroup();

#ifdef Q_OS_MAC
    if (windowY<30) {
        windowY=30;
    }
#endif

    resize(windowWidth, windowHeight);
    move(windowX, windowY);
}


/**
 * Restores the position of the dock windows.
 */
void QG_MainWindow::restoreDocks() {
    RS_DEBUG->print("QG_MainWindow::restoreDocks");
    
    RS_SETTINGS->beginGroup("/Geometry");
    QVariant docks = RS_SETTINGS->value("/DockWindows" QT_VERSION_STR, "");
    RS_SETTINGS->endGroup();

    restoreState(docks.toByteArray());

    RS_DEBUG->print("QG_MainWindow::restoreDocks: OK");
}





/**
 * Handles hotkeys. Don't let it to the default handler of Qt.
 * it will consume them also if a text field is active
 * which means it's impossible to enter a command.
 */
void QG_MainWindow::keyPressEvent(QKeyEvent* e) {

    // timer
    static QTime ts = QTime();
    static QString firstKey = "";

    // single key codes:
    switch (e->key()) {
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Meta:
    case Qt::Key_Alt:
    case Qt::Key_CapsLock: {
            QMainWindow::keyPressEvent(e);

            // forward to actions:
            RS_GraphicView* graphicView = getGraphicView();
            if (graphicView!=NULL) {
                graphicView->keyPressEvent(e);
            }
            e->accept();
        }
        break;

    case Qt::Key_Escape:
        firstKey = "";
        slotBack();
        e->accept();
        break;

    case Qt::Key_Return:
        if (firstKey.isEmpty()) {
            slotEnter();
            e->accept();
        }
        break;

    /*
    case Qt::Key_Plus:
        if (firstKey.isEmpty()) {
            actionHandler.slotZoomIn();
            e->accept();
        }
        break;

    case Qt::Key_Minus:
        if (firstKey.isEmpty()) {
            actionHandler.slotZoomOut();
            e->accept();
        }
        break;
    */

    default:
        e->ignore();
        break;
    }

    if (e->isAccepted()) {
        return;
    }

    QTime now = QTime::currentTime();

    // multi key codes:
    if (ts.msecsTo(now)<2000) {
        QString code =
            QString("%1%2").arg(firstKey).arg(QChar(e->key())).toLower();

        //if (actionHandler.keycode(code)==false && RS_GuiAction::triggerByKeycode(code)==false) {
        if (RS_GuiAction::triggerByKeycode(code)==false) {
            ts = now;
            if (QChar(e->key()).isPrint()) {
                firstKey += e->key();
            }
        }
        else {
            firstKey="";
        }
    }
    else {
        ts = now;
        if (QChar(e->key()).isPrint()) {
            firstKey = e->key();
            // 20071113: enable one key keycodes
            if (RS_GuiAction::triggerByKeycode(e->text())==true) {
                firstKey="";
            }
        }
    }

    QMainWindow::keyPressEvent(e);
}



/**
 * Handles hotkeys. Don't let it to the default handler of Qt.
 * it will consume them also if a text field is active
 * which means it's impossible to enter a command.
 */
void QG_MainWindow::keyReleaseEvent(QKeyEvent* e) {

    switch (e->key()) {
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Meta:
    case Qt::Key_Alt:
    case Qt::Key_CapsLock: {
            QMainWindow::keyReleaseEvent(e);

            // forward to actions:
            RS_GraphicView* graphicView = getGraphicView();
            if (graphicView!=NULL) {
                graphicView->keyReleaseEvent(e);
            }
            e->accept();
        }
        break;
    }

    QMainWindow::keyReleaseEvent(e);
}



/**
 * Handles right-clicks for moving back to the last cad tool bar.
 */
void QG_MainWindow::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::RightButton && QG_DIALOGFACTORY->getCadToolBar()!=NULL) {
        QG_DIALOGFACTORY->getCadToolBar()->showToolBarMain();
    }
    e->accept();
}
    
    

/**
 * Shows the given message in the status bar and appends it to the message
 * history in the command widget if available.
 */
void QG_MainWindow::slotMessage(const QString& text) {

    statusBar()->showMessage(text, 2000);
    
    if (QG_DIALOGFACTORY->getCommandWidget()!=NULL) {
        QG_DIALOGFACTORY->getCommandWidget()->appendHistory(text);
    }
}

/**
 * Shows the given error on the command line.
 */
void QG_MainWindow::slotError(const QString& msg) {
    if (QG_DIALOGFACTORY->getCommandWidget()!=NULL) {
        QG_DIALOGFACTORY->getCommandWidget()->appendHistory(msg);
    }
}


/**
 * Creates a new MDI window with the given document or a new
 *  document if 'doc' is NULL.
 */
QG_MdiWindow* QG_MainWindow::slotFileNew(RS_Document* doc) {
    RS_DEBUG->print("QG_MainWindow::slotFileNew");

    static int id = 0;
    id++;

    slotMessage(tr("Creating new file..."));

    QG_MdiWindow* w = new QG_MdiWindow(doc, workspace, this);
    workspace->addWindow(w);
    connect(w, SIGNAL(signalClosing()),
            this, SLOT(slotFileClosing()));

    if (w->getDocument()->rtti()==RS2::EntityBlock) {
        w->setWindowTitle(tr("Block '%1'").arg(((RS_Block*)(w->getDocument()))->getName()));
    } else {
        w->setWindowTitle(tr("unnamed document %1").arg(id));
    }
    w->setWindowIcon(QIcon(":/qcad/images/document.png"));
        
    // only graphics offer block lists, blocks don't
    RS_Graphic* graphic = w->getDocument()->getGraphic();
    if (graphic!=NULL) {
        RS_DEBUG->print("QG_MainWindow::slotFileNew: penToolBar address: %d", (int)QG_DIALOGFACTORY->getPenToolBar());
        RS_DEBUG->print("QG_MainWindow::slotFileNew: layerWidget address: %d", (int)QG_DIALOGFACTORY->getLayerWidget());
        /*
        graphic->addLayerListListener((RS_LayerListListener*)QG_DIALOGFACTORY->getPenToolBar());
        graphic->addLayerListListener((RS_LayerListListener*)QG_DIALOGFACTORY->getLayerWidget());
        */
        /*
        RS_PtrList<RS_LayerListListener> layerListListeners;
        layerListListeners = QG_DIALOGFACTORY->getLayerWidgets();
        for (int i=0; i<layerListListeners.size(); ++i) {
            //layerListListeners[i]->layerListChanged();
            graphic->addLayerListListener(layerListListeners[i]);
        }
        */
        /*
        for (int i=0; QG_DIALOGFACTORY->getLayerWidget(i)!=NULL; ++i) {
            graphic->addLayerListListener(QG_DIALOGFACTORY->getLayerWidget(i));
        }
        */
        /*
        connect(graphic->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*)), 
            QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotRedraw()));
        */
        //reconnectWidgets(w);

        RS_DEBUG->print("QG_MainWindow::slotFileNew: handle block list listener");
        graphic->addBlockListListener(QG_DIALOGFACTORY->getBlockWidget());
        graphic->activateBlock(NULL);
        RS_DEBUG->print("QG_MainWindow::slotFileNew: handle block list listener: OK");
    }

    if (workspace->windowList().count()<=1) {
        w->showMaximized();
    } else {
        w->show();
    }

    //QG_DIALOGFACTORY->updateUiConnections();
    slotWindowActivated(w);

    slotMessage(tr("New Drawing created."));

    setFocus();
    
    RS_DEBUG->print("QG_MainWindow::slotFileNew: OK");
    
    return w;
}



/**
 * Called when a MDI window is actually about to close. Used to 
 * detach widgets from the document.
 */
void QG_MainWindow::slotFileClosing() {
    RS_DEBUG->print("QG_MainWindow::slotFileClosing");

    /*
    if (QG_DIALOGFACTORY->getPenToolBar()!=NULL) {
        QG_DIALOGFACTORY->getPenToolBar()->setLayerList(NULL);
    }
    */
    //QG_DIALOGFACTORY->setLayerList(NULL);
    /*
    if (QG_DIALOGFACTORY->getLayerWidget()!=NULL) {
        QG_DIALOGFACTORY->getLayerWidget()->setLayerList(NULL);
    }
    */
    /*
    for (int i=0; i<QG_DIALOGFACTORY->getLayerWidgets().size(); ++i) {
        QG_DIALOGFACTORY->getLayerWidgets().at(i)->setLayerList(NULL);
    }
    */
    if (QG_DIALOGFACTORY->getBlockWidget()!=NULL) {
        QG_DIALOGFACTORY->getBlockWidget()->setBlockList(NULL);
    }
    if (QG_DIALOGFACTORY->getCoordinateWidget()!=NULL) {
        QG_DIALOGFACTORY->getCoordinateWidget()->setGraphic(NULL);
    }
    
#ifdef RS_CAM
    if (QG_DIALOGFACTORY->getSimulationControls()!=NULL) {
        QG_DIALOGFACTORY->getSimulationControls()->setGraphicView(NULL);
    }
#endif

    // 20080124: NULL will not update blocklist, etc.
    slotWindowActivated(NULL);
    //slotWindowActivated(getMdiWindow());
    
    RS_DEBUG->print("QG_MainWindow::slotFileClosing: OK");
}



/**
 * Menu file -> open.
 */
QG_MdiWindow* QG_MainWindow::slotFileOpen() {
    RS_DEBUG->print("QG_MainWindow::slotFileOpen()");

    QG_MdiWindow* ret = NULL;

    RS2::FormatType type = RS2::FormatUnknown;

    QStringList fileNames = QG_FileDialog::getOpenFileNames(this, &type);
    
    for (int i=0; i<fileNames.count(); ++i) {
        ret = slotFileOpen(fileNames.at(i), type);
    }

    return ret;
}



/**
 * Opens the given file.
 */
QG_MdiWindow* QG_MainWindow::slotFileOpen(const QString& fileName,
                                 RS2::FormatType type) {

    RS_DEBUG->print("QG_MainWindow::slotFileOpen");

    if (fileName.isEmpty()) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "QG_MainWindow::slotFileOpen: "
            "empty file name given");
        slotMessage(tr("Opening aborted"));
        return NULL;
    }

    RS_DEBUG->print("QG_MainWindow::slotFileOpen: creating new doc window");
    // Create new document window:
    QG_MdiWindow* w = slotFileNew();
    if (w==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "QG_MainWindow::slotFileOpen: "
            "cannot create new MDI window");
        return NULL;
    }

    RS_APP->processEvents(QEventLoop::AllEvents, 1000);

    RS_DEBUG->print("QG_MainWindow::slotFileOpen: linking layer list");
    
    // link the block widget to the new document:
    if (QG_DIALOGFACTORY->getBlockWidget()!=NULL) {
        QG_DIALOGFACTORY->getBlockWidget()->setBlockList(
            w->getDocument()->getBlockList());
    }
    // link coordinate widget to graphic
    if (QG_DIALOGFACTORY->getCoordinateWidget()!=NULL) {
        QG_DIALOGFACTORY->getCoordinateWidget()->setGraphic(w->getGraphic());
    }
#ifdef RS_CAM
    // link the cam simulation widget to the new document:
    if (QG_DIALOGFACTORY->getSimulationControls()!=NULL) {
        QG_DIALOGFACTORY->getSimulationControls()->setGraphicView(
            w->getGraphicView());
    }
#endif

    RS_DEBUG->print("QG_MainWindow::slotFileOpen: open file");
    
    RS_APP->processEvents(QEventLoop::AllEvents, 1000);
    
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // open the file in the new view:
    if (w->slotFileOpen(fileName, type)==false) {
        // error
        QApplication::restoreOverrideCursor();
        QMessageBox::information(this, QMessageBox::tr("Warning"),
                                 tr("Cannot open the file\n%1\nPlease "
                                    "check your access rights, the file format and file extension.")
                                 .arg(fileName),
                                 QMessageBox::Ok);
        w->setForceClosing(true);
        w->close();
        QApplication::restoreOverrideCursor();
        return NULL;
    }

    if (type==RS2::FormatDxf1) {
        setCurrentFile(fileName + " (DXF 1)");
    } else {
        setCurrentFile(fileName);
    }

    // update caption:
    QString stripped = QFileInfo(fileName).fileName();
    w->setWindowTitle(stripped);

    // update coordinate widget format:
    RS_DIALOGFACTORY->updateCoordinateWidget(RS_Vector(0.0,0.0),
            RS_Vector(0.0,0.0),
            true);
        
    emit(gridChanged(w->getGraphic()->isGridOn()));
    
    //w->getGraphic()->activateLayer("0");

    QString message=tr("Loaded document: %1").arg(fileName);

    slotMessage(message);

    QApplication::restoreOverrideCursor();
    
    RS_DEBUG->print("QG_MainWindow::slotFileOpen: OK");

    return w;
}



/**
 * Menu file -> save.
 */
bool QG_MainWindow::slotFileSave() {
    RS_DEBUG->print("QG_MainWindow::slotFileSave()");

    bool ret = true;

    slotMessage(tr("Saving drawing..."));

    QG_MdiWindow* w = getMdiWindow();
    QString name;
    if (w!=NULL) {
        if (w->getDocument()->getFilename().isEmpty()) {
            return slotFileSaveAs();
        } else {
            bool cancelled;
            if (w->slotFileSave(cancelled)) {
                if (!cancelled) {
                    name = w->getDocument()->getFilename();
                }
                else {
                    ret = false;
                }
            } else {
                // error
                QMessageBox::information(
                    this, QMessageBox::tr("Warning"),
                    tr("Cannot save the file\n%1\nPlease "
                       "check the permissions.")
                    .arg(w->getDocument()->getFilename()),
                    QMessageBox::Ok);
                ret = false;
            }
        }
    }

    QString message;
    if (ret) {
        message = tr("Saved drawing: %1").arg(name);
    }
    else {
        message = tr("Drawing %1 was NOT saved.").arg(name);
    }

    slotMessage(message);

    return ret;
}



/**
 * Menu file -> save as.
 */
bool QG_MainWindow::slotFileSaveAs(const QString& caption) {
    RS_DEBUG->print("QG_MainWindow::slotFileSaveAs()");

    bool ret = true;

    slotMessage(tr("Saving drawing under new filename..."));

    QG_MdiWindow* w = getMdiWindow();
    QString name;
    if (w!=NULL) {
        bool cancelled;
        if (w->slotFileSaveAs(cancelled, caption)) {
            if (!cancelled) {
                name = w->getDocument()->getFilename();
                setCurrentFile(name);
                w->setWindowTitle(name);
            }
            else {
                ret = false;
            }
        } else {
            // error
            QMessageBox::information(
                this, QMessageBox::tr("Warning"),
                tr("Cannot save the file\n%1\nPlease "
                   "check the permissions.")
                .arg(w->getDocument()->getFilename()),
                QMessageBox::Ok);
            ret = false;
        }
    }

    QString message;
    if (ret) {
        message = tr("Saved drawing: %1").arg(name);
    }
    else {
        message = tr("Drawing %1 was NOT saved.").arg(name);
    }
    
    slotMessage(message);

    return ret;
}



/**
 * Sets the current file name. This updates the windows title and
 * the recent file menu.
 */
void QG_MainWindow::setCurrentFile(const QString& fileName) {
    RS_DEBUG->print("QG_MainWindow::setCurrentFile: %s", 
        (const char*)fileName.toLatin1());
    QString curFile = fileName;
    if (curFile.isEmpty()) {
        setWindowTitle(getApplicationName());
    }
    else {
        QString stripped = QFileInfo(curFile).fileName();

        QG_MdiWindow* m = getMdiWindow();
        if (m!=NULL) {
            m->setWindowTitle(stripped);
        }
        //setWindowTitle(tr("%1 - [%2]").arg(getApplicationName())
        //                            .arg(stripped));
    }
    
    QStringList files = RS_SETTINGS->value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > QG_MAX_RECENT_FILES) {
        files.removeLast();
    }

    RS_SETTINGS->setValue("recentFileList", files);
    updateRecentFileActions();
    RS_DEBUG->print("QG_MainWindow::setCurrentFile: OK");
}



/**
 * Updates the recent file list in the file menu.
 */
void QG_MainWindow::updateRecentFileActions() {
    RS_DEBUG->print("QG_MainWindow::updateRecentFileActions");

    QStringList files = RS_SETTINGS->value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), QG_MAX_RECENT_FILES);

    for (int i=0; i<numRecentFiles; ++i) {
        if (recentFileActions[i]!=NULL) {
            QString stripped = QFileInfo(files[i]).fileName();
            QString text = tr("&%1 %2").arg(i + 1).arg(stripped);
            recentFileActions[i]->setText(text);
            recentFileActions[i]->setStatusTip(files[i]);
            recentFileActions[i]->setData(files[i]);
            recentFileActions[i]->setVisible(true);
        }
    }
    for (int i=numRecentFiles; i<QG_MAX_RECENT_FILES; ++i) {
        if (recentFileActions[i]!=NULL) {
            recentFileActions[i]->setVisible(false);
        }
    }

    if (separatorActRecentFiles!=NULL) {
        separatorActRecentFiles->setVisible(numRecentFiles > 0);
    }
    
    RS_DEBUG->print("QG_MainWindow::updateRecentFileActions: OK");
}



/**
 * Called when a recently opened file is chosen from the list in the
 * file menu.
 */
void QG_MainWindow::slotFileOpenRecent() {
    RS_DEBUG->print("QG_MainWindow::slotFileOpenRecent");
    
    slotMessage(tr("Opening recent file..."));
    
    QAction *action = qobject_cast<QAction*>(sender());
    if (action!=NULL) {
        QString fileName = action->data().toString();
        RS_DEBUG->print("QG_MainWindow::slotFileOpenRecent: "
            "file: %s", 
            (const char*)fileName.toLatin1());
        if (fileName.endsWith(" (DXF 1)")) {
            slotFileOpen(fileName.left(fileName.length()-8), RS2::FormatDxf1);
        }
        else {
            slotFileOpen(fileName, RS2::FormatUnknown);
        }
    }
    
    RS_DEBUG->print("QG_MainWindow::slotFileOpenRecent: OK");
}



/**
 * Menu file -> export.
 */
void QG_MainWindow::slotFileExport() {
    RS_DEBUG->print("QG_MainWindow::slotFileExport()");

    slotMessage(tr("Exporting drawing..."));

    QG_MdiWindow* w = getMdiWindow();
    QString fn;
    if (w!=NULL) {

        // read default settings:
        RS_SETTINGS->beginGroup("/Paths");
        RS_String defDir = RS_SETTINGS->readEntry("/ExportImage",
                           RS_SYSTEM->getHomeDir());
        RS_String defFilter = RS_SETTINGS->readEntry("/ExportImageFilter",
                              "Portable Network Graphic (*.png)");
        RS_SETTINGS->endGroup();

        bool cancel = false;

        QFileDialog fileDlg(0);

        QList<QByteArray> formats = QImageWriter::supportedImageFormats();
        QStringList filters;

        for (QList<QByteArray>::Iterator it = formats.begin();
                it!=formats.end(); ++it) {

            QString format = (*it);
            QString st;
            if (format=="JPEG") {
                st = QString("%1 (*.%2 *.jpg)")
                     .arg(QG_DialogFactory::extToFormat(format))
                     .arg(QString(format).toLower());
            } else {
                st = QString("%1 (*.%2)")
                     .arg(QG_DialogFactory::extToFormat(format))
                     .arg(QString(format).toLower());
            }
            filters.append(st);
        }

        fileDlg.setFilters(filters);
        fileDlg.setAcceptMode(QFileDialog::AcceptSave);
        fileDlg.setFileMode(QFileDialog::AnyFile);
        //fileDlg.setViewMode(QFileDialog::Detail);
        fileDlg.setWindowTitle(QObject::tr("Export Image"));
        fileDlg.setDirectory(defDir);
        fileDlg.selectFilter(defFilter);

        if (fileDlg.exec()==QDialog::Accepted) {
            QStringList fns = fileDlg.selectedFiles();
            if (fns.count()>=1) {
                fn = fns.at(0);
                cancel = false;
            }
        } else {
            cancel = true;
        }

        // store new default settings:
        if (!cancel) {
            RS_SETTINGS->beginGroup("/Paths");
            RS_SETTINGS->writeEntry("/ExportImage", QFileInfo(fn).absolutePath());
            RS_SETTINGS->writeEntry("/ExportImageFilter",
                                    fileDlg.selectedFilter());
            RS_SETTINGS->endGroup();

            // find out extension:
            QString filter = fileDlg.selectedFilter();
            QString format = "";
            int i = filter.indexOf("(*.");
            if (i!=-1) {
                int i2 = filter.indexOf(QRegExp("[) ]"), i);
                format = filter.mid(i+3, i2-(i+3));
                format = format.toUpper();
            }

            // append extension to file:
            if (!QFileInfo(fn).fileName().contains(".")) {
                fn.append("." + format.toLower());
            }

            // show options dialog:
            QG_DlgImageOptions dlg(this);
            dlg.setGraphicSize(w->getGraphic()->getSize());
            if (dlg.exec()) {
                bool ret = slotFileExport(fn, format, dlg.getSize(),
                                          dlg.isBackgroundBlack());
                if (ret) {
                    QString message = tr("Exported: %1").arg(fn);
                    slotMessage(message);
                }
            }
        }
    }

}



/**
 * Exports the drawing as a bitmap.
 *
 * @param name File name.
 * @param format File format (e.g. "png")
 * @param size Size of the bitmap in pixel
 * @param black true: Black background, false: white
 * @param bw true: black/white export, false: color
 */
bool QG_MainWindow::slotFileExport(const QString& name,
        const QString& format, QSize size, bool black, bool bw) {

    QG_MdiWindow* w = getMdiWindow();
    if (w==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFileExport: "
            "no window opened");
        return false;
    }

    RS_Graphic* graphic = w->getDocument()->getGraphic();
    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFileExport: "
            "no graphic");
        return false;
    }

    slotMessage(tr("Exporting..."));
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    bool ret = false;
    QPixmap* buffer = new QPixmap(size);
    RS_PainterQt* painter = new RS_PainterQt(buffer);

    // black background:
    if (black) {
        painter->setBackground(RS_Color(0,0,0));
    }
    // white background:
    else {
        painter->setBackground(RS_Color(255,255,255));
    }

    // black/white:
    if (bw) {
        painter->setDrawingMode(RS2::ModeBW);
    }

    painter->eraseRect(0,0, size.width(), size.height());

    RS_StaticGraphicView gv(size.width(), size.height(), painter);
    if (black) {
        gv.setBackground(RS_Color(0,0,0));
    } else {
        gv.setBackground(RS_Color(255,255,255));
    }
    gv.setContainer(graphic);
    gv.zoomAuto(false);
    gv.drawEntity(graphic, true);

    QImageWriter imageWriter(name, format.toLatin1());
    if (imageWriter.write(buffer->toImage())) {
        ret = true;
    }
    else {
        RS_DIALOGFACTORY->commandMessage(tr("Image writer error: ") + imageWriter.errorString());
        ret = false;
    }

    QApplication::restoreOverrideCursor();

    // GraphicView deletes painter
    painter->end();
    delete buffer;

    if (ret) {
        slotMessage(tr("Export complete"));
    } else {
        slotMessage(tr("Export failed!"));
    }

    return ret;
}


/**
 * Menu file -> close.
 */
void QG_MainWindow::slotFileClose() {
    RS_DEBUG->print("QG_MainWindow::slotFileClose()");

    QG_MdiWindow* m = getMdiWindow();
    if (m!=NULL) {
        m->close();
    }
}



/**
 * Menu file -> print.
 *
 * @param file: name of PDF file to create a PDF file instead of printing.
 */
void QG_MainWindow::slotFilePrint(const QString& file) {
    RS_DEBUG->print("QG_MainWindow::slotFilePrint()");
    
    slotFilePrintPreview(true);

    QG_MdiWindow* w = getMdiWindow();
    if (w==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFilePrint: "
            "no window opened");
        return;
    }

    RS_Graphic* graphic = w->getDocument()->getGraphic();
    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFilePrint: "
            "no graphic");
        return;
    }

    slotMessage(tr("Printing..."));

    QPrinter* printer;
    printer = new QPrinter(QPrinter::HighResolution);
    //printer = new QPrinter(QPrinter::ScreenResolution);
        
    if (!file.isNull()) {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(file);
    }

    bool landscape = false;
    if (graphic->getPaperFormat(&landscape)==RS2::Custom) {
        printer->setPaperSize(QSize(graphic->getPaperSize().y, graphic->getPaperSize().x), QPrinter::Millimeter);
        printer->setOrientation(QPrinter::Landscape);
    }
    else {
        printer->setPaperSize(RS2::rsToQtPaperFormat(graphic->getPaperFormat(&landscape)));
        if (landscape) {
            printer->setOrientation(QPrinter::Landscape);
        } else {
            printer->setOrientation(QPrinter::Portrait);
        }
    }

    
    if (file.isNull()) {
        RS_SETTINGS->beginGroup("/Print");

        // will only print to file after creating pdf once under os x:
        //printer->setOutputFileName(RS_SETTINGS->readEntry("/FileName", ""));

        printer->setColorMode((QPrinter::ColorMode)RS_SETTINGS->readNumEntry("/ColorMode", (int)QPrinter::Color));
        RS_String printProg = RS_SETTINGS->readEntry("/PrintProgram", "");
        if (!printProg.isEmpty()) {
            printer->setPrintProgram(printProg);
        }
        RS_SETTINGS->endGroup();
    }
    
    // printer setup:
    QPrintDialog printDialog(printer, this);
    if (!file.isNull() || printDialog.exec() == QDialog::Accepted) {
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

        printer->setFullPage(true);

        if (!file.isNull()) {
            printer->setOutputFileName(file);
        }
        
        RS_PainterQt* painter = new RS_PainterQt();
    
        if (!file.isNull()) {
            painter->setPdfOutput(true);
        }
        
        painter->begin(printer);
        painter->setDrawingMode(w->getGraphicView()->getDrawingMode());

        RS_StaticGraphicView gv(printer->width(), printer->height(), painter);
        gv.setPrinting(true);
        gv.setBorders(0,0,0,0);

        double fx = (double)printer->width() / printer->widthMM()
                    * RS_Units::getFactorToMM(graphic->getUnit());
        double fy = (double)printer->height() / printer->heightMM()
                    * RS_Units::getFactorToMM(graphic->getUnit());

        double f = (fx+fy)/2;

        double scale = graphic->getPaperScale();

        gv.setOffset((int)(graphic->getPaperInsertionBase().x * f),
                     (int)(graphic->getPaperInsertionBase().y * f));
        gv.setFactor(f*scale);

        gv.setContainer(graphic);

        RS_DEBUG->print("QG_MainWindow::slotFilePrint(): draw");
        gv.drawEntity(graphic, true);
        RS_DEBUG->print("QG_MainWindow::slotFilePrint(): draw: OK");

#ifdef RS_DEMO
        QString ast(3, QChar(0x002a));
        QString s = QString("\nThis drawing was produced with a \n"
            "%1 version that is not intended for\n"
            "production use. Professional versions are\n"
            "available from http://www.ribbonsoft.com for\n"
            "a small license fee.\n"
            "With your purchase you are supporting the\n"
            "development of future versions of QCad.\n")
            .arg(getApplicationName());
        if (!s.contains(QString("") + QChar(0x72) + 
            QChar(0x69) + QChar(0x62) + QChar(0x62) + 
            QChar(0x6f) + QChar(0x6e))) {
            
            QMessageBox::information(this, 
                QMessageBox::tr("Warning"),
                "Binary modification detected. Aborting...",
                QMessageBox::Ok);
            delete painter;
            exit(1);
        }
        painter->drawText(0, 0, painter->device()->width(),
            painter->device()->height(),
            Qt::AlignCenter, 
            ast + s + ast);
#endif

        // GraphicView deletes painter
        RS_DEBUG->print("QG_MainWindow::slotFilePrint(): end painting");
        painter->end();

        RS_SETTINGS->beginGroup("/Print");
        RS_SETTINGS->writeEntry("/ColorMode", (int)printer->colorMode());
        RS_SETTINGS->writeEntry("/FileName", printer->outputFileName());
        RS_SETTINGS->endGroup();
        QApplication::restoreOverrideCursor();
    }
    else {
        RS_DEBUG->print("QG_MainWindow::slotFilePrint: cancelled");
    }

    // we might abort the print here...
    delete printer;

    slotMessage(tr("Printing complete"));
    
    RS_DEBUG->print("QG_MainWindow::slotFilePrint: done");
}



/**
 * Close Event. Called when the user tries to close the app.
 */
void QG_MainWindow::closeEvent(QCloseEvent* ce) {
    RS_DEBUG->print("QG_MainWindow::closeEvent()");

    slotFileQuit();
    ce->ignore();

    RS_DEBUG->print("QG_MainWindow::closeEvent(): OK");
}



/**
 * Menu file -> print preview.
 */
void QG_MainWindow::slotFilePrintPreview(bool on) {
    RS_DEBUG->print("QG_MainWindow::slotFilePrintPreview()");

    RS_DEBUG->print("  creating MDI window");
    QG_MdiWindow* parent = getMdiWindow();
    if (parent==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFilePrintPreview: "
            "no window opened");
        return;
    }

    // close print preview:
    if (!on) {
        RS_DEBUG->print("QG_MainWindow::slotFilePrintPreview(): off");
        if (parent->getGraphicView()->isPrintPreview()) {
            RS_DEBUG->print("QG_MainWindow::slotFilePrintPreview(): close");
            slotFileClose();
        }
    }

    // open print preview:
    else {
        // look for an existing print preview:
        QG_MdiWindow* ppv = parent->getPrintPreview();
        if (ppv!=NULL) {
            RS_DEBUG->print("QG_MainWindow::slotFilePrintPreview(): show existing");
            slotWindowActivated(ppv);
            ppv->showNormal();
        } else {
            if (!parent->getGraphicView()->isPrintPreview()) {
                RS_DEBUG->print("QG_MainWindow::slotFilePrintPreview(): create");

                QG_MdiWindow* w = new QG_MdiWindow(parent->getDocument(), workspace, this);
                workspace->addWindow(w);
                parent->addChildWindow(w);

                w->setWindowTitle(tr("Print preview for %1").arg(parent->windowTitle()));
                w->setWindowIcon(QIcon(":/qcad/images/document.png"));
                w->getGraphicView()->setPrintPreview(true);
                w->getGraphicView()->setBackground(RS_Color(255,255,255));
                w->getGraphicView()->setDefaultAction(
                    new RS_ActionFilePrintPreview(*w->getDocument(), *w->getGraphicView()));

                // only graphics offer block lists, blocks don't
                RS_DEBUG->print("  adding listeners");
                //reconnectWidgets();
                //QG_DIALOGFACTORY->setLayerList(w->getDocument()->getLayerList());
                //QG_DIALOGFACTORY->setCurrentDocument(w->getDocument());
                //QG_DIALOGFACTORY->updateUiConnections();

                RS_Graphic* graphic = w->getDocument()->getGraphic();
                if (graphic!=NULL) {
                    // Link the layer list to the pen tool bar
                    //graphic->addLayerListListener(QG_DIALOGFACTORY->getPenToolBar());
                    // Link the layer list to the layer widget
                    //graphic->addLayerListListener(QG_DIALOGFACTORY->getLayerWidget());
                    
                    /*
                    RS_List<RS_LayerListListener*> layerListListeners = QG_DIALOGFACTORY->getLayerWidgets();
                    graphic->addLayerListListeners(layerListListeners);
                    */

                    // Link the block list to the block widget
                    graphic->addBlockListListener(QG_DIALOGFACTORY->getBlockWidget());

                    // Center by default:
                    RS_Vector pgInsBase =   
                        graphic->getVariableVector("$PLIMMIN", RS_Vector(false));
                    if (!pgInsBase.valid) {
                        graphic->centerToPage();
                    }
                }

                RS_DEBUG->print("  showing MDI window");

                if (workspace->windowList().isEmpty()) {
                    w->showMaximized();
                } else {
                    w->show();
                }
                w->getGraphicView()->zoomPage();
                setFocus();

                slotWindowActivated(w);
            }
        }
    }
}



/**
 * PDF Export.
 */
void QG_MainWindow::slotFilePdf() {
    RS_DEBUG->print("QG_MainWindow::slotFilePdf()");
    
    slotFilePrintPreview(true);

    QString fn = QFileDialog::getSaveFileName(
                    this,
                    QString(),
                    QString(),
                    "Portable Document Format (*.pdf)");

    if (fn.length()==0) {
        return;
    }
    else {
        if (!fn.endsWith(".pdf", Qt::CaseInsensitive)) {
            fn += ".pdf";
        }
        slotFilePrint(fn);
    }
}



/**
 * SVG Export.
 */
void QG_MainWindow::slotFileSvg() {
    QString fn = QFileDialog::getSaveFileName(
                    this,
                    QString(),
                    QString(),
                    "Scalable Vector Graphics (*.svg)");

    if (fn.length()==0) {
        return;
    }

    if (!fn.endsWith(".svg", Qt::CaseInsensitive)) {
        fn += ".svg";
    }

    QG_MdiWindow* w = getMdiWindow();
    if (w==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFilePrint: "
            "no window opened");
        return;
    }

    RS_Graphic* graphic = w->getDocument()->getGraphic();
    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::slotFilePrint: "
            "no graphic");
        return;
    }

    RS_FilterSvg filter;
    filter.fileExport(*graphic, fn, RS2::FormatSvg);
}




/**
 * Menu file -> quit.
 */
void QG_MainWindow::slotFileQuit() {
    RS_DEBUG->print("QG_MainWindow::slotFileQuit()");

    slotMessage(tr("Exiting application..."));

    if (queryExit(false)) {
        qApp->exit(0);
    }
    
    RS_DEBUG->print("QG_MainWindow::slotFileQuit(): OK");
}



/**
 * Forces termination of QCad (demo version).
 */
void QG_MainWindow::slotFileDemoQuit() {
    RS_DEBUG->print("QG_MainWindow::slotFileDemoQuit()");

    slotMessage(tr("Exiting application..."));

    queryExit(true);
    qApp->exit(0);
}



/**
 * Message box on last window exit.
 */
bool QG_MainWindow::queryExit(bool force) {
    RS_DEBUG->print("QG_MainWindow::queryExit()");

    bool succ = true;

    // demo:
    if (force) {
        QMessageBox::information(
            this, QMessageBox::tr("Warning"),
            tr("This is a %1 version which terminates\n"
            "automatically after 10min or after it has been\n"
            "used for more than 100 hours in total. This\n"
            "software is not intended for production use.\n"
            "Please buy a full version of the application\n"
            "for a small license fee from\n%2.")
            .arg(getApplicationName())
#ifdef QC_CUSTOM_VENDOR
            .arg(QC_CUSTOM_VENDOR),
#else
            .arg("RibbonSoft, http://www.ribbonsoft.com"),
#endif
            QMessageBox::Ok);
    }

    QWidgetList list = workspace->windowList();
    for (int i=0; i<list.size(); ++i) {
        QG_MdiWindow* mdiWindow = (QG_MdiWindow*)list.at(i);
        
        if (mdiWindow->isSecondaryView()==false) {
            succ = mdiWindow->closeMdi(force) && requestClosingOfChild(mdiWindow);
            if (!succ) {
                break;
            }
        }
    }
    if (succ) {
        storeSettings();
    }

    RS_DEBUG->print("QG_MainWindow::queryExit(): OK");

    return succ;
}



/**
 * Called when a document window was activated.
 */
void QG_MainWindow::slotWindowActivated(QWidget* widget) {
    RS_DEBUG->print("QG_MainWindow::slotWindowActivated");
    
    if (widget==NULL) {
        RS_DEBUG->print("QG_MainWindow::slotWindowActivated: widget==NULL");
        //QG_MdiWindow* m = getMdiWindow();
        //emit windowsChanged(m!=NULL && m->getDocument()!=NULL);
        //return;
    }

    if (widget!=NULL && workspace!=NULL && workspace->activeWindow()!=widget) {
        workspace->setActiveWindow(widget);
    }

    QG_MdiWindow* m = getMdiWindow();
    if (m!=NULL && m->getDocument()!=NULL) {
        //bool showByBlock = m->getDocument()->rtti()==RS2::EntityBlock;

        /*
        if (QG_DIALOGFACTORY->getPenToolBar()!=NULL) {
            QG_DIALOGFACTORY->getPenToolBar()->setLayerList(
                m->getDocument()->getLayerList());
        }
        */
        //QG_DIALOGFACTORY->setLayerList(m->getDocument()->getLayerList());
        /*
        if (QG_DIALOGFACTORY->getLayerWidget()!=NULL) {
            QG_DIALOGFACTORY->getLayerWidget()->setLayerList(
                m->getDocument()->getLayerList());
        }
        */
        /*
        for (int i=0; i<QG_DIALOGFACTORY->getLayerWidgets().size(); ++i) {
            QG_DIALOGFACTORY->getLayerWidgets().at(i)->setLayerList(
                m->getDocument()->getLayerList());
        }
        */
        QG_DIALOGFACTORY->updateUiConnections();

        if (QG_DIALOGFACTORY->getCoordinateWidget()!=NULL) {
            QG_DIALOGFACTORY->getCoordinateWidget()->setGraphic(
                m->getGraphic());
        }

#ifdef RS_CAM
        if (QG_DIALOGFACTORY->getSimulationControls()!=NULL) {
            QG_DIALOGFACTORY->getSimulationControls()->setGraphicView(
                m->getGraphicView());
        }
#endif

        if (QG_DIALOGFACTORY->getPropertyEditor()!=NULL) {
            QG_DIALOGFACTORY->getPropertyEditor()->update(
                m->getDocument());
        }

        if (QG_DIALOGFACTORY->getBlockWidget()!=NULL) {
            // Only graphics show blocks. (blocks don't)
            if (m->getDocument()->rtti()==RS2::EntityGraphic) {
                QG_DIALOGFACTORY->getBlockWidget()->setBlockList(
                    m->getDocument()->getBlockList());
            } else {
                QG_DIALOGFACTORY->getBlockWidget()->setBlockList(NULL);
            }
        }
        
        // Update all inserts in this graphic (blocks might have changed):
        m->getDocument()->updateInserts();
        m->getGraphicView()->redraw();
        
        // set snapmode from snapping menu
        updateSnapMode();
        
        // set pen from pen toolbar
        if (QG_DIALOGFACTORY->getPenToolBar()!=NULL) {
            slotPenChanged(QG_DIALOGFACTORY->getPenToolBar()->getPen());
        }

        // update toggle button status:
        if (m->getGraphic()!=NULL) {
            emit(gridChanged(m->getGraphic()->isGridOn()));
            emit(printPreviewChanged(m->getGraphicView()->isPrintPreview()));
        }
    } 
    else {
        RS_DEBUG->print("QG_MdiWindow::slotWindowActivated: "
            "MDI window is invalid");
        
        if (QG_DIALOGFACTORY->getCoordinateWidget()!=NULL) {
            QG_DIALOGFACTORY->getCoordinateWidget()->setGraphic(NULL);
        }

#ifdef RS_CAM
        if (QG_DIALOGFACTORY->getSimulationControls()!=NULL) {
            QG_DIALOGFACTORY->getSimulationControls()->setGraphicView(NULL);
        }
#endif

        if (QG_DIALOGFACTORY->getBlockWidget()!=NULL) {
            QG_DIALOGFACTORY->getBlockWidget()->setBlockList(NULL);
        }
    }
    
    // Disable/Enable menu and toolbar items
    emit windowsChanged(m!=NULL && m->getDocument()!=NULL);

    RS_DEBUG->print("QG_MainWindow::slotWindowActivated: OK");
}



/**
 * Called when the menu 'windows' is about to be shown.
 * This is used to update the window list in the menu.
 */
void QG_MainWindow::slotWindowsMenuAboutToShow() {
    RS_DEBUG->print("QG_MainWindow::slotWindowsMenuAboutToShow");

    if (windowsMenu==NULL) {
        return;
    }

    windowsMenu->clear();
    windowsMenu->addAction(closeAllAct);
    windowsMenu->addSeparator();
    windowsMenu->addAction(tileAct);
    windowsMenu->addAction(cascadeAct);
    windowsMenu->addSeparator();
    windowsMenu->addAction(nextAct);
    windowsMenu->addAction(previousAct);
    windowsMenu->addAction(separatorActWin);

    QList<QWidget*> windows = workspace->windowList();
    separatorActWin->setVisible(!windows.isEmpty());
    for (int i=0; i<windows.size(); ++i) {
        QG_MdiWindow* child = qobject_cast<QG_MdiWindow*>(windows.at(i));

        QString text;
        if (i < 9) {
            text = tr("&%1. %2").arg(i + 1).arg(child->windowTitle());
                                //.arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1. %2").arg(i + 1).arg(child->windowTitle());
                               //.arg(child->userFriendlyCurrentFile());
        }
    
        RS_DEBUG->print("QG_MainWindow::slotWindowsMenuAboutToShow: "
            "adding entry for window");

        QAction* action  = windowsMenu->addAction(text);
        RS_Document* doc = child->getDocument();
        if (doc!=NULL) {
            action->setStatusTip(doc->getFilename());
        }
        action->setCheckable(true);
        action->setChecked(child == getMdiWindow());
        connect(action, SIGNAL(triggered()), 
                windowMapper, SLOT(map()));
        windowMapper->setMapping(action, child);
    }
    
    RS_DEBUG->print("QG_MainWindow::slotWindowsMenuAboutToShow: OK");
}



/**
 * Called when the user selects a document window from the
 * window list.
 */
void QG_MainWindow::slotWindowsMenuActivated(int id) {
    RS_DEBUG->print("QG_MainWindow::slotWindowsMenuActivated");

    QWidget* w = workspace->windowList().at(id);
    if (w!=NULL) {
        w->showNormal();
    }
    
    RS_DEBUG->print("QG_MainWindow::slotWindowsMenuActivated: OK");
}



/**
 * Tiles MDI windows horizontally.
 */
void QG_MainWindow::slotTileHorizontal() {
    RS_DEBUG->print("QG_MainWindow::slotTileHorizontal");

    QWidgetList windows = workspace->windowList();
    if (windows.count()==0) {
        return;
    }

    int heightForEach = workspace->height() / windows.count();
    int y = 0;
    for (int i=0; i<int(windows.count()); ++i) {
        QWidget *window = windows.at(i);
        if (window->windowState()==Qt::WindowMaximized) {
            // prevent flicker
            window->hide();
            window->showNormal();
        }
        int preferredHeight = window->minimumHeight()
                              + window->parentWidget()->baseSize().height();
        int actHeight = qMax(heightForEach, preferredHeight);

        window->parentWidget()->setGeometry(0, y,
                                            workspace->width(), actHeight);
        y+=actHeight;
    }
    
    RS_DEBUG->print("QG_MainWindow::slotTileHorizontal: OK");
}



/**
 * Tiles MDI windows vertically.
 */
void QG_MainWindow::slotTileVertical() {
    workspace->tile();
}



/**
 * Called when something changed in the pen tool bar 
 * (e.g. color, width, style).
 */
void QG_MainWindow::slotPenChanged(const RS_Pen& pen) {
    RS_DEBUG->print("QG_MainWindow::slotPenChanged");

    QG_MdiWindow* m = getMdiWindow();
    if (m!=NULL) {
        m->slotPenChanged(pen);
    }

    RS_DEBUG->print("QG_MainWindow::slotPenChanged: OK");
}

    
    
/**
 * Goes back to the previous menu or one step in the current action.
 */
void QG_MainWindow::slotBack() {
    RS_GraphicView* graphicView = getGraphicView();
    if (graphicView!=NULL) {
        graphicView->back();
    } else {
        if (QG_DIALOGFACTORY->getCadToolBar()!=NULL) {
            QG_DIALOGFACTORY->getCadToolBar()->showToolBar(RS2::ToolBarMain);
        }
    }
}



/**
 * Goes one step further in the current action.
 */
void QG_MainWindow::slotEnter() {

    if (QG_DIALOGFACTORY->getCommandWidget()==NULL || 
        !QG_DIALOGFACTORY->getCommandWidget()->checkFocus()) {

        if (QG_DIALOGFACTORY->getCadToolBar()!=NULL) {
            QG_DIALOGFACTORY->getCadToolBar()->forceNext();
        } else {
            RS_GraphicView* graphicView = getGraphicView();
            if (graphicView!=NULL) {
                graphicView->enter();
            }
        }
    }
}



/**
 * Sets the keyboard focus on the command line.
 */
void QG_MainWindow::slotFocusCommandLine() {
    if (QG_DIALOGFACTORY->getCommandWidget()!=NULL && 
        QG_DIALOGFACTORY->getCommandWidget()->isVisible()) {
        QG_DIALOGFACTORY->getCommandWidget()->setFocus();
    }
}
    

/**
 * Hands focus back to the application window. In the rare event
 * of a escape press from the layer widget (e.g after switching desktops
 * in XP).
 */
void QG_MainWindow::slotFocus() {
    setFocus();
}



/**
 * Inserts the recent file actions into the given menu.
 */
void QG_MainWindow::insertRecentFileActions(QMenu* menu) {
    if (menu==NULL) {
        return;
    }
    
    separatorActRecentFiles = menu->addSeparator();

    for (int i = 0; i < QG_MAX_RECENT_FILES; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(slotFileOpenRecent()));
        menu->addAction(recentFileActions[i]);
    }
    
    updateRecentFileActions();
}


/**
 * Implementation from RS_MainWindowInterface.
 * Redraws all mdi windows.
 */
void QG_MainWindow::redrawAll() {
    if (workspace!=NULL) {
        QWidgetList windows = workspace->windowList();
        for (int i = 0; i < int(windows.count()); ++i) {
            QG_MdiWindow* m = (QG_MdiWindow*)windows.at(i);
            if (m!=NULL) {
                QG_GraphicView* gv = m->getGraphicView();
                if (gv!=NULL) {
                    gv->redraw();
                }
            }
        }
    }
}



/**
 * Implementation from RS_MainWindowInterface.
 * Updates all grids of all graphic views.
 */
void QG_MainWindow::updateGrids() {
    if (workspace!=NULL) {
        QWidgetList windows = workspace->windowList();
        for (int i = 0; i < int(windows.count()); ++i) {
            QG_MdiWindow* m = (QG_MdiWindow*)windows.at(i);
            if (m!=NULL) {
                QG_GraphicView* gv = m->getGraphicView();
                if (gv!=NULL) {
                    gv->updateGrid();
                }
            }
        }
    }
}



void QG_MainWindow::reconnectWidgets(QG_MdiWindow* /*w*/) {

    /*
    RS_DEBUG->print("QG_MainWindow::reconnectWidgets");
    if (w==NULL) {
        w = getMdiWindow();
    }

    if (w==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::reconnectWidgets: "
            "no window opened");
        return;
    }

    RS_Graphic* graphic = w->getDocument()->getGraphic();
    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::reconnectWidgets: "
            "no graphic");
        return;
    }
    
    QG_GraphicView* graphicView = w->getGraphicView();
    if (graphicView==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "QG_MainWindow::reconnectWidgets: "
            "no graphic view");
        return;
    }
        
    disconnect(graphic->getLayerList(), 0, 0, 0);

    connect(graphic->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*)), 
        graphicView, SLOT(slotRedraw()));
    connect(graphic->getLayerList(), SIGNAL(signalLayerToggled(RS_Layer*)), 
        graphicView, SLOT(slotRedraw()));

    connect(graphic->getLayerList(), SIGNAL(signalLayerActivated(RS_Layer*)), 
        QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerActivated(RS_Layer*)));
    connect(graphic->getLayerList(), SIGNAL(signalLayerAdded(RS_Layer*)), 
        QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerAdded(RS_Layer*)));
    connect(graphic->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*)), 
        QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerEdited(RS_Layer*)));
    connect(graphic->getLayerList(), SIGNAL(signalLayerRemoved(RS_Layer*)), 
        QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerRemoved(RS_Layer*)));
    connect(graphic->getLayerList(), SIGNAL(signalLayerToggled(RS_Layer*)), 
        QG_DIALOGFACTORY->getLayerWidget(), SLOT(slotLayerToggled(RS_Layer*)));
    
    RS_DEBUG->print("QG_MainWindow::reconnectWidgets: OK");
    */
}
    
    
void QG_MainWindow::updateSnapMode() {
    //RS_GuiAction::triggerByType(RS_ActionSetSnapMode::getId(), RS_ActionSetSnapMode::getSnapMode());

    //RS_ActionSetSnapMode::factory(RS_ActionSetSnapMode::getSnapMode());

    /*
    TODO
    RS_GuiAction::triggerByType(RS_ActionSetSnapMode::getSnapMode());
    RS_GuiAction::triggerByType(RS_ActionSetSnapRestriction::getSnapRestriction());
    if (RS_ActionLockRelativeZero::isLocked()) {
        RS_GuiAction::triggerByType(RS2::ActionLockRelativeZero);
    }
    else {
        RS_GuiAction::triggerByType(RS2::ActionUnlockRelativeZero);
    }
    */
}
    
