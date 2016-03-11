/*****************************************************************
 * $Id: std_en13555qminl.h 0001 2016-01-24T16:31:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EN13555QMINL_H
#define STD_EN13555QMINL_H

#include "rb_objectatomic.h"

/**
 * Gasket QminL property according EN13555
 */
class STD_EN13555QminL : public RB_ObjectAtomic {

public:
    STD_EN13555QminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    STD_EN13555QminL(STD_EN13555QminL* obj);
    virtual ~STD_EN13555QminL();

private:
    void createMembers();

};

#endif /*STD_EN13555QMINL_H*/
