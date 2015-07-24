/*****************************************************************
 * $Id: peng_determinepedclass.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_DETERMINEPEDCLASS
#define PENG_DETERMINEPEDCLASS

#include "rb_string.h"


/**
 * Operation class to check and update the equipment- and line table
 * as per the PED (Pressure Equipment Directive) specification
 */
class PENG_DeterminePedClass {

public:
    PENG_DeterminePedClass();
    virtual ~PENG_DeterminePedClass();
	
    void execute(const RB_String& type, const RB_String& assess,
                const RB_String& fluidState, const RB_String& fluidGroup,
                double maxPress, double maxTemp, double maxVol, double nomSize,
                const RB_String& addInfo, const RB_String& unstableGas);

    double getPsv() { return mPsv; }
    double getPsdn() { return mPsdn; }
    int getTable() { return mTable; }
    RB_String getCategory() { return mCategory; }
    RB_String getNote() { return mNote; }
    RB_String getModules() { return mModules; }

private:
    void setCategory(double ps, double v, int dn);
    void setModuleAndException();

    double mPsv;
    double mPsdn;
    int mTable;
    RB_String mCategory;
    RB_String mNote;
    RB_String mModules;
};

#endif /*PENG_DETERMINEPEDCLASS*/
