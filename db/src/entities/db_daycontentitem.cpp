/*******************************************************************************
 * $Id: db_daycontentitem.cpp 2200 2014-12-15 10:44:04Z rutger $
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
#include "db_daycontentitem.h"

#include "db_activityitem.h"
#include "db_weekitem.h"
#include "db_dayitem.h"
#include "db_calendarview.h"

/**
 * Constructor
 * @param dayItem
 * @param parent
 * @param scene
 */
DB_DayContentItem::DB_DayContentItem(DB_DayItem* dayItem, QGraphicsItem* parent,
                                     QGraphicsScene* scene)
                : DB_CalendarItem(parent, scene), mDayItem(dayItem) {
    mColumns = 1; 
    dragStart = dragEnd = 0;
    dragSelection = false;

//    setHandlesChildEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    // Hover events are delivered when there is no current mouse grabber item.
}

void DB_DayContentItem::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem* option,
                              QWidget* /*widget*/) {
    int left = 0; // left indent
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();
    QPalette p = QApplication::palette();
    painter->setPen(QPen(p.color(QPalette::Active, QPalette::Mid)));

    if(mDayItem->mDate.dayOfWeek() < 6) {
        // 9:00 to 17:00
        painter->setBrush(QBrush(p.color(QPalette::Active, QPalette::Base)));
        painter->drawRect(0, height*9/24, width, height*8/24);

        // 0:00 to 8:00
        painter->setBrush(QBrush(p.color(QPalette::Active, QPalette::AlternateBase)));
        painter->drawRect(0,0,width,height*9/24);

        // 18:00 to 23:00
        painter->drawRect(0,height*17/24, width, height*7/24);
    } else {
        // Day off - the same weekday, but in a different color
        painter->setBrush(QBrush(p.color(QPalette::Active, QPalette::Base)));
        painter->drawRect(0, height*9/24, width, height*8/24);

        painter->setBrush(QBrush(p.color(QPalette::Active, QPalette::AlternateBase)));
        painter->drawRect(0, 0, width, height*9/24);

        painter->drawRect(0,height*17/24, width, height*7/24);
    }

    // Draw lines
    // Boundary hours
    for (int i = 0; i < 24; i++) {
        painter->drawLine(left, i*10*4, left+width, i*10*4);
    }

    painter->setPen(QPen(p.color(QPalette::Active, QPalette::Mid)));

    for (int i = 0; i < 24; i++) {
         painter->drawLine(left, i*10*4 + 10 * 2, left+width, i*10*4 + 10 * 2);
    }

    // State
    if(option->state & QStyle::State_MouseOver) {
        // nothing?
    }

    // Selection
    if(dragSelection) {
        painter->setPen(Qt::NoPen);
        QColor color = p.color(QPalette::Active, QPalette::Highlight);
        color.setAlpha(50);
        painter->setBrush(QBrush(color));
        painter->drawRect(0, dragStart, width, dragEnd);
    }
}

/**
 * Recreate all the graphic items
 */
