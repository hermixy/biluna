/*******************************************************************************
 * $Id: db_calendarstyle.cpp 2198 2014-11-17 21:34:16Z rutger $
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
#include "db_calendarstyle.h"

DB_CalendarStyle::DB_CalendarStyle()
    : weekNumberFont("arial", 30),
      dayNumberFont("arial", 24),
      dayNameFont("arial", 16),
      appointmentSubjectFont("arial", 16),
      clockBarFont("arial", 16),
      expandedDayNumberFont("Arial", 24),
      collapsedDayNumberFont("Arial", 18) {

    weekLeftMargin = 50;
    weekRightMargin = 16;
    quarterHeight = 10;

    expandedWeekHeaderHeight = 40;

    // original
//    pastDayGradient.setColorAt(0, QColor(199,199,199));
//    pastDayGradient.setColorAt(1, QColor(191,191,191));

//    todayGradient.setColorAt(0, QColor(229,182,114));
//    todayGradient.setColorAt(1, QColor(229,153,45));

//    comingDayGradient.setColorAt(0, QColor(255,255,255));
//    comingDayGradient.setColorAt(1, QColor(215,215,215));

//    pastWeekendGradient.setColorAt(0, QColor(169,169,169));
//    pastWeekendGradient.setColorAt(1, QColor(161,161,161));

//    weekendGradient.setColorAt(0, QColor(199,152,84));
//    weekendGradient.setColorAt(1, QColor(199,123,15));

//    comingWeekendGradient.setColorAt(0, QColor(225,225,225));
//    comingWeekendGradient.setColorAt(1, QColor(185,185,185));

//    pastDayPen.setColor(QColor(127,127,127));
//    todayPen.setColor(QColor(0,0,0));
//    comingDayPen.setColor(QColor(63,63,63));

    // new
    setGlobalPalette();
}

void DB_CalendarStyle::setGlobalPalette() {
    QPalette p = QApplication::palette();
    pastDayGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Dark));
    pastDayGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Dark));

    todayGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Highlight).lighter());
    todayGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Highlight));

    comingDayGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Light));
    comingDayGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Midlight));

    pastWeekendGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Dark));
    pastWeekendGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Dark));

    weekendGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Midlight));
    weekendGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Mid));

    comingWeekendGradient.setColorAt(0, p.color(QPalette::Active, QPalette::Midlight));
    comingWeekendGradient.setColorAt(1, p.color(QPalette::Active, QPalette::Mid));

    pastDayPen.setColor(p.color(QPalette::Active, QPalette::Button));
    todayPen.setColor(p.color(QPalette::Active, QPalette::WindowText));
    comingDayPen.setColor(p.color(QPalette::Active, QPalette::WindowText));

    shadowColor = p.color(QPalette::Active, QPalette::Shadow);
    baseColor = p.color(QPalette::Active, QPalette::Base);
    textColor = p.color(QPalette::Active, QPalette::Text);
    highlightColor = p.color(QPalette::Active, QPalette::Highlight);
    highlightedTextColor = p.color(QPalette::Active, QPalette::HighlightedText);
    toolTipBaseColor = p.color(QPalette::Active, QPalette::ToolTipBase);
}

/**
* @return always 30.
*/
int DB_CalendarStyle::collapsedWeekHeight() const {
    return 30;
}
