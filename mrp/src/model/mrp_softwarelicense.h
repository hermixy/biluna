/*****************************************************************
 * $Id: MRP_route.h 1725 2012-07-29 14:25:31Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SOFTWARELICENSE_H
#define MRP_SOFTWARELICENSE_H

#include "rb_objectcontainer.h"

/**
  Class for licenses.
 */
class MRP_SoftwareLicense : public RB_ObjectContainer {

public:
    MRP_SoftwareLicense(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    MRP_SoftwareLicense(MRP_SoftwareLicense* obj);
    virtual ~MRP_SoftwareLicense();

private:
    void createMembers();

};

#endif // MRP_SOFTWARELICENSE_H
