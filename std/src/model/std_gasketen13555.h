/*****************************************************************
 * $Id: std_gasketen13555.h 0001 2016-01-06T17:05:37 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_GASKETEN13555_H
#define STD_GASKETEN13555_H

#include "rb_objectatomic.h"

/**
 * Gasket properties of EN13555
 */
class DB_EXPORT STD_GasketEN13555 : public RB_ObjectAtomic {

public:
    STD_GasketEN13555(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_GasketEN13555(STD_GasketEN13555* obj);
    virtual ~STD_GasketEN13555();

private:
    void createMembers();

};

#endif /*STD_GASKETEN_H*/
