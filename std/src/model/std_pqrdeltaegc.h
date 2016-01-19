/*****************************************************************
 * $Id: std_pqrdeltaegc.h 0001 2016-01-06T17:56:03 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_PQRDELTAEGC_H
#define STD_PQRDELTAEGC_H

#include "rb_objectatomic.h"

/**
 * Values (P_QR or deltae_Gc) for gasket relaxation, parent class is STD_GasketEN
 */
class DB_EXPORT STD_PqrDeltaeGc : public RB_ObjectAtomic {

public:
    STD_PqrDeltaeGc(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_PqrDeltaeGc(STD_PqrDeltaeGc* obj);
    virtual ~STD_PqrDeltaeGc();

private:
    void createMembers();

};

#endif /*STD_PQRDELTAEGC_H*/
