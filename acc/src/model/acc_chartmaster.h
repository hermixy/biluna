/*****************************************************************
 * $Id: acc_chartmaster.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Feb 24, 2010 10:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CHARTMASTER_H
#define ACC_CHARTMASTER_H

#include "rb_objectcontainer.h"

/**
 * Class for chart of accounts
 */
class ACC_ChartMaster : public RB_ObjectContainer {

public:
    ACC_ChartMaster(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    ACC_ChartMaster(ACC_ChartMaster* obj);
    virtual ~ACC_ChartMaster();

private:
    void createMembers();

};

#endif // ACC_CHARTMASTER_H
