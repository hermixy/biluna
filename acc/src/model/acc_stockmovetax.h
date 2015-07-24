/*****************************************************************
 * $Id: acc_stockmovetax.h 1800 2012-11-13 20:22:10Z rutger $
 * Created: Mar 25, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKMOVETAX_H
#define ACC_STOCKMOVETAX_H

#include "rb_objectcontainer.h"

/**
 * Class for taxes on stock moves
 */
class ACC_StockMoveTax : public RB_ObjectContainer {

public:
    ACC_StockMoveTax(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_StockMoveTax(ACC_StockMoveTax* obj);
    virtual ~ACC_StockMoveTax();

private:
    void createMembers();

};

#endif // ACC_STOCKMOVETAX_H
