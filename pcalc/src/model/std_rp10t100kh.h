/*****************************************************************
 * $Id: std_rp10t100kh.h 0001 2016-05-21T09:59:34 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RP10T100KH_H
#define STD_RP10T100KH_H

#include "rb_objectatomic.h"

/**
 * Creep value 1% at time of 100000 hour
 */
class STD_Rp10T100kh : public RB_ObjectAtomic {

public:
    STD_Rp10T100kh(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Rp10T100kh",
               RB_ObjectFactory* f = NULL);
    STD_Rp10T100kh(STD_Rp10T100kh* obj);
    virtual ~STD_Rp10T100kh();

private:
    void createMembers();

};

#endif /*STD_RP10T100KH_H*/
