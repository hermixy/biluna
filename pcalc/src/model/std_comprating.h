/*****************************************************************
 * $Id: std_comprating.h 0001 2016-07-20T15:54:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_COMPRATING_H
#define STD_COMPRATING_H

#include "rb_objectcontainer.h"

/**
 * Table with all possible Ratings or Schedules for components
 * in this Dimension standard. This object is parent of,
 * for example, the (flange) Component and (flange) Facing
 */
class STD_CompRating : public RB_ObjectContainer {

public:
    STD_CompRating(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_CompRating",
               RB_ObjectFactory* f = NULL);
    STD_CompRating(STD_CompRating* obj);
    virtual ~STD_CompRating();

private:
    void createMembers();

};

#endif /*STD_COMPRATING_H*/
