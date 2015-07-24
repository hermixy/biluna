/*****************************************************************
 * $Id: acc_stockcategory.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKCATEGORY_H
#define ACC_STOCKCATEGORY_H

#include "rb_objectcontainer.h"

/**
 * Class for stock category, which is a virtual container for stock items
 */
class ACC_StockCategory : public RB_ObjectContainer {

public:
    ACC_StockCategory(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_StockCategory(ACC_StockCategory* branch);
    virtual ~ACC_StockCategory();

private:
    void createMembers();

};

#endif // ACC_STOCKCATEGORY_H
