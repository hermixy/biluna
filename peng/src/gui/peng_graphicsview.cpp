/*****************************************************************
 * $Id: peng_graphicsview.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Mar 24, 2008 11:15:59 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_graphicsview.h"

#include <QMimeData>
#include <QWheelEvent>
#include <cmath>
//#include "gv_component.h"
//#include "gv_drawing.h"
//#include "gv_graphicsscene.h"
#include "rb_debug.h"
//#include "rb_math.h"
#include "rb_settings.h"

PENG_GraphicsView::PENG_GraphicsView(QWidget* parent) : QGraphicsView(parent) {

	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
	// this->setRenderHints(QPainter::Antialiasing);
	// this->setOptimizationFlags(QGraphicsView::DontClipPainter);
	this->setInteractive(true);
	
//	mScaleFactor = 1.0;
//	mGrid = NULL;

    mBeingScaled = false;
    setAcceptDrops(true);
    // setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); for animation better

//    connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)),
//            this, SLOT(slotSliderValueChanged(int)));
//    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)),
//            this, SLOT(slotSliderValueChanged(int)));

}

PENG_GraphicsView::~PENG_GraphicsView() {
    // to prevent deletion of drawing
    setScene(NULL);
    RB_DEBUG->print("PENG_GraphicsView::~PENG_GraphicsView() OK");
}

/**
 * Set current mouse cursor type
 * @param mouse cursor shape
 */
void PENG_GraphicsView::setMouseCursor(Qt::CursorShape c) {

	QGraphicsView::setCursor(c);
	
}

/**
 * Set scene and create background with grid and drawing border, overrides
 * basic setScene()
 * @param dwg drawing
 */
void PENG_GraphicsView::setScene(QGraphicsScene* drawing) {
    QGraphicsView::setScene(drawing);

    // NULL is used when scene is detached from graphicsview
    if (drawing != NULL) {
//        GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(drawing);
//        dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
        viewport()->repaint(viewport()->rect());
    }
}

/**
 * Get viewport height
 */
int PENG_GraphicsView::getHeight() {
    return viewport()->height();
}

/**
 * Get viewport width
 */
int PENG_GraphicsView::getWidth() {
    return viewport()->width();
}

/**
 * Scrollbar slider changed position: generate and paint grid
 */
void PENG_GraphicsView::slotSliderValueChanged(int /* value */) {
//    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
//    if (!dwg || mBeingScaled) return;
//
//    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());
}

void PENG_GraphicsView::wheelEvent(QWheelEvent* event) {
    scaleView(1 / (std::pow((double)2, -event->delta() / 240.0)) );
}

/**
 * Scale the view
 * @param sf additional scaling to be done on view
 */
void PENG_GraphicsView::scaleView(double sf) {
    mBeingScaled = true; // prevent again repainting by slotSliderValueChanged()

    double totalScaleFactor = matrix().scale(sf, sf)
                        .mapRect(QRectF(0, 0, 1, 1)).width();
    if (totalScaleFactor < 0.36 || totalScaleFactor > 28) {
        return;
    }

//    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
//    dwg->setScaleFactor(totalScaleFactor);
    scale(sf, sf);
//    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());

    mBeingScaled = false;
}

/**
 * Drag enter event chatched here only to accept proposed action
 */
void PENG_GraphicsView::dragEnterEvent(QDragEnterEvent* e) {
    RB_DEBUG->print("PENG_GraphicsView::dragEnterEvent()");
    // can set filter here for acceptable MIME types
    e->acceptProposedAction();
}

/**
 * Drag move event chatched here only to accept proposed action
 */
void PENG_GraphicsView::dragMoveEvent(QDragMoveEvent* e) {
    // RB_DEBUG->print("%s [%i]", __PRETTY_FUNCTION__, __LINE__);
    e->acceptProposedAction();
}

/**
 * Drop event chatched here only to activate the GV_ActionDrawSymbol
 */
void PENG_GraphicsView::dropEvent(QDropEvent* e) {
    RB_DEBUG->print("PENG_GraphicsView::dropEvent()");

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
void PENG_GraphicsView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);

//    GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
//    dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
}
