/*****************************************************************
 * $Id: scan_scanresult.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_scanresult.h"


/**
 * Constructor
 */
SCAN_ScanResult::SCAN_ScanResult(const RB_String& Id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer(Id, p, n, f) {
    createMembers();
}


/**
 * Copy constructor
 */
SCAN_ScanResult::SCAN_ScanResult(SCAN_ScanResult* scanRes)
                    : RB_ObjectContainer(scanRes) {
    createMembers();
    *this = *scanRes;
}

/**
 * Destructor
 */
SCAN_ScanResult::~SCAN_ScanResult() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
 * Create members:
 * title title of the scan
 * scan_id ID of the original scan
 * contact_id ID of ACC_Contacts
 */
void SCAN_ScanResult::createMembers() {
    addMember("title", "-", "title", RB2::MemberChar125);
    addMember("scan_id", "-", "", RB2::MemberChar40);
    addMember("contact_id", "-", "", RB2::MemberChar40);

}
