/*****************************************************************
 * $Id: std_qsminl.h 0001 2016-01-06T17:30:59 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_QSMINL_H
#define STD_QSMINL_H

#include "rb_objectatomic.h"

/**
 * Minimum gasket stress (QsminL) to seal at off loading, parent class is STD_GasketEN
 */
class DB_EXPORT STD_QsminL : public RB_ObjectAtomic {

public:
    STD_QsminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_QsminL(STD_QsminL* obj);
    virtual ~STD_QsminL();

private:
    void createMembers();

};

#endif /*STD_QSMINL_H*/
