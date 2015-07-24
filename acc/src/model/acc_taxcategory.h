/*****************************************************************
 * $Id: acc_taxcategory.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 22, 2010 9:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXCATEGORY_H
#define ACC_TAXCATEGORY_H

#include "rb_objectcontainer.h"

/**
 * Class for tax categories, such as luxury items,
 * food and cloathing, exempt items
 */
class ACC_TaxCategory : public RB_ObjectContainer {

public:
    ACC_TaxCategory(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxCategory(ACC_TaxCategory* obj);
    virtual ~ACC_TaxCategory();

private:
    void createMembers();

};

#endif // ACC_TAXCATEGORY_H
