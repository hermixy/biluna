/*****************************************************************
 * $Id: peng.h 2155 2014-08-04 16:50:28Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_H
#define PENG_H

#include "rb_string.h"

/**
 * Class namespace for various enums of equipment types, piping component types,
 * flags, etc.
 */
class PENG2 {
public:


    /**
     * Type of action being executed by the application 
     */
    enum ActionType {
        ActionNone,                         /**< Invalid or no action */

        ActionBase,                         /** Base component type action */
        ActionNozzleWN,                     /** WN Nozzle complex type action */

        ActionDefault                       /** Default or all actions */
    };


    /**
     * Component type which has a relation to the PCC2 pipe class component type
     */
    enum CompType {
        CompNone,        					/** Invalid component type. */
    	
    	// Attachments
        CompClip,                           /** Clip for insulation or support */
        CompLug,                            /** Lifting lug */
        CompTrunnion,                       /** Lifting trunnion (nozzle type) */
    	
    	// Base components
        CompBend,							/** Bend or elbow */
        CompBlock,							/** Block, beam or plate */
        CompCone,							/** Cone or reducer */
        CompCylinder,						/** Cylinder, pipe or shell */
        CompDisc,							/** Blind flange, blind, tubesheet */
        CompFlange,							/** Flange */
        CompHalfSphere,						/** Half sphere or sphere head */
        CompHead,							/** Head or cap */
        CompOlet,							/** Weldolet, threadolet, nipolet, sockolet */
        CompRing,							/** Ring such as orifice plate */
        CompTee,							/** Tee */
        CompValve,							/** Valve */

    	// Cover, flat
        CompCoverRB,                        /** Round bolted cover*/
        CompCoverRW,                        /** Round welded cover*/
        CompCoverSB,                        /** Square bolted cover*/
        CompCoverSW,                        /** Square welded cover*/

    	// Cylinder
        CompChannel,                        /** Channel of a heat exchanger */
        CompConeConcentric,                 /** Cone concentric */
        CompConeEccentric,                  /** Cone eccentric */
        CompShell,                          /** Shell of a vessel or a heat exchanger */
    	
    	// Flange
        CompHubFlange,                      /** Hub flange */
        CompIntegralFlange,                 /** Integral flange */
        CompLapJoint,                       /** Lap joint flange */
        CompLongWeldNeckFlange,             /** Long weld neck flange */
        CompLooseFlange,                    /** Loose flange */
        CompWeldNeckFlange,                 /** Weld neck flange */
    	
    	// Head
        CompElliptical,                     /** Elliptical head */
        CompHemispherical,                  /** Hemispherical head */
        CompKlopperform,                    /** Klopperform head */
        CompKorbogen,                       /** Korbogen head */
    	
    	// Internal plates and specials
        CompBaffleFull,                     /** Full baffle */
        CompBaffleLeft,                     /** Baffle left cut */
        CompBaffleRight,                    /** Baffle right cut */
        CompBaffleTwoSegmentalInside,       /** 2 segmental baffle (inside cut) */
        CompBaffleTwoSegmentalOutside,      /** 2 segmental baffle (outsid cut) */
        CompCircular,                       /** Circular internal ring */
        CompTubesheet,                      /** Tubesheet */
    	
    	// Nozzle components
        CompManhole20,                      /** Manhole 20 inch */
        CompManhole24,                      /** Manhole 24 inch */
        CompNozzleLJ,                       /** Lap Joint nozzle */
        CompNozzleLWN,                      /** Long Weld Neck nozzle */
        CompNozzleSO,                       /** Slip On nozzle */
        CompNozzleWE,                       /** Weld End nozzle */
        CompNozzleWN,                       /** weld neck nozzle, pipe with weld neck flange, default unreinforced */
    	
    	// Piping components
        CompCapWE,                          /** Cap, weld ends */
        CompElbowWE,                        /** Elbow weld ends */
        CompFlangeBlind,                    /** Blind flange */
        CompFlangeWN,                       /** Weld neck flange */
        CompOletWE,                         /** Weldolet (weld ends) */
        CompOrificePlate,                   /** Orifice plate */
        CompTeeWE,                          /** Welded Tee */
        CompValveFL,                        /** Flanged valve */
    	
