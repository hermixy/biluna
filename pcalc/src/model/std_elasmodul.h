/*****************************************************************
 * $Id: std_elasmodulvalue.h 0001 2016-05-21T11:58:22 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_ELASMODULVALUE_H
#define STD_ELASMODULVALUE_H

#include "rb_objectatomic.h"

/**
 * Modulus of elasticity
 */
class STD_ElasModul : public RB_ObjectAtomic {

public:
    STD_ElasModul(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_ElasModul",
               RB_ObjectFactory* f = NULL);
    STD_ElasModul(STD_ElasModul* obj);
    virtual ~STD_ElasModul();

private:
    void createMembers();

};

#endif /*STD_ELASMODULVALUE_H*/
