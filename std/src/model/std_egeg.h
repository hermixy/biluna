/*****************************************************************
 * $Id: std_egeg.h 0001 2016-01-06T18:00:12 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EGEG_H
#define STD_EGEG_H

#include "rb_objectatomic.h"

/**
 * Elasticity (E_G) and thickness (e_G) of gasket at gasket stress and temperature, parent class is STD_GasketEN
 */
class DB_EXPORT STD_EGeG : public RB_ObjectAtomic {

public:
    STD_EGeG(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_EGeG(STD_EGeG* obj);
    virtual ~STD_EGeG();

private:
    void createMembers();

};

#endif /*STD_EGEG_H*/
