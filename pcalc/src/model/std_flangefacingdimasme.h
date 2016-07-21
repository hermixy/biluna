/*****************************************************************
 * $Id: std_flangefacingdimasme.h 0001 2016-07-21T14:19:46 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_FLANGEFACINGDIMASME_H
#define STD_FLANGEFACINGDIMASME_H

#include "rb_objectatomic.h"

/**
 * Flange facing dimensions as per ASME B16.5
 */
class STD_FlangeFacingDimAsme : public RB_ObjectAtomic {

public:
    STD_FlangeFacingDimAsme(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "STD_FlangeFacingDimAsme",
               RB_ObjectFactory* f = NULL);
    STD_FlangeFacingDimAsme(STD_FlangeFacingDimAsme* obj);
    virtual ~STD_FlangeFacingDimAsme();

private:
    void createMembers();

};

#endif /*STD_FLANGEFACINGDIMASME_H*/
