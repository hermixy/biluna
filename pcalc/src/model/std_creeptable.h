/*****************************************************************
 * $Id: std_creeptable.h 0001 2016-05-21T09:30:40 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_CREEPTABLE_H
#define STD_CREEPTABLE_H

#include "rb_objectcontainer.h"

/**
 * Creep table, root for creep values. Creep is elongation of material after time
 */
class STD_CreepTable : public RB_ObjectContainer {

public:
    STD_CreepTable(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_CreepTable",
               RB_ObjectFactory* f = NULL);
    STD_CreepTable(STD_CreepTable* obj);
    virtual ~STD_CreepTable();

private:
    void createMembers();

};

#endif /*STD_CREEPTABLE_H*/
