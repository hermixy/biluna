/*****************************************************************
 * $Id: acc_stockmaster.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKMASTER_H
#define ACC_STOCKMASTER_H

#include "rb_objectcontainer.h"

/**
 * Class for stock master or stock items which are part of a stock category
 */
class ACC_StockMaster : public RB_ObjectContainer {

public:
    ACC_StockMaster(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_StockMaster(ACC_StockMaster* branch);
    virtual ~ACC_StockMaster();

private:
    void createMembers();

};

#endif // ACC_STOCKMASTER_H
