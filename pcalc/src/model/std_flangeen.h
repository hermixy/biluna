/*****************************************************************
 * $Id: std_flangeen.h 0001 2016-07-07T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEEN_H
#define STD_FLANGEEN_H

#include "rb_objectatomic.h"

/**
 * Flange dimensions, based on EN1092-1
 */
class STD_FlangeEn : public RB_ObjectAtomic {

public:
    STD_FlangeEn(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeEn",
               RB_ObjectFactory* f = NULL);
    STD_FlangeEn(STD_FlangeEn* obj);
    virtual ~STD_FlangeEn();

private:
    void createMembers();

};

#endif /*STD_FLANGEen_H*/
