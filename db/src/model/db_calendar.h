/*******************************************************************************
 * $Id: db_calendar.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_CALENDAR_H
#define DB_CALENDAR_H

#include <QColor>
#include <QList>
#include <QMetaType>
#include <QString>

#include "db_activity.h"
#include "rb_objectcontainer.h"

/**
 @class DB_Calendar
 @brief Class of storage of calendars
*/

/**
Example of use:

@code 
    DB_Calendar *cal = new DB_Calendar(0);
    cal->setName("Holiday DB_Calendar");
    cal->setColor(QColor(56, 128, 189));
    cal->setSelected(false);

    DB_Activity *app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(19,0,0)),
                     QDateTime(QDate(2008,12,23),QTime(23,0,0)));

    app->setSubject("Holiday party!");
    app->setPlace("Secret place"); 
    app->setDescription("Secret holiday party at secret place. Everybody is welcome!");
    cal->insertAppointment(app);
@endcode

Create Calendar with index 0. Method setName() set its name.
The method setColor() allows you to specify the desired color of the calendar,
and setSelected() - highlight the desired status.
Create a DB_Activity or appointment (see the example in the description).
Add a new appointment to the calendar with insertAppointment().
*/ 
class DB_EXPORT DB_Calendar : public RB_ObjectContainer {

public:
    DB_Calendar(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "DB_Calendar", RB_ObjectFactory* f = NULL);
    DB_Calendar(DB_Calendar* calendar);
    virtual ~DB_Calendar();

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString& description);

    bool isSelected() const { return mSelected; }
    void setSelected(bool selected) { mSelected = selected; }

    QColor color() const;
    void setColor(const QColor &color);

    QList<DB_Activity *> getActivities(const QDate &date);
    bool insertActivity(DB_Activity *appointment);
    bool updateActivity(const DB_Activity &appointment);
    bool removeActivity(DB_Activity *appointment);

    bool operator==(const DB_Calendar &other) const;
    bool operator!=(const DB_Calendar &other) const;

private:
    void createMembers();

    bool mSelected;
    QColor mColor;

    QString mName;
    QString mDescription;
};

//Q_DECLARE_METATYPE(DB_Calendar)

#endif
