/*****************************************************************
 * $Id: acc_taxgroupauth.h 1719 2012-07-25 10:26:12Z rutger $
 * Created: Feb 24, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXGROUPAUTH
#define ACC_TAXGROUPAUTH

#include "rb_objectcontainer.h"

/**
 * Class for tax group authorities. These authorities in a tax group
 * are the ACC_TaxAuthority applicable for this group
 */
class ACC_TaxGroupAuth : public RB_ObjectContainer {

public:
    ACC_TaxGroupAuth(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxGroupAuth(ACC_TaxGroupAuth* obj);
    virtual ~ACC_TaxGroupAuth();

private:
    void createMembers();

};

#endif // ACC_TAXGROUPAUTH
