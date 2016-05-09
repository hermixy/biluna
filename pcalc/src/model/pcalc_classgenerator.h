/*****************************************************************
 * $Id: pcalc_classgenerator.h 0001 2016-05-09T08:15:35 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_CLASSGENERATOR_H
#define PCALC_CLASSGENERATOR_H

#include "rb_objectcontainer.h"

/**
 * Data class for code calculation generator and parent of code formulae
 */
class PCALC_ClassGenerator : public RB_ObjectContainer {

public:
    PCALC_ClassGenerator(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_ClassGenerator(PCALC_ClassGenerator* obj);
    virtual ~PCALC_ClassGenerator();

private:
    void createMembers();

};

#endif /*PCALC_CLASSGENERATOR_H*/
