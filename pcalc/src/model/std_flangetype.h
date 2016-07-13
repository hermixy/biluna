/*****************************************************************
 * $Id: std_flangetype.h 0001 2016-07-13T15:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGETYPE_H
#define STD_FLANGETYPE_H

#include "rb_objectatomic.h"

/**
 * Flange type as defined in the parent dimension standard
 */
class STD_FlangeType : public RB_ObjectAtomic {

public:
    STD_FlangeType(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeType",
               RB_ObjectFactory* f = NULL);
    STD_FlangeType(STD_FlangeType* obj);
    virtual ~STD_FlangeType();

private:
    void createMembers();

};

#endif /*STD_FLANGETYPE_H*/
