/*****************************************************************
 * $Id: gv_graphicsview.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Mar 24, 2008 11:15:59 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_graphicsview.h"

#include <QWheelEvent>
#include "gv_component.h"
#include "gv_drawing.h"
#include "gv_graphicsscene.h"
#include "rb_debug.h"
#include "RMath.h"
#include "rb_settings.h"

GV_GraphicsView::GV_GraphicsView(QWidget* parent) : QGraphicsView(parent) {

    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
    // this->setRenderHints(QPainter::Antialiasing);
    // this->setOptimizationFlags(QGraphicsView::DontClipPainter);
    this->setInteractive(true);

    mBeingScaled = false;
    setAcceptDrops(true);
    // setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); for animation better

    connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(slotSliderValueChanged(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(slotSliderValueChanged(int)));

}

GV_GraphicsView::~GV_GraphicsView() {
    // to prevent deletion of drawing
    setScene(NULL);
    RB_DEBUG->print("GV_GraphicsView::~GV_GraphicsView() OK");
}

/**
 * Set current mouse cursor type
 * @param mouse cursor shape
 */
void GV_GraphicsView::setMouseCursor(Qt::CursorShape c) {

    QGraphicsView::setCursor(c);

/*
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
    }
*/
}

/**
 * Set scene and create background with grid and drawing border, overrides
 * basic setScene()
 * @param dwg drawing
 */
void GV_GraphicsView::setScene(QGraphicsScene* drawing) {
    QGraphicsView::setScene(drawing);

    // NULL is used when scene is detached from graphicsview
    if (drawing != NULL) {
        GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(drawing);
        dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
        viewport()->repaint(viewport()->rect());
    }
}

/**
 * Get viewport height
 */
int GV_GraphicsView::getHeight() {
    return viewport()->height();
}

/**
 * Get viewport width
 */
int GV_GraphicsView::getWidth() {
    return viewport()->width();
}

/**
 * Zoom in
 */
void GV_GraphicsView::zoomIn() {
    scaleView(pow(2.0, 0.5) );
}

/**
 * Zoom out
 */
void GV_GraphicsView::zoomOut() {
    scaleView(1 / pow(2.0, 0.5) );
}

/**
 * Scrollbar slider changed position: generate and paint grid
 */
void GV_GraphicsView::slotSliderValueChanged(int /* value */) {
    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
    if (!dwg || mBeingScaled) return;

    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());
}

void GV_GraphicsView::wheelEvent(QWheelEvent* event) {
    scaleView(1 / (pow((double)2, -event->delta() / 240.0)) );
}

/**
 * Scale the view
 * @param sf additional scaling to be done on view
 */
void GV_GraphicsView::scaleView(double sf) {
    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
    if (!dwg) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_GraphicsView::scaleView()"
                        " scene = NULL ERROR");
        return;
    }

    mBeingScaled = true; // prevent again repainting by slotSliderValueChanged()

    double totalScaleFactor = matrix().scale(sf, sf)
                        .mapRect(QRectF(0, 0, 1.0, 1.0)).width();
    if (totalScaleFactor < 0.36 || totalScaleFactor > 28) {
        return;
    }

    dwg->setScaleFactor(totalScaleFactor);
    scale(sf, sf);
    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());

    mBeingScaled = false;
}

/**
 * Drag enter event chatched here only to accept proposed action
 */
void GV_GraphicsView::dragEnterEvent(QDragEnterEvent* e) {
    RB_DEBUG->print("GV_GraphicsView::dragEnterEvent()");
    // can set filter here for acceptable MIME types
    e->acceptProposedAction();
}

/**
 * Drag move event chatched here only to accept proposed action
 */
void GV_GraphicsView::dragMoveEvent(QDragMoveEvent* e) {
    e->acceptProposedAction();
}

/**
 * Drop event chatched here only to activate the GV_ActionDrawSymbol
 */
void GV_GraphicsView::dropEvent(QDropEvent* e) {
    RB_DEBUG->print("GV_GraphicsView::dropEvent()");

    if (e->mimeData()->hasText()) {
        if (e->mimeData()->text() == "GV_Symbol") {
//			GV_ActionDrawSymbol::factory();

        } else if (e->mimeData()->text() == "GV_Line") {
//			GV_ActionDrawLine::factory();

        }
    }

    QGraphicsView::dropEvent(e);
}

/**
 * Resize of graphicsview, no should be done within this event
 */
void GV_GraphicsView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);

    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
    if (!dwg) return;
    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
}
