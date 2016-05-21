/*****************************************************************
 * $Id: std_elasmodultable.h 0001 2016-05-21T09:33:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_ELASMODULTABLE_H
#define STD_ELASMODULTABLE_H

#include "rb_objectcontainer.h"

/**
 * Modulus of elasticity table, root elasticity values
 */
class STD_ElasModulTable : public RB_ObjectContainer {

public:
    STD_ElasModulTable(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_ElasModulTable",
               RB_ObjectFactory* f = NULL);
    STD_ElasModulTable(STD_ElasModulTable* obj);
    virtual ~STD_ElasModulTable();

private:
    void createMembers();

};

#endif /*STD_ELASMODULTABLE_H*/
