/*****************************************************************
 * $Id: std_rpt200kh.h 0001 2016-05-21T10:06:33 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RPT200KH_H
#define STD_RPT200KH_H

#include "rb_objectatomic.h"

/**
 * Creep value 0.2% at time of 200000 hour
 */
class STD_RpT200kh : public RB_ObjectAtomic {

public:
    STD_RpT200kh(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_RpT200kh",
               RB_ObjectFactory* f = NULL);
    STD_RpT200kh(STD_RpT200kh* obj);
    virtual ~STD_RpT200kh();

private:
    void createMembers();

};

#endif /*STD_RPT200KH_H*/