    	// Platforms
        CompLadderNoCage,                   /** Ladder without cage */
        CompLadderWithCage,                 /** Ladder with cage */
        CompPlatformCircular090,            /** Circular platform 90 degree */
        CompPlatformCircular180,            /** Circular platform 180 degree */
        CompPlatformCircular270,            /** Circular platform 270 degree */
        CompPlatformRectangular,            /** Rectangular platform, usually top platform */
    	
    	// Rings
        CompVacuumRing,                     /** Outside vacuum ring */
    	
    	// Supports
        CompLegFourBeam,                    /** Four beam legs */
        CompLegFourTube,                    /** Four tube legs */
        CompSaddle,                         /** Saddle */
        CompSkirtCone,                      /** Cone skirt */
        CompSkirtCylinder,                  /** Cylindrical skirt */

        CompDefault     					/** Default or all components */
    };


    /**
     *  Main direction of equipment
     */
    enum Direction {
        DirNone,                            /** Invalid equipment direction */

        DirHorizontal,                      /** Horizontal */
        DirVertical,                        /** Vertical */

        DirDefault                          /** Default or undefined direction */
    };


    /**
     *  Type of equipment
     */
    enum EqType {
        EqNone,                             /** Invalid equipment type. */

        EqColumn,                           /** Column or tower */
        EqFilter,                           /** Filter */
        EqHorizontalVessel,                 /** Horizontal vessel or drum */
        EqVerticalVessel,                   /** Vertical vessel or drum */
        EqStorageTank,                      /** Storage tank */

        EqCentrifugalPumpTopTop             /** Centrifugal pump top suction top discharge */
    };

    /**
     * Setting category, used for settings dialog to order
     * setting type is categories
     */
    enum SettingCategory {
        SettingCatNone = 0,                 /** Invalid setting category */
        SettingCatCustom = 1,               /** Custom field setting category */
        SettingCatDefault                   /** Default setting category */
    };

    enum SettingType {
        SettingNone = 0,                    /** Invalid setting type */
        SettingCustomLabel1 = 1,            /** Custom field lable 1 */
        SettingCustomLabel2 = 2,            /** Custom field lable 2 */
        SettingCustomLabel3 = 3,            /** Custom field lable 3 */
        SettingCustomLabel4 = 4,            /** Custom field lable 4 */
        SettingCustomLabel5 = 5,            /** Custom field lable 5 */
        SettingCustomLabel6 = 6,            /** Custom field lable 6 */
        SettingCustomLabel7 = 7,            /** Custom field lable 7 */
        SettingCustomLabel8 = 8,            /** Custom field lable 8 */
        SettingCustomLabel9 = 9,            /** Custom field lable 9 */
        SettingCustomLabel10 = 10,          /** Custom field lable 10 */
        SettingDefault                      /** Default setting type */
    };

