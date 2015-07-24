/*****************************************************************
 * $Id: acc_factorcompany.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Mar 18, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_FACTORCOMPANY_H
#define ACC_FACTORCOMPANY_H

#include "rb_objectcontainer.h"

/**
 * Class for factor companies
 */
class ACC_FactorCompany : public RB_ObjectContainer {

public:
    ACC_FactorCompany(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_FactorCompany(ACC_FactorCompany* obj);
    virtual ~ACC_FactorCompany();

private:
    void createMembers();

};

#endif // ACC_FACTORCOMPANY_H
