/*******************************************************************************
 * $Id: db_scrollbar.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_SCROLLBAR_H
#define DB_SCROLLBAR_H

#include "db_calendaritem.h"

class DB_ScrollAreaItem;

/**
* @class DB_ScrollBar
* @brief
*/
class DB_EXPORT DB_ScrollBar : public DB_CalendarItem {

public:
    DB_ScrollBar(DB_ScrollAreaItem *scrollArea = 0, QGraphicsScene *scene = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    virtual void layoutChanged();

    qreal getValue() const;
    void scrollTo(qreal value);
    void scrollBy(qreal scrollBy);

    void setMinimum(qreal min);
    void setMaximum(qreal max);

    virtual void onResize(const QSizeF &size, const QSizeF &oldSize);
    void ensureVisibility(qreal y);
    int orientation;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    qreal mMin;
    qreal mMax;
    qreal mValue;
    qreal mFactor;

    qreal sliderMax;
    qreal sliderPos;
    qreal sliderHeight;

    int pressedControl;

    DB_ScrollAreaItem* mArea;
    //ScrollHandle mHandle;
};

#endif