    /** Map component type to string (component type name) */
	static RB_String compToString(PENG2::CompType compType) {
		switch (compType) {
	    	// Attachments
			case PENG2::CompClip : return "Clip"; break;
			case PENG2::CompLug : return "Lifting Lug"; break;
			case PENG2::CompTrunnion : return "Lifting Trunnion"; break;
    		// Base components
			case PENG2::CompBend : return "Bend"; break;		
			case PENG2::CompBlock : return "Block"; break;	
			case PENG2::CompCone : return "Cone"; break;		
			case PENG2::CompCylinder : return "Cylinder"; break;		
			case PENG2::CompDisc : return "Disc"; break;	
			case PENG2::CompFlange : return "Flange"; break;		
			case PENG2::CompHalfSphere : return "Half Sphere"; break;		
			case PENG2::CompHead : return "Head"; break;	
			case PENG2::CompOlet : return "Olet"; break;		
			case PENG2::CompRing : return "Ring"; break;		
			case PENG2::CompTee : return "Tee"; break;	
			case PENG2::CompValve : return "Valve"; break;		
	    	// Cover
			case PENG2::CompCoverRB : return "Round Bolted Cover"; break;
			case PENG2::CompCoverRW : return "Round Welded Cover"; break;
			case PENG2::CompCoverSB : return "Square Bolted Cover"; break;
			case PENG2::CompCoverSW : return "Square Welded Cover"; break;
	    	// Cylinder
			case PENG2::CompChannel : return "Channel HE"; break;
			case PENG2::CompConeConcentric : return "Cone Concentric"; break;
			case PENG2::CompConeEccentric : return "Cone Eccentric"; break;
			case PENG2::CompShell : return "Shell"; break;
	    	// Flange
			case PENG2::CompHubFlange : return "Hub Flange"; break;
			case PENG2::CompIntegralFlange : return "Integral Flange"; break;
			case PENG2::CompLapJoint : return "Lap Joint Flange"; break;
			case PENG2::CompLongWeldNeckFlange : return "LWN Flange"; break;
			case PENG2::CompLooseFlange : return "Loose Flange"; break;
	    	case PENG2::CompWeldNeckFlange : return "WN Flange"; break;
	    	// Head
	    	case PENG2::CompElliptical : return "Elliptical Head"; break;
	    	case PENG2::CompHemispherical : return "Hemispherical Head"; break;
	    	case PENG2::CompKlopperform : return "Klopperform Head"; break;
	    	case PENG2::CompKorbogen : return "Korbogen Head"; break;
	    	// Internal plates and specials
	    	case PENG2::CompBaffleFull : return "Full Baffle"; break;
	    	case PENG2::CompBaffleLeft : return "Baffle Left-cut"; break;
	    	case PENG2::CompBaffleRight : return "Baffle Right-cut"; break;
	    	case PENG2::CompBaffleTwoSegmentalInside : return "Two Segmental Inside-Cut"; break;
	    	case PENG2::CompBaffleTwoSegmentalOutside : return "Two Segmental Outside-Cut"; break;
	    	case PENG2::CompCircular : return "Circular Ring"; break;
	    	case PENG2::CompTubesheet : return "Tubesheet"; break;
	    	// Nozzle
	    	case PENG2::CompManhole20 : return "Manhole 20 inch"; break;
	    	case PENG2::CompManhole24 : return "Manhole 24 inch"; break;
	    	case PENG2::CompNozzleLJ : return "Lap Joint Nozzle"; break;
	    	case PENG2::CompNozzleLWN : return "LWN Nozzle"; break;
	    	case PENG2::CompNozzleSO : return "Slip-on Nozzle"; break;
	    	case PENG2::CompNozzleWE : return "Weld End Nozzle"; break;
	    	case PENG2::CompNozzleWN : return "WN Nozzle"; break;
	    	// Piping
	    	case PENG2::CompCapWE : return "Cap WE"; break;
	    	case PENG2::CompElbowWE : return "Elbow WE"; break;
	    	case PENG2::CompFlangeBlind : return "Blind Flange"; break;
	    	case PENG2::CompFlangeWN : return "WN Flange"; break;
	    	case PENG2::CompOletWE : return "Weldolet"; break;
	    	case PENG2::CompOrificePlate : return "Orifice Plate"; break;
	    	case PENG2::CompTeeWE : return "Tee WE"; break;
	    	case PENG2::CompValveFL : return "Flanged Valve"; break;
	    	// Platforms
	    	case PENG2::CompLadderNoCage : return "Ladder without Cage"; break;
	    	case PENG2::CompLadderWithCage : return "Ladder with Cage"; break;
	    	case PENG2::CompPlatformCircular090 : return "Circular Platform 90 degree"; break;
	    	case PENG2::CompPlatformCircular180 : return "Circular Platform 180 degree"; break;
	    	case PENG2::CompPlatformCircular270 : return "Circular Platform 270 degree"; break;
	    	case PENG2::CompPlatformRectangular : return "Rectangular Platform"; break;
	    	// Rings
	    	case PENG2::CompVacuumRing : return "Vacuum Ring"; break;
	    	// Supports
	    	case PENG2::CompLegFourBeam : return "Four Beam Leg"; break;
	    	case PENG2::CompLegFourTube : return "Four Tube Leg"; break;
	    	case PENG2::CompSaddle : return "Saddle"; break;
	    	case PENG2::CompSkirtCone : return "Cone Skirt"; break;
	    	case PENG2::CompSkirtCylinder : return "Cylinder Skirt"; break;
			
			default :
				break;
		}
		
		return "none";
	}
	
