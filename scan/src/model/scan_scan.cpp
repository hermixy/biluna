/*****************************************************************
 * $Id: scan_scan.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_scan.h"


/**
 * Constructor
 */
SCAN_Scan::SCAN_Scan(const RB_String& Id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer(Id, p, n, f) {
    createMembers();
}


/**
 * Copy constructor
 */
SCAN_Scan::SCAN_Scan(SCAN_Scan* scan)
                    : RB_ObjectContainer(scan) {
    createMembers();
    *this = *scan;
}

/**
 * Destructor
 */
SCAN_Scan::~SCAN_Scan() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
 * Create members:
 * title is the title of the scan
 */
void SCAN_Scan::createMembers() {
    addMember(this, "title", "-", "None", RB2::MemberChar125);
}
