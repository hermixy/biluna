/*******************************************************************************
 * $Id: db_calendarview.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_CALENDARVIEW_H
#define DB_CALENDARVIEW_H

#include <QGraphicsView>
#include <QList>

#include "db_calendaritem.h"
#include "db_calendarstyle.h"
#include "db_weekitem.h"
#include "db_calendar.h"
#include "db_activity.h"

/**
* @class DB_CalendarView
* @brief Class to display the calendar content
*/ 
class DB_EXPORT DB_CalendarView : public QGraphicsView {

    Q_OBJECT

public:
    DB_CalendarView(QWidget* parent = 0);
    virtual ~DB_CalendarView();

    enum DisplayMode{
        DisplayFullWeeks = 0,
        DisplayOnlyRange = 1
    };

    int displayMode() const { return mDisplayMode; } 
    int expandedDayOfWeek() const { return mExpandedDayOfWeek; }
    int expandedWeekNumber() const;

    DB_WeekItem* expandedWeekItem() const { return mExpandedWeekItem; }
    int weekCount() const { return mWeekCount; }
    int dayWidth(int dayOfWeek) const {
        if(dayOfWeek >= 0 && dayOfWeek < 21) return mDayWidths[dayOfWeek]; else return 0;
    }

    DB_CalendarStyle* style() const { return mStyle; }
    QList<DB_Calendar*>* calendars() { return mCalendars; }

    void createNewActivity(const QDateTime& start, const QDateTime& end);
    void editActivity(DB_Activity* activity);
    void selectActivity(DB_Activity* activity);


public slots:
    void expandDate(const QDate& date);
    void expandWeekDay(int weekNumber, int dayOfWeek);
    void expandDayOfWeek(int dayOfWeek);
    void expandWeek(int weekNumber);
    void expandWeek(DB_WeekItem* week);
    void collapseAll();

    void setRange(const QDate& start, const QDate& end);
    void setMonth(int year, int month);

    void setDisplayMode(DisplayMode mode);
    void setCalendars(QList<DB_Calendar*>* calendars);

//    void showActivityForm(DB_Activity* appointment);

    void dataChanged();
    void layoutChanged();

protected:
    void resizeEvent(QResizeEvent* event);
    void changeEvent(QEvent *e);

private slots:
    void onFormClosed(DB_Activity* appointment);

private:
    void changeGlobalPalette();

    int mWeekMode;
    int mDisplayMode;

    QDate mStartDate;
    QDate mEndDate;

    QDate mRangeStart;
    QDate mRangeEnd;

    QDate mExpandedDate;

    int mWeekCount;

    DB_CalendarItem *mHeader;
    DB_CalendarItem *mContentPane;

    QList <DB_WeekItem *> mWeeks;

    int mExpandedDayOfWeek;
    DB_WeekItem* mExpandedWeekItem;

    DB_CalendarStyle* mStyle;

    int mDayWidths[21];

    QList<DB_Calendar*>* mCalendars;

    // HACK: to draw the bottom border line and left scrollbar line
    QGraphicsLineItem* mBottomLine;
    QGraphicsLineItem* mLeftScrollLine;
};

#endif // DB_CALENDARVIEW_H
