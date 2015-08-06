/*****************************************************************
 * $Id: crm_sysseqno.cpp 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 27, 2014 15:34:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_sysseqno.h"


CRM_SysSeqNo::CRM_SysSeqNo (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


CRM_SysSeqNo::CRM_SysSeqNo(CRM_SysSeqNo* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


CRM_SysSeqNo::~CRM_SysSeqNo() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Members are:
 * - modeltype integer number as per XXX_ModelFactory::ModelType
 * - seqnumbername name of sequence type for user reference
 * - sequencenumber the (last used) sequence number
 * - modelsubtype_id ID of the model sub-type for example the ACC_ChartMaster ID,
 * to differentiate groups within the same table
 */
void CRM_SysSeqNo::createMembers() {
    addMember("modeltype", "-", 0, RB2::MemberInteger);
    addMember("seqnumbername", "-", "<NEW>", RB2::MemberChar125);
    addMember("sequencenumber", "-", 0, RB2::MemberInteger);
    addMember("modelsubtype_id", "-", "0", RB2::MemberChar40);
}
