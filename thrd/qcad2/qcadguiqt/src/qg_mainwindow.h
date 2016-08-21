/****************************************************************************
** $Id: qg_mainwindow.h 7137 2007-11-14 21:05:42Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
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

#ifndef QG_MAINWINDOW_H
#define QG_MAINWINDOW_H

#include "rs_mainwindowinterface.h"

#include "QMainWindow"
#include "QMdiArea"

#include "rs.h"
//#include "rs_actionhandler.h"

#include "qg_recentfiles.h"
#include "qg_mdiwindow.h"

#define QG_MAX_RECENT_FILES 8

class QSignalMapper;

class RS_Document;


/**
 * Interface for main application windows.
 */
class CAD_EXPORT QG_MainWindow : public QMainWindow, public RS_MainWindowInterface {
    Q_OBJECT

public:
    QG_MainWindow();
    virtual ~QG_MainWindow() {}
    
    virtual void storeSettings();
    virtual void restoreSettings();
    virtual void restoreDocks();

    virtual void insertWindowsMenu();
    virtual void insertRecentFileActions(QMenu* menu);
    virtual void initMdi();
    
    /**
     * Default implementation from RS_MainWindowInterface.
     *
     * @return Pointer to the graphic view of the currently active document
     * window or NULL if no window is available.
     */
    virtual RS_GraphicView* getGraphicView() {
        QG_MdiWindow* m = getMdiWindow();
        if (m!=NULL) {
            return m->getGraphicView();
        }
        return NULL;
    }
    
    /**
     * Default implementation from RS_MainWindowInterface.
     *
     * @return Pointer to the graphic document of the currently active document
     * window or NULL if no window is available.
     */
    virtual RS_Document* getDocument() {
        RS_DEBUG->print("QG_MainWindow::getDocument");

        QG_MdiWindow* m = getMdiWindow();
        if (m!=NULL) {
            RS_DEBUG->print("QG_MainWindow::getDocument: "
                "return document of mdi window");
            return m->getDocument();
        }
        else {
            RS_DEBUG->print("QG_MainWindow::getDocument: "
                "mdi window is NULL");
            return NULL;
        }
    }
    
    
    /**
     * Default implementation from RS_MainWindowInterface.
     */
    virtual void createNewDocument(
        const RS_String& fileName = RS_String::null, RS_Document* doc=NULL) {
        
        slotFileNew(doc);
        if (fileName!=RS_String::null && getDocument()!=NULL) {
            getDocument()->setFilename(fileName);
        }
    }
    
    virtual QMainWindow* getMainWindow() {
        return this;
    }
    virtual QG_MdiWindow* getMdiWindow() {
        if (getWorkspace()!=NULL) {
            return (QG_MdiWindow*)getWorkspace()->activeSubWindow();
        }
        else {
            return NULL;
        }
    }

    virtual QMdiArea* getWorkspace() {
        return workspace;
    }
    /*
    virtual RS_ActionHandler* getActionHandler() {
        return &actionHandler;
    }
    */
    virtual QString getApplicationName() = 0;
    
    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void loadDocument(const RS_String& fileName) {
        slotFileOpen(fileName, RS2::FormatUnknown);
    }

    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void closeDocument() {
        slotFileClose();
    }
    
    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void saveDocument(const RS_String& fileName) {
        QG_MdiWindow* w = getMdiWindow();
        if (w!=NULL) {
            w->getDocument()->setFilename(fileName);
            setCurrentFile(fileName);
            slotFileSave();
        }
    }
    
    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void resizeWindow(int width, int height) {
        RS_DEBUG->print("QC_ApplicationWindow::resizeWindow");
        resize(width, height);
    }
    
    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void setFocus2() {
        setFocus();
    }

    /**
     * Implementation from RS_MainWindowInterface.
     */
    virtual void graphicViewRedrawn() {}
    
    virtual void redrawAll();
    void updateGrids();

    virtual bool requestClosingOfChild(QG_MdiWindow* /*child*/) { return true; }

    virtual void reconnectWidgets(QG_MdiWindow* w=NULL);
    void updateSnapMode();
    
protected:
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void closeEvent(QCloseEvent*);

public slots:
    virtual QG_MdiWindow* slotFileNew(RS_Document* doc=NULL);
    virtual QG_MdiWindow* slotFileOpen();
    virtual QG_MdiWindow* slotFileOpen(const QString& fileName, RS2::FormatType type);
    virtual bool slotFileSave();
    virtual bool slotFileSaveAs(const QString& caption=QString());
    virtual void slotFileClosing();
    virtual void slotFileExport();
    virtual bool slotFileExport(const QString& name, const QString& format, 
        QSize size, bool black, bool bw=false);
    virtual void slotFileClose();
    virtual void slotFilePrint(const QString& file = QString::null);
    virtual void slotFilePdf();
    virtual void slotFileSvg();
    virtual void slotFilePrintPreview(bool on);
    virtual void slotFileQuit();
    virtual void slotFileDemoQuit();
    virtual bool queryExit(bool force);
    
    virtual void setCurrentFile(const QString &fileName);
    virtual void updateRecentFileActions();
    virtual void slotFileOpenRecent();

    virtual void slotWindowActivated(QMdiSubWindow *w=NULL);
    virtual void slotWindowsMenuAboutToShow();
    virtual void slotWindowsMenuActivated(int);
    virtual void slotTileHorizontal();
    virtual void slotTileVertical();

    virtual void slotPenChanged(const RS_Pen& p);
    
    virtual void slotBack();
    virtual void slotEnter();
    
    virtual void slotFocus();
    virtual void slotFocusCommandLine();
    virtual void slotMessage(const QString& text);
    virtual void slotError(const QString& msg);
    

signals:
    void windowsChanged(bool windowsLeft);
    void gridChanged(bool on);
    void printPreviewChanged(bool on);

protected:
    //RS_ActionHandler actionHandler;
    QMdiArea* workspace;
    QSignalMapper* windowMapper;
    
    QMenu* windowsMenu;
    
    QAction* closeAllAct;
    QAction* tileAct;
    QAction* cascadeAct;
    QAction* nextAct;
    QAction* previousAct;
    QAction* separatorActWin;
    
    QAction* recentFileActions[QG_MAX_RECENT_FILES];
    QAction* separatorActRecentFiles;
};

#endif

