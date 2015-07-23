/*******************************************************************************
 * $Id: db_scrollbar.cpp 2034 2013-11-14 15:34:56Z rutger $
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
#include "db_scrollbar.h"
#include "db_scrollareaitem.h"

/**
 * @param scrollArea
 * @param scene
 */
DB_ScrollBar::DB_ScrollBar(DB_ScrollAreaItem *scrollArea, QGraphicsScene *scene)
            : DB_CalendarItem(scrollArea, scene), mArea(scrollArea) {
    mMin = 0.0;
    mMax = 0.0;
    mValue = 0.0;
    mFactor = 1.0;

    sliderPos = 0;
    sliderMax = 0;
    pressedControl = 0;

    mBoundingRect.adjust(0,0,16,16);
}

void DB_ScrollBar::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* /*option*/,
                         QWidget* /*widget*/) {
    int width = (int)mBoundingRect.width() - 1;
    int height = (int)mBoundingRect.height();

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(161, 161, 129)));
    painter->drawRect(0, height/3, width, height/3);

    painter->setBrush(QBrush(QColor(191, 191, 191)));
    painter->drawRect(0, 0, width, height/3);
    painter->drawRect(0, (height * 2)/3, width, height/3);

    painter->setPen(QPen(QColor(63, 63, 63)));
    painter->setBrush(QBrush(QColor(255, 255, 255, 127)));
    painter->drawRect(0, sliderPos, width, sliderHeight);
}

void DB_ScrollBar::layoutChanged() {
    qreal height = mBoundingRect.height();

    sliderHeight = height * mFactor;
    sliderPos = mValue * mFactor;

    mArea->mItem->setPos(0, -mValue);

    mArea->update();
}

/**
 * @param size
 * @param oldSize
 */
void DB_ScrollBar::onResize(const QSizeF& size, const QSizeF& oldSize) {
    qreal posFactor = 0;

    if (oldSize.height() != 0) {
        posFactor = size.height() / oldSize.height();
    }

    if (posFactor != 1) {
        if (mMax != 0) {
            mFactor = size.height() / mMax;
        } else {
            mFactor = 0;
        }

        if (mFactor > 1) {
            // Scale is greater than 1
            scrollTo(0);
        } else {
            // Otherwise new value
            scrollTo(posFactor * mValue);
        }
    }
}

/**
 * @param min
 */
void DB_ScrollBar::setMinimum(const qreal min) {
    if (min != mMin) {
        mMin = min;
        layoutChanged();
    }
}

/**
 * @param min - new maximum value
 */
void DB_ScrollBar::setMaximum(const qreal max)
{
    if (max != mMax) {
        mMax = max;

        if (mMax != 0) {
            mFactor = mBoundingRect.height() / mMax;
        } else {
            mFactor = 0;
        }

        if (mFactor >= 1) {
            mValue = 0;
        }

        layoutChanged();
    }
}

/**
 * @return value corresponding to the current position of the slider.
 */
qreal DB_ScrollBar::getValue() const {
    return mValue;
}

/**
 * @param value - new value
 */
void DB_ScrollBar::scrollTo(qreal value) {
    if (value != mValue) {
        if (mFactor > 1) {
            mValue = 0;
        } else {
            if (value < mMin) {
                mValue = mMin;
            } else if (value > mMax - mBoundingRect.height()) {
                mValue = mMax - mBoundingRect.height();
            } else {
                mValue = value;
            }
        }

        layoutChanged();
    }
}
/**
 * @param scrollBy
 */
void DB_ScrollBar::scrollBy(qreal scrollBy) {
    if (mFactor < 1) {
        scrollTo(mValue + scrollBy);
    }
}

/**
 * @param y
 */
void DB_ScrollBar::ensureVisibility(qreal y) {
    // if more tech. value and the height of the drawing
    if (y > mValue + mBoundingRect.height()) {
        scrollBy(10);
    } else if(y < mValue) {
        scrollBy(-10);
    }
}

/**
 * @param event
 */
void DB_ScrollBar::mousePressEvent(QGraphicsSceneMouseEvent* event)  {
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    // if the mouse is not over the slider and is pressed
    if (event->pos().y() < sliderPos
            || event->pos().y() > sliderPos + sliderHeight) {
        return;
    }

    pressedControl = 1;
    setCursor(Qt::ClosedHandCursor);
 }

/**
 * @param event
 */
void DB_ScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (pressedControl == 1) {
        if (mFactor != 0) {
            scrollBy((event->pos().y() - event->lastPos().y()) / mFactor);
        }
	}
}

/**
* @param event
*/
void DB_ScrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/) {
    pressedControl = 0;
    setCursor(Qt::ArrowCursor);
}

