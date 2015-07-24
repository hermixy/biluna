/*****************************************************************
 * $Id: acc_suppliernote.h 1650 2012-05-23 21:09:46Z rutger $
 * Created: May 23, 2012 10:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SUPPLIERNOTE_H
#define ACC_SUPPLIERNOTE_H

#include "rb_objectcontainer.h"

/**
 * Class for supplier note
 */
class ACC_SupplierNote : public RB_ObjectContainer {

public:
    ACC_SupplierNote(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_SupplierNote(ACC_SupplierNote* obj);
    virtual ~ACC_SupplierNote();

private:
    void createMembers();

};

#endif // ACC_SUPPLIERNOTE_H
