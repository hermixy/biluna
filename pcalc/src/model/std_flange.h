/*****************************************************************
 * $Id: std_flange.h 0001 2016-07-07T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGE_H
#define STD_FLANGE_H

#include "rb_objectatomic.h"

/**
 * Flange dimensions, based on EN1092-1
 */
class STD_Flange : public RB_ObjectAtomic {

public:
    STD_Flange(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Flange",
               RB_ObjectFactory* f = NULL);
    STD_Flange(STD_Flange* obj);
    virtual ~STD_Flange();

private:
    void createMembers();

};

#endif /*STD_FLANGE_H*/
