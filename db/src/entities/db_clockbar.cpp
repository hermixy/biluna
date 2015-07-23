/*******************************************************************************
 * $Id: db_clockbar.cpp 1973 2013-08-19 15:00:11Z rutger $
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
#include "db_clockbar.h"

/**
* @param parent
* @param scene
*/
DB_ClockBar::DB_ClockBar(QGraphicsItem *parent, QGraphicsScene *scene)
            : DB_CalendarItem(parent, scene) {
    prepareGeometryChange();

    QFont font("Arial", 18, QFont::Normal);
    QFontMetrics fm(font);

    // Add 0,0,10 + fm.width ("MM") and 24 * 10 * 4
    // to the existing coordinates of the rectangle.
    mBoundingRect.adjust(0,0,10 + fm.width("MM"), 24*10*4);
}

void DB_ClockBar::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem* /*option*/,
                        QWidget* /*widget*/) {
    int width = (int)mBoundingRect.width();
    int height = (int)mBoundingRect.height();

    painter->setBrush(QBrush(QColor(200, 200, 200)));
//    painter->drawRect(0.5, 0.5, width-0.5, height-0.5);
    painter->drawRect(0, 0, width, height);

    QFont font("Arial", 18, QFont::Normal);
    QFontMetrics fm(font);
    QRectF hourRect(-5, 0, fm.width("MM"), fm.height()); // = fm.tightBoundingRect("MM");

    painter->setFont(font);
    painter->setPen(QPen(QColor(80,80,80)));

    int y = 0; 

    // From 0 to 24 hours
    for (int i = 0; i < 24; i++) {
        painter->drawLine(0, y, width, y);
        painter->drawText(hourRect, Qt::AlignRight, QString::number(i));
        // Adding to the coordinates of the rectangle 40
        hourRect.adjust(0, 4*10, 0, 4*10);
        // Increase by 40 y
        y += 4*10;
    }

    font.setPixelSize(10);
    painter->setFont(font);
    fm = painter->fontMetrics();
    y = 0;
    int x = hourRect.left() + hourRect.width();

    // 24 times
    for (int i = 0; i < 24; i++) {
        // Appends 00 minutes every hour to the text
        painter->drawText(x, y + fm.ascent(), "00");
        y += 4*10;
    }
}
