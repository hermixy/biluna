/*****************************************************************
 * $Id: std_compserie.h 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_COMPSERIE_H
#define STD_COMPSERIE_H

#include "rb_objectatomic.h"

/**
 * Component serie such as flange facing type as per parent dimension standard
 */
class STD_CompSerie : public RB_ObjectAtomic {

public:
    STD_CompSerie(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_CompSerie",
               RB_ObjectFactory* f = NULL);
    STD_CompSerie(STD_CompSerie* obj);
    virtual ~STD_CompSerie();

private:
    void createMembers();

};

#endif /*STD_COMPSERIE_H*/
