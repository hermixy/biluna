/****************************************************************************
** $Id: qg_graphicview.h 7383 2007-11-29 16:37:42Z andrew $
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

#ifndef QG_GRAPHICVIEW_H
#define QG_GRAPHICVIEW_H

#include <qwidget.h>
#include <qlabel.h>
#include <qscrollbar.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qurl.h>
#include <qfiledialog.h>
#include <QWheelEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QEvent>
#include <QResizeEvent>
#include <QTabletEvent>
#include <QMouseEvent>

#include "rs_graphicview.h"
//#include "rs_layerlistlistener.h"
#include "rs_blocklistlistener.h"
#include "rs_painterqt.h"

#include "qg_ruler.h"
#include "qg_scrollbar.h"



/**
 * This is the Qt implementation of a widget which can view a 
 * graphic. 
 *
 * Instances of this class can be linked to layer lists using
 * addLayerListListener().
 */
class QG_GraphicView: public QWidget,
            public RS_GraphicView,
            /*public QFilePreview,*/
            /*public RS_LayerListListener,*/
            public RS_BlockListListener {
    Q_OBJECT

public:
    QG_GraphicView(QWidget* parent=0, Qt::WindowFlags f=0);
    virtual ~QG_GraphicView();

    virtual int getWidth();
    virtual int getHeight();
    virtual void redraw();
    virtual void redrawPreview();
    virtual void restore();
    virtual void setAlwaysAutoZoom(bool aaz) {
        alwaysAutoZoom = aaz;
    }
    virtual void adjustOffsetControls();
    virtual void adjustZoomControls();
    virtual RS_Painter* createPainter();
    virtual RS_Painter* createDirectPainter();
    virtual RS_Painter* createSimPainter();
    virtual void destroyPainter();
    virtual void setBackground(const RS_Color& bg);
    virtual void setMouseCursor(RS2::CursorType c);
    virtual void updateGridStatusWidget(const RS_String& text);
    virtual void showRulers(bool on);

public slots:
    void slotRedraw() {
        redraw();
    }

    // Methods from RS_LayerListListener Interface:
    /*
    void layerEdited(RS_Layer*) {
        redraw();
    }
    void layerRemoved(RS_Layer*) {
        redraw();
    }
    void layerToggled(RS_Layer*) {
        redraw();
    }
    void layerAdded(RS_Layer*) {
        RS_DEBUG->print("QG_GraphicView::layerAdded");
    }
    */

protected:
    virtual void emulateMouseMoveEvent();
    virtual void emulateMousePressEvent();
    virtual void emulateMouseReleaseEvent();
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
#if QT_VERSION>=0x030000    
    virtual void tabletEvent(QTabletEvent* e);
#endif    
    virtual void leaveEvent(QEvent*);
    virtual void enterEvent(QEvent*);
    virtual void focusInEvent(QFocusEvent*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent* e);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    //void previewUrl(const QUrl &u);

private slots:
    void slotHScrolled(int value);
    void slotVScrolled(int value);

private:
    //! Buffer containing always only the original drawing
    QPixmap* bufferOri;
    //! Buffer containing the drawing (can be overdrawn with previews)
    QPixmap* bufferTmp;
    bool useBufferTmp;
    //! Always autozoom when view is resized
    bool alwaysAutoZoom;
    //! cached horizontal scrollbar height (klingaard@gmail.com)
    unsigned int hbarHeight;
    //! cached vertical scrollbar width (klingaard@gmail.com)
    unsigned int vbarWidth;
    
protected:
    int lastWidth;
    int lastHeight;

    //! Small frame to cover intersection of rulers
    QFrame* rulerFrame;
    //! Horizontal ruler.
    QG_Ruler* hRuler;
    //! Vertical ruler.
    QG_Ruler* vRuler;
    //! Horizontal scrollbar.
    QG_ScrollBar* hScrollBar;
    //! Vertical scrollbar.
    QG_ScrollBar* vScrollBar;
    //! Layout used to fit in the view and the scrollbars.
    QGridLayout* layout;
    //! Label for grid spacing.
    QLabel* gridStatus;
    //! CAD mouse cursor
    QCursor* curCad;
    //! Delete mouse cursor
    QCursor* curDel;
    //! Select mouse cursor
    QCursor* curSelect;
    //! Magnifying glass mouse cursor
    QCursor* curMagnifier;
    //! Hand mouse cursor
    QCursor* curHand;
};

#endif
