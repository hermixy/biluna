/****************************************************************************
** $Id: qg_graphicview.cpp 9378 2008-03-10 20:45:32Z andrew $
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

#include "qg_graphicview.h"

#include <qbitmap.h>
#include <qcursor.h>
#include <qlabel.h>
#include <QWheelEvent>
#include <QPixmap>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QGridLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QTabletEvent>
#include <QMouseEvent>
#include <QMessageBox>

#include "rs_actionzoomin.h"
#include "rs_actionzoompan.h"
#include "rs_actionzoomscroll.h"
#include "rs_actionmodifydelete.h"
#include "rs_actionselectsingle.h"
#include "rs_fileinfo.h"
#include "rs_graphic.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_painterqt.h"
#include "rs_patternlist.h"

#include "qg_dialogfactory.h"
#include "qg_ruler.h"


#define QG_SCROLLMARGIN 800


/**
 * Constructor.
 */
QG_GraphicView::QG_GraphicView(QWidget* parent, Qt::WFlags f)
        : QWidget(parent, f), RS_GraphicView() {

    setBackground(background);
    bufferOri = NULL;
    bufferTmp = NULL;
    useBufferTmp = false;
    //bufferChanged = false;
    lastWidth = 0;
    lastHeight = 0;
    alwaysAutoZoom = false;
        
    borderLeft = 10;
    borderTop = 10;
    borderRight = 10;
    borderBottom = 10;

    // layout that holds rulers, scroll bars and grid info widget
    layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 0);
    layout->setColumnStretch(3, 0);
    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 1);
    layout->setRowStretch(2, 0);
    
    rulerFrame = new QFrame(this);
    rulerFrame->setAutoFillBackground(true);
    rulerFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QPalette p = rulerFrame->palette();
    QColor bg = p.color(QPalette::Window);
    p.setColor(QPalette::Window, bg.darker(120));
    rulerFrame->setPalette(p);
    layout->addWidget(rulerFrame, 0, 0);
    
    // horizontal ruler
    hRuler = new QG_Ruler(true, this);
    hRuler->setGraphicView(this);
    layout->addWidget(hRuler, 0, 1, 1, 2);
    layout->setRowMinimumHeight(0, hRuler->sizeHint().height());
    //connect(hScrollBar, SIGNAL(valueChanged(int)),
    //        this, SLOT(slotHScrolled(int)));
    
    // vertical ruler
    vRuler = new QG_Ruler(false, this);
    vRuler->setGraphicView(this);
    layout->addWidget(vRuler, 1, 0, 1, 1);
    layout->setColumnMinimumWidth(0, vRuler->sizeHint().width());
    
    RS_SETTINGS->beginGroup("/Appearance");
    showRulers(RS_SETTINGS->readEntry("/ShowRulers", "1")=="1");
    RS_SETTINGS->endGroup();

    // horizontal scrollbar
    hScrollBar = new QG_ScrollBar(Qt::Horizontal, this);
    hScrollBar->setSingleStep(50);
    layout->addWidget(hScrollBar, 2, 0, 1, 2);
    layout->setRowMinimumHeight(2, hScrollBar->sizeHint().height());
    connect(hScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotHScrolled(int)));
    hbarHeight = hScrollBar->sizeHint().height();

    // vertical scrollbar
    vScrollBar = new QG_ScrollBar(Qt::Vertical, this);
    vScrollBar->setSingleStep(50);
    layout->addWidget(vScrollBar, 0, 3, 2, 1);
    layout->setColumnMinimumWidth(3, vScrollBar->sizeHint().width());
    connect(vScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slotVScrolled(int)));
    vbarWidth = vScrollBar->sizeHint().width();

