/*****************************************************************
 * $Id: db_simplereport.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Sept 27, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_simplereport.h"


DB_SimpleReport::DB_SimpleReport(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

DB_SimpleReport::DB_SimpleReport(DB_SimpleReport* simple) : RB_ObjectAtomic(simple) {
	createMembers();
    *this = *simple;
}
	
DB_SimpleReport::~DB_SimpleReport() {
	// clean up of members and children is done in RB_ObjectBase
	// and ObjectContainer
}

/**
 * Create members:
 * - title title of the report
 * - subtitle subtitle of the report
 * - columnwidth column widths in percent separated by semicolon,
 *   also the number of columns is determined based on this list
 * - alternatingrow use alternating color for the rows
 * - firstdatafull use the first full row width for the first data
 * - sqlstring SQL statement to retrieve the report data from the database
 */
void DB_SimpleReport::createMembers() {
    addMember(this, "title", "-", "", RB2::MemberChar125);
    addMember(this, "subtitle", "-", "", RB2::MemberChar125);
    addMember(this, "columnwidths", "-", "", RB2::MemberChar125);
    addMember(this, "alternatingrow", "-", 0, RB2::MemberInteger);
    addMember(this, "firstdatafull", "-", 0, RB2::MemberInteger);
    addMember(this, "sqlstring", "-", "", RB2::MemberString);
}
