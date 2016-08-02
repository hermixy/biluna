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
    /** ASME B16.5 flange facing types */
    enum AsmeFlangeFacingType {
        AsmeFlangeFacingFFS,	/** Small Female Face (small Spigot and Recess)	*/
        AsmeFlangeFacingFMS,	/** Small Male Face (on end of pipe, small Spigot and Recess) */
        AsmeFlangeFacingRF,     /** Raised Face 2 or 7 mm */
        AsmeFlangeFacingRTJ,	/**	Ring (Type) Joint */
        AsmeFlangeFacingSRF,	/**	Spigot and Recess Female (large face) */
        AsmeFlangeFacingSRM,	/**	Spigot and Recess Male (large face) */
        AsmeFlangeFacingTGF,	/**	Tongue and Groove Female */
        AsmeFlangeFacingTGM     /**	Tongue and Groove Male */
    };

    /** ASME B16.5 flange types */
    enum AsmeFlangeType {
        AsmeFlangeBLD,          /** Blind */
        AsmeFlangeLPD,          /** Lapped */
        AsmeFlangeSOW,          /** Slip-On Welding */
        AsmeFlangeSW,           /** Socket Welding (NPS 1/2 to 3 Only) */
        AsmeFlangeTHRD,         /** Threaded */
        AsmeFlangeWN            /** Welding Neck */
    };


    /**
     * Component type which has a relation to material allowable stress
     */
    enum CompType {
        CompNone,
        CompBend,
        CompBolt,
        CompCone,
        CompCylinder,
        CompElbow,
        CompFlange,
        CompGasket,
        CompHalfSphere,
        CompHead,
        CompNut,
        CompOlet,
        CompTee,
        CompValve,
        CompWasher,
        CompDefault
    };

    /** EN 1092-1 flange facing types */
    enum EnFlangeFacingType {
        EnFlangeFacingA,        /** Flat face */
        EnFlangeFacingB,        /** Raised face */
        EnFlangeFacingC,        /** Tongue */
        EnFlangeFacingD,        /** Groove */
        EnFlangeFacingE,        /** Spigot */
        EnFlangeFacingF,        /** Recess */
        EnFlangeFacingG,        /** O-ring spigot */
        EnFlangeFacingH         /** O-ring groove */
    };

    /** EN 1092-1 flange types */
    enum EnFlangeType {
        EnFlange01,             /** Plate flange for welding */
        EnFlange0232,           /** Loose plate flange with weld-on collar */
        EnFlange0233,           /** Loose plate flange with lapped pipe end */
        EnFlange0235,           /** Loose plate flange with weld ring neck */
        EnFlange0236,           /** Loose plate flange with pressed collar with long neck */
        EnFlange0237,           /** Loose plate flange with pressed collar */
        EnFlange0434,           /** Loose plate flange with weld-neck collar */
        EnFlange05,             /** Blind flange */
        EnFlange11,             /** Weld-neck flange */
        EnFlange12,             /** Hubbed slip-on flange for welding */
        EnFlange13,             /** Hubbed threaded flange */
        EnFlange21              /** Integral flange */
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
    enum MatStruct {
        MatStructNone,
        MatStructFerritic,
        MatStructAustenitic,
        MatStructMartensitic,
        MatStructAusteniticFerritic,
        MatStructCasting,
        MatStructNonFerrous,
        MatStructDefault
    };

    static double inchToMm (double value) {
        return 25.4 * value;
    }

    /**
     * Convert pipe sizes from NPS Nominal Pipe Size to DN Nominal Diameter
     */
    static int npsToDn(double size) {
        if (size == 0.125) return 6;
        else if (size == 0.25) return 8;
        else if (size == 0.375) return 10;
        else if (size == 0.5) return 15;
        else if (size == 0.75) return 20;
        else if (size == 1) return 25;
        else if (size == 1.125) return 32;
        else if (size == 1.5) return 40;
        else if (size == 2) return 50;
        else if (size == 2.5) return 65;
        else if (size == 3) return 80;
        else if (size == 3.5) return 90;
        else if (size == 4) return 100;
        else if (size == 5) return 125;
        else if (size == 6) return 150;
        else if (size == 8) return 200;
        else if (size == 10) return 250;
        else if (size == 12) return 300;
        else if (size == 14) return 350;
        else if (size == 16) return 400;
        else if (size == 18) return 450;
        else if (size == 20) return 500;
        else if (size == 22) return 550;
        else if (size == 24) return 600;
        else if (size == 26) return 650;
        else if (size == 28) return 700;
        else if (size == 30) return 750;
        else if (size == 32) return 800;
        else if (size == 34) return 850;
        else if (size == 36) return 900;
        else if (size == 38) return 950;
        else if (size == 40) return 1000;
        else if (size == 42) return 1050;
        else if (size == 44) return 1100;
        else if (size == 46) return 1150;
        else if (size == 48) return 1200;
        else if (size == 52) return 1300;
        else if (size == 56) return 1400;
        else if (size == 60) return 1500;
        else if (size == 64) return 1600;
        else if (size == 68) return 1700;
        else if (size == 72) return 1800;
        else if (size == 76) return 1900;
        else if (size == 80) return 2000;
        return -1;
    }

    /**
     * Convert pipe sizes from DN Nominal Diameter to NPS Nominal Pipe Size
     */
    static double dnToNps(int size) {
        if (size == 6) return 0.125;
        else if (size == 8) return 0.25;
        else if (size == 10) return 0.375;
        else if (size == 15) return 0.5;
        else if (size == 20) return 0.75;
        else if (size == 25) return 1;
        else if (size == 32) return 1.125;
        else if (size == 40) return 1.5;
        else if (size == 50) return 2;
        else if (size == 65) return 2.5;
        else if (size == 80) return 3;
        else if (size == 90) return 3.5;
        else if (size == 100) return 4;
        else if (size == 125) return 5;
        else if (size == 150) return 6;
        else if (size == 200) return 8;
        else if (size == 250) return 10;
        else if (size == 300) return 12;
        else if (size == 350) return 14;
        else if (size == 400) return 16;
        else if (size == 450) return 18;
        else if (size == 500) return 20;
        else if (size == 550) return 22;
        else if (size == 600) return 24;
        else if (size == 650) return 26;
        else if (size == 700) return 28;
        else if (size == 750) return 30;
        else if (size == 800) return 32;
        else if (size == 850) return 34;
        else if (size == 900) return 36;
        else if (size == 950) return 38;
        else if (size == 1000) return 40;
        else if (size == 1050) return 42;
        else if (size == 1100) return 44;
        else if (size == 1150) return 46;
        else if (size == 1200) return 48;
        else if (size == 1300) return 52;
        else if (size == 1400) return 56;
        else if (size == 1500) return 60;
        else if (size == 1600) return 64;
        else if (size == 1700) return 68;
        else if (size == 1800) return 72;
        else if (size == 1900) return 76;
        else if (size == 2000) return 80;
        return -1;
    }
};

#endif /*STD_H*/
