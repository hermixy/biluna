/*****************************************************************
 * $Id: std_rp10t10kh.h 0001 2016-05-21T09:57:18 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RP10T10KH_H
#define STD_RP10T10KH_H

#include "rb_objectatomic.h"

/**
 * Creep value 1% at time of 10000 hour
 */
class STD_Rp10T10kh : public RB_ObjectAtomic {

public:
    STD_Rp10T10kh(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Rp10T10kh",
               RB_ObjectFactory* f = NULL);
    STD_Rp10T10kh(STD_Rp10T10kh* obj);
    virtual ~STD_Rp10T10kh();

private:
    void createMembers();

};

#endif /*STD_RP10T10KH_H*/
