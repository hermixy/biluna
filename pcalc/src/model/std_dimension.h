/*****************************************************************
 * $Id: std_dimension.h 0001 2016-07-07T17:02:15 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_DIMENSION_H
#define STD_DIMENSION_H

#include "rb_objectcontainer.h"

/**
 * Dimension standards
 */
class STD_Dimension : public RB_ObjectContainer {

public:
    STD_Dimension(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Dimension",
               RB_ObjectFactory* f = NULL);
    STD_Dimension(STD_Dimension* obj);
    virtual ~STD_Dimension();

private:
    void createMembers();

};

#endif /*STD_DIMENSION_H*/
