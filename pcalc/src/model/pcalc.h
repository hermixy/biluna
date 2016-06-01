/*****************************************************************
 * $Id: peng.h 2155 2014-08-04 16:50:28Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_H
#define PCALC_H

#include "rb_string.h"

/**
 * Class namespace for various enums of equipment types, piping component types,
 * flags, etc.
 */
class PCALC2 {
public:

    /** Type of flange for GUI */
    enum FlangeType {
        FlangeBlind,
        FlangeIntegral,
        FlangeLoose
    };

    /** Material class */
    enum MatClass {
        MatClassNone,
        MatClassCarbonSteel,
        MatClassHighAlloy,
        MatClassLowAlloy,
        MatClassStainlessSteel,
        MatClassNonFerrous,
        MatClassDefault
    };

    /** Material type */
    enum MatType {
        MatTypeNone,
        MatTypeBar,
        MatTypeBolting,
        MatTypeCasting,
        MatTypeFitting,
        MatTypeForging,
        MatTypeNut,
        MatTypePipe,
        MatTypePlate,
        MatTypeRod,
        MatTypeTube,
        MatTypeDefault
    };

    /** Material main structure */
    enum MatStructure {
        MatStructureNone,
        MatStructureFerritic,
        MatStructureAustenitic,
        MatStructureMartensitic,
        MatStructureAusteniticFerritic,
        MatStructureNonFerrous,
        MatStructureOther
    };

    static double MPaToBar (double pressureMPa) {
        return 10.0 * pressureMPa;
    }
    
    static double BarToMPa (double pressureBar) {
        return 0.1 * pressureBar;
    }
};

#endif /*PCALC_H*/
