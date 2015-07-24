/*****************************************************************
 * $Id: acc_stockmove.h 2191 2014-10-27 20:31:51Z rutger $
 * Created: Mar 25, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKMOVE_H
#define ACC_STOCKMOVE_H

#include "rb_objectcontainer.h"

/**
 * Data class that records all stock moves for further processing in 
 * accounting, if relevant. Moves can be: from one location to another,
 * used in manufacturing and used to assemble a new stock item.
 */
class ACC_StockMove : public RB_ObjectContainer {

public:
    ACC_StockMove(const RB_String& id = "", RB_ObjectBase* p = NULL,
                   const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_StockMove(ACC_StockMove* obj);
    virtual ~ACC_StockMove();

private:
    void createMembers();

};

#endif // ACC_STOCKMOVE_H
