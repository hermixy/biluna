/*****************************************************************
 * $Id: gv.h 1973 2013-08-19 15:00:11Z rutger $
 * Created: Apr 7, 2008 8:50:56 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


// NOT USED




#ifndef GV_H
#define GV_H

// Windoze XP can't handle the original MAX/MINDOUBLE's
#define RS_MAXDOUBLE 1.0E+10
#define RS_MINDOUBLE -1.0E+10


/**
 * Class namespace for various enums along with some simple
 * wrapper methods for converting the enums to the Qt
 * counterparts.
 */
class GV2 {
	
public:
	
    /**
     * Action types used by action factories.
     */
    enum ActionType {
    	// old gv will not be necessary in the future
        ActionNone,					/**< Invalid action id. */
        
        ActionAddEquipment,			/**< Add actual equipment symbol */
        ActionAddLine,				/**< Add line as part of a pipe line */
        ActionAddSymbol,			/**< Add symbol as part of a pipe line */
        ActionAddText,				/**< Add undefined text to the drawing */
        
        ActionMoveItem,				/**< Item is selected and moved over the scene */
        ActionSelectItem,			/**< Item can be selected, this is start action */

        ActionDefault,				/**< Default or all action types. */
        
        // snap actions from QCad rs.h
        ActionSnapFree = 11000,
        ActionSnapGrid = 11010,
        ActionSnapEndpoint = 11020,
        ActionSnapOnEntity = 11030,
        ActionSnapCenter = 11040,
        ActionSnapMiddle = 11050,
        ActionSnapDist = 11060,
        ActionSnapIntersection = 11070,
        ActionSnapIntersectionManual = 11080,
        ActionSnapReference = 11090,
        ActionSnapCoordinate = 11100,
        ActionSnapCoordinatePolar = 11110,
        ActionSnapAuto = 11120,

        ActionRestrictNothing = 12000,
        ActionRestrictOrthogonal = 12010,
        ActionRestrictHorizontal = 12020,
        ActionRestrictVertical = 12030
    };
	
    /**
     * Component type which are used in the P&ID's
     */
    enum CompType {
    	CompNone,					/**< Invalid component type. */
    	
    	CompLine,					/**< Atomic drawing type: line. */
    	CompSymbol,					/**< Atomic drawing type: symbol. */
    	
    	CompDefault					/**< Default or all component types. */
    };

    /**
     * Units
     */
    enum Unit {
        None = 0,               /**< No unit (unit from parent) */

        Inch = 1,               /**< Inch */
        Foot = 2,               /**< Foot: 12 Inches */
        Mile = 3,               /**< Mile: 1760 Yards = 1609 m */
        Millimeter = 4,         /**< Millimeter: 0.001m */
        Centimeter = 5,         /**< Centimeter: 0.01m */
        Meter = 6,              /**< Meter */
        Kilometer = 7,          /**< Kilometer: 1000m */
        Microinch = 8,          /**< Microinch: 0.000001 */
        Mil = 9,                /**< Mil = 0.001 Inch*/
        Yard = 10,              /**< Yard: 3 Feet */
        Angstrom = 11,          /**< Angstrom: 10^-10m  */
        Nanometer = 12,         /**< Nanometer: 10^-9m  */
        Micron = 13,            /**< Micron: 10^-6m  */
        Decimeter = 14,         /**< Decimeter: 0.1m */
        Decameter = 15,         /**< Decameter: 10m */
        Hectometer = 16,        /**< Hectometer: 100m */
        Gigameter = 17,         /**< Gigameter: 1000000m */
        Astro = 18,             /**< Astro: 149.6 x 10^9m */
        Lightyear = 19,         /**< Lightyear: 9460731798 x 10^6m */
        Parsec = 20,            /**< Parsec: 30857 x 10^12 */
        Plu = 21,               /**< PLU: Plotter units used in HP/GL. 40plu = 1mm */

        LastUnit = 22           /**< Used to iterate through units */
    };


    /**
     * Format for length values.
     */
    enum LinearFormat {
        /** Scientific (e.g. 2.5E+05) */
        Scientific,
        /** Decimal (e.g. 9.5)*/
        Decimal,
        /** Engineering (e.g. 7' 11.5")*/
        Engineering,
        /** Architectural (e.g. 7'-9 1/8")*/
        Architectural,
        /** Fractional (e.g. 7 9 1/8) */
        Fractional
    };

    /**
     * Angle Units.
     */
    enum AngleUnit {
        Deg,               /**< Degrees */
        Rad,               /**< Radians */
        Gra                /**< Gradians */
    };

    /**
     * Display formats for angles.
     */
    enum AngleFormat {
        /** Degrees with decimal point (e.g. 24.5°) */
        DegreesDecimal,
        /** Degrees, Minutes and Seconds (e.g. 24°30'5'') */
        DegreesMinutesSeconds,
        /** Gradians with decimal point (e.g. 390.5)*/
        Gradians,
        /** Radians with decimal point (e.g. 1.57)*/
        Radians,
        /** Surveyor's units */
        Surveyors
    };

    // End not used


	//
        // Start ex GV2, update coding
	//
	
	
	/**
	 * Component visibility
	 */
	enum CompVisibility {
		/** Hidden */
		CompHidden,
		/** Solid */
		CompSolid,
		/** Transparent */
		CompTransparent,
		/** Wire frame */
		CompWireFrame
	};
	
    /** 
     * Node type: origine, common port node, custom port node, global model node
     */
    enum NodeType {
    	/** Invalid node type */
    	NodeNone,	
    	
    	/** Node of component origine port as parent type, if connect type this
    	 	node is the origine of the line, equipment or plant */
    	NodeOrigine,
    	/** Node of other port than origine port of component */
    	NodePort,
    	/** Node of custom connection (other than standard ports) 
    	    or origine for branch */
    	NodeCustom,
    	/** Global node of the model (not a local component or equipment node) */ 
    	NodeGlobal,
    	/** Node is connected to parent TODO: used? and description is not clear */
    	NodeParent,
    	/** Interconnecting node such as end of pipe line to equipment nozzle */
    	NodeInterconnecting,

    	/** Default node for passing node data around */
    	NodeDefault
    }; // only enum end with semicolon 

    /**
     * Side used for side of a point relativ to an entity
     * (right hand or left hand side)
     */
    enum Side {
        RightHand, LeftHand, NoSide
    };

	//
        // End ex GV2
	//
	
};

#endif /*GV_H*/
