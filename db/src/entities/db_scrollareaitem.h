/*******************************************************************************
 * $Id: db_scrollareaitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_SCROLLAREAITEM_H
#define DB_SCROLLAREAITEM_H

#include "db_calendaritem.h"
#include "db_scrollbar.h"

/**
 @class DB_ScrollAreaItem
 @brief A class that implements the scrolling area for a calendar
*/
class DB_EXPORT DB_ScrollAreaItem : public DB_CalendarItem {

public:
    DB_ScrollAreaItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void setItem(DB_CalendarItem *item);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    virtual void layoutChanged();
    void scrollTo(qreal value);
    void ensureVisibility(qreal x, qreal y);

    qreal scrollBarWidth() const;

    virtual void onResize(const QSizeF& size, const QSizeF& oldSize);

//    int length;
//    int pos;

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event);

private:
    DB_ScrollBar mVertical;
    DB_CalendarItem mContent;
    DB_CalendarItem *mItem;

    friend class DB_ScrollBar;
};

#endif