	/** Map string to component type */
	static PENG2::CompType stringToComp(const RB_String& strComp) {
    	// Attachments
		if (strComp == "Clip") return PENG2::CompClip;
    	else if (strComp == "Lifting Lug") return PENG2::CompLug;
    	else if (strComp == "Lifting Trunnion") return PENG2::CompTrunnion;
		// Base components
    	else if (strComp == "Bend") return PENG2::CompBend;
		else if (strComp == "Block") return PENG2::CompBlock;
		else if (strComp == "Cone") return PENG2::CompCone;
		else if (strComp == "Cylinder") return PENG2::CompCylinder;
		else if (strComp == "Disc") return PENG2::CompDisc;
		else if (strComp == "Flange") return PENG2::CompFlange;
		else if (strComp == "Half Sphere") return PENG2::CompHalfSphere;
		else if (strComp == "Head") return PENG2::CompHead;
		else if (strComp == "Olet") return PENG2::CompOlet;
		else if (strComp == "Ring") return PENG2::CompRing;
		else if (strComp == "Tee") return PENG2::CompTee;
		else if (strComp == "Valve") return PENG2::CompValve;
    	// Cover, flat
		else if (strComp == "Round Bolted Cover") return PENG2::CompCoverRB;
    	else if (strComp == "Round Welded Cover") return PENG2::CompCoverRW;
    	else if (strComp == "Square Bolted Cover") return PENG2::CompCoverSB;
    	else if (strComp == "Square Welded Cover") return PENG2::CompCoverSW;
    	// Cylinder
    	else if (strComp == "Channel HE") return PENG2::CompChannel;
    	else if (strComp == "Cone Concentric") return PENG2::CompConeConcentric;
    	else if (strComp == "Cone Eccentric") return PENG2::CompConeEccentric;
    	else if (strComp == "Shell") return PENG2::CompShell;
    	// Flange
    	else if (strComp == "Hub Flange") return PENG2::CompHubFlange;
    	else if (strComp == "Integral Flange") return PENG2::CompIntegralFlange;
    	else if (strComp == "Lap Joint Flange") return PENG2::CompLapJoint;
    	else if (strComp == "LWN Flange") return PENG2::CompLongWeldNeckFlange;
    	else if (strComp == "Loose Flange") return PENG2::CompLooseFlange;
    	else if (strComp == "WN Flange") return PENG2::CompWeldNeckFlange;
    	// Head
    	else if (strComp == "Elliptical Head") return PENG2::CompElliptical;
    	else if (strComp == "Hemispherical Head") return PENG2::CompHemispherical;
    	else if (strComp == "Klopperform Head") return PENG2::CompKlopperform;
    	else if (strComp == "Korbogen Head") return PENG2::CompKorbogen;
    	// Internal plates and specials
    	else if (strComp == "Full Baffle") return PENG2::CompBaffleFull;
    	else if (strComp == "Baffle Left-Cut") return PENG2::CompBaffleLeft;
    	else if (strComp == "Baffle Right-Cut") return PENG2::CompBaffleRight;
    	else if (strComp == "Two Segmental Inside-Cut") return PENG2::CompBaffleTwoSegmentalInside;
    	else if (strComp == "Two Segmental Outside-Cut") return PENG2::CompBaffleTwoSegmentalOutside;
    	else if (strComp == "Circular Ring") return PENG2::CompCircular;
    	else if (strComp == "Tubesheet") return PENG2::CompTubesheet;
    	// Nozzle components
    	else if (strComp == "Manhole 20 inch") return PENG2::CompManhole20;
    	else if (strComp == "Manhole 24 inch") return PENG2::CompManhole24;
    	else if (strComp == "Lap Joint Nozzle") return PENG2::CompNozzleLJ;
    	else if (strComp == "LWN Nozzle") return PENG2::CompNozzleLWN;
    	else if (strComp == "Slip-on Nozzle") return PENG2::CompNozzleSO;
    	else if (strComp == "Weld End Nozzle") return PENG2::CompNozzleWE;
    	else if (strComp == "WN Nozzle") return PENG2::CompNozzleWN;
    	// Piping components
    	else if (strComp == "Cap WE") return PENG2::CompCapWE;
    	else if (strComp == "Elbow WE") return PENG2::CompElbowWE;
    	else if (strComp == "Blind Flange") return PENG2::CompFlangeBlind;
    	else if (strComp == "WN Flange") return PENG2::CompFlangeWN;
    	else if (strComp == "Weldolet") return PENG2::CompOletWE;
    	else if (strComp == "Orifice Plate") return PENG2::CompOrificePlate;
    	else if (strComp == "Tee WE") return PENG2::CompTeeWE;
    	else if (strComp == "Flanged Valve") return PENG2::CompValveFL;
    	// Platforms
    	else if (strComp == "Ladder without Cage") return PENG2::CompLadderNoCage;
    	else if (strComp == "Ladder with Cage") return PENG2::CompLadderWithCage;
    	else if (strComp == "Circular Platform 90 degree") return PENG2::CompPlatformCircular090;
    	else if (strComp == "Circular Platform 180 degree") return PENG2::CompPlatformCircular180;
    	else if (strComp == "Circular Platform 270 degree") return PENG2::CompPlatformCircular270;
    	else if (strComp == "Rectangular Platform") return PENG2::CompPlatformRectangular;
    	// Rings
    	else if (strComp == "Vacuum Ring") return PENG2::CompVacuumRing;
    	// Supports
    	else if (strComp == "Four Beam Leg") return PENG2::CompLegFourBeam;
    	else if (strComp == "Four Tube Leg") return PENG2::CompLegFourTube;
    	else if (strComp == "Saddle") return PENG2::CompSaddle;
    	else if (strComp == "Cone Skirt") return PENG2::CompSkirtCone;
    	else if (strComp == "Cylindrical Skirt") return PENG2::CompSkirtCylinder;
		
		return PENG2::CompNone;
	}

