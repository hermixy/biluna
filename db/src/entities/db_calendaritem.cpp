/*******************************************************************************
 * $Id: db_calendaritem.cpp 2198 2014-11-17 21:34:16Z rutger $
 * Created: Apr 3, 2013 10:00:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *
 * Originally by Jukka-Pekka Makela. 2008
 * Contact: jpm@mehteenit.net
 * The original of this file was part of QuickCalendar.
 ******************************************************************************/

#include <QtWidgets>
#include "db_calendaritem.h"

/**
 * Constructor
 * @param parent
 * @param scene
 */
DB_CalendarItem::DB_CalendarItem(QGraphicsItem* parent, QGraphicsScene* /*scene*/)
    : QGraphicsItem(parent/*, scene*/) {
    // nothing
}

/**
* @return painter path shape based on bounding rectangle
*/
QPainterPath DB_CalendarItem::shape() const {
    QPainterPath result;
    result.addRect(boundingRect());
    return result;
}

/**
 * Paint item, does nothing
 * @param painter
 * @param option
 * @param widget
 */
void DB_CalendarItem::paint(QPainter* /*painter*/,
                            const QStyleOptionGraphicsItem* /*option*/,
                            QWidget* /*widget*/) {
    //    painter->setPen(QColor(0,0,0,128));
    //    painter->drawRect(0,0,mBoundingRect.width(),mBoundingRect.height());
}

/**
 * @return the size of the drawing area.
 */
QRectF DB_CalendarItem::boundingRect() const {
    return mBoundingRect;
}
/**
 * Set the size of the drawing area, used?
 * @param size
 */
void DB_CalendarItem::setSize(const QSize& size) {
    setSize((qreal)size.width(), (qreal)size.height());
}

/**
* @param width
* @param height
*/
void DB_CalendarItem::setSize(qreal width, qreal height) {
    if(width != mBoundingRect.width() || height != mBoundingRect.height()) {
        // If the new size is different from the current
        prepareGeometryChange();
        QSizeF old = mBoundingRect.size();
        QSizeF size(width, height);
        mBoundingRect.setWidth(width);
        mBoundingRect.setHeight(height);
        onResize(size, old);
        update();
    }
}

/**
 * Handler resize. This default implementation does nothing
 * @param size
 * @param oldSize
 */
void DB_CalendarItem::onResize(const QSizeF& /*size*/, const QSizeF& /*oldSize*/) {
    // nothing
}

/**
 * Handler layout changed. This default implementation does nothing
 */
void DB_CalendarItem::layoutChanged() {
    // nothing
}

/**
 * Handler data changed. This default implementation does nothing
 */
void DB_CalendarItem::dataChanged() {
    // nothing
}
