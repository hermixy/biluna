/*******************************************************************************
 * $Id: db_weekdayheaderitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_WEEKDAYHEADERITEM_H
#define DB_WEEKDAYHEADERITEM_H

#include "db_calendaritem.h"
#include <QString>
#include <QLinearGradient>
#include <QPen>
#include <QFont>

class DB_CalendarView;

class DB_EXPORT DB_WeekDayHeaderItem : public DB_CalendarItem
{
public:
    DB_WeekDayHeaderItem(DB_CalendarView *calendarView,
                      int dayOfWeek,
                      QGraphicsItem *parent = 0,
                      QGraphicsScene *scene = 0);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
//    virtual QRectF boundingRect() const;

    virtual void layoutChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QColor mToolTipBaseColor;
    QLinearGradient mGradient;
    QPen mFontPen;
    QPen mPen;
    QFont mFont;

    DB_CalendarView *mCalendarView;
    int mDayOfWeek;

    QString mDayTitle;
    int mAlign;
};

#endif
