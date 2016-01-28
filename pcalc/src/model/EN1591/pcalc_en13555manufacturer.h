/*****************************************************************
 * $Id: pcalc_en13555manufacturer.h 0001 2016-01-25T21:20:35 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555MANUFACTURER_H
#define PCALC_EN13555MANUFACTURER_H

#include "rb_objectcontainer.h"

/**
 * Gasket manufacturer
 */
class PCALC_En13555Manufacturer : public RB_ObjectContainer {

public:
    PCALC_En13555Manufacturer(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_En13555Manufacturer(PCALC_En13555Manufacturer* obj);
    virtual ~PCALC_En13555Manufacturer();

private:
    void createMembers();

};

#endif /*PCALC_EN13555MANUFACTURER_H*/
