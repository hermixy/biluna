/*****************************************************************
 * $Id: acc_contact.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_contact.h"

ACC_Contact::ACC_Contact (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Contact::ACC_Contact(ACC_Contact* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Contact::~ACC_Contact() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members. Members are based on SugarCRM. The parent is ACC_Customer,
 * alternative parent is ACC_Project (not yet ACC_Supplier)
 * - salutation Mr. or Mrs. etc.
 * - firstname first name of contact
 * - lastname last name
 * - leadsource source where the lead or contact originated
 * - title job title such as Director or Manager
 * - department company department where contact is working
 * - reportsto_id ID of superior or manager
 * - birhtdate date of birth
 * - donotcall 1 if contact does not want to be called
 * - phonehome home phone number
 * - phonemobile mobile phone number
 * - phonework direct work phone number
 * - phoneother alternative phone number
 * - phonefax fax number
 * - email1 main email address
 * - email2 alternative email address
 * - emailoptout 1 if contact does not want to receive emails
 * - invalidemail 1 if email address is invalid
 * - assistant name of contact assistant
 * - assistantphone phone number of assistant
 * - primaryaddress1 - 6 primary address lines
 * - altaddress1 - 6 alternative address lines
 * - description additional description about this contact
 * - rm_id ID of the CRM perspective account (CRM_Project.id) or SRM (SRM_Project.id)
 */
void ACC_Contact::createMembers() {
    addMember("salutation", "-", "", RB2::MemberChar20);
    addMember("firstname", "-", "", RB2::MemberChar125);
    addMember("lastname", "-", "", RB2::MemberChar125);
    addMember("leadsource", "-", "", RB2::MemberChar125);
    addMember("title", "-", "", RB2::MemberChar125);
    addMember("department", "-", "", RB2::MemberChar125);
    addMember("reportsto_id", "-", "", RB2::MemberChar40);
    addMember("birthdate", "-", 0, RB2::MemberDateTime);
    addMember("donotcall", "-", 0, RB2::MemberInteger);
    addMember("phonehome", "-", "", RB2::MemberChar20);
    addMember("phonemobile", "-", "", RB2::MemberChar20);
    addMember("phonework", "-", "", RB2::MemberChar20);
    addMember("phoneother", "-", "", RB2::MemberChar20);
    addMember("phonefax", "-", "", RB2::MemberChar20);
    addMember("email1", "-", "", RB2::MemberChar125);
    addMember("email2", "-", "", RB2::MemberChar125);
    addMember("voip", "-", "", RB2::MemberChar125);
    addMember("emailoptout", "-", 0, RB2::MemberInteger);
    addMember("invalidemail", "-", 0, RB2::MemberInteger);
    addMember("assistant", "-", "", RB2::MemberChar125);
    addMember("assistantphone", "-", "", RB2::MemberChar20);
    addMember("primaryaddress1", "-", "", RB2::MemberChar125);
    addMember("primaryaddress2", "-", "", RB2::MemberChar125);
    addMember("primaryaddress3", "-", "", RB2::MemberChar125);
    addMember("primaryaddress4", "-", "", RB2::MemberChar125);
    addMember("primaryaddress5", "-", "", RB2::MemberChar125);
    addMember("primaryaddress6", "-", "", RB2::MemberChar40);
    addMember("altaddress1", "-", "", RB2::MemberChar125);
    addMember("altaddress2", "-", "", RB2::MemberChar125);
    addMember("altaddress3", "-", "", RB2::MemberChar125);
    addMember("altaddress4", "-", "", RB2::MemberChar125);
    addMember("altaddress5", "-", "", RB2::MemberChar125);
    addMember("altaddress6", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar2500);

    // customer/supplier relation management
    addMember("rm_id", "-", "0", RB2::MemberChar40);
}
