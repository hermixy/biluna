/*****************************************************************
 * $Id: scan_report.cpp 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_report.h"


SCAN_Report::SCAN_Report (const RB_String& id, RB_ObjectBase* p,
                                  const RB_String& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SCAN_Report::SCAN_Report(SCAN_Report* report)
                                    : RB_ObjectContainer(report) {
    createMembers();
    *this = *report;
}

SCAN_Report::~SCAN_Report () {
    // deletion of children and members is done in RB_ObjectBase
    // and RB_ObjectContainer
}

/**
 * Create member definition
 * title is the report title
 */
void SCAN_Report::createMembers() {
    RB_String uuid = "";
    addMember(uuid, this, "title", "-", "Title", RB2::MemberChar125);
}
