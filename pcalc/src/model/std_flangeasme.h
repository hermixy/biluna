/*****************************************************************
 * $Id: std_flangeasme.h 0001 2016-07-21T14:41:36 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEASME_H
#define STD_FLANGEASME_H

#include "rb_objectatomic.h"

/**
 * Flange dimension as per ASME B16.5
 */
class STD_FlangeAsme : public RB_ObjectAtomic {

public:
    STD_FlangeAsme(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeAsme",
               RB_ObjectFactory* f = NULL);
    STD_FlangeAsme(STD_FlangeAsme* obj);
    virtual ~STD_FlangeAsme();

private:
    void createMembers();

};

#endif /*STD_FLANGEASME_H*/
