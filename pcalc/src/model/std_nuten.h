/*****************************************************************
 * $Id: std_nuten.h 0001 2016-07-31T15:11:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_NUTEN_H
#define STD_NUTEN_H

#include "rb_objectatomic.h"

/**
 * Dimensions of nuts according EN
 */
class STD_NutEn : public RB_ObjectAtomic {

public:
    STD_NutEn(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_NutEn",
               RB_ObjectFactory* f = NULL);
    STD_NutEn(STD_NutEn* obj);
    virtual ~STD_NutEn();

private:
    void createMembers();

};

#endif /*STD_NUTEN_H*/
