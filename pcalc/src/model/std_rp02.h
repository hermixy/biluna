/*****************************************************************
 * $Id: std_rp02.h 0001 2016-05-21T13:37:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RP02_H
#define STD_RP02_H

#include "rb_objectatomic.h"

/**
 * 0.2% proof (or yield) strength
 */
class STD_Rp02 : public RB_ObjectAtomic {

public:
    STD_Rp02(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Rp02",
               RB_ObjectFactory* f = NULL);
    STD_Rp02(STD_Rp02* obj);
    virtual ~STD_Rp02();

private:
    void createMembers();

};

#endif /*STD_RP02_H*/