void DB_DayContentItem::dataChanged() {
    qDeleteAll(childItems());

    int minEndTime = 0;
    int currentColumn = 0;
    QVector<int> columns; // Vector stores the number four hours, that end of the meeting
    columns << 0;

    QListIterator <DB_Activity*> i(mDayItem->mActivities);

    while (i.hasNext()) {
        DB_Activity* appointment = i.next();

        DB_ActivityItem *item = new DB_ActivityItem(appointment, mDayItem, this);
        minEndTime = 24 * 4;

        // Find the minimum time the meeting ended, in all series
        for (int j = 0; j < columns.size(); j++) {
            if(columns[j] < minEndTime) {
                minEndTime = columns[j];
                currentColumn = j;
            }
        }
        
        if (appointment->startQuarter(mDayItem->mDate) < minEndTime) {
            // If the quarter of an hour of the meeting end time is less than the minimum
            item->setColumn(columns.size());
            item->setColumnSpan(1);
            columns.append(appointment->endQuarter(mDayItem->mDate)); 
        } else {
            // Get room for a quarter of an hour for the meeting
            int beginTime = appointment->startQuarter(mDayItem->mDate);
            int columnSpan = 1;

            for(int j=0;j<columns.size();j++) {
                if (columns[j] <= beginTime) {
                    // If the time is less than the start time
                    int width = 1;

                    // Remaining part of the array
                    for (int k = j+1; k < columns.size(); k++) {
                        // Count the number of consecutive cells, in which
                        // the content is less than or equal to beginTime
                        if(columns[k] <= beginTime) {
                            width++;
                        } else {
                            break;
                        }
                    }

                    // If the width is greater than the number of rows covered by parts
                    if (width > columnSpan) {
                        columnSpan = width;
                        currentColumn = j;
                    }
                }
            }

            item->setColumn(currentColumn);
            item->setColumnSpan(columnSpan);

            // For all elements of the current to the current period plus
            for(int j=currentColumn;j<currentColumn+columnSpan;j++) {
                // We define the value of a quarter of the meeting
                columns[j] = appointment->endQuarter(mDayItem->mDate);
            }
        }
    }

    mColumns = columns.size();
}


void DB_DayContentItem::layoutChanged() {
    int left = 0;
        //int top = 0;
    int width = (int)mBoundingRect.width();
        //int height = mBoundingRect.height();

    int quarterHeight = 10;
    int columnWidth = width / mColumns;

    QListIterator<QGraphicsItem*> j(childItems());

    while(j.hasNext()) {
        DB_ActivityItem* item = (DB_ActivityItem*)j.next();

        const DB_Activity *appointment = item->mActivity;

        int itemTop = quarterHeight * appointment->startQuarter(mDayItem->mDate); 
        int itemHeight = (quarterHeight * appointment->endQuarter(mDayItem->mDate)) - itemTop;

        if(item->column() == 0) {
            item->setPos(left + (columnWidth * item->column()),itemTop);
            item->setSize((width%mColumns) + (columnWidth*item->columnSpan()),itemHeight);
        } else {
            item->setPos(left + (width%mColumns) + (columnWidth * item->column()), itemTop);
            item->setSize(columnWidth*item->columnSpan(),itemHeight);
        }

        item->layoutChanged();
    }
}

/**
* @param event
*/
void DB_DayContentItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    mStartY = (int)event->pos().y();
    dragStart = (int)event->pos().y() - mStartY % 20;
    dragEnd = 0;
    dragSelection = true;
}

/**
* @param event
*/
void DB_DayContentItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if(dragSelection) {
        mEndY = (int)event->pos().y();
        dragEnd = mEndY + 20 - mEndY % 20 - dragStart;

        //mDayItem->mWeekItem->mScrollArea.ensureVisibility(0, event->pos().y());

        update();
    }
}

/**
* @param event
*/
void DB_DayContentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/) {
    if(dragSelection && dragEnd != 0 && std::abs(mEndY - mStartY) > 4) {
        int startHour = dragStart / 40;
        int startMinute = 0;

        if(dragStart%40 > 0) {
            startMinute = 30;
        }

        QDateTime start(mDayItem->mDate, QTime(startHour, startMinute, 0));
        int endHour = (dragStart + dragEnd) / 40;
        int endMinute = 0;

        if((dragStart + dragEnd)%40 > 0) {
            endMinute = 30;
        }

        if(endHour == 24) {
            endHour = 23;
            endMinute = 59;
        }

        QDateTime end(mDayItem->mDate, QTime(endHour, endMinute, 0));

        if(startHour < endHour) {
            mDayItem->mCalendarView->createNewActivity(start, end);
        } else {
            mDayItem->mCalendarView->createNewActivity(end, start);
        }
    }

    dragSelection = false;
}
