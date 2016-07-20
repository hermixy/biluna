/*****************************************************************
 * $Id: std_flangefacingdim.h 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEFACINGDIM_H
#define STD_FLANGEFACINGDIM_H

#include "rb_objectatomic.h"

/**
 * Flange facing dimensions
 */
class STD_FlangeFacingDim : public RB_ObjectAtomic {

public:
    STD_FlangeFacingDim(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeFacingDim",
               RB_ObjectFactory* f = NULL);
    STD_FlangeFacingDim(STD_FlangeFacingDim* obj);
    virtual ~STD_FlangeFacingDim();

private:
    void createMembers();

};

#endif /*STD_FLANGEFACINGDIM_H*/
