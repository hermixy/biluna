/*****************************************************************
 * $Id: std_flangetypelimit.h 0001 2016-07-14T14:22:22 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGETYPELIMIT_H
#define STD_FLANGETYPELIMIT_H

#include "rb_objectatomic.h"

/**
 * Flange size limits based on parent model flange type
 */
class STD_FlangeTypeLimit : public RB_ObjectAtomic {

public:
    STD_FlangeTypeLimit(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeTypeLimit",
               RB_ObjectFactory* f = NULL);
    STD_FlangeTypeLimit(STD_FlangeTypeLimit* obj);
    virtual ~STD_FlangeTypeLimit();

private:
    void createMembers();

};

#endif /*STD_FLANGETYPELIMIT_H*/
