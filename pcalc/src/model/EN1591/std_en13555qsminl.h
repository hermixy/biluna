/*****************************************************************
 * $Id: std_en13555qsminl.h 0001 2016-01-24T16:47:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EN13555QSMINL_H
#define STD_EN13555QSMINL_H

#include "rb_objectatomic.h"

/**
 * EN13555 gasket property QsminL
 */
class STD_EN13555QsminL : public RB_ObjectAtomic {

public:
    STD_EN13555QsminL(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    STD_EN13555QsminL(STD_EN13555QsminL* obj);
    virtual ~STD_EN13555QsminL();

private:
    void createMembers();

};

#endif /*STD_EN13555QSMINL_H*/
