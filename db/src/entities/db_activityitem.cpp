/*******************************************************************************
 * $Id: db_activityitem.cpp 2034 2013-11-14 15:34:56Z rutger $
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

#include <QtCore>
#include <QtWidgets>

#include "db_activityitem.h"
#include "db_calendar.h"
#include "db_weekitem.h"
#include "db_calendarview.h"

/**
 * Constructor
 */
DB_ActivityItem::DB_ActivityItem(DB_Activity *appointment, DB_DayItem *dayItem,
                                 QGraphicsItem *parent, QGraphicsScene *scene) :
                            DB_CalendarItem(parent, scene),
                            mActivity(appointment),
                            mDayItem(dayItem),
                            mFont("Arial", 12, QFont::Bold) {
    mColumn = 1; // By default, items are located in the first row
    mColumnSpan = 1; // Span only one row

    setFlag(QGraphicsItem::ItemIsSelectable);
}

void DB_ActivityItem::paint(QPainter* painter,
                            const QStyleOptionGraphicsItem* /*option*/,
                            QWidget* /*widget*/) {
    // painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setFont(mFont);
    // font metrics?
    painter->setPen(QPen(mColor));
    QColor color = mColor;
    color.setAlpha(159);
    painter->setBrush(QBrush(color));
    painter->drawRoundedRect(mBoundingRect, 10, 10);

    // Set text from the time of the meeting and the subject of the meeting
    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawText(mBoundingRect.adjusted(3,3,0,0),
                      Qt::AlignLeft | Qt::TextWordWrap,
                      mActivity->startTime().toString("HH:mm") + " " + mSubject);
    // painter->setRenderHint(QPainter::Antialiasing, false);
}

void DB_ActivityItem::layoutChanged() {
    mColor = mActivity->calendar()->color();
    mSubject = mActivity->subject();
}

/**
 * Show the form setting a meeting
 * @param event
 */
void DB_ActivityItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* /*event*/) {
    mDayItem->mCalendarView->editActivity(mActivity);
}

/**
* @param event
*/
void DB_ActivityItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/) {
    mDayItem->mCalendarView->expandDate(mDayItem->mDate);
    mDayItem->mCalendarView->selectActivity(mActivity);
}