	/**
     * line sizes in rows for insulation tables
     */
//    enum LineSize {
//        /** Flag for Undoables. */
//        FlagUndone      = 1<<0,
//        /** Entity Visibility. */
//        FlagVisible     = 1<<1,
//        /** Entity attribute (e.g. color) is defined by layer. */
//        FlagByLayer     = 1<<2,
//        /** Entity attribute (e.g. color) defined by block. */
//        FlagByBlock     = 1<<3,
//        /** Layer frozen. */
//        FlagFrozen      = 1<<4,
//        /** Layer frozen by default. */
//        FlagDefFrozen   = 1<<5,
//        /** Layer locked. */
//        FlagLocked      = 1<<6,
//        /** Used for invalid pens. */
//        FlagInvalid     = 1<<7,
//        /** Entity in current selection. */
//        FlagSelected    = 1<<8,
//        /** Polyline closed? */
//        FlagClosed      = 1<<9,
//        /** Flag for temporary entities (e.g. hatch) */
//        FlagTemp        = 1<<10,
//        /** Flag for processed entities (optcontour) */
//        FlagProcessed   = 1<<11,
//        /** Startpoint selected */
//        FlagSelected1   = 1<<12,
//        /** Endpoint selected */
//        FlagSelected2   = 1<<13,
//		/** Entity is highlighted temporarily (as a user action feedback) */
//		FlagHighlighted = 1<<14
//    }; // only enum end with semicolon 

	
    
};

#endif /*PENG_H*/
