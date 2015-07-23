/*******************************************************************************
 * $Id: db_weekheaderitem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_WEEKHEADERITEM_H
#define DB_WEEKHEADERITEM_H

#include "db_calendaritem.h"

/**
 * TODO: Not used
 */
class DB_EXPORT DB_WeekHeaderItem : public DB_CalendarItem {

public:
    DB_WeekHeaderItem();
    ~DB_WeekHeaderItem();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    virtual void layoutChanged();
    virtual void dataChanged();

private:
    QList <DayHeaderItem *> dayHeaders;
}

#endif // DB_WEEKHEADERITEM_H
