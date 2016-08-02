/*****************************************************************
 * $Id: std_gasket.h 0001 2016-08-02T19:29:18 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_GASKET_H
#define STD_GASKET_H

#include "rb_objectatomic.h"

/**
 * Gasket dimensions as per standard such as ASME B16.20 and EN1514
 * (TODO: ex VES StdGasket table)
 */
class STD_Gasket : public RB_ObjectAtomic {

public:
    STD_Gasket(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Gasket",
               RB_ObjectFactory* f = NULL);
    STD_Gasket(STD_Gasket* obj);
    virtual ~STD_Gasket();

private:
    void createMembers();

};

#endif /*STD_GASKET_H*/
