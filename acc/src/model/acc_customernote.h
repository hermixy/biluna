/*****************************************************************
 * $Id: acc_customernote.h 1479 2011-11-17 15:51:46Z rutger $
 * Created: Nov 14, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTOMERNOTE_H
#define ACC_CUSTOMERNOTE_H

#include "rb_objectcontainer.h"

/**
 * Class for customer note
 */
class ACC_CustomerNote : public RB_ObjectContainer {

public:
    ACC_CustomerNote(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CustomerNote(ACC_CustomerNote* obj);
    virtual ~ACC_CustomerNote();

private:
    void createMembers();

};

#endif // ACC_CUSTOMERNOTE_H
