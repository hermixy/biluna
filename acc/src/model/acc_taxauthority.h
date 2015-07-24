/*****************************************************************
 * $Id: acc_taxauthority.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 22, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXAUTHORITY_H
#define ACC_TAXAUTHORITY_H

#include "rb_objectcontainer.h"

/**
 * Class for tax authorities
 */
class ACC_TaxAuthority : public RB_ObjectContainer {

public:
    ACC_TaxAuthority(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxAuthority(ACC_TaxAuthority* obj);
    virtual ~ACC_TaxAuthority();

private:
    void createMembers();

};

#endif // ACC_TAXAUTHORITY_H
