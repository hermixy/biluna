/*****************************************************************
 * $Id: std_material.h 0001 2016-05-21T09:15:12 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_MATERIAL_H
#define STD_MATERIAL_H

#include "rb_objectcontainer.h"

/**
 * Material of construction, root for all material properties. Currently only metals.
 */
class STD_Material : public RB_ObjectContainer {

public:
    STD_Material(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_Material",
               RB_ObjectFactory* f = NULL);
    STD_Material(STD_Material* obj);
    virtual ~STD_Material();

private:
    void createMembers();

};

#endif /*STD_MATERIAL_H*/
