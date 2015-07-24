/*****************************************************************
 * $Id: srm_supplierdetail.h 2101 2014-02-19 11:55:29Z rutger $
 * Created: Feb 18, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SUPPLIERDETAIL_H
#define SRM_SUPPLIERDETAIL_H

#include "rb_objectatomic.h"

/**
 * Class for the supplier (purchase and private) details
 */
class SRM_SupplierDetail : public RB_ObjectAtomic {

public:
    SRM_SupplierDetail(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_SupplierDetail(SRM_SupplierDetail* obj);
    virtual ~SRM_SupplierDetail();

private:
    void createMembers();

};

#endif // SRM_SUPPLIERDETAIL_H
