/*******************************************************************************
 * $Id: db_weekdayheaderitem.cpp 2198 2014-11-17 21:34:16Z rutger $
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
#include "db_weekdayheaderitem.h"
#include "db_calendarview.h"

DB_WeekDayHeaderItem::DB_WeekDayHeaderItem(DB_CalendarView* calendarView,
                                           int dayOfWeek,
                                           QGraphicsItem* parent,
                                           QGraphicsScene* scene)
                    : DB_CalendarItem(parent, scene),
                      mCalendarView(calendarView),
                      mDayOfWeek(dayOfWeek),
                      mDayTitle(QDate::longDayName(dayOfWeek)),
                      mAlign(Qt::AlignCenter) {
    setAcceptHoverEvents(true);
}

void DB_WeekDayHeaderItem::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget* /*widget*/) {
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();
    painter->setClipRect(mBoundingRect);

    painter->setPen(mPen);
    painter->setBrush(mGradient);
    painter->drawRect(0, 0, width, height);

    if (option->state & QStyle::State_MouseOver) {
        painter->setBrush(mToolTipBaseColor);
        painter->drawRect(0,0,width,height);
    }

    // Day of the week top header
    painter->setFont(mFont);
    painter->setPen(mFontPen);
    painter->drawText(mBoundingRect, mAlign, mDayTitle);

}

/**
 * To catch mouse over state
 * @brief DB_WeekDayHeaderItem::boundingRect
 * @return
 */
//QRectF DB_WeekDayHeaderItem::boundingRect() const {
//    QRectF rect = mBoundingRect;
// //    rect.setHeight(mCalendarView->height());
//    return rect;
//}

void DB_WeekDayHeaderItem::mousePressEvent(QGraphicsSceneMouseEvent* /*event*/) {
    // nothing
}

void DB_WeekDayHeaderItem::mouseMoveEvent(QGraphicsSceneMouseEvent* /*event*/) {
    // nothing
}

void DB_WeekDayHeaderItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/) {
    int weekNumber = mCalendarView->expandedWeekNumber();

    if (weekNumber > 0 && mCalendarView->expandedDayOfWeek() == 0) {
        mCalendarView->expandWeekDay(weekNumber, mDayOfWeek);
    } else {
        mCalendarView->expandDayOfWeek(mDayOfWeek);
    }
}

void DB_WeekDayHeaderItem::layoutChanged() {
    int width = (int)mBoundingRect.width();
    int height = mCalendarView->style()->expandedWeekHeaderHeight; // (int)mBoundingRect.height();

    DB_CalendarStyle *style = mCalendarView->style();

    mFont = style->collapsedDayNumberFont;
    mDayTitle.clear();

    QFontMetrics fm(mFont);
    if(fm.width(QDate::longDayName(mDayOfWeek)) + 10 < width)     {
        mDayTitle.append(QDate::longDayName(mDayOfWeek));
        mAlign = Qt::AlignCenter | Qt::AlignVCenter;
    } else {
        mDayTitle.append(QDate::shortDayName(mDayOfWeek));

        if(fm.width(mDayTitle) > width) {
            mAlign = Qt::AlignLeft | Qt::AlignVCenter;
        } else {
            mAlign = Qt::AlignCenter | Qt::AlignVCenter;
        }
    }

    if(mDayOfWeek < 6) {
        mGradient = style->comingDayGradient;
    } else {
        mGradient = style->comingWeekendGradient;
    }

    mGradient.setFinalStop(0,height); // does not really change

    mPen.setColor(style->shadowColor);
    mFontPen.setColor(style->textColor);
    mToolTipBaseColor = style->toolTipBaseColor;
    mToolTipBaseColor.setAlpha(172);
}
