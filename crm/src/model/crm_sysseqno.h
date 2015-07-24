/*****************************************************************
 * $Id: crm_sysseqno.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 27, 2014 15:34:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SYSSEQNO_H
#define CRM_SYSSEQNO_H

#include "rb_objectatomic.h"

/**
 * Class for system sequence numbers, which are counters. For example
 * the last used invoice number is maintained here. During the creation of a
 * new invoice the last invoice number is retrieved from this class and updated
 * with 1 to represent to new invoice number.
 */
class CRM_SysSeqNo : public RB_ObjectAtomic {

public:
    CRM_SysSeqNo(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_SysSeqNo(CRM_SysSeqNo* obj);
    virtual ~CRM_SysSeqNo();

private:
    void createMembers();

};

#endif // CRM_SYSSEQNO_H
