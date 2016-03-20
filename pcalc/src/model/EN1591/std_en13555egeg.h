/*****************************************************************
 * $Id: std_en13555egeg.h 0001 2016-01-24T16:56:54 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EN13555EGEG_H
#define STD_EN13555EGEG_H

#include "rb_objectatomic.h"

/**
 * EN13555 gasket properties E_G and e_G
 */
class STD_EN13555EGeG : public RB_ObjectAtomic {

public:
    STD_EN13555EGeG(const QString& id = "", RB_ObjectBase* p = NULL,
                      const QString& n = "", RB_ObjectFactory* f = NULL);
    STD_EN13555EGeG(STD_EN13555EGeG* obj);
    virtual ~STD_EN13555EGeG();

private:
    void createMembers();

};

#endif /*STD_EN13555EGEG_H*/
