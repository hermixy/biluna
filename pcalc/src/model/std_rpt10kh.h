/*****************************************************************
 * $Id: std_rpt10kh.h 0001 2016-05-21T10:03:58 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RPT10KH_H
#define STD_RPT10KH_H

#include "rb_objectatomic.h"

/**
 * Creep value 0.2% at time of 10000 hour
 */
class STD_RpT10kh : public RB_ObjectAtomic {

public:
    STD_RpT10kh(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_RpT10kh",
               RB_ObjectFactory* f = NULL);
    STD_RpT10kh(STD_RpT10kh* obj);
    virtual ~STD_RpT10kh();

private:
    void createMembers();

};

#endif /*STD_RPT10KH_H*/
