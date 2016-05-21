/*****************************************************************
 * $Id: std_extpresstable.h 0001 2016-05-21T09:42:52 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_EXTPRESSTABLE_H
#define STD_EXTPRESSTABLE_H

#include "rb_objectcontainer.h"

/**
 * Table of external pressures, root of pressure values
 */
class STD_ExtPressTable : public RB_ObjectContainer {

public:
    STD_ExtPressTable(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_ExtPressTable",
               RB_ObjectFactory* f = NULL);
    STD_ExtPressTable(STD_ExtPressTable* obj);
    virtual ~STD_ExtPressTable();

private:
    void createMembers();

};

#endif /*STD_EXTPRESSTABLE_H*/
