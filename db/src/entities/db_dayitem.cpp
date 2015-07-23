/*******************************************************************************
 * $Id: db_dayitem.cpp 2198 2014-11-17 21:34:16Z rutger $
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

#include "db_dayitem.h"
#include "db_weekitem.h"
#include "db_daycontentitem.h"
#include "db_activityitem.h"
#include "db_calendarstyle.h"
#include "db_calendarview.h"

/**
 * Constructor
 * @param calendar
 * @param date
 * @param parent
 * @param scene
 */
DB_DayItem::DB_DayItem(DB_CalendarView* calendarView, const QDate& date,
                       QGraphicsItem* parent, QGraphicsScene* scene)
            : DB_CalendarItem(parent, scene), mCalendarView(calendarView),
              mDate(date), mClockIcon(":/images/icons/clock.png") {

    mContentItem = new DB_DayContentItem(this); 
    mIsOutOfRange = false;

    setAcceptHoverEvents(true);
}


void DB_DayItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                       QWidget* /*widget*/) {
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();
    painter->setClipRect(mBoundingRect);

    painter->setPen(mPen);
    painter->setBrush(mGradient);
    painter->drawRect(0, 0, width, height);

    if(option->state & QStyle::State_MouseOver)     {
        painter->setBrush(mToolTipBaseColor);
        painter->drawRect(0,0,width,height);
    }

    QFontMetrics fm(mFont);
    painter->setFont(mFont);
    painter->setPen(mFontPen);
    painter->drawText(5, 5, width-5, height, mAlign, mCaption);

    if(mActivities.count() > 0 && mCalendarView->weekCount() > 1) {
        int iconLeft = fm.width(QString::number(mDate.day()));
        painter->drawPixmap(iconLeft + 10, fm.height() - fm.ascent() + 1, mClockIcon);
    }
}

void DB_DayItem::dataChanged() {
    mActivities.clear();

    QListIterator <DB_Calendar *> i(*mCalendarView->calendars());

    while(i.hasNext()) {
        DB_Calendar *cal = i.next();

        if(cal->isSelected()) {
            QListIterator <DB_Activity *> j(cal->getActivities(mDate));

            while(j.hasNext()) {
                DB_Activity *app = j.next();
                mActivities.append(app);
            }
        }
    }

    qSort(mActivities.begin(), mActivities.end(), DB_Activity::before);
    mContentItem->dataChanged();

}


void DB_DayItem::layoutChanged() {
    int left = (int)this->pos().x();
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();

    DB_CalendarStyle *style = mCalendarView->style();

    mContentItem->setPos(left,0);
    mContentItem->setSize(width, style->quarterHeight * 4 * 24);

    if(mDate < QDate::currentDate()) {
        if(mDate.dayOfWeek() < 6) {
            mGradient = style->pastDayGradient;
        } else {
            mGradient = style->pastWeekendGradient;
        }

        mGradient.setFinalStop(0,height);
        mFontPen = style->pastDayPen;
    } else if(mDate == QDate::currentDate()) {
        if(mDate.dayOfWeek() < 6) {
            mGradient = style->todayGradient;
        } else {
            mGradient = style->weekendGradient;
        }

        mGradient.setFinalStop(0,height);
        mFontPen = style->todayPen;
    } else {
        if(mDate.dayOfWeek() < 6) {
            mGradient = style->comingDayGradient;
        } else {
            mGradient = style->comingWeekendGradient;
        }

        mGradient.setFinalStop(0,height);
        mFontPen = style->comingDayPen;
    }

    if(mCalendarView->expandedWeekItem() != 0) {
        if(mCalendarView->expandedWeekItem()->hasDate(mDate)) {
            mFont = style->expandedDayNumberFont;
        } else {
            mFont = style->collapsedDayNumberFont;
        }
    } else {
        mFont = style->dayNumberFont;
    }

    if(mCalendarView->weekCount() == 1) {
        QString longCaption = QString::number(mDate.day())
                + "." + QString::number(mDate.month())
                + " " + mDate.longDayName(mDate.dayOfWeek());
        mFont = QFont("Arial", 16, QFont::Normal);
        QFontMetrics fm(mFont);
        mAlign = Qt::AlignVCenter;

        if(fm.width(longCaption) + 10 < width) {
            mCaption = longCaption;
            mAlign |= Qt::AlignCenter;
        } else {
            mCaption = QString::number(mDate.day()) + "."
                    + QString::number(mDate.month()) + " "
                    + mDate.shortDayName(mDate.dayOfWeek());

            if(fm.width(mCaption) < width) {
                mAlign |= Qt::AlignCenter;
            } else {
                mAlign |= Qt::AlignLeft;
            }
        }
    } else {
        mCaption = QString::number(mDate.day());
        mAlign = Qt::AlignLeft;
    }

    mPen.setColor(style->shadowColor);
    mToolTipBaseColor = style->toolTipBaseColor;
    mToolTipBaseColor.setAlpha(172);

    mContentItem->layoutChanged();
    update();
}


void DB_DayItem::mousePressEvent(QGraphicsSceneMouseEvent */*event*/) {
    // nothing
}

void DB_DayItem::mouseMoveEvent(QGraphicsSceneMouseEvent */*event*/) {
    // nothing
}

void DB_DayItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    mCalendarView->expandDate(mDate);
}
