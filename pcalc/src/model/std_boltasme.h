/*****************************************************************
 * $Id: std_boltasme.h 0001 2016-07-26T20:39:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_BOLTASME_H
#define STD_BOLTASME_H

#include "rb_objectatomic.h"

/**
 * Dimensions of ASME bolts as per standard such as ASME B1.1 UNC
 */
class STD_BoltAsme : public RB_ObjectAtomic {

public:
    STD_BoltAsme(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_BoltAsme",
               RB_ObjectFactory* f = NULL);
    STD_BoltAsme(STD_BoltAsme* obj);
    virtual ~STD_BoltAsme();

private:
    void createMembers();

};

#endif /*STD_BOLTASME_H*/
