/*****************************************************************
 * $Id: std_flangefacing.h 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEFACING_H
#define STD_FLANGEFACING_H

#include "rb_objectatomic.h"

/**
 * Flange facing type as per parent dimension standard
 */
class STD_FlangeFacing : public RB_ObjectAtomic {

public:
    STD_FlangeFacing(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeFacing",
               RB_ObjectFactory* f = NULL);
    STD_FlangeFacing(STD_FlangeFacing* obj);
    virtual ~STD_FlangeFacing();

private:
    void createMembers();

};

#endif /*STD_FLANGEFACING_H*/
