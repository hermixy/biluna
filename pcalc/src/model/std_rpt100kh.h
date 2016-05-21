/*****************************************************************
 * $Id: std_rpt100kh.h 0001 2016-05-21T10:02:08 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RPT100KH_H
#define STD_RPT100KH_H

#include "rb_objectatomic.h"

/**
 * Creep value 0.2% at time of 100000 hour
 */
class STD_RpT100kh : public RB_ObjectAtomic {

public:
    STD_RpT100kh(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_RpT100kh",
               RB_ObjectFactory* f = NULL);
    STD_RpT100kh(STD_RpT100kh* obj);
    virtual ~STD_RpT100kh();

private:
    void createMembers();

};

#endif /*STD_RPT100KH_H*/
