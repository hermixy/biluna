/*****************************************************************
 * $Id: acc_glsum.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Oct 6, 2010 8:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_GLSUM_H
#define ACC_GLSUM_H

#include "rb_objectcontainer.h"

/**
 * Class for General Ledger (monthly) summary
 */
class ACC_GlSum : public RB_ObjectContainer {

public:
    ACC_GlSum(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_GlSum(ACC_GlSum* obj);
    virtual ~ACC_GlSum();

private:
    void createMembers();

};

#endif // ACC_GLSUM_H
