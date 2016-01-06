/*****************************************************************
 * $Id: std_qminl.h 0001 2016-01-06T17:16:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_QMINL_H
#define STD_QMINL_H

#include "rb_objectatomic.h"

/**
 * Minimum stress (QminL) to seal gasket at assembly
 * Parent class is STD_GasketEN
 */
class DB_EXPORT STD_QminL : public RB_ObjectAtomic {

public:
    STD_QminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_QminL(STD_QminL* obj);
    virtual ~STD_QminL();

private:
    void createMembers();

};

#endif /*STD_QMINL_H*/
