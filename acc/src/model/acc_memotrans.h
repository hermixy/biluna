/*****************************************************************
 * $Id: acc_memotrans.h 1516 2012-01-15 14:40:32Z rutger $
 * Created: Jan 13, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MEMOTRANS_H
#define ACC_MEMOTRANS_H

#include "rb_objectcontainer.h"

/**
 * Class for memorandum transactions
 */
class ACC_MemoTrans : public RB_ObjectContainer {

public:
    ACC_MemoTrans(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_MemoTrans(ACC_MemoTrans* obj);
    virtual ~ACC_MemoTrans();

private:
    void createMembers();

};

#endif // ACC_MEMOTRANS_H
