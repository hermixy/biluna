/*****************************************************************
 * $Id: srm_sysseqno.cpp 2073 2014-01-25 17:07:56Z rutger $
 * Created: Jan 25, 2014 15:34:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_sysseqno.h"


SRM_SysSeqNo::SRM_SysSeqNo (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SRM_SysSeqNo::SRM_SysSeqNo(SRM_SysSeqNo* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SRM_SysSeqNo::~SRM_SysSeqNo() {
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
void SRM_SysSeqNo::createMembers() {
    addMember(this, "modeltype", "-", 0, RB2::MemberInteger);
    addMember(this, "seqnumbername", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "sequencenumber", "-", 0, RB2::MemberInteger);
    addMember(this, "modelsubtype_id", "-", "0", RB2::MemberChar40);
}
