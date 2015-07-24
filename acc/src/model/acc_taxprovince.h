/*****************************************************************
 * $Id: acc_taxprovince.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 180, 2010 4:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXPROVINCE_H
#define ACC_TAXPROVINCE_H

#include "rb_objectcontainer.h"

/**
 * Class for tax provinces, the stock location/warehouse tax province is
 * part of the procedure when determining the tax rate
 */
class ACC_TaxProvince : public RB_ObjectContainer {

public:
    ACC_TaxProvince(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxProvince(ACC_TaxProvince* obj);
    virtual ~ACC_TaxProvince();

private:
    void createMembers();

};

#endif // ACC_TAXPROVINCE_H
