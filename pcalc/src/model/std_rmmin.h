/*****************************************************************
 * $Id: std_rmmin.h 0001 2016-05-21T10:17:36 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RMMIN_H
#define STD_RMMIN_H

#include "rb_objectatomic.h"

/**
 * Minimum tensile strength at temperature
 */
class STD_RmMin : public RB_ObjectAtomic {

public:
    STD_RmMin(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_RmMin",
               RB_ObjectFactory* f = NULL);
    STD_RmMin(STD_RmMin* obj);
    virtual ~STD_RmMin();

private:
    void createMembers();

};

#endif /*STD_RMMIN_H*/
