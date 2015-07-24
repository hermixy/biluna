/*****************************************************************
 * $Id: acc_taxgroup.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXGROUP_H
#define ACC_TAXGROUP_H

#include "rb_objectcontainer.h"

/**
 * Class for tax group. Customer and supplier
 * have a tax group which is usually a national related taxregime.
 * A taxgroup has one or more taxgroup taxes
 * and related percentage levels. The tax and tax levels come from
 * ACC_TaxAuthority.
 */
class ACC_TaxGroup : public RB_ObjectContainer {

public:
    ACC_TaxGroup(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxGroup(ACC_TaxGroup* obj);
    virtual ~ACC_TaxGroup();

private:
    void createMembers();

};

#endif // ACC_TAXGROUP_H
