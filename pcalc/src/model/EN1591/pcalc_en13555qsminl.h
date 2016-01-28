/*****************************************************************
 * $Id: pcalc_en13555qsminl.h 0001 2016-01-24T16:47:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555QSMINL_H
#define PCALC_EN13555QSMINL_H

#include "rb_objectatomic.h"

/**
 * EN13555 gasket property QsminL
 */
class PCALC_En13555QsminL : public RB_ObjectAtomic {

public:
    PCALC_En13555QsminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_En13555QsminL(PCALC_En13555QsminL* obj);
    virtual ~PCALC_En13555QsminL();

private:
    void createMembers();

};

#endif /*PCALC_EN13555QSMINL_H*/
