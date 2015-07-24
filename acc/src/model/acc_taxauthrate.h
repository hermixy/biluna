/*****************************************************************
 * $Id: acc_taxauthrate.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 22, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXAUTHRATE_H
#define ACC_TAXAUTHRATE_H

#include "rb_objectcontainer.h"

/**
 * Class for tax authority rates
 */
class ACC_TaxAuthRate : public RB_ObjectContainer {

public:
    ACC_TaxAuthRate(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TaxAuthRate(ACC_TaxAuthRate* obj);
    virtual ~ACC_TaxAuthRate();

private:
    void createMembers();

};

#endif // ACC_TAXAUTHRATE_H
