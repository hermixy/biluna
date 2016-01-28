/*****************************************************************
 * $Id: pcalc_en13555qminl.h 0001 2016-01-24T16:31:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555QMINL_H
#define PCALC_EN13555QMINL_H

#include "rb_objectatomic.h"

/**
 * Gasket QminL property according EN13555
 */
class PCALC_En13555QminL : public RB_ObjectAtomic {

public:
    PCALC_En13555QminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_En13555QminL(PCALC_En13555QminL* obj);
    virtual ~PCALC_En13555QminL();

private:
    void createMembers();

};

#endif /*PCALC_EN13555QMINL_H*/
