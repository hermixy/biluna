/*****************************************************************
 * $Id: std_flangefacingtype.h 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEFACINGTYPE_H
#define STD_FLANGEFACINGTYPE_H

#include "rb_objectatomic.h"

/**
 * Flange facing type as per parent dimension standard
 */
class STD_FlangeFacingType : public RB_ObjectAtomic {

public:
    STD_FlangeFacingType(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeFacingType",
               RB_ObjectFactory* f = NULL);
    STD_FlangeFacingType(STD_FlangeFacingType* obj);
    virtual ~STD_FlangeFacingType();

private:
    void createMembers();

};

#endif /*STD_FLANGEFACINGTYPE_H*/
