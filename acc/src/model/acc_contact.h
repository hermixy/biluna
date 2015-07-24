/*****************************************************************
 * $Id: acc_contact.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Feb 21, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CONTACT_H
#define ACC_CONTACT_H

#include "rb_objectcontainer.h"

/**
 * Class for contact persons
 */
class ACC_Contact : public RB_ObjectContainer {

public:
    ACC_Contact(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_Contact(ACC_Contact* obj);
    virtual ~ACC_Contact();

private:
    void createMembers();

};

#endif // ACC_AREA_H