//#ifndef QT_NO_XCURSOR
    // Mouse Cursors:
    QBitmap bmp;
    QPixmap cur1(":/qcad/images/cur_cad_bmp.png");
    bmp = QPixmap(":/qcad/images/cur_cad_mask.png");
    cur1.setMask(bmp);
    curCad = new QCursor(cur1, 15, 15);

    QPixmap cur2(":/qcad/images/cur_glass_bmp.png");
    bmp = QPixmap(":/qcad/images/cur_glass_mask.png");
    cur2.setMask(bmp);
    curMagnifier = new QCursor(cur2, 12, 12);

    QPixmap cur3(":/qcad/images/cur_del_bmp.png");
    bmp = QPixmap(":/qcad/images/cur_del_mask.png");
    cur3.setMask(bmp);
    curDel = new QCursor(cur3, 15, 15);

    QPixmap cur4(":/qcad/images/cur_select_bmp.png");
    bmp = QPixmap(":/qcad/images/cur_select_mask.png");
    cur4.setMask(bmp);
    curSelect = new QCursor(cur4, 15, 15);

    QPixmap cur5(":/qcad/images/cur_hand_bmp.png");
    bmp = QPixmap(":/qcad/images/cur_hand_mask.png");
    cur5.setMask(bmp);
    curHand = new QCursor(cur5, 15, 15);
/*
#else
    // No individual cursors for the Mac
    curCad = NULL;
    curMagnifier = NULL;
    curDel = NULL;
    curSelect = NULL;
    curHand = NULL;
#endif
*/

    // Dummy widgets for scrollbar corners:
    //layout->addWidget(new QWidget(this), 1, 1);
    //QWidget* w = new QWidget(this);
    //w->setEraseColor(QColor(255,0,0));
    gridStatus = new QLabel("-", this);
    gridStatus->setAlignment(Qt::AlignRight);
    // causes some graphic view issues:
    //gridStatus->setAutoFillBackground(true);
    layout->addWidget(gridStatus, 2, 2, 1, 2);
    layout->setColumnMinimumWidth(2, 50);
    
    setMouseTracking(true);
    // flickering under win:
    setFocusPolicy(Qt::WheelFocus);
    //setFocusPolicy(Qt::NoFocus);
    setAcceptDrops(true);

}



/**
 * Destructor
 */
QG_GraphicView::~QG_GraphicView() {
    if (painter!=NULL) {
        ((RS_PainterQt*)painter)->end();
        delete painter;
    }
    if (bufferOri!=NULL) {
        delete bufferOri;
        bufferOri = NULL;
    }
    if (bufferTmp!=NULL) {
        delete bufferTmp;
        bufferTmp = NULL;
    }
    
    delete curCad;
    delete curMagnifier;
    delete curDel;
    delete curSelect;
    delete curHand;
    
    curCad = NULL;
    curMagnifier = NULL;
    curDel = NULL;
    curSelect = NULL;
    curHand = NULL;

    cleanUp();
}



/**
 * @return width of widget.
 */
int QG_GraphicView::getWidth() {
    // ### Performance improvement by klingaard@gmail.com
    return width() - vbarWidth;
}



/**
 * @return height of widget.
 */
int QG_GraphicView::getHeight() {
    // ### Performance improvement by klingaard@gmail.com
    return height() - hbarHeight;
}



/**
 * Changes the current background color of this view.
 */
void QG_GraphicView::setBackground(const RS_Color& bg) {
    RS_GraphicView::setBackground(bg);
}



/**
 * Sets the mouse cursor to the given type.
 */
void QG_GraphicView::setMouseCursor(RS2::CursorType c) {

    switch (c) {
    default:
    case RS2::ArrowCursor:
        setCursor(Qt::ArrowCursor);
        break;
    case RS2::UpArrowCursor:
        setCursor(Qt::UpArrowCursor);
        break;
    case RS2::CrossCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::WaitCursor:
        setCursor(Qt::WaitCursor);
        break;
    case RS2::IbeamCursor:
        setCursor(Qt::IBeamCursor);
        break;
    case RS2::SizeVerCursor:
        setCursor(Qt::SizeVerCursor);
        break;
    case RS2::SizeHorCursor:
        setCursor(Qt::SizeHorCursor);
        break;
    case RS2::SizeBDiagCursor:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case RS2::SizeFDiagCursor:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case RS2::SizeAllCursor:
        setCursor(Qt::SizeAllCursor);
        break;
    case RS2::BlankCursor:
        setCursor(Qt::BlankCursor);
        break;
    case RS2::SplitVCursor:
        setCursor(Qt::SplitVCursor);
        break;
    case RS2::SplitHCursor:
        setCursor(Qt::SplitHCursor);
        break;
    case RS2::PointingHandCursor:
        setCursor(Qt::PointingHandCursor);
        break;
    case RS2::ForbiddenCursor:
        setCursor(Qt::ForbiddenCursor);
        break;
    case RS2::WhatsThisCursor:
        setCursor(Qt::WhatsThisCursor);
        break;

//#ifndef QT_NO_XCURSOR
    case RS2::CadCursor:
        setCursor(*curCad);
        break;
    case RS2::DelCursor:
        setCursor(*curDel);
        break;
    case RS2::SelectCursor:
        setCursor(*curSelect);
        break;
    case RS2::MagnifierCursor:
        setCursor(*curMagnifier);
        break;
    case RS2::MovingHandCursor:
        setCursor(*curHand);
        break;
/*
#else
        // Reduced cursor selection for the Mac and older linuxes
    case RS2::CadCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::DelCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::SelectCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::MagnifierCursor:
        setCursor(Qt::CrossCursor);
        break;
    case RS2::MovingHandCursor:
        setCursor(Qt::PointingHandCursor);
        break;
#endif
*/
    }

}



