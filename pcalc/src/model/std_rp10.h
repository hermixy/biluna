/*****************************************************************
 * $Id: std_rp10.h 0001 2016-05-21T13:34:38 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RP10_H
#define STD_RP10_H

#include "rb_objectatomic.h"

/**
 * 1% Proof (or yield) strength
 */
class STD_Rp10 : public RB_ObjectAtomic {

public:
    STD_Rp10(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Rp10",
               RB_ObjectFactory* f = NULL);
    STD_Rp10(STD_Rp10* obj);
    virtual ~STD_Rp10();

private:
    void createMembers();

};

#endif /*STD_RP10_H*/
