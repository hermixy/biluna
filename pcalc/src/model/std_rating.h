/*****************************************************************
 * $Id: std_rating.h 0001 2016-07-20T15:54:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_RATING_H
#define STD_RATING_H

#include "rb_objectcontainer.h"

/**
 * Table with all possible Ratings or Schedules for components
 * in this Dimension standard. This object is parent of,
 * for example, the (flange) Component and (flange) Facing
 */
class STD_Rating : public RB_ObjectContainer {

public:
    STD_Rating(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Rating",
               RB_ObjectFactory* f = NULL);
    STD_Rating(STD_Rating* obj);
    virtual ~STD_Rating();

private:
    void createMembers();

};

#endif /*STD_RATING_H*/
