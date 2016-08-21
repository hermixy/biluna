/****************************************************************************
** $Id: qc_mdiwindow.h 2623 2005-08-26 21:33:41Z andrew $
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

#ifndef QG_MDIWINDOW_H
#define QG_MDIWINDOW_H

#include <QCloseEvent>
#include <QMdiSubWindow>

#include "rs.h"
#include "rs_document.h"
#include "rs_graphic.h"
#include "rs_ptrlist.h"

#include "qg_graphicview.h"

class RS_EventHandler;
class RS_Pen;

class QG_MainWindow;


/**
 * MDI document window. A widget that combines a document and a view
 * and is usually used in MDI environments.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT QG_MdiWindow: public QMdiSubWindow {
    Q_OBJECT

public:
    QG_MdiWindow(RS_Document* doc, QWidget* parent, QG_MainWindow* appWin);
    ~QG_MdiWindow();

    void detach();

    void initDoc(RS_Document* doc=NULL);
    void initView();
    
    virtual QSize sizeHint() const {
        if (parentWidget()!=NULL) {
            return parentWidget()->size()/4*3;
        }
        else {
            return QWidget::sizeHint();
        }
    }

public slots:

    void slotPenChanged(const RS_Pen& p);

    void slotFileNew();
    bool slotFileOpen(const QString& fileName, RS2::FormatType type);
    bool slotFileSave(bool &cancelled);
    bool slotFileSaveAs(bool &cancelled, const QString& caption=QString());
    bool slotFileClose(bool force);

public:
    /** @return Pointer to graphic view */
    QG_GraphicView* getGraphicView() {
        return graphicView;
    }

    /** @return Pointer to document */
    RS_Document* getDocument() {
        return document;
    }
    
    /** @return Pointer to graphic or NULL */
    RS_Graphic* getGraphic() {
        return document->getGraphic();
    }

    /** @return Pointer to current event handler */
    RS_EventHandler* getEventHandler() {
        if (graphicView!=NULL) {
            return graphicView->getEventHandler();
        }
        else {
            return NULL;
        }
    }

    void addChildWindow(QG_MdiWindow* w);
    void removeChildWindow(QG_MdiWindow* w);
    QG_MdiWindow* getPrintPreview();

    /**
     * Sets the parent window that will be notified if this 
     */
    void setParentWindow(QG_MdiWindow* p) {
        RS_DEBUG->print("setParentWindow");
        parentWindow = p;
    }

    /**
     * @return The MDI window id.
     */
    int getId() {
        return id;
    }

    bool closeMdi(bool force, bool ask=true);

    void setForceClosing(bool on) {
        forceClosing = on;
    }
    bool isSecondaryView() {
        return !owner;
    }

signals:
    void signalClosing();

protected:
    virtual void closeEvent(QCloseEvent*);

private:
    /** window ID */
    int id;
    /** ID counter */
    static int idCounter;
    /** Graphic view */
    QG_GraphicView* graphicView;
    /** Document */
    RS_Document* document;
    /** Does the window own the document? */
    bool owner;
    /**
     * List of known child windows that show blocks, print previews, 
     *  other views of the same drawing.
     */
    RS_PtrList<QG_MdiWindow> childWindows;
    /**
     * Pointer to parent window which needs to know if this window 
     * is closed or NULL.
     */
    QG_MdiWindow* parentWindow;
    QG_MainWindow* appWin;

    /**
     * If flag is set, the user will not be asked before closing this file.
     */
    bool forceClosing;
};


#endif

