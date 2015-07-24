/*****************************************************************
 * $Id: acc_cogsglposting.h 1844 2012-12-23 14:38:49Z rutger $
 * Created: Apr 02, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COGSGLPOSTING_H
#define ACC_COGSGLPOSTING_H

#include "rb_objectcontainer.h"

/**
 * Class for Cost of Goods Sold General Ledger postings
 */
class ACC_CogsGlPosting : public RB_ObjectContainer {

public:
    ACC_CogsGlPosting(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_CogsGlPosting(ACC_CogsGlPosting* obj);
    virtual ~ACC_CogsGlPosting();

private:
    void createMembers();

};

#endif // ACC_COGSGLPOSTING_H