/**
 * Sets the text for the grid status widget in the left bottom corner.
 */
void QG_GraphicView::updateGridStatusWidget(const RS_String& text) {
    gridStatus->setText(text);
}
    
    
    
void QG_GraphicView::showRulers(bool on) {
    hRuler->setVisible(on);
    vRuler->setVisible(on);
    rulerFrame->setVisible(on);

    if (on) {
        borderLeft = 30;
        borderTop = 30;
    }
    else {
        borderLeft = 10;
        borderTop = 10;
    }
}



/**
 * Redraws the widget.
 */
void QG_GraphicView::redraw() {

    if (simulationRunning) {
        //return;
    }

    // never run twice
    static bool running = false;
    if (!running) {
        running = true;

        if (isUpdateEnabled()) {
            if (simulationRunning) {
                useBufferTmp = true;
            }
            else {
                useBufferTmp = false;
            }
            repaint();
        }

        running = false;
    }
    else {
        RS_DEBUG->print("QG_GraphicView::redraw: already running");
    }
}



/**
 * Redraws the widget with the current preview.
 */
void QG_GraphicView::redrawPreview() {
    RS_DEBUG->print("QG_GraphicView::redrawPreview");
    repaint();
    RS_DEBUG->print("QG_GraphicView::redrawPreview: OK");
}


/**
 * Restores the widget (quick).
 */
void QG_GraphicView::restore() {
    RS_DEBUG->print("QG_GraphicView::restore");
    createDirectPainter();
    repaint();
    destroyPainter();
    RS_DEBUG->print("QG_GraphicView::restore");
}


void QG_GraphicView::resizeEvent(QResizeEvent* e) {
    RS_DEBUG->print("QG_GraphicView::resizeEvent");
    if (alwaysAutoZoom) {
        zoomAuto();
    }

    adjustOffsetControls();
    adjustZoomControls();
    updateGrid();
    useBufferTmp = false;
    e->accept();

    RS_DEBUG->print("QG_GraphicView::resizeEvent: OK");
}



