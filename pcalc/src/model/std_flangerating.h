/*****************************************************************
 * $Id: std_flangerating.h 0001 2016-07-14T14:22:22 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGERATING_H
#define STD_FLANGERATING_H

#include "rb_objectcontainer.h"

/**
 * Flange rating based on parent model flange type
 */
class STD_FlangeRating : public RB_ObjectContainer {

public:
    STD_FlangeRating(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeRating",
               RB_ObjectFactory* f = NULL);
    STD_FlangeRating(STD_FlangeRating* obj);
    virtual ~STD_FlangeRating();

private:
    void createMembers();

};

#endif /*STD_FLANGERATING_H*/
