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

#include "qg_mdiwindow.h"

#include <QCloseEvent>
#include <QApplication>
#include <QCursor>

#include "rs_actiondefault.h"
#include "rs_application.h"
#include "rs_document.h"
#include "rs_pen.h"

#include "qg_graphicview.h"
#include "qg_exitdialog.h"
#include "qg_filedialog.h"
#include "qg_mainwindow.h"


int QG_MdiWindow::idCounter = 0;

/**
 * Constructor.
 *
 * @param doc Pointer to an existing document or NULL if a new 
 *   document should be created for this window.
 * @param parent Parent widget. For example a workspace or tabed widget, ...
 */
QG_MdiWindow::QG_MdiWindow(RS_Document* doc, QWidget* parent, 
    QG_MainWindow* appWin) 
    : QMdiSubWindow(parent) {

    setAttribute(Qt::WA_DeleteOnClose);

    owner = false;
    forceClosing = false;
    initDoc(doc);
    initView();
    id = idCounter++;
    childWindows.setAutoDelete(false);
    parentWindow = NULL;

    this->appWin = appWin;

    if (document!=NULL) {
        if (document->getLayerList()!=NULL) {
            // Link the graphic view to the document's layer list
            //document->getLayerList()->addListener(graphicView);
            /*
            connect(document->getLayerList(), SIGNAL(signalLayerEdited(RS_Layer*),
                graphicView, SLOT(slotRedraw())));
            connect(document->getLayerList(), SIGNAL(signalLayerRemoved(RS_Layer*),
                graphicView, SLOT(slotRedraw())));
            connect(document->getLayerList(), SIGNAL(signalLayerToggled(RS_Layer*),
                graphicView, SLOT(slotRedraw())));
            */
        }
        if (document->getBlockList()!=NULL) {
            // Link the graphic view to the document's block list
            document->getBlockList()->addListener(graphicView);
        }
    }
}



/**
 * Destructor.
 *
 * Deletes the document associated with this window.
 */
QG_MdiWindow::~QG_MdiWindow() {
    RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow");
    /*
    RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: owner of doc: %d", (int)owner);
    if (document!=NULL && document->getLayerList()!=NULL) {
        RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 000a");
        document->getLayerList()->removeListener(graphicView);
        RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 000b");
    }
    RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 001");

    if (document!=NULL && document->getBlockList()!=NULL) {
        document->getBlockList()->removeListener(graphicView);
    }
    RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 002");

    if (owner==true && document!=NULL) {
        RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 003");
        delete document;
        RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: 004");
    }
    document = NULL;
    */
    detach();
    RS_DEBUG->print("QG_MdiWindow::~QG_MdiWindow: OK");
}



/**
 * Detaches the document from this MDI window. Used in preparation for
 * deletion. Note that the destructor is not good for that since Qt
 * deletes the widget not immediately.
 */
void QG_MdiWindow::detach() {
    RS_DEBUG->print("QG_MdiWindow::detach");
    if (document!=NULL) {
        if (document->getLayerList()!=NULL) {
            RS_DEBUG->print("QG_MdiWindow::detach: 000a");
            //document->getLayerList()->removeListener(graphicView);
            RS_DEBUG->print("QG_MdiWindow::detach: 000b");
        }
        RS_DEBUG->print("QG_MdiWindow::detach: 001");

        if (document->getBlockList()!=NULL) {
            document->getBlockList()->removeListener(graphicView);
        }
        RS_DEBUG->print("QG_MdiWindow::detach: 002");

        if (owner==true) {
            RS_DEBUG->print("QG_MdiWindow::detach: 003");
            delete document;
            RS_DEBUG->print("QG_MdiWindow::detach: 004");
        }
    }
    document = NULL;
    
    // This is a view depending on another view (e.g. a block view)
    if (parentWindow!=NULL) {
        parentWindow->removeChildWindow(this);
        emit(signalClosing());
        parentWindow = NULL;
    }
    RS_DEBUG->print("QG_MdiWindow::detach: OK");
}



/**
 * Adds another MDI window to the list of known windows that
 * depend on this one. This can be another view or a view for
 * a particular block.
 */
void QG_MdiWindow::addChildWindow(QG_MdiWindow* w) {
    RS_DEBUG->print("RS_MDIWindow::addChildWindow()");

    childWindows.append(w);
    w->setParentWindow(this);

    RS_DEBUG->print("children: %d", childWindows.count());
}



/**
 * Removes a child window.
 *
 * @see addChildWindow
 */
void QG_MdiWindow::removeChildWindow(QG_MdiWindow* w) {
    RS_DEBUG->print("RS_MDIWindow::removeChildWindow()");

    bool suc = childWindows.remove(w);
    RS_DEBUG->print("successfully removed child window: %d", (int)suc);

    RS_DEBUG->print("children: %d", childWindows.count());

}



/**
 * @return pointer to the first print preview of this drawing or NULL.
 */
