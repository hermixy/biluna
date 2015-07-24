/*****************************************************************
 * $Id: crm_template.h 2067 2014-01-21 20:10:28Z rutger $
 * Created: Mar 06, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_TEMPLATE_H
#define CRM_TEMPLATE_H

#include "rb_objectatomic.h"

/**
 * Class for sales template such as mail or email
 */
class CRM_Template : public RB_ObjectAtomic {

public:
    CRM_Template(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    CRM_Template(CRM_Template* obj);
    virtual ~CRM_Template();

private:
    void createMembers();

};

#endif // CRM_TEMPLATE_H
