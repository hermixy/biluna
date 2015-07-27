/*****************************************************************
 * $Id: gv_gvmdi.cpp 911 2009-01-15 21:12:49Z rutger $
 * Created: Mar 24, 2008 11:15:59 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_gvmdi.h"

#include "gv_actiondrawline.h"
#include "gv_actiondrawsymbol.h"
#include "gv_component.h"
#include "gv_drawing.h"
#include "rb_debug.h"
#include "rb_math.h"
#include "rb_settings.h"

GV_GvMdi::GV_GvMdi(QWidget* parent) : GV_GraphicsView(parent) {
	RB_DEBUG->print("GV_GvMdi::GV_GvMdi()");
	mBeingScaled = false;
    setAcceptDrops(true);
    // setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate); for animation better
    
    connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)), 
    		this, SLOT(slotSliderValueChanged(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), 
    		this, SLOT(slotSliderValueChanged(int)));
}

GV_GvMdi::~GV_GvMdi() {
	RB_DEBUG->print("GV_GvMdi::~GV_GvMdi()");

//	GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(scene());
	GV_GraphicsView::setScene(NULL);
//	if (dwg) {
//		delete dwg;
//	}

	RB_DEBUG->print("GV_GvMdi::~GV_GvMdi() OK");
}

/**
 * Set scene and create background with grid and drawing border, overrides
 * basic setScene()
 * @param dwg drawing 
 */
void GV_GvMdi::setScene(QGraphicsScene* drawing) {
	GV_GraphicsView::setScene(drawing);
	
	// NULL is used when scene is detached from graphicsview
	if (drawing != NULL) {
		GV_Drawing* dwg = (GV_Drawing*)drawing;
		dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
		viewport()->repaint(viewport()->rect());
	}
}

/**
 * Scale view, generate and paint grid
 * @param scaleFactor factor to scale the drawing
 */
void GV_GvMdi::scaleView(double scaleFactor) {
	mBeingScaled = true; // prevent again repainting by slotSliderValueChanged()
	
    GV_GraphicsView::scaleView(scaleFactor);
	GV_Drawing* dwg = (GV_Drawing*)scene();
	dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());
    
    mBeingScaled = false;
}

/**
 * Scrollbar slider changed position: generate and paint grid
 */
void GV_GvMdi::slotSliderValueChanged(int /* value */) {
	GV_Drawing* dwg = (GV_Drawing*)scene();
	if (!dwg || mBeingScaled) return;

	dwg->updateGrid(mapToScene(viewport()->rect()).boundingRect());
    viewport()->repaint(viewport()->rect());
}

/**
 * Get viewport height
 */
int GV_GvMdi::getHeight() {
	return viewport()->height();
}

/**
 * Get viewport width
 */
int GV_GvMdi::getWidth() {
	return viewport()->width();
}

/**
 * Drag enter event chatched here only to accept proposed action
 */
void GV_GvMdi::dragEnterEvent(QDragEnterEvent* e) {
	RB_DEBUG->print("GV_GvMdi::dragEnterEvent()");
	// can set filter here for acceptable MIME types
	e->acceptProposedAction();
}

/**
 * Drag move event chatched here only to accept proposed action
 */
void GV_GvMdi::dragMoveEvent(QDragMoveEvent* e) {
	// RB_DEBUG->print("%s [%i]", __PRETTY_FUNCTION__, __LINE__);
	e->acceptProposedAction();
}

/**
 * Drop event chatched here only to activate the GV_ActionDrawSymbol
 */
void GV_GvMdi::dropEvent(QDropEvent* e) {
	RB_DEBUG->print("GV_GvMdi::dropEvent()");

	if (e->mimeData()->hasText()) {
		if (e->mimeData()->text() == "GV_Symbol") {
			GV_ActionDrawSymbol::factory();

		} else if (e->mimeData()->text() == "GV_Line") {
			GV_ActionDrawLine::factory();

		}
	}

	GV_GraphicsView::dropEvent(e);
}