QG_MdiWindow* QG_MdiWindow::getPrintPreview() {
    for (int i=0; i<childWindows.count(); ++i) {
        if (childWindows.at(i)->getGraphicView()->isPrintPreview()) {
            return childWindows.at(i);
        }
    }

    return NULL;
}



/**
 * Closes this MDI window.
 *
 * @param force Disable cancel button (for demo version).
 * @param ask Ask user before closing.
 */
bool QG_MdiWindow::closeMdi(bool force, bool ask) {
    // should never happen:
    if (document==NULL) {
        return true;
    }

    bool ret = false;

    // This is a view depending on another view (e.g. a block view) and we 
    //   don't need to ask the user for closing since there is still 
    //   another view available (the parent view).
    if (parentWindow!=NULL) {
        RS_DEBUG->print("  closing child window");
        RS_DEBUG->print("    notifying parent about closing this window");
        parentWindow->removeChildWindow(this);
        emit(signalClosing());
        ret = true;
    }

    // This is the main graphic document. Ask user for closing.
    else if (!ask || slotFileClose(force)) {
        RS_DEBUG->print("  closing graphic");
        // close all child windows:
        bool done;
        do {
            done=true;
            if (childWindows.count()>0) {
                childWindows.at(0)->close();
                done=false;
            }
        } while (!done);

        emit(signalClosing());

        ret = true;
    }

    // User decided not to close graphic document:
    else {
        ret = false;
    }

    return (ret || force);
}



/**
 * Called by Qt when the user closes this MDI window.
 */
void QG_MdiWindow::closeEvent(QCloseEvent* ce) {

    RS_DEBUG->print("QG_MdiWindow::closeEvent begin");

    if (closeMdi(false, !forceClosing) && appWin->requestClosingOfChild(this)) {
        RS_DEBUG->print("QG_MdiWindow::closeEvent: accept");
        ce->accept();
    } else {
        RS_DEBUG->print("QG_MdiWindow::closeEvent: ignore");
        ce->ignore();
    }

    RS_DEBUG->print("QG_MdiWindow::closeEvent end");
}



/**
 * Init the document.
 *
 * @param type Document type. RS:EntityGraphic or RS2::EntityBlock
 * @param container Entity container to be used as document or NULL
 * if a new document should be created.
 */
void QG_MdiWindow::initDoc(RS_Document* doc) {

    RS_DEBUG->print("QG_MdiWindow::initDoc()");

    if (doc==NULL) {
        document = new RS_Graphic();
        document->newDoc();
        owner = true;
    } else {
        document = doc;
        owner = false;
    }

}



/**
 * Init the view.
 */
void QG_MdiWindow::initView() {
    RS_DEBUG->print("QG_MdiWindow::initView");

    if (document==NULL) {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_MdiWindow::initView(): document is NULL");
        return;
    }
    
    RS_SETTINGS->beginGroup("/Appearance");
    double defaultZoom = 
        RS_SETTINGS->readEntry("/DefaultZoom", "4.0").toDouble();
    RS_SETTINGS->endGroup();
    
    RS_DEBUG->print("QG_MdiWindow::initView: 001");

    graphicView = new QG_GraphicView(this);
    graphicView->setContainer(document);
    graphicView->setFactor(defaultZoom);
    graphicView->setOffset(50, 50);
    //graphicView->setBorders(10, 10, 10, 10);
    RS_DEBUG->print("QG_MdiWindow::initView: 002");
    graphicView->setDefaultAction(new RS_ActionDefault(*document, *graphicView));
    RS_DEBUG->print("QG_MdiWindow::initView: 003");

    RS_DEBUG->print("QG_MdiWindow::initView: 004");
    QVBoxLayout* vbl = new QVBoxLayout();
    vbl->setSpacing(0);
    vbl->setMargin(0);
    vbl->addWidget(graphicView);
    
    RS_DEBUG->print("QG_MdiWindow::initView: 005");

    graphicView->setFocus();
    
    RS_DEBUG->print("QG_MdiWindow::initView: 006");
    
    setLayout(vbl);
    RS_DEBUG->print("QG_MdiWindow::initView: 007");
    show();
    RS_DEBUG->print("QG_MdiWindow::initView: 008");
    graphicView->show();
    
    RS_DEBUG->print("QG_MdiWindow::initView: OK");
}



/**
 * Called when the current pen (color, style, width) has changed.
 * Sets the active pen for the document in this MDI window.
 */
void QG_MdiWindow::slotPenChanged(const RS_Pen& pen) {
    if (document!=NULL) {
        document->setActivePen(pen);
    }
}



/**
 * Creates a new empty document in this MDI window.
 */
void QG_MdiWindow::slotFileNew() {
    RS_DEBUG->print("QG_MdiWindow::slotFileNew");
    if (document!=NULL && graphicView!=NULL) {
        document->newDoc();
        graphicView->redraw();
    }
    RS_DEBUG->print("QG_MdiWindow::slotFileNew: OK");
}



