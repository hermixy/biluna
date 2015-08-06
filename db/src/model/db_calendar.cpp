/*******************************************************************************
 * $Id: db_calendar.cpp 2034 2013-11-14 15:34:56Z rutger $
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

#include "db_calendar.h"

/**
 * Constructor
 */
DB_Calendar::DB_Calendar(const RB_String& id, RB_ObjectBase* p,
                         const RB_String& n, RB_ObjectFactory* f)
            : RB_ObjectContainer (id, p, n, f) {
    createMembers();

    mSelected = false;
    mColor = QColor(127,127,127);

    mName = "";
    mDescription = "";
}

DB_Calendar::DB_Calendar(DB_Calendar* calendar) : RB_ObjectContainer(calendar) {
    createMembers();
    *this = *calendar;

    mSelected = false;
    mColor = calendar->color();

    mName = calendar->name();
    mDescription = calendar->description();
}


DB_Calendar::~DB_Calendar() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
    RB_DEBUG->print("DB_Calendar::~DB_Calendar()");
}

QString DB_Calendar::name() const {
    return getValue("mname").toString();
}

void DB_Calendar::setName(const QString& name) {
    setValue("mname", name);
}

QString DB_Calendar::description() const {
    return getValue("description").toString();
}

void DB_Calendar::setDescription(const QString &description) {
    setValue("description", description);
}

QColor DB_Calendar::color() const {
    QColor col = QColor::fromRgb(getValue("color").toInt());
    return col;
}

void DB_Calendar::setColor(const QColor& color) {
    setValue("color", color.toRgb().value());
}


/**
 * Compares calendars by key values
 * @param other other calendar
 * @return true if equal
 */
bool DB_Calendar::operator==(const DB_Calendar& other) const {
    if(this == &other)
        return true;
    else if(other.getId() == this->getId())
        return true;
    else
        return false;
}

/**
 * Compares calendars by key values.
 * @param other other calendar
 * @return true if unequal
 */
bool DB_Calendar::operator!=(const DB_Calendar& other) const {
    return !(*this == other);
}

/**
 * Get list of activities (appointments or meetings)
 * @param date
 * @return list of activities
 */
QList<DB_Activity*> DB_Calendar::getActivities(const QDate& date) {
    QList<DB_Activity*> result;
    RB_ObjectContainer* actList = getContainer("DB_ActivityList");
    RB_ObjectIterator* iter = actList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* act = iter->currentObject();

        if (act->getValue("mstart").toDate() <= date
                && act->getValue("mend").toDate() >= date) {
            result.append(dynamic_cast<DB_Activity*>(act));
        }
    }

    delete iter;
    return result;
}

/**
 * Insert new activity (appointment or meeting)
 * @param appointment activity
 */
bool DB_Calendar::insertActivity(DB_Activity* appointment) {
    RB_ObjectContainer* actList = this->getContainer("DB_ActivityList");
    actList->addObject(appointment);
    return true;
}

/**
 * TODO: implement
 */
bool DB_Calendar::updateActivity(const DB_Activity &/*appointment*/) {
//    if(!mActivities->contains(appointment))
//    {
//        mActivities->replace(mActivities->indexOf(appointment), appointment);
//        return true;
//    }

    return false;
}

/**
 * Remove activity
 * @param appointment activity
 */
bool DB_Calendar::removeActivity(DB_Activity *appointment) {
    RB_ObjectContainer* actList = this->getContainer("DB_ActivityList");
    actList->remove(appointment, true); // true = delete object
    return true;
}

/**
 * Create members:
 * - mname name of the calendar
 * - description description of the calendar
 * - color color of the activity items
 */
void DB_Calendar::createMembers() {
    addMember("mname", "-", "<New>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("color", "-", "", RB2::MemberChar40);
}

