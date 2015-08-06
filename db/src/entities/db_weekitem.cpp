/*******************************************************************************
 * $Id: db_weekitem.cpp 2198 2014-11-17 21:34:16Z rutger $
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
#include "db_weekitem.h"
#include "db_dayitem.h"
#include "db_calendarstyle.h"
#include "db_calendarview.h"

DB_WeekItem::DB_WeekItem(DB_CalendarView* calendarView, const QDate& start,
                         const QDate& end, QGraphicsItem* parent,
                         QGraphicsScene*scene)
            : DB_CalendarItem(parent, scene),
              mCalendarView(calendarView),
              mRangeStart(start),
              mRangeEnd(end) {
    setAcceptHoverEvents(true);

    mFont = mCalendarView->style()->collapsedDayNumberFont;

    dayHeaderContainer = new DB_CalendarItem(this);

    mScrollPane = new DB_CalendarItem();
    mClockBar = new DB_ClockBar(mScrollPane);
    dayContentContainer = new DB_CalendarItem(mScrollPane);

    mScrollArea = new DB_ScrollAreaItem(this);
    mScrollArea->setItem(mScrollPane);

    if (mCalendarView->displayMode() == DB_CalendarView::DisplayFullWeeks)     {
        mDate = mRangeStart.addDays(-(mRangeStart.dayOfWeek() - 1));

        if (mDate.daysTo(mRangeEnd) > 6) {
            mRangeEnd = mDate.addDays(6);
        }

        for (int i = 1; i <= 7; i++) {
            DB_DayItem *day = new DB_DayItem(mCalendarView, mDate.addDays(i-1), dayHeaderContainer);
            day->mContentItem->setParentItem(dayContentContainer);

            if(mDate.addDays(i-1) < mRangeStart || mDate.addDays(i-1) > mRangeEnd) {
                day->setOutOfRange(true);
            }

            mDays.append(day);
        }
    } else if (mCalendarView->displayMode() == DB_CalendarView::DisplayOnlyRange) {
        mDate = mRangeStart;

        for (int i = 1; i <= mRangeStart.daysTo(mRangeEnd) + 1; i++) {
            DB_DayItem *day = new DB_DayItem(mCalendarView, mDate.addDays(i-1), dayHeaderContainer);
            day->mContentItem->setParentItem(dayContentContainer);
            mDays.append(day);
        }
    }
}

void DB_WeekItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget* /*widget*/) {
    int width = (int)mBoundingRect.width() - 1;
    int height = (int)mBoundingRect.height();
    painter->setClipRect(mBoundingRect);

    painter->setPen(mPen);
    painter->setBrush(mGradient);
    painter->drawRect(0, 0, width, height);

    if (option->state & QStyle::State_MouseOver) {
        painter->setBrush(mToolTipBaseColor);
        painter->drawRect(0,0,width,height);
    }

    // Week left header
    QFontMetrics fm(mFont);
    painter->setFont(mFont);
    painter->setPen(mFontPen);
    painter->drawText(10, fm.ascent() + 3, QString::number(mDate.weekNumber()));
}

bool DB_WeekItem::hasDate(const QDate &date) {
    if(mCalendarView->displayMode() == DB_CalendarView::DisplayFullWeeks)     {
        if(date >= mDate && date <= mDate.addDays(6)) {
            return true;
        } else {
            return false;
        }
    } else if(mCalendarView->displayMode() == DB_CalendarView::DisplayOnlyRange) {
        if(date >= mRangeEnd && date <= mRangeEnd) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}

int DB_WeekItem::dayOfWeek(const QDate &date) {
    if (mCalendarView->displayMode() == DB_CalendarView::DisplayFullWeeks) {
        if (date >= mDate && date <= mDate.addDays(6)) {
            return date.dayOfWeek();
        } else {
            return -1;
        }
    } else if(mCalendarView->displayMode() == DB_CalendarView::DisplayOnlyRange) {
        if (date >= mRangeStart && date <= mRangeEnd) {
            return mRangeStart.daysTo(date) + 1;
        } else {
            return -1;
        }
    }

    return -1;
}

void DB_WeekItem::dataChanged() {
    QListIterator<DB_DayItem*> i(mDays);

    while(i.hasNext()) {
        DB_DayItem *day = i.next();
        day->dataChanged();
    }
}

void DB_WeekItem::layoutChanged() {
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();

    DB_CalendarStyle* style = mCalendarView->style();

    mClockBar->setSize(style->weekLeftMargin, style->quarterHeight * 4 * 24);

    int headerHeight = style->expandedWeekHeaderHeight;
    int workWidth = width - style->weekLeftMargin - style->weekRightMargin;

    dayHeaderContainer->setPos(style->weekLeftMargin, 0);

    if (mCalendarView->expandedWeekItem() == this) {
        dayHeaderContainer->setSize(workWidth, style->expandedWeekHeaderHeight);

        dayContentContainer->setPos(style->weekLeftMargin, 0);
        dayContentContainer->setSize(workWidth, style->quarterHeight * 4 * 24);

        mScrollPane->setSize(workWidth, style->quarterHeight * 4 * 24);
        mScrollArea->setPos(0, style->expandedWeekHeaderHeight);
        mScrollArea->setSize(width, height - style->expandedWeekHeaderHeight);
        mScrollArea->setVisible(true);
    } else {
        dayHeaderContainer->setVisible(true);
        dayHeaderContainer->setSize(workWidth, height);
        headerHeight = height;
        mScrollArea->setVisible(false);
    }

    int dayLeft = 0;
    int dayOfWeek = mDate.dayOfWeek() - 1;
    QListIterator <DB_DayItem *> i(mDays);

    while(i.hasNext()) {
        DB_DayItem *day = i.next();
        int dayWidth = mCalendarView->dayWidth(dayOfWeek);
        day->setPos(dayLeft,0);
        day->setSize(dayWidth, headerHeight);
        dayLeft += dayWidth;
        day->layoutChanged();
        dayOfWeek++;
    }

    int gradientHeight = (int)mBoundingRect.height();

    if (mCalendarView->expandedWeekItem() == this) {
        gradientHeight = style->expandedWeekHeaderHeight;
    }

    bool isCurrentWeek = QDate::currentDate().weekNumber() == mDate.weekNumber()
            && QDate::currentDate().year() == mDate.year();

    if(isCurrentWeek) {
        mGradient = style->todayGradient;
    } else if(mDate < QDate::currentDate()) {
        mGradient = style->pastDayGradient;
    } else {
        mGradient = style->comingDayGradient;
    }

    mGradient.setFinalStop(0, gradientHeight);

    mPen.setColor(style->shadowColor);
    mFontPen.setColor(style->textColor);
    mToolTipBaseColor = style->toolTipBaseColor;
    mToolTipBaseColor.setAlpha(172);
}

void DB_WeekItem::mousePressEvent(QGraphicsSceneMouseEvent* /*event*/) {
    // nothing
}

void DB_WeekItem::mouseMoveEvent(QGraphicsSceneMouseEvent* /*event*/) {
    // nothing
}

void DB_WeekItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    //event->pos().x() > mMonthItem->mStyle.weekLeftMargin + clockWidth
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    mCalendarView->expandWeek(this);
}
