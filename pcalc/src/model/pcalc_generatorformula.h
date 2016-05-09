/*****************************************************************
 * $Id: pcalc_generatorformula.h 0001 2016-05-09T08:49:41 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef PCALC_GENERATORFORMULA_H
#define PCALC_GENERATORFORMULA_H

#include "rb_objectatomic.h"

/**
 * Formula for calculation generator class
 */
class DB_EXPORT PCALC_GeneratorFormula : public RB_ObjectAtomic {

public:
    PCALC_GeneratorFormula(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = "", RB_ObjectFactory* f = NULL);
    PCALC_GeneratorFormula(PCALC_GeneratorFormula* obj);
    virtual ~PCALC_GeneratorFormula();

private:
    void createMembers();

};

#endif /*PCALC_GENERATORFORMULA_H*/
