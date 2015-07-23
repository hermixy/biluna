/*******************************************************************************
 * $Id: db_scrollareaitem.cpp 2034 2013-11-14 15:34:56Z rutger $
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
#include "db_scrollareaitem.h"
#include "db_dayitem.h"

DB_ScrollAreaItem::DB_ScrollAreaItem(QGraphicsItem *parent, QGraphicsScene *scene)
                : DB_CalendarItem(parent, scene), mVertical(this, scene),
                  mContent(this, scene), mItem(0) {

    mContent.setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}


void DB_ScrollAreaItem::setItem(DB_CalendarItem *item) {
    if(item != 0) {
        item->setParentItem(&mContent);
        mVertical.setMaximum(item->boundingRect().height());
    }

    mItem = item;
}

/**
* @brief The background color of the field is given by (127,127,127)
* @param painter
* @param option
*/
void DB_ScrollAreaItem::paint(QPainter* painter,
                              const QStyleOptionGraphicsItem* /*option*/,
                              QWidget* /*widget*/) {
    int left = 0;
    int top = 0;
    int width = (int)mBoundingRect.width() - 1;
    int height = (int)mBoundingRect.height();

    painter->setBrush(QBrush(QColor(127, 127, 127)));
    painter->drawRect(left, top, width, height);
}

void DB_ScrollAreaItem::onResize(const QSizeF& /*size*/, const QSizeF& /*oldSize*/) {
    layoutChanged();
}

/**
* @brief Handles layout changes
*/
void DB_ScrollAreaItem::layoutChanged() {
    //int left = 0;
    //int top = 0;
    qreal width = mBoundingRect.width();
    qreal height = mBoundingRect.height();

    qreal verticalBar = mVertical.boundingRect().width();

    mContent.setSize(width - verticalBar, height);
    mVertical.setPos(width - verticalBar, 0);
    mVertical.setSize(verticalBar, height);

    if(mItem != 0) {
        mVertical.setMaximum(mItem->boundingRect().height());
    }

    // mVertical.layoutChanged();
}

void DB_ScrollAreaItem::scrollTo(qreal value) {
    mVertical.scrollTo(value);
}

/**
 * Only y used, since the horizontal scrolling is not supported.
 */
void DB_ScrollAreaItem::ensureVisibility(qreal /*x*/, qreal y) {
    mVertical.ensureVisibility(y);
}

qreal DB_ScrollAreaItem::scrollBarWidth() const {
    return mVertical.boundingRect().width();
}

void DB_ScrollAreaItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
    // Changes the position of the scroll bar based on changes of the wheel
    // with a quarter with the opposite sign (because the front wheel movement
    // recovery slider of the scroll bar)
    mVertical.scrollBy(-event->delta() / 4);
	//??
    layoutChanged();
}
