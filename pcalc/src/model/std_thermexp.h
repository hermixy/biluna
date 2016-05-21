/*****************************************************************
 * $Id: std_thermexpvalue.h 0001 2016-05-21T13:04:41 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_THERMEXP_H
#define STD_THERMEXP_H

#include "rb_objectatomic.h"

/**
 * Thermal expansion values
 */
class STD_ThermExp : public RB_ObjectAtomic {

public:
    STD_ThermExp(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_ThermExp",
               RB_ObjectFactory* f = NULL);
    STD_ThermExp(STD_ThermExp* obj);
    virtual ~STD_ThermExp();

private:
    void createMembers();

};

#endif /*STD_THERMEXP_H*/
