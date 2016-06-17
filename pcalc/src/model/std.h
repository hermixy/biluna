/*****************************************************************
 * $Id: peng.h 2155 2016-06-17 16:50:28Z rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef STD_H
#define STD_H


/**
 * Class namespace for various enums of material types, properties etc.
 */
class STD2 {
public:

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

};

#endif /*STD_H*/
