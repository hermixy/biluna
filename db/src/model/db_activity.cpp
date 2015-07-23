/*******************************************************************************
 * $Id: db_activity.cpp 2183 2014-10-02 14:33:40Z rutger $
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

#include "db_activity.h"

#include "db_calendar.h"

/**
 * Constructor
 */
DB_Activity::DB_Activity(const QString &id, RB_ObjectBase *p,
                         const QString &n, RB_ObjectFactory *f)
            : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}

DB_Activity::DB_Activity(DB_Activity* activity) {
    createMembers();
    *this = *activity;

}

DB_Activity::~DB_Activity() {
    // nothing
}

DB_Calendar* DB_Activity::calendar() const {
    return dynamic_cast<DB_Calendar*>(getGrandParent());
}

/**
 * @param startDateTime
 * @param endDateTime
 */
void DB_Activity::setDateTimes(const QDateTime& startDateTime, const QDateTime& endDateTime) {
    if(startDateTime <= endDateTime) {
        mStartDateTime = startDateTime;
        mEndDateTime = endDateTime;
    } else {
        mEndDateTime = mStartDateTime = startDateTime;
    }

    setValue("mstart", mStartDateTime);
    setValue("mend", mEndDateTime);
}

/**
 * @param startTime
 * @param endTime
 */
void DB_Activity::setTimes(const QTime& startTime, const QTime& endTime) {
    if(startTime <= endTime) {
        mStartDateTime.setTime(startTime);
        mEndDateTime.setTime(endTime);
    } else {
        mStartDateTime.setTime(startTime);
        mEndDateTime.setTime(startTime);
    }

    setValue("mstart", mStartDateTime);
    setValue("mend", mEndDateTime);
}

/**
 * Set start date and time of a meeting
 * @param startDateTime
 */
void DB_Activity::setStartDateTime(const QDateTime& startDateTime) {
    if(startDateTime <= mEndDateTime) {
        mStartDateTime = startDateTime;
    } else {
        mStartDateTime = mEndDateTime;
    }

    setValue("mstart", mStartDateTime);
}

/**
 * Set end date and time of a meeting
 * @param endDateTime
 */
void DB_Activity::setEndDateTime(const QDateTime &endDateTime) {
    if(endDateTime >= mStartDateTime) {
        mEndDateTime = endDateTime;
    } else {
        mEndDateTime = mStartDateTime;
    }

    setValue("mend", mEndDateTime);
}

/**
 * Returns the number of quarters of an hour, which will begin meeting
 * If the date is the start date of the meeting, the method returns
 * the number of quarters of an hour in which to start a meeting.
 * If the date is less than the end date of the meeting, the function returns 0.
 * Otherwise -1.
 * @param date
 */
int DB_Activity::startQuarter(const QDate &date) const {
    QTime time = mStartDateTime.time();

    if(date == mStartDateTime.date())
        return ((time.hour() * 60) + time.minute()) / 15;
    else if(date <= mEndDateTime.date())
        return 0;
    else
        return -1;
}

/**
 * Returns the number of quarters of an hour, which will end meeting
 * If the date is the start date of the meeting, the method returns
 * the number of quarters of an hour in which to start a meeting.
 * If the date is less than the end date of the meeting, the function returns 0.
 * Otherwise -1.
 */
int DB_Activity::endQuarter(const QDate& date) const {
    QTime time = mEndDateTime.time();

    if(date == mEndDateTime.date())
        return ((time.hour() * 60) + time.minute()) / 15;
    else if(mEndDateTime.date() > date)
        return (24 * 60) / 15;
    else
        return -1;
}

/**
 * @returns	true, if the meeting app1 will start earlier than app2
 */
bool DB_Activity::before(DB_Activity* act1, DB_Activity* act2) {
    return act1->startDateTime() < act2->startDateTime();
}

/**
 * Compares the activity (appointment or meeting) based on the key
 * @param other other activity (meeting or appointment)
 */
bool DB_Activity::operator==(const DB_Activity& other) const {
    if(this == &other)
        return true;
    else if(other.getValue("parent").toString() == this->getValue("parent").toString())
        return true;
    else
        return false;
}

/**
 * Compares the activity (appointment or meeting) based on the key
 * @param other other activity (meeting or appointment)
 */
bool DB_Activity::operator!=(const DB_Activity& other) const {
    return !(*this == other);
}

/**
 * Compares meetings starting times
 * @param other other activity (meeting or appointment)
 */
bool DB_Activity::operator<(const DB_Activity &other) const {
    if(this == &other)
        return false;
    else if(other.mStartDateTime > mStartDateTime)
        return true;
    else
        return false;
}

/**
 * Create members:
 * - activitytype_id activity type ID
 * - activitycode code by user for easy reference of activity, usually capitals
 * - subject subject of activity
 * - mstart start date/time of activity
 * - mend end date/time of activity
 * - isreminder 1 if this is reminder for later date
 * - remindertime time when reminder is activated
 * - priority_id priority ID such as none, low, medium and high
 * - description description of activity
 * - status_id status ID such as not started, ongoing and completed
 * - location location indicator where activity takes place
 * - mvalue money value in case of opportunity
 * - muser_id user ID (in DB)
 */
void DB_Activity::createMembers() {
    addMember(this, "activitytype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "activitycode", "-", "", RB2::MemberChar40);
    addMember(this, "subject", "-", "", RB2::MemberChar125);
    addMember(this, "mstart", "-", "", RB2::MemberDateTime);
    addMember(this, "mend", "-", "", RB2::MemberDateTime);
    addMember(this, "isreminder", "-", 0, RB2::MemberInteger);
    addMember(this, "remindertime", "day", 0, RB2::MemberInteger);
    addMember(this, "priority_id", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "", RB2::MemberChar2500);
    addMember(this, "status_id", "-", 0, RB2::MemberInteger);
    addMember(this, "location", "-", "", RB2::MemberChar125);
    addMember(this, "mvalue", "currency", 0.0, RB2::MemberDouble);
    addMember(this, "activityuser_id", "-", "0", RB2::MemberChar40);
}

