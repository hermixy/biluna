/*****************************************************************
 * $Id: std_thermexptable.h 0001 2016-05-21T09:36:30 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_THERMEXPTABLE_H
#define STD_THERMEXPTABLE_H

#include "rb_objectcontainer.h"

/**
 * Thermal expansion table, root of expansion values
 */
class STD_ThermExpTable : public RB_ObjectContainer {

public:
    STD_ThermExpTable(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_ThermExpTable",
               RB_ObjectFactory* f = NULL);
    STD_ThermExpTable(STD_ThermExpTable* obj);
    virtual ~STD_ThermExpTable();

private:
    void createMembers();

};

#endif /*STD_THERMEXPTABLE_H*/
