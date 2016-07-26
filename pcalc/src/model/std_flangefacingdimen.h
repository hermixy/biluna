/*****************************************************************
 * $Id: std_flangefacingdimen.h 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEFACINGDIMEN_H
#define STD_FLANGEFACINGDIM_H

#include "rb_objectatomic.h"

/**
 * EN flange facing dimensions
 */
class STD_FlangeFacingDimEn : public RB_ObjectAtomic {

public:
    STD_FlangeFacingDimEn(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeFacingDimEn",
               RB_ObjectFactory* f = NULL);
    STD_FlangeFacingDimEn(STD_FlangeFacingDimEn* obj);
    virtual ~STD_FlangeFacingDimEn();

private:
    void createMembers();

};

#endif /*STD_FLANGEFACINGDIMEN_H*/
