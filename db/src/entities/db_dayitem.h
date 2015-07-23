/*******************************************************************************
 * $Id: db_dayitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_DAYITEM_H
#define DB_DAYITEM_H

#include <QBrush>
#include <QDate>
#include <QLinearGradient>
#include <QPen>
#include <QFont>
#include <QPixmap>

#include "db_calendaritem.h"
#include "db_daycontentitem.h"
#include "db_activity.h"

class DB_CalendarView;

/**
 @class DB_DayItem
 @brief Class for the graphical display of information about the day of the week
*/
class DB_EXPORT DB_DayItem : public DB_CalendarItem {

public:
    DB_DayItem(DB_CalendarView *calendar,
            const QDate &date,
            QGraphicsItem *parent = 0,
            QGraphicsScene *scene = 0);
	
    void setDate(const QDate &date);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
     
    virtual void layoutChanged();
    virtual void dataChanged();
    
    bool isOutOfRange() const { return mIsOutOfRange; }
    void setOutOfRange(bool outOfRange) { mIsOutOfRange = outOfRange; }

    QDate getDate() const { return mDate; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event); 
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mIsOutOfRange;

    DB_CalendarView *mCalendarView;
    QDate mDate;

    DB_DayContentItem *mContentItem;
    QBrush mBrush;

    QColor mToolTipBaseColor;
    QLinearGradient mGradient;
    QPen mFontPen;
    QPen mPen;
    QFont mFont;

    QString mCaption;
    int mAlign;

    QList<DB_Activity*> mActivities;

    QPixmap mClockIcon;

//    friend class DB_MonthItem;
    friend class DB_WeekItem;
    friend class DB_DayContentItem;
    friend class DB_ActivityItem;
};

#endif
