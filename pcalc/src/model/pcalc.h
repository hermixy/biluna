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
    /**
     * Component type which has a relation to the PCC2 pipe class component type
     */
    enum CompType {
        CompNone = 0,       	/** Invalid component type. */

        CompCap = 1,			/** Cap or head */
        CompCylinder = 2,		/** Cylinder or cone, pipe or shell */
        CompElbow = 3,          /** Elbow */
        CompFlange = 4,			/** Flange */
        CompHalfSphere = 5,		/** Half sphere or sphere head */
        CompOlet = 6,			/** Weldolet, threadolet, nipolet, sockolet */
        CompTee = 7,			/** Tee */

        CompDefault             /** All or default component type */
    };

    /** EN1591 flange types for GUI */
    enum FlangeType {
        FlangeBlind,
        FlangeIntegral,
        FlangeLoose
    };

    static double MPaToBar (double pressureMPa) {
        return 10.0 * pressureMPa;
    }
    
    static double BarToMPa (double pressureBar) {
        return 0.1 * pressureBar;
    }
};

#endif /*PCALC_H*/
