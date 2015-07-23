/*******************************************************************************
 * $Id: db_calendaritem.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_CALENDARITEM_H
#define DB_CALENDARITEM_H

#include <QGraphicsItem>
#include <QPainterPath>
#include <QSizeF>

#include "db_global.h"


/**
* @class DB_CalendarItem
* @brief The base class for all graphical calendar items.
*/ 
class DB_EXPORT DB_CalendarItem : public QGraphicsItem {

public:
    DB_CalendarItem(QGraphicsItem* parent = 0,
                 QGraphicsScene* scene = 0);
    
    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget);

	virtual QPainterPath shape () const;
    QRectF boundingRect() const;
    void setSize(const QSize& size);
    void setSize(qreal width, qreal height);

    virtual void onResize(const QSizeF& size, const QSizeF& oldSize);
    virtual void layoutChanged();
    virtual void dataChanged();

protected:
    QRectF mBoundingRect;
};

#endif
