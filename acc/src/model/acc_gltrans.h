/*****************************************************************
 * $Id: acc_gltrans.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 1, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_GLTRANS_H
#define ACC_GLTRANS_H

#include "rb_objectcontainer.h"

/**
 * Class for general ledger transactions
 */
class ACC_GlTrans : public RB_ObjectContainer {

public:
    ACC_GlTrans(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_GlTrans(ACC_GlTrans* obj);
    virtual ~ACC_GlTrans();

private:
    void createMembers();

};

#endif // ACC_GLTRANS_H
