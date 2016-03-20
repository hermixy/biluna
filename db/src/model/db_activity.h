/*******************************************************************************
 * $Id: db_activity.h 2248 2015-06-21 09:13:00Z rutger $
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

#ifndef DB_ACTIVITY_H
#define DB_ACTIVITY_H

#include <QDate>
#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QTime>

#include "rb_objectatomic.h"

class DB_Calendar;

/**
* @class DB_Activity
* @brief Base class for storing information about meetings
*/ 

/**
Example of use:
@code
    DB_Activity* app = new DB_Activity();
    app->setDateTimes(QDateTime(QDate(2008,12,23),QTime(19,0,0)),
                     QDateTime(QDate(2008,12,23),QTime(23,0,0)));

    app->setSubject("Holiday party!");
    app->setPlace("Secret place"); 
    app->setDescription("Secret holiday party at secret place. Everybody is welcome!");

    DB_Calendar* cal = new DB_Calendar(0);
    cal->insertDB_Activity(app);
@endcode

Creates an DB_Activity. Method setDateTimes () set the start
and end of the meeting. The subject of the meeting can set
the method setSubject (), a meeting place - a method setPlace (),
and the method setDescription () gives a description.
Create an object of class DB_Calendar. Add a New DB_Activity to
the calendar call insertDB_Activity ().
*/

class DB_EXPORT DB_Activity : public RB_ObjectAtomic {

public:
    DB_Activity(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "DB_Activity", RB_ObjectFactory* f = NULL);
    DB_Activity(DB_Activity* activity);
    virtual ~DB_Activity();

    DB_Calendar* calendar() const;

    QString subject() const { return getValue("subject").toString(); /*return mSubject;*/ }
    void setSubject(const QString &subject) { setValue("subject", subject); /*mSubject = subject;*/ }

    QString place() const { return getValue("location").toString(); /*return mPlace;*/ }
    void setPlace(const QString &place) { setValue("location", place); /*mPlace = place;*/ }

    QString description() const { return getValue("description").toString(); /*return mDescription;*/ }
    void setDescription(const QString &description) { setValue("description", description); /*mDescription = description;*/ }

    QTime startTime() const { return mStartDateTime.time(); }
    QDate startDate() const { return mStartDateTime.date(); }
    QDateTime startDateTime() const { return mStartDateTime; }
    void setStartDateTime(const QDateTime &startDateTime);

    QTime endTime() const { return mEndDateTime.time(); }
    QDate endDate() const { return mEndDateTime.date(); }
    QDateTime endDateTime() const {return mEndDateTime; }
    void setEndDateTime(const QDateTime &endDateTime);

    void setDateTimes(const QDateTime &startDateTime, const QDateTime &endDateTime);
    void setTimes(const QTime &startTime, const QTime &endTime); 

    int startQuarter(const QDate &date) const;
    int endQuarter(const QDate &date) const;

    static bool before(DB_Activity *act1, DB_Activity *act2);

    bool operator==(const DB_Activity &other) const;
    bool operator!=(const DB_Activity &other) const;
    bool operator<(const DB_Activity &other) const;

private:
    void createMembers();

    DB_Calendar* mCalendar;

    QString mSubject;
    QString mPlace;
    QString mDescription;

    QDateTime mStartDateTime;
    QDateTime mEndDateTime;

};

Q_DECLARE_METATYPE(DB_Activity)

#endif
