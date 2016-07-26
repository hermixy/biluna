/*****************************************************************
 * $Id: std_nutasme.h 0001 2016-07-26T20:48:47 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_NUTASME_H
#define STD_NUTASME_H

#include "rb_objectatomic.h"

/**
 * Dimensions of nut as per ASME such as ASME B18.2.2 heavy hex nuts
 */
class STD_NutAsme : public RB_ObjectAtomic {

public:
    STD_NutAsme(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_NutAsme",
               RB_ObjectFactory* f = NULL);
    STD_NutAsme(STD_NutAsme* obj);
    virtual ~STD_NutAsme();

private:
    void createMembers();

};

#endif /*STD_NUTASME_H*/