void QG_GraphicView::emulateMouseMoveEvent() {
    RS_MouseEvent e(QEvent::MouseMove, QPoint(mx, my),
                    Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    mouseMoveEvent(&e);
}

void QG_GraphicView::emulateMousePressEvent() {
    RS_MouseEvent e(QEvent::MouseButtonPress, QPoint(mx, my),
                    Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mousePressEvent(&e);
}

void QG_GraphicView::emulateMouseReleaseEvent() {
    RS_MouseEvent e(QEvent::MouseButtonRelease, QPoint(mx, my),
                    Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mouseReleaseEvent(&e);
}



void QG_GraphicView::mousePressEvent(QMouseEvent* e) {
    // pan zoom with middle mouse button
    if (e->button()==Qt::MidButton /*|| (e->state()==Qt::LeftButton|Qt::AltButton)*/) {
        if (container!=NULL) {
            saveView();
            setCurrentAction(new RS_ActionZoomPan(*container, *this));
        }
        else {
            RS_DEBUG->print(RS_Debug::D_WARNING, 
                "QG_GraphicView::mousePressEvent: No container set");
        }
    }

    RS_GraphicView::mousePressEvent(e);
}


void QG_GraphicView::mouseReleaseEvent(QMouseEvent* e) {
    RS_GraphicView::mouseReleaseEvent(e);
}


void QG_GraphicView::mouseMoveEvent(QMouseEvent* e) {
    RS_GraphicView::mouseMoveEvent(e);
}


/**
 * support for the wacom graphic tablet.
 */
void QG_GraphicView::tabletEvent(QTabletEvent* e) {
    if (underMouse()) {
        switch (e->device()) {
        case QTabletEvent::Eraser:
            if (e->type()==QEvent::TabletRelease) {
                if (container!=NULL) {

                    RS_ActionSelectSingle* a =
                        new RS_ActionSelectSingle(*container, *this);
                    setCurrentAction(a);
                    QMouseEvent ev(QEvent::MouseButtonRelease, e->pos(),
                                   Qt::LeftButton, Qt::LeftButton,
                                   Qt::NoModifier);
                    mouseReleaseEvent(&ev);
                    a->finish();

                    if (container->countSelected()>0) {
                        setCurrentAction(
                            new RS_ActionModifyDelete(*container, *this));
                    }
                }
            }
            break;

        case QTabletEvent::Stylus:
        case QTabletEvent::Puck:
            if (e->type()==QEvent::TabletPress) {
                QMouseEvent ev(QEvent::MouseButtonPress, e->pos(),
                               Qt::LeftButton, Qt::LeftButton,
                               Qt::NoModifier);
                mousePressEvent(&ev);
            } else if (e->type()==QEvent::TabletRelease) {
                QMouseEvent ev(QEvent::MouseButtonRelease, e->pos(),
                               Qt::LeftButton, Qt::LeftButton,
                               Qt::NoModifier);
                mouseReleaseEvent(&ev);
            } else if (e->type()==QEvent::TabletMove) {
                QMouseEvent ev(QEvent::MouseMove, e->pos(),
                               Qt::NoButton, Qt::NoButton,
                               Qt::NoModifier);
                mouseMoveEvent(&ev);
            }
            break;

        default:
            break;
        }
    }

    // a 'mouse' click:
    /*
    if (e->pressure()>10 && lastPressure<10) {
        QMouseEvent e(QEvent::MouseButtonPress, e->pos(), 
           Qt::LeftButton, Qt::LeftButton);
        mousePressEvent(&e);
}
    else if (e->pressure()<10 && lastPressure>10) {
        QMouseEvent e(QEvent::MouseButtonRelease, e->pos(), 
           Qt::LeftButton, Qt::LeftButton);
        mouseReleaseEvent(&e);
}    else if (lastPos!=e->pos()) {
        QMouseEvent e(QEvent::MouseMove, e->pos(), 
           Qt::NoButton, 0);
        mouseMoveEvent(&e);
}

    lastPressure = e->pressure();
    lastPos = e->pos();
    */
}

void QG_GraphicView::leaveEvent(QEvent* e) {
    RS_GraphicView::mouseLeaveEvent();
    QWidget::leaveEvent(e);
}


void QG_GraphicView::enterEvent(QEvent* e) {
    RS_GraphicView::mouseEnterEvent();
    QWidget::enterEvent(e);
}


void QG_GraphicView::focusOutEvent(QFocusEvent* e) {
    QWidget::focusOutEvent(e);
}


void QG_GraphicView::focusInEvent(QFocusEvent* e) {
    RS_GraphicView::mouseEnterEvent();
    QWidget::focusInEvent(e);
}


/**
 * mouse wheel event. zooms in/out or scrolls when
 * shift or ctrl is pressed.
 */
void QG_GraphicView::wheelEvent(QWheelEvent *e) {
    if (container==NULL) {
        return;
    }

    RS_Vector mouse = toGraph(RS_Vector(e->x(), e->y()));

    bool scroll = false;
    RS2::Direction direction = RS2::Up;

    // scroll up / down:
    if (e->modifiers()==Qt::ControlModifier) {
        scroll = true;
        if (e->delta()>0) {
            direction = RS2::Up;
        } else {
            direction = RS2::Down;
        }
    }

    // scroll left / right:
    else if (e->modifiers()==Qt::ShiftModifier) {
        scroll = true;
        if (e->delta()>0) {
            direction = RS2::Right;
        } else {
            direction = RS2::Left;
        }
    }

    if (scroll) {
        if (container!=NULL) {
            setCurrentAction(new RS_ActionZoomScroll(direction,
                             *container, *this));
        }
    }

    // zoom in / out:
    else if (e->modifiers()==Qt::NoModifier) {
        if (container!=NULL) {
            if (e->delta()>0) {
                setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                                     RS2::In, RS2::Both,
                                                     mouse));
            } else {
                setCurrentAction(new RS_ActionZoomIn(*container, *this,
                                                     RS2::Out, RS2::Both,
                                                     mouse));
            }
        }
    }

    e->accept();
}


void QG_GraphicView::keyPressEvent(QKeyEvent* e) {
    if (container==NULL) {
        e->ignore();
        return;
    }

    bool scroll = false;
    RS2::Direction direction = RS2::Up;

    switch (e->key()) {
    case Qt::Key_Left:
        scroll = true;
        direction = RS2::Right;
        break;
    case Qt::Key_Right:
        scroll = true;
        direction = RS2::Left;
        break;
    case Qt::Key_Up:
        scroll = true;
        direction = RS2::Up;
        break;
    case Qt::Key_Down:
        scroll = true;
        direction = RS2::Down;
        break;
    default:
        scroll = false;
        break;
    }

    if (scroll) {
        setCurrentAction(new RS_ActionZoomScroll(direction,
                         *container, *this));
    }

    RS_GraphicView::keyPressEvent(e);
}


void QG_GraphicView::keyReleaseEvent(QKeyEvent* e) {
    RS_GraphicView::keyReleaseEvent(e);
}


/**
 * Called whenever the graphic view has changed.
 * Adjusts the scrollbar ranges / steps.
 */
void QG_GraphicView::adjustOffsetControls() {
    static bool running = false;

    if (running) {
        return;
    }

    running = true;

    RS_DEBUG->print("QG_GraphicView::adjustOffsetControls");

    if (container==NULL || hScrollBar==NULL || vScrollBar==NULL) {
        RS_DEBUG->print("QG_GraphicView::adjustOffsetControls: abort");
        return;
    }

    disableUpdate();
    int ox = getOffsetX();
    int oy = getOffsetY();
    
    RS_Vector min = container->getMin();
    RS_Vector max = container->getMax();

    // no drawing yet - still allow to scroll
    if (max.x < min.x+1.0e-6 || 
        max.y < min.y+1.0e-6 ||
        max.x > RS_MAXDOUBLE || 
        max.x < RS_MINDOUBLE || 
        min.x > RS_MAXDOUBLE || 
        min.x < RS_MINDOUBLE ||
        max.y > RS_MAXDOUBLE || 
        max.y < RS_MINDOUBLE || 
        min.y > RS_MAXDOUBLE || 
        min.y < RS_MINDOUBLE ) {
        min = RS_Vector(-10,-10);
        max = RS_Vector(100,100);
    }
    
    int minVal = (int)(min.x * getFactor().x 
            - QG_SCROLLMARGIN - getBorderLeft());
    int maxVal = (int)(max.x * getFactor().x 
            - getWidth() + QG_SCROLLMARGIN + getBorderRight());

    hScrollBar->setValue(0);
    if (minVal<=maxVal) {
        hScrollBar->setRange(minVal, maxVal);
        RS_DEBUG->print("QG_GraphicView::adjustOffsetControls: "
            "hScrollBar range: %d...%d", minVal, maxVal);
    }

    minVal = (int)(getHeight() - max.y * getFactor().y 
            - QG_SCROLLMARGIN - getBorderTop());
    maxVal = (int)(QG_SCROLLMARGIN + getBorderBottom() 
            - (min.y * getFactor().y));

    if (minVal<=maxVal) {
        vScrollBar->setRange(minVal, maxVal);
    }

    hScrollBar->setPageStep((int)(getWidth()));
    vScrollBar->setPageStep((int)(getHeight()));

    hScrollBar->setValue(-ox);
    RS_DEBUG->print("QG_GraphicView::adjustOffsetControls: "
        "hScrollBar value: %d", -ox);
    vScrollBar->setValue(oy);
    
    slotHScrolled(-ox);
    slotVScrolled(oy);

    enableUpdate();

    RS_DEBUG->print("QG_GraphicView::adjustOffsetControls: OK");

    running = false;
}


/**
 * override this to adjust controls and widgets that
 * control the zoom factor of the graphic.
 */
void QG_GraphicView::adjustZoomControls() {}


/**
 * Slot for horizontal scroll events.
 */
void QG_GraphicView::slotHScrolled(int value) {
    if (hScrollBar->maximum()==hScrollBar->minimum()) {
        centerOffsetX();
    } else {
        setOffsetX(-value);
    }
    updateGrid();
    hRuler->update();
    redraw();
}


/**
 * Slot for vertical scroll events.
 */
void QG_GraphicView::slotVScrolled(int value) {
    if (vScrollBar->maximum()==vScrollBar->minimum()) {
        centerOffsetY();
    } else {
        setOffsetY(value);
    }
    updateGrid();
    vRuler->update();
    redraw();
}



/**
 * Creates a new painter for the buffer of this widget and returns a 
 * pointer to it. The class variable 'painter' also
 * points to that object.
 */
RS_Painter* QG_GraphicView::createPainter() {
    RS_DEBUG->print("QG_GraphicView::createPainter begin");

    if (lastWidth!=getWidth() || lastHeight!=getHeight()) {
        destroyPainter();
        if (bufferOri!=NULL) {
            delete bufferOri;
            bufferOri = NULL;
        }
        if (bufferTmp!=NULL) {
            delete bufferTmp;
            bufferTmp = NULL;
        }
        lastWidth = getWidth();
        lastHeight = getHeight();
    }

    if (bufferOri==NULL) {
        RS_DEBUG->print("QG_GraphicView::createPainter: "
            "creating bufferOri: %d,%d", getWidth(), getHeight());
        bufferOri = new QPixmap(getWidth(), getHeight());
    }
    
    if (painter!=NULL && ((RS_PainterQt*)painter)->device()!=bufferOri) {
        destroyPainter();
    }
    
    if (painter==NULL) {
        painter = new RS_PainterQt(bufferOri);
        painter->setDrawingMode(drawingMode);
    }

    RS_DEBUG->print("QG_GraphicView::createPainter end");

    return painter;
}



/**
 * Creates a new painter for this widget and returns a 
 * pointer to it. The class variable 'painter' also
 * automatically points to that object.
 */
RS_Painter* QG_GraphicView::createDirectPainter() {

    RS_DEBUG->print("QG_GraphicView::createDirectPainter begin");

    destroyPainter();
    
    if (bufferTmp==NULL) {
        bufferTmp = new QPixmap(getWidth(), getHeight());
    }
    
    if (bufferTmp!=NULL) {
        if (bufferOri!=NULL) {
        
            RS_DEBUG->print(RS_Debug::D_TESTING, 
                "QG_GraphicView::createDirectPainter: copy ori bufferOri");
            
            QPainter wPainter;
            wPainter.begin(bufferTmp);
            wPainter.drawPixmap(0,0,*bufferOri);
            wPainter.end();
        }
    }
    painter = new RS_PainterQt(bufferTmp);

    RS_DEBUG->print("QG_GraphicView::createDirectPainter end");

    return painter;
}


/**
 * Creates a new painter for simulations.
 */
RS_Painter* QG_GraphicView::createSimPainter() {

    RS_DEBUG->print("QG_GraphicView::createSimPainter begin");

    if (painter!=NULL && bufferTmp!=NULL && ((RS_PainterQt*)painter)->device()==bufferTmp) {
        return painter;
    }

    destroyPainter();
    
    if (bufferTmp==NULL) {
        RS_DEBUG->print("QG_GraphicView::createSimPainter: creating new bufferTmp");
        bufferTmp = new QPixmap(getWidth(), getHeight());
    }
    painter = new RS_PainterQt(bufferTmp);
    painter->setBackground(background);

    RS_DEBUG->print("QG_GraphicView::createSimPainter end");

    return painter;
}



/**
 * Deletes the painter.
 */
void QG_GraphicView::destroyPainter() {

    RS_DEBUG->print("QG_GraphicView::destroyPainter begin");

    if (painter!=NULL) {
        RS_DEBUG->print("QG_GraphicView::destroyPainter: deleting painter");
        delete painter;
        painter = NULL;
        RS_DEBUG->print("QG_GraphicView::destroyPainter: deleting painter: OK");
    }

    RS_DEBUG->print("QG_GraphicView::destroyPainter end");
}



/**
 * Handles paint events by redrawing the graphic in this view.
 * usually that's very fast since we only paint the buffer we
 * have from the last call..
 */
void QG_GraphicView::paintEvent(QPaintEvent *) {
    RS_DEBUG->print("QG_GraphicView::paintEvent");
    if (simulationRunning) {
        RS_DEBUG->print("QG_GraphicView::paintEvent: simulation is running");
        destroyPainter();
        if (bufferTmp!=NULL) {
            RS_PainterQt wPainter(this);
            wPainter.drawPixmap(0,0,*bufferTmp);
            setPainter(&wPainter);
            drawEntity(preview);
            drawAbsoluteZero();
            setPainter(NULL);
            wPainter.end();
        }
    }
    else if (useBufferTmp) {
        RS_DEBUG->print("QG_GraphicView::paintEvent: bufferTmp");
        createDirectPainter();
        destroyPainter();
        
        if (bufferTmp!=NULL) {
            RS_PainterQt wPainter(this);
            wPainter.drawPixmap(0,0,*bufferTmp);
            setPainter(&wPainter);
            drawEntity(preview);
            drawRelativeZero();
            setPainter(NULL);
            wPainter.end();
        }
    }
    else {
        RS_DEBUG->print("QG_GraphicView::paintEvent: bufferOri");
        createPainter();
        ((RS_PainterQt*)painter)->setBackground(background);
        ((RS_PainterQt*)painter)->eraseRect(0,0,getWidth(), getHeight());
        drawIt();
        destroyPainter();
    
        if (bufferOri!=NULL) {
            RS_PainterQt wPainter(this);
            wPainter.drawPixmap(0,0,*bufferOri);
            setPainter(&wPainter);
            drawRelativeZero();
            setPainter(NULL);
            wPainter.end();
        }
    }
    useBufferTmp = true;
        
    if (RS_DIALOGFACTORY!=NULL && RS_DIALOGFACTORY->getMainWindow()!=NULL) {
        RS_DIALOGFACTORY->getMainWindow()->graphicViewRedrawn();
    }

    RS_DEBUG->print("QG_GraphicView::paintEvent: OK");
}



/**
 * Previews the given url for the file open dialog.
 */
/*
void QG_GraphicView::previewUrl(const QUrl &u) {
    
    RS_DEBUG->print("QG_GraphicView::previewUrl");
    
    if (container!=NULL && container->rtti()==RS2::EntityGraphic) {
        ((RS_Graphic*)container)->open(u.path(), RS2::FormatUnknown);
        zoomAuto();
    }
    
    RS_DEBUG->print("QG_GraphicView::previewUrl: OK");
}
*/


void QG_GraphicView::dragEnterEvent(QDragEnterEvent* event) {
	RS_DEBUG->print("QG_GraphicView::dragEnterEvent");

    /*
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasImage()){
    	event->acceptProposedAction();
    }
    */
    
    RS_GraphicView::dragEnterEvent(event);
}


void QG_GraphicView::dragMoveEvent(QDragMoveEvent* event) {
	RS_DEBUG->print("QG_GraphicView::dragMoveEvent");

    RS_GraphicView::dragMoveEvent(event);
}


void QG_GraphicView::dropEvent(QDropEvent* event) {
    RS_DEBUG->print("QG_GraphicView::dropEvent");

    /*
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasImage()){
    	// TODO this should be moved to the "main drawing area"
    	QMessageBox::information(
    			this, tr("QG_GraphicView::dropEvent"), tr("canvas got the drag"));
    }

    event->acceptProposedAction();
    
    RS_DEBUG->print("QG_GraphicView::dropEvent: OK");
    */
    
    RS_GraphicView::dropEvent(event);
}
