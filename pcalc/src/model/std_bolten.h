/*****************************************************************
 * $Id: std_bolten.h 0001 2016-07-31T15:05:32 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_BOLTEN_H
#define STD_BOLTEN_H

#include "rb_objectatomic.h"

/**
 * Bolt dimensions as per EN standards
 */
class STD_BoltEn : public RB_ObjectAtomic {

public:
    STD_BoltEn(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_BoltEn",
               RB_ObjectFactory* f = NULL);
    STD_BoltEn(STD_BoltEn* obj);
    virtual ~STD_BoltEn();

private:
    void createMembers();

};

#endif /*STD_BOLTEN_H*/
