/*****************************************************************
 * $Id: acc_transdoc.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Sep 5, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TRANSDOC_H
#define ACC_TRANSDOC_H

#include "rb_objectcontainer.h"

/**
 * Class for GL transaction document, the document is the controlling
 * invoice, bill or reference which is the 'paper' proof/validation
 * for the transaction
 */
class ACC_TransDoc : public RB_ObjectContainer {

public:
    ACC_TransDoc(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_TransDoc(ACC_TransDoc* obj);
    virtual ~ACC_TransDoc();

private:
    void createMembers();

};

#endif // ACC_TRANSDOC_H

