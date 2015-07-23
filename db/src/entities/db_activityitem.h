/*******************************************************************************
 * $Id: db_activityitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_ACTIVITYITEM_H
#define DB_ACTIVITYITEM_H

#include <QColor>
#include <QFont>
#include <QString>

#include "db_activity.h"
#include "db_calendaritem.h"
#include "db_dayitem.h"
 
/**
 * Class for the graphical representation of information about meetings
 */
class DB_EXPORT DB_ActivityItem : public DB_CalendarItem {    

public:
    DB_ActivityItem(DB_Activity* appointment,
                    DB_DayItem* dayItem,
                    QGraphicsItem* parent = 0,
                    QGraphicsScene* scene = 0);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget);
	/**
     * @return row number in which the element is located.
     */
    int column() const { return mColumn; } 
	/**
     * @param column - row number
     */
	void setColumn(int column) { mColumn = column; }
	/**
     * @return number of rows, overlapping elements.
     */
    int columnSpan() const { return mColumnSpan; }
	/**
     * @param columnSpan - number of rows, overlapping elements.
     */
    void setColumnSpan(int columnSpan) { mColumnSpan = columnSpan; }

    void layoutChanged();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    DB_Activity *mActivity;
    DB_DayItem *mDayItem;
    QColor mColor; // item color
    QFont mFont;
    QString mSubject;

    int mColumn;
    int mColumnSpan;

    friend class DB_DayContentItem;
};


#endif
