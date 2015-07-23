/*******************************************************************************
 * $Id: db_daycontentitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_DAYCONTENTITEM_H
#define DB_DAYCONTENTITEM_H

#include "db_calendaritem.h"

 
class DB_DayItem;

/**
* @class DB_DayContentItem
* @brief Class to display the content of the day of the week
*/ 
class DB_EXPORT DB_DayContentItem : public DB_CalendarItem
{

public:
    DB_DayContentItem(DB_DayItem* dayItem, QGraphicsItem* parent = 0,
                      QGraphicsScene* scene = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);

    virtual void layoutChanged();
    virtual void dataChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    DB_DayItem *mDayItem;

    // Number of rows?
    int mColumns;
    int dragStart;
    int dragEnd;
    // To determine whether the selection is small and not intended
    int mStartY;
    int mEndY;

    // A flag indicating whether the mouse is highlighted any region
    bool dragSelection;
};

#endif
