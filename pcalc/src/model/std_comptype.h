/*****************************************************************
 * $Id: std_comptype.h 0001 2016-07-13T15:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_COMPTYPE_H
#define STD_COMPTYPE_H

#include "rb_objectcontainer.h"

/**
 * Component type such as flange type as defined
 * in the parent dimension standard
 */
class STD_CompType : public RB_ObjectContainer {

public:
    STD_CompType(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_CompType",
               RB_ObjectFactory* f = NULL);
    STD_CompType(STD_CompType* obj);
    virtual ~STD_CompType();

private:
    void createMembers();

};

#endif /*STD_COMPTYPE_H*/
