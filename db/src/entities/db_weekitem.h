/*******************************************************************************
 * $Id: db_weekitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_WEEKITEM_H
#define DB_WEEKITEM_H
 
#include <QGraphicsItemGroup>
#include <QFont>

#include "db_calendaritem.h"
#include "db_scrollareaitem.h"
#include "db_clockbar.h"

class DB_CalendarView;
class DB_DayItem;

class DB_EXPORT DB_WeekItem : public DB_CalendarItem
{

public:
    DB_WeekItem(DB_CalendarView* calendarView,
             const QDate& date,
             const QDate& end,
             QGraphicsItem* parent = 0,
             QGraphicsScene* scene = 0);
    ~DB_WeekItem() {}

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget);

    virtual void layoutChanged();
    virtual void dataChanged();

    bool hasDate(const QDate& date);
    int dayOfWeek(const QDate& date);
    QList<DB_DayItem*> getDayItems() { return mDays; }
    QDate getStartDate() const { return mRangeStart; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    DB_CalendarView* mCalendarView;

    DB_CalendarItem* dayHeaderContainer;
    DB_CalendarItem* dayContentContainer;

    DB_CalendarItem* mScrollPane;
    DB_ScrollAreaItem* mScrollArea;

    DB_ClockBar* mClockBar;

    QDate mDate;
    QDate mRangeStart;
    QDate mRangeEnd;

    QList<DB_DayItem*> mDays;

    QColor mToolTipBaseColor;
    QLinearGradient mGradient;
    QPen mFontPen;
    QPen mPen;
    QFont mFont;

    friend class DB_CalendarView;
    friend class DB_DayItem;
    friend class DB_DayContentItem;
    friend class DB_ActivityItem;
};

#endif
