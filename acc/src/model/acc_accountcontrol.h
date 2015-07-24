/*****************************************************************
 * $Id: acc_accountcontrol.h 1519 2012-01-18 17:12:44Z rutger $
 * Created: Nov 11, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACCOUNTCONTROL_H
#define ACC_ACCOUNTCONTROL_H

#include "rb_objectcontainer.h"

/**
 * Class for control of account in the Chart of Accounts. The function
 * control number (1-999) defines how the VAT is calculated,
 * the control numbers 1000 and above indicate that this account is a journal.
 * Refer also to acc.h: ACC2::ControlType
 * Note: this class is not part of the ACC_ObjectFactory
 */
class ACC_AccountControl : public RB_ObjectContainer {

public:
    ACC_AccountControl(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_AccountControl(ACC_AccountControl* obj);
    virtual ~ACC_AccountControl();

private:
    void createMembers();

};

#endif // ACC_ACCOUNTCONTROL_H
