/*****************************************************************
 * $Id: std_en13555manufacturer.h 0001 2016-01-25T21:20:35 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EN13555MANUFACTURER_H
#define STD_EN13555MANUFACTURER_H

#include "rb_objectcontainer.h"

/**
 * Gasket manufacturer
 */
class STD_EN13555Manufacturer : public RB_ObjectContainer {

public:
    STD_EN13555Manufacturer(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    STD_EN13555Manufacturer(STD_EN13555Manufacturer* obj);
    virtual ~STD_EN13555Manufacturer();

private:
    void createMembers();

};

#endif /*STD_EN13555MANUFACTURER_H*/
