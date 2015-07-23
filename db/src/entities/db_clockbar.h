/*******************************************************************************
 * $Id: db_clockbar.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_CLOCKBAR_H
#define DB_CLOCKBAR_H

#include "db_calendaritem.h"
#include "db_calendarstyle.h"

#include <QSize>

/**
* @class DB_ClockBar
* @brief Class to display a list of 0:00 hours to 23:00
*/ 
class DB_EXPORT DB_ClockBar : public DB_CalendarItem {

public:
    DB_ClockBar(QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget);
};

#endif
