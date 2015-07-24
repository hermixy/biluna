/*****************************************************************
 * $Id: srm_template.h 2069 2014-01-21 21:54:24Z rutger $
 * Created: Mar 06, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_TEMPLATE_H
#define SRM_TEMPLATE_H

#include "rb_objectatomic.h"

/**
 * Class for template for document to supplier such as mail or email
 */
class SRM_Template : public RB_ObjectAtomic {

public:
    SRM_Template(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SRM_Template(SRM_Template* obj);
    virtual ~SRM_Template();

private:
    void createMembers();

};

#endif // SRM_TEMPLATE_H
