/*****************************************************************
 * $Id: srm_sysseqno.h 2073 2014-01-25 17:07:56Z rutger $
 * Created: Jan 25, 2014 15:34:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SYSSEQNO_H
#define SRM_SYSSEQNO_H

#include "rb_objectatomic.h"

/**
 * Class for system sequence numbers, which are counters. For example
 * the last used invoice number is maintained here. During the creation of a
 * new invoice the last invoice number is retrieved from this class and updated
 * with 1 to represent to new invoice number.
 */
class SRM_SysSeqNo : public RB_ObjectAtomic {

public:
    SRM_SysSeqNo(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_SysSeqNo(SRM_SysSeqNo* obj);
    virtual ~SRM_SysSeqNo();

private:
    void createMembers();

};

#endif // SRM_SYSSEQNO_H