/**
 * Opens the given file in this MDI window.
 */
bool QG_MdiWindow::slotFileOpen(const QString& fileName, RS2::FormatType type) {

    RS_DEBUG->print("QG_MdiWindow::slotFileOpen");
    bool ret = false;

    if (document!=NULL && !fileName.isEmpty()) {
        document->newDoc();

        // cosmetics..
        RS_APP->processEvents(QEventLoop::AllEvents, 1000);
        
        ret = document->open(fileName, type);

        if (ret) {
            //QString message=tr("Loaded document: ")+fileName;
            //statusBar()->message(message, 2000);

            RS_DEBUG->print("QG_MdiWindow::slotFileOpen: autoZoom");
            graphicView->zoomAuto(false);
            RS_DEBUG->print("QG_MdiWindow::slotFileOpen: autoZoom: OK");
        } else {
            RS_DEBUG->print("QG_MdiWindow::slotFileOpen: failed");
        }
    } else {
        RS_DEBUG->print("QG_MdiWindow::slotFileOpen: cancelled");
        //statusBar()->message(tr("Opening aborted"), 2000);
    }

    RS_DEBUG->print("QG_MdiWindow::slotFileOpen: OK");

    return ret;
}



/**
 * Saves the current file.
 *
 * @return true if the file was saved successfully.
 *         false if the file could not be saved or the document 
 *         is invalid.
 */
bool QG_MdiWindow::slotFileSave(bool &cancelled) {
    RS_DEBUG->print("QG_MdiWindow::slotFileSave()");
    bool ret = false;
    cancelled = false;

    if (document!=NULL) {
        if (document->getFilename().isEmpty()) {
            ret = slotFileSaveAs(cancelled);
        } else {
            QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
            ret = document->save();
            QApplication::restoreOverrideCursor();
        }
    }

    return ret;
}



/**
 * Saves the current file. The user is asked for a new filename 
 * and format.
 *
 * @return true if the file was saved successfully or the user cancelled.
 *         false if the file could not be saved or the document 
 *         is invalid.
 */
bool QG_MdiWindow::slotFileSaveAs(bool &cancelled, const QString& caption) {
    RS_DEBUG->print("QG_MdiWindow::slotFileSaveAs");
    bool ret = false;
    cancelled = false;
    RS2::FormatType t = RS2::FormatDxf;

    QString suggestion;
    if (document!=NULL) {
        suggestion = document->getFilename();
    }

//#ifndef Q_OS_MACX
    QString fn = QG_FileDialog::getSaveFileName(this, &t, caption, suggestion);
/*
#else
    QString fn = QFileDialog::getSaveFileName(
                    this,
                    caption,
                    QString(),
                    "Drawings (*.dxf)");
#endif
*/
    RS_DEBUG->print("QG_MdiWindow::slotFileSaveAs: filename: %s", 
        (const char*)fn.toLatin1());
    if (document!=NULL && !fn.isEmpty()) {
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
        ret = document->saveAs(fn, t);
        QApplication::restoreOverrideCursor();
    } else {
        // cancel is not an error - returns true
        ret = true;
        cancelled = true;
    }

    return ret;
}



/**
 * Requests the closing of this MDI window.
 *
 * @param force Force closing by disabling the cancel button (for demo versions).
 */
bool QG_MdiWindow::slotFileClose(bool force) {
    RS_DEBUG->print("QG_MdiWindow::slotFileClose()");

    bool succ = true;
    int exit = 0;

    if(document!=NULL && document->isModified()) {
        QG_ExitDialog dlg(this);
        dlg.setForce(force);
        if (document->getFilename().isEmpty()) {
            dlg.setText(tr("The drawing has been modified.\nDo you want to save your changes or discard them?"));
        } else {
            dlg.setSaveAsEnabled(false);
            QString fn = document->getFilename();
            if (fn.length() > 50) {
                fn = QString("%1...%2").arg(fn.left(24)).arg(fn.right(24));
            }
            dlg.setText(tr("The drawing \"%1\" has been modified.\nDo you want to save your changes or discard them?")
                        .arg(fn));
        }
        dlg.setTitle(tr("Closing Drawing"));

        bool again;
        //bool cancelled;
        do {
            again = false;
            exit = dlg.exec();

            switch (exit) {
            case 0: // cancel
                succ = false;
                break;
            case 1: // leave
                succ = true;
                break;
            case 2: // save
                //succ = slotFileSave(cancelled);
                succ = appWin->slotFileSave();
                //again = !succ || cancelled;
                again = !succ;
                break;
            case 3: // save as
                //succ = slotFileSaveAs(cancelled);
                succ = appWin->slotFileSaveAs();
                //again = !succ || cancelled;
                again = !succ;
                break;
            default:
                break;
            }
        } while (again);
    } else {
        succ = true;
    }

    return succ;
}

