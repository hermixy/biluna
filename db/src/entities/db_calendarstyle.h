/*******************************************************************************
 * $Id: db_calendarstyle.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_CALENDARSTYLE_H
#define DB_CALENDARSTYLE_H

#include <QFont>
#include <QList>
#include <QDate>
#include <QLinearGradient>
#include <QPen>
#include "db_global.h"

/**
* @class DB_CalendarStyle
* @brief Class to store information about the appearance of a calendar
*/
class DB_EXPORT DB_CalendarStyle {

public:
    DB_CalendarStyle();

    virtual void setGlobalPalette();
    int collapsedWeekHeight() const;

    QFont weekNumberFont;
    QFont dayNumberFont;
    QFont dayNameFont;

    QFont appointmentSubjectFont;
    QFont clockBarFont;

    int weekLeftMargin;
    int weekRightMargin;
    int quarterHeight;

    int expandedWeekHeaderHeight;

    QPen pastDayPen;
    QPen todayPen;
    QPen comingDayPen;

    QLinearGradient pastDayGradient;
    QLinearGradient todayGradient;
    QLinearGradient comingDayGradient;
    
    QLinearGradient pastWeekendGradient;
    QLinearGradient weekendGradient;
    QLinearGradient comingWeekendGradient;

    QFont expandedDayNumberFont;
    QFont collapsedDayNumberFont;

    QColor shadowColor;
    QColor baseColor;
    QColor textColor;
    QColor highlightColor;
    QColor highlightedTextColor;
    QColor toolTipBaseColor;
};

#endif
