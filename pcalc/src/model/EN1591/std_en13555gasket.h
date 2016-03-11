/*****************************************************************
 * $Id: std_en13555gasket.h 0001 2016-01-25T19:20:30 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EN13555GASKET_H
#define STD_EN13555GASKET_H

#include "rb_objectcontainer.h"

/**
 * Gasket type for EN13555 tests and EN1591 calculations
 */
class STD_EN13555Gasket : public RB_ObjectContainer {

public:
    STD_EN13555Gasket(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    STD_EN13555Gasket(STD_EN13555Gasket* obj);
    virtual ~STD_EN13555Gasket();

private:
    void createMembers();

};

#endif /*STD_EN13555GASKET_H*/
