/*****************************************************************
 * $Id: pcalc_en13555pqrdeltaegc.h 0001 2016-01-24T16:53:06 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555PQRDELTAEGC_H
#define PCALC_EN13555PQRDELTAEGC_H

#include "rb_objectatomic.h"

/**
 * EN13555 gasket properties P_QR and delta_e_GC
 */
class PCALC_EN13555PqrDeltaeGC : public RB_ObjectAtomic {

public:
    PCALC_EN13555PqrDeltaeGC(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_EN13555PqrDeltaeGC(PCALC_EN13555PqrDeltaeGC* obj);
    virtual ~PCALC_EN13555PqrDeltaeGC();

private:
    void createMembers();

};

#endif /*PCALC_EN13555PQRDELTAEGC_H*/
