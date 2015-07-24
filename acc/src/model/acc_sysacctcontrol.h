/*****************************************************************
 * $Id: acc_sysacctcontrol.h 1792 2012-10-15 19:54:04Z rutger $
 * Created: Nov 11, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSACCTCONTROL_H
#define ACC_SYSACCTCONTROL_H

#include "rb_objectcontainer.h"

/**
 * System class for control of account in the Chart of Accounts. The function
 * control number (1-999) defines how the VAT is calculated,
 * the control numbers 1000 and above indicate that this account is a journal.
 * Refer also to acc.h: ACC2::ControlType and ACC_SysAcctControl
 *
 * NOTE: this class is NOT part of the ACC_ObjectFactory
 */
class ACC_SysAcctControl : public RB_ObjectContainer {

public:
    ACC_SysAcctControl(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SysAcctControl(ACC_SysAcctControl* obj);
    virtual ~ACC_SysAcctControl();

private:
    void createMembers();

};

#endif // ACC_SYSACCTCONTROL_H
