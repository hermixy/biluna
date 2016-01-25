/*****************************************************************
 * $Id: pcalc_en13555gasket.h 0001 2016-01-25T19:20:30 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555GASKET_H
#define PCALC_EN13555GASKET_H

#include "rb_objectcontainer.h"

/**
 * Gasket type for EN13555 tests and EN1591 calculations
 */
class DB_EXPORT PCALC_En13555Gasket : public RB_ObjectContainer {

public:
    PCALC_En13555Gasket(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    PCALC_En13555Gasket(PCALC_En13555Gasket* obj);
    virtual ~PCALC_En13555Gasket();

private:
    void createMembers();

};

#endif /*PCALC_EN13555GASKET_H*/
