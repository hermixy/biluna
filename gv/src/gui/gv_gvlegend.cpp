/*****************************************************************
 * $Id: gv_gvlegend.cpp 1148 2010-05-08 11:53:42Z rutger $
 * Created: Oct 18, 2008 3:32:52 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_gvlegend.h"

#include "gv_actiondrawsymbol.h"
#include "gv_dialogfactory.h"
#include "gv_legendwidget.h"
#include "gv_line.h"
#include "gv_symbol.h"
#include "rb_debug.h"


GV_GvLegend::GV_GvLegend(QWidget* parent) : GV_GraphicsView(parent) {
	RB_DEBUG->print("GV_GvLegend::GV_GvLegend()");
//	setAcceptDrops(true);
	
}

GV_GvLegend::~GV_GvLegend() {
	RB_DEBUG->print("GV_GvLegend::~GV_GvLegend()");
	// nothing
	RB_DEBUG->print("GV_GvLegend::~GV_GvLegend() OK");
}

/**
 * @return selected component (line or symbol)
 */
GV_Component* GV_GvLegend::getSelectedComponent() {
	RB_DEBUG->print("GV_GvLegend::getSelectedComponent()");

	GV_Component* topLevelItem = NULL; // not a port or node
	QList<QGraphicsItem*> itemList = scene()->items();
	int count = itemList.size();

	for (int i = 0; i < count && !topLevelItem; i++) {
		QGraphicsItem *item = itemList.at(i);

		if (item->isSelected()) {
			QGraphicsItem* tmpItem = item->topLevelItem();

			if (tmpItem->type() == QGraphicsItem::UserType + 1) {
				topLevelItem = dynamic_cast<GV_Symbol*>(tmpItem);
			} else if (tmpItem->type() == QGraphicsItem::UserType + 2) {
				topLevelItem = dynamic_cast<GV_Line*>(tmpItem);
			}

		}
	}

	return topLevelItem;
}

/**
 * Mouse press event only catch here to set dragStartPosition, event
 * is passed to GV_GraphicsView
 */
void GV_GvLegend::mousePressEvent(QMouseEvent* e) {
	 if (e->button() == Qt::LeftButton) {
		 mDragStartPosition = e->pos();
	 }
	 GV_GraphicsView::mousePressEvent(e);
 }

/**
 * Mouse move event catched here only to allow for initiation of drag event
 */
void GV_GvLegend::mouseMoveEvent(QMouseEvent* e) {
//	RB_DEBUG->print("GV_GvLegend::mouseMoveEvent()");

	if (!(e->buttons() & Qt::LeftButton)) {
		// for hover enter and leave
		GV_GraphicsView::mouseMoveEvent(e);
		return;
	}

	// if not minimum distance for start for drag/drop
	if ((e->pos() - mDragStartPosition).manhattanLength()
						< QApplication::startDragDistance()) {
		return;
	}

	GV_Component* selComp = getSelectedComponent();
	if (!selComp) return;

	if (selComp && selComp->getName() == "GV_Symbol") {
		dragSymbol(selComp);
	} else if (selComp->getName() == "GV_Line") {
		dragLine(selComp);
	}
}

/**
 * Create drag action for symbol
 * @param comp symbol
 */
void GV_GvLegend::dragSymbol(GV_Component* comp) {
	RB_DEBUG->print("GV_GvLegend::dragSymbol()");

	GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(comp);
	if (!symbol) return;

	QDrag* drag = new QDrag(this);
	QMimeData* mime = new QMimeData;
	mime->setText("GV_Symbol");

	QVariant var;
	var.setValue(dynamic_cast<void*>(symbol));
	mime->setImageData(var); // use image data for passing void* pointer

	drag->setMimeData(mime); // required for drag

	QPixmap pixmap(symbol->path().boundingRect().width() + symbol->pen().width(),
					symbol->path().boundingRect().height() + symbol->pen().width());
	pixmap.fill(Qt::white);

	QPainter painter(&pixmap);
	painter.translate(-symbol->path().boundingRect().topLeft().x() + symbol->pen().width()/2,
						-symbol->path().boundingRect().topLeft().y() + symbol->pen().width()/2);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(symbol->pen());
	painter.setBrush(symbol->brush());
	painter.drawPath(symbol->path());
	painter.end();

	pixmap.setMask(pixmap.createHeuristicMask());

	drag->setPixmap(pixmap);
	drag->setHotSpot(QPoint(-5.0, 5.0 + drag->pixmap().height()));
	drag->exec();
}

/**
 * Create drag action for line
 * @param comp line component
 */
void GV_GvLegend::dragLine(GV_Component* comp) {
	RB_DEBUG->print("GV_GvLegend::dragLine()");

	GV_Line* line = dynamic_cast<GV_Line*>(comp);
	if (!line) return;

	QDrag* drag = new QDrag(this);
	QMimeData* mime = new QMimeData;
	mime->setText("GV_Line");

	QVariant var;
	var.setValue(dynamic_cast<void*>(line));
	mime->setImageData(var); // use image data for passing void* pointer

	drag->setMimeData(mime); // required for drag

	QPixmap pixmap(line->path().boundingRect().width() + line->pen().width(),
					line->path().boundingRect().height() + line->pen().width());
	pixmap.fill(Qt::white);

	QPainter painter(&pixmap);
	painter.translate(-line->path().boundingRect().topLeft().x() + line->pen().width()/2,
						-line->path().boundingRect().topLeft().y() + line->pen().width()/2);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(line->pen());
    painter.setBrush(line->pen().brush());
	painter.drawPath(line->path());
	painter.end();

	pixmap.setMask(pixmap.createHeuristicMask());

	drag->setPixmap(pixmap);
	drag->setHotSpot(QPoint(-5.0, 5.0 + drag->pixmap().height()));
	drag->exec();
}
