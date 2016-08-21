/****************************************************************************
** $Id: rs.h 10544 2008-04-21 12:47:57Z andrew $
**
** Copyright (C) 2001-2007 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef RS_H
#define RS_H

#include <qnamespace.h>
#include <QPrinter>
#include "cad_global.h"

// Windoze XP can't handle the original MAX/MINDOUBLE's
#define RS_MAXDOUBLE 1.0E+10
#define RS_MINDOUBLE -1.0E+10


/**
 * Class namespace for various enums along with some simple
 * wrapper methods for converting the enums to the Qt
 * counterparts.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS2 {
public:

    /**
     * Flags.
     */
    enum Flags {
        /** Flag for Undoables. */
        FlagUndone      = 1<<0,
        /** Entity Visibility. */
        FlagVisible     = 1<<1,
        /** Entity attribute (e.g. color) is defined by layer. */
        FlagByLayer     = 1<<2,
        /** Entity attribute (e.g. color) defined by block. */
        FlagByBlock     = 1<<3,
        /** Layer frozen. */
        FlagFrozen      = 1<<4,
        /** Layer frozen by default. */
        FlagDefFrozen   = 1<<5,
        /** Layer locked. */
        FlagLocked      = 1<<6,
        /** Used for invalid pens. */
        FlagInvalid     = 1<<7,
        /** Entity in current selection. */
        FlagSelected    = 1<<8,
        /** Polyline closed? */
        FlagClosed      = 1<<9,
        /** Flag for temporary entities (e.g. hatch) */
        FlagTemp        = 1<<10,
        /** Flag for processed entities (optcontour) */
        FlagProcessed   = 1<<11,
        /** Startpoint selected */
        FlagSelected1   = 1<<12,
        /** Endpoint selected */
        FlagSelected2   = 1<<13,
        /** Entity is highlighted temporarily (as a user action feedback) */
        FlagHighlighted = 1<<14,
        /** Entity is selected. Selection is made visible to user. */
        FlagSelectionVisible = 1<<15,
        /** Entity is flagged for removal. */
        FlagRemove = 1<<16
        /** Used for override pens */
        //FlagOverride = 1<<17
    };

    /**
     * Variable types used by RS_VariableDict and RS_Variable.
     */
    enum VariableType {
        VariableString,
        VariableInt,
        VariableDouble,
        VariableVector,
        VariableVoid
    };

    /**
     * File types. Used by file dialogs. Note: the extension might not
     * be enough to distinguish file types.
     */
    enum FormatType {
        FormatUnknown,       /**< Unknown / unsupported format. */
        FormatDxf1,          /**< QCad 1 compatibility DXF format. */
        FormatDxf,           /**< DXF format. 2000. */
        FormatDxf12,         /**< DXF format. R12. */
        FormatCxf,           /**< CAM Expert Font format. */
        FormatCam,           /**< CAM Expert CAM format (NC, CNC, D, ..) */
        FormatRedf,          /**< REDF format for RWBE */
        FormatDxf3d,         /**< 3D DXF with 3d faces for walls, windows, doors, ... */
        FormatDxfArch,       /**< DXF with meta data for architecture extensions */
        FormatFlt,           /**< OpenFlight FLT format */
        FormatDae,           /**< Collada DAE format */
        FormatNif,           /**< Gamebryo NIF format */
        FormatSvg,           /**< SVG format */
        Format3ds,           /**< 3DS format */
        FormatRwbe,          /**< RWBE project format */
        FormatShp,           /**< ESRI shape file format */

        FormatLast           /**< Used for iteration */
    };

    /**
     * Entity types returned by the rtti() method
     */
    enum EntityType {
        EntityUnknown,      /**< Unknown */
        EntityContainer,    /**< Container */
        EntityDocument,     /**< Document */
        EntityPreview,      /**< Container with preview */
        EntityBlock,        /**< Block (Group definition) */
        EntityFontChar,     /**< Font character */
        EntityInsert,       /**< Insert (Group instance) */
        EntityGraphic,      /**< Graphic with layers */
        EntityPoint,        /**< Point */
        EntityLine,         /**< Line */
        EntityPolyline,     /**< Polyline */
        EntityVertex,       /**< Vertex (part of a polyline) */
        EntityArc,          /**< Arc */
        EntityCircle,       /**< Circle */
        EntityEllipse,      /**< Ellipse */
        EntitySolid,        /**< Solid */
        Entity3dFace,       /**< 3d Face */
        EntityConstructionLine, /**< Construction line */
        EntityText,         /**< Text */
        EntityDimension,    /**< Any Dimension */
        EntityDimAligned,   /**< Aligned Dimension */
        EntityDimLinear,    /**< Linear Dimension */
        EntityDimRadial,    /**< Radial Dimension */
        EntityDimDiametric, /**< Diametric Dimension */
        EntityDimAngular,   /**< Angular Dimension */
        EntityDimLeader,    /**< Leader Dimension */
        EntityDimOrdinate,  /**< Ordinate Dimension */
        EntityHatch,        /**< Hatch */
        EntityImage,        /**< Image */
        EntitySpline,       /**< Spline */
        EntityUser          /**< User defined entity. Use this to identify 
                                 entities that are added in a separate library
                                 and resort to C++ RTTI for RTTI. */

#if 0
        EntityWall,         /**< Wall for the QCadArch module */
        EntityAperture,     /**< Aperture for the QCadArch module */
        EntityWindow,       /**< Window for the QCadArch module */
        EntityDoor,         /**< Door for the QCadArch module */
        EntitySlab,         /**< Slab (floor, ceiling) for QCadArch module */
        EntityStairs,       /**< Flight of stairs for QCadArch module */
        EntityRoof,         /**< Roof for QCadArch module */
        EntitySetDressing,  /**< Set dressing item */
	EntityColumn,
	EntityBeam,
	EntityLight
#endif
    };

    
    /**
     * Action types used by action factories.
     */
    enum ActionType {
        /** Invalid action id. */
        /*
        ActionNone = 0,
        ActionBack = 10,
        ActionForward = 20,
        ActionDefault = 30,
        ActionReset = 40,

        ActionFileNew = 1000,
        ActionFileOpen = 1010,
        ActionFileSave = 1020,
        ActionFileSaveAs = 1030,
        ActionFileExport = 1040,
        ActionFileClose = 1050,
        ActionFilePrint = 1060,
        ActionFilePrintPreview = 1070,
        ActionFilePdf = 1080,
        ActionFileSvg = 1090,
        ActionFileQuit = 1100,
        
        ActionEditUndo = 2000,
        ActionEditRedo = 2010,
        ActionEditCut = 2020,
        ActionEditCutNoSelect = 2030,
        ActionEditCutAutoRef = 2040,
        ActionEditCutAutoRefNoSelect = 2050,
        ActionEditCopy = 2060,
        ActionEditCopyNoSelect = 2070,
        ActionEditCopyAutoRef = 2080,
        ActionEditCopyAutoRefNoSelect = 2090,
        ActionEditPaste = 2100,

        ActionViewStatusBar = 3000,
        ActionViewLayerList = 3010,
        ActionViewBlockList = 3020,
        ActionViewCommandLine = 3030,
        ActionViewOptionToolbar = 3040,
        ActionViewGrid = 3050,
        ActionViewDraft = 3060,

        ActionZoomIn = 4000,
        ActionZoomOut = 4010,
        ActionZoomAuto = 4020,
        ActionZoomAutoY = 4030,
        ActionZoomWindow = 4040,
        ActionZoomPan = 4050,
        ActionZoomRedraw = 4060,
        ActionZoomScroll = 4070,
        ActionZoomPrevious = 4080,

        ActionSelectSingle = 5000,
        ActionSelectContour = 5010,
        ActionSelectWindow = 5020,
        ActionDeselectWindow = 5030,
        ActionSelectAll = 5040,
        ActionDeselectAll = 5050,
        ActionSelectIntersected = 5060,
        ActionDeselectIntersected = 5070,
        ActionSelectInvert = 5080,
        ActionSelectLayer = 5090,
        ActionSelectDouble = 5100,

        ActionDrawArc = 6000,
        ActionDrawArc3P = 6010,
        ActionDrawArcParallel = 6020,
        ActionDrawArcTangential = 6030,
        ActionDrawCircle = 6040,
        ActionDrawCircle2P = 6050,
        ActionDrawCircle3P = 6060,
        ActionDrawCircleCR = 6070,
        ActionDrawCircleParallel = 6080,
        ActionDrawEllipseArcAxis = 6090,
        ActionDrawEllipseAxis = 6100,
        ActionDrawHatch = 6110,
        ActionDrawHatchNoSelect = 6120,
        ActionDrawImage = 6130,
        ActionDrawLine = 6140,
        //ActionDrawLineAngle = 6150,
        ActionDrawLineBisector = 6160,
        ActionDrawLineFree = 6170,
        ActionDrawLineHorVert = 6180,
        ActionDrawLineHorizontal = 6190,
        ActionDrawLineOrthogonal = 6200,
        ActionDrawLineParallel = 6210,
        ActionDrawLineParallelThrough = 6220,
        ActionDrawLinePolygon = 6230,
        ActionDrawLinePolygon2 = 6240,
        ActionDrawLineRectangle = 6250,
        ActionDrawLineRelAngle = 6260,
        ActionDrawLineTangent1 = 6270,
        ActionDrawLineTangent2 = 6280,
        ActionDrawLineVertical = 6290,
        ActionDrawPoint = 6300,
        ActionDrawSpline = 6310,
        ActionDrawPolyline = 6320,
        ActionDrawText = 6330,

        ActionUpdateWalls = 7000,
        ActionDrawWallInterior = 7010,
        ActionDrawWallExterior = 7020,
        ActionDrawWallFromEntityInterior = 7030,
        ActionDrawWallFromEntityExterior = 7040,
        ActionDrawWallParallelInterior = 7050,
        ActionDrawWallParallelExterior = 7060,
        ActionDrawWindow = 7070,
        ActionDrawDoor = 7080,
        ActionDrawSlab = 7090,
        ActionDrawSlabAuto = 7100,
        ActionDrawSlabHole = 7110,
        ActionDrawStairsStraight = 7120,
        ActionDrawStairsLLeft = 7130,
        ActionDrawStairsLRight = 7140,
        ActionDrawStairsULeft = 7150,
        ActionDrawStairsURight = 7160,
        ActionDrawRoofFlat = 7170,
        ActionDrawRoofHip = 7180,
        ActionDrawRoofAutoFlat = 7190,
        ActionDrawRoofAutoHip = 7200,
        ActionAddFloor = 7210,
        ActionRemoveFloor = 7220,
        ActionDropSetDressing = 7230,
        ActionDrawColumn = 7240,
        ActionDrawBeam = 7250,
        ActionDrawLight = 7260,
        
        ActionPolylineAdd = 8000,
        ActionPolylineAppend = 8010,
        ActionPolylineDel = 8020,
        ActionPolylineDelBetween = 8030,
        ActionPolylineTrim = 8040,
        ActionPolylineEquidistant = 8050,
        ActionPolylineFromSegments = 8060,

        ActionDimAligned = 9000,
        ActionDimLinear = 9010,
        ActionDimLinearVer = 9020,
        ActionDimLinearHor = 9030,
        ActionDimRadial = 9040,
        ActionDimDiametric = 9050,
        ActionDimAngular = 9060,
        ActionDimLeader = 9070,
        ActionDimOrdinate = 9080,

        ActionModifyAttributes = 10000,
        ActionModifyAttributesNoSelect = 10010,
        ActionModifyDelete = 10020,
        ActionModifyDeleteNoSelect = 10030,
        ActionModifyDeleteQuick = 10040,
        ActionModifyDeleteFree = 10050,
        ActionModifyMove = 10060,
        ActionModifyMoveNoSelect = 10070,
        ActionModifyRotate = 10080,
        ActionModifyRotateNoSelect = 10090,
        ActionModifyScale = 10100,
        ActionModifyScaleNoSelect = 10110,
        ActionModifyMirror = 10120,
        ActionModifyMirrorNoSelect = 10130,
        ActionModifyMoveRotate = 10140,
        ActionModifyMoveRotateNoSelect = 10150,
        ActionModifyRotate2 = 10160,
        ActionModifyRotate2NoSelect = 10170,
        ActionModifyEntity = 10180,
        ActionModifyTrim = 10190,
        ActionModifyTrim2 = 10200,
        ActionModifyTrimAmount = 10210,
        ActionModifyDivide = 10220,
        ActionModifyDivide2 = 10230,
        ActionModifyStretch = 10240,
        ActionModifyBevel = 10250,
        ActionModifyRound = 10260,
        ActionModifyText = 10270,
        ActionModifyToBack = 10280,
        ActionModifyToBackNoSelect = 10290,
        ActionModifyToFront = 10300,
        ActionModifyToFrontNoSelect = 10310,
        ActionModifyTriangulate = 10320,
        ActionModifyTriangulateNoSelect = 10330,
        ActionModifyDoBooleanOperation = 10340,
        ActionModifyDoBooleanOperationNoSelect = 10350,
        ActionModifyIsoProject = 10360,
        ActionModifyIsoProjectNoSelect = 10370,
        ActionModifyExplodeText = 10380,
        ActionModifyExplodeTextNoSelect = 10390,
        ActionModifyExplode = 10400,
        ActionModifyExplodeNoSelect = 10410,
        */

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
        ActionRestrictVertical = 12030,

        /*
        ActionSetRelativeZero = 13000,
        ActionToggleLockRelativeZero = 13010,
        ActionLockRelativeZero = 13020,
        ActionUnlockRelativeZero = 13030,

        ActionInfoInside = 14000,
        ActionInfoDist = 14010,
        ActionInfoDist2 = 14020,
        ActionInfoAngle = 14030,
        ActionInfoTotalLength = 14040,
        ActionInfoTotalLengthNoSelect = 14050,
        ActionInfoArea = 14060,

        ActionLayersDuplicate = 15000,
        ActionLayersDefreezeAll = 15010,
        ActionLayersFreezeAll = 15020,
        ActionLayersAdd = 15030,
        ActionLayersRemove = 15040,
        ActionLayersEdit = 15050,
        ActionLayersToggleView = 15060,
        ActionLayersToggleLock = 15070,

        ActionBlocksDefreezeAll = 16000,
        ActionBlocksFreezeAll = 16010,
        ActionBlocksAdd = 16020,
        ActionBlocksRemove = 16030,
        ActionBlocksAttributes = 16040,
        ActionBlocksEdit = 16050,
        ActionBlocksEditInsert = 16060,
        ActionBlocksInsert = 16070,
        ActionBlocksToggleView = 16080,
        ActionBlocksCreate = 16090,
        ActionBlocksCreateNoSelect = 16100,
        
        ActionLibraryInsert = 18000,

        ActionOptionsGeneral = 19000,
        ActionOptionsDrawing = 19010,

        ActionToolRegenerateDimensions = 20000,

        ActionScriptOpenIde = 21000,
        ActionScriptRun = 21010,

        ActionCamExportAuto = 22000,
        ActionCamReorder = 22010,

        ActionDebugEntities = 23000,
        ActionDebugResize = 23010,
        
        ActionPARISDebugCreateContainer = 24000
        */
    };

    /**
     * Entity ending. Used for returning which end of an entity is ment.
     */
    enum Ending {
        EndingStart,    /**< Start point. */
        EndingEnd,      /**< End point. */
        EndingNone      /**< Neither. */
    };

    /**
     * Update mode for non-atomic entities that need to be updated when 
     * they change. e.g. texts, inserts, ...
     */
    enum UpdateMode {
        NoUpdate,       /**< No automatic updates. */
        Update,         /**< Always update automatically when modified. */
        PreviewUpdate   /**< Update automatically but only for previews (quick update) */
    };

    /**
     * Drawing mode.
     */
    enum DrawingMode {
        ModeFull,       /**< Draw everything always detailed (default) */
        ModeAuto,       /**< Draw details when reasonable */
        ModePreview,    /**< Draw only in black/white without styles */
        ModeXor,        /**< XOR mode for moving previews */
        ModeBW          /**< Black/white. Can be used for printing. */
    };
    
    /**
     * Reference point mode.
     */
    enum RefMode {
        RefAll,           /**< All reference points. */
        RefMoving         /**< Only reference points for moving entities. */
    };

    /**
     * Undoable rtti.
     */
    enum UndoableType {
        UndoableUnknown,    /**< Unknown undoable */
        UndoableEntity,     /**< Entity */
        UndoableLayer       /**< Layer */
    };

    /**
     * Toolbar ID's.
     */
    enum ToolBarId {
        ToolBarMain,        /**< Main (menu). */
        ToolBarPoints,      /**< Points. */
        ToolBarLines,       /**< Lines. */
        ToolBarArcs,        /**< Arcs. */
        ToolBarCircles,     /**< Circles. */
        ToolBarEllipses,    /**< Ellipses. */
        ToolBarSplines,     /**< Splines. */
        ToolBarPolylines,   /**< Polylines. */
        ToolBarText,        /**< Text. */
        ToolBarDim,         /**< Dimensions. */
        ToolBarSnap,        /**< Snap. */
        ToolBarModify,      /**< Modify. */
        ToolBarSelect,      /**< Select. */
        ToolBarInfo,        /**< Information */
        ToolBarArchitecture /**< Architecture tools. */
    };

    /**
     * Units.
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
        /** Degrees with decimal point (e.g. 24.5�) */
        DegreesDecimal,
        /** Degrees, Minutes and Seconds (e.g. 24�30'5'') */
        DegreesMinutesSeconds,
        /** Gradians with decimal point (e.g. 390.5)*/
        Gradians,
        /** Radians with decimal point (e.g. 1.57)*/
        Radians,
        /** Surveyor's units */
        Surveyors
    };

    /**
     * Enum of levels of resolving when iterating through an entity tree.
     */
    enum ResolveLevel {
        /** Groups are not resolved */
        ResolveNone,
        /**
         * all Entity Containers are resolved 
         * (including Texts, Polylines, ...) 
         */
        ResolveAll
    };

    /**
     * Direction used for scrolling actions.
     */
    enum Direction {
        Up, Left, Right, Down
    };
    
    /**
     * Orientation used for contours.
     */
    enum ContourOrientation {
        ClockWise, CounterClockWise
    };
    
    /**
     * Side used for side of a point relativ to an entity 
     * (right hand or left hand side)
     */
    enum Side {
        RightHand, LeftHand, NoSide
    };

    /**
     * Vertical alignments.
     */
    enum VAlign {
        VAlignTop,      /**< Top. */
        VAlignMiddle,   /**< Middle */
        VAlignBottom    /**< Bottom */
    };

    /**
     * Horizontal alignments.
     */
    enum HAlign {
        HAlignLeft,     /**< Left */
        HAlignCenter,   /**< Centered */
        HAlignRight     /**< Right */
    };

    /**
     * Text drawing direction.
     */
    enum TextDrawingDirection {
        LeftToRight,     /**< Left to right */
        TopToBottom,     /**< Top to bottom */
        ByStyle          /**< Inherited from associated text style */
    };

    /**
     * Line spacing style for texts.
     */
    enum TextLineSpacingStyle {
        AtLeast,        /**< Taller characters will override */
        Exact           /**< Taller characters will not override */
    };

    /**
     * Leader path type.
     */
    enum LeaderPathType {
        Straight,      /**< Straight line segments */
        Spline         /**< Splines */
    };

    /**
     * Direction for zooming actions.
     */
    enum ZoomDirection {
        In, Out
    };

    /**
     * Axis specification for zooming actions.
     */
    enum Axis {
        OnlyX, OnlyY, Both
    };

    enum IsoProjectionType {
        IsoTop, IsoFront, IsoSide
    };

    /**
     * Mouse button and keyboard state for mouse events.
     */
    enum ButtonState {
        NoButton        = Qt::NoButton,
        LeftButton      = Qt::LeftButton,
        RightButton     = Qt::RightButton,
        MidButton       = Qt::MidButton
    };

    /**
     * Wrapper for Qt
     */
    static RS2::ButtonState qtToRsButtonState(Qt::MouseButton t) {
        return (RS2::ButtonState)t;
    }

    /**
     * Enum of line styles:
     */
    enum LineType {
        NoPen = 0,            /**< No line at all. */
        SolidLine = 1,        /**< Normal line. */

        DotLine = 2,          /**< Dotted line. */
        DotLine2 = 3,         /**< Dotted line small. */
        DotLineX2 = 4,        /**< Dotted line large. */

        DashLine = 5,         /**< Dashed line. */
        DashLine2 = 6,        /**< Dashed line small. */
        DashLineX2 = 7,       /**< Dashed line large. */

        DashDotLine = 8,      /**< Alternate dots and dashes. */
        DashDotLine2 = 9,     /**< Alternate dots and dashes small. */
        DashDotLineX2 = 10,   /**< Alternate dots and dashes large. */

        DivideLine = 11,      /**< dash, dot, dot. */
        DivideLine2 = 12,     /**< dash, dot, dot small. */
        DivideLineX2 = 13,    /**< dash, dot, dot large. */

        CenterLine = 14,      /**< dash, small dash. */
        CenterLine2 = 15,     /**< dash, small dash small. */
        CenterLineX2 = 16,    /**< dash, small dash large. */

        BorderLine = 17,      /**< dash, dash, dot. */
        BorderLine2 = 18,     /**< dash, dash, dot small. */
        BorderLineX2 = 19,    /**< dash, dash, dot large. */

        LineByLayer = -1,     /**< Line type defined by layer not entity */
        LineByBlock = -2      /**< Line type defined by block not entity */
    };

    /**
     * Wrapper for Qt
     */
    static Qt::PenStyle rsToQtLineType(RS2::LineType t) {
        switch (t) {
        case NoPen:
            return Qt::NoPen;
            break;
        case SolidLine:
            return Qt::SolidLine;
            break;
        case DotLine:
        case DotLine2:
        case DotLineX2:
            return Qt::DotLine;
            break;
        case DashLine:
        case DashLine2:
        case DashLineX2:
            return Qt::DashLine;
            break;
        case DashDotLine:
        case DashDotLine2:
        case DashDotLineX2:
            return Qt::DashDotLine;
            break;
        case DivideLine:
        case DivideLine2:
        case DivideLineX2:
            return Qt::DashDotDotLine;
            break;
        case CenterLine:
        case CenterLine2:
        case CenterLineX2:
            return Qt::DashDotLine;
            break;
        case BorderLine:
        case BorderLine2:
        case BorderLineX2:
            return Qt::DashDotLine;
            break;
        case LineByLayer:
        case LineByBlock:
        default:
            return Qt::SolidLine;
            break;
        }
        return Qt::SolidLine;
    }

    /**
     * \brief Struct that stores a line type pattern (e.g. dash dot dot).
     */
    struct LineTypePatternStruct {
        double* pattern;
        int num;
    }
    LineTypePattern;

    /**
     * Enum of line widths. The values of the enums correspond to
     * the values used in the DXF file format.
     */
    enum LineWidth {
        Width00 = 0,       /**< Width 1.  (0.00mm) */
        Width01 = 5,       /**< Width 2.  (0.05mm) */
        Width02 = 9,       /**< Width 3.  (0.09mm) */
        Width03 = 13,      /**< Width 4.  (0.13mm) */
        Width04 = 15,      /**< Width 5.  (0.15mm) */
        Width05 = 18,      /**< Width 6.  (0.18mm) */
        Width06 = 20,      /**< Width 7.  (0.20mm) */
        Width07 = 25,      /**< Width 8.  (0.25mm) */
        Width08 = 30,      /**< Width 9.  (0.30mm) */
        Width09 = 35,      /**< Width 10. (0.35mm) */
        Width10 = 40,      /**< Width 11. (0.40mm) */
        Width11 = 50,      /**< Width 12. (0.50mm) */
        Width12 = 53,      /**< Width 13. (0.53mm) */
        Width13 = 60,      /**< Width 14. (0.60mm) */
        Width14 = 70,      /**< Width 15. (0.70mm) */
        Width15 = 80,      /**< Width 16. (0.80mm) */
        Width16 = 90,      /**< Width 17. (0.90mm) */
        Width17 = 100,     /**< Width 18. (1.00mm) */
        Width18 = 106,     /**< Width 19. (1.06mm) */
        Width19 = 120,     /**< Width 20. (1.20mm) */
        Width20 = 140,     /**< Width 21. (1.40mm) */
        Width21 = 158,     /**< Width 22. (1.58mm) */
        Width22 = 200,     /**< Width 23. (2.00mm) */
        Width23 = 211,     /**< Width 24. (2.11mm) */
        WidthByLayer = -1, /**< Line width defined by layer not entity. */
        WidthByBlock = -2, /**< Line width defined by block not entity. */
        WidthDefault = -3,  /**< Line width defaults to the predefined line width. */
        //WidthScreen = -4   /**< Line width irrelevant, use fixed screen width. */
    };

    /**
     * Converts and int (see DXF doc) to a line width struct
     */
    static LineWidth intToLineWidth(int w) {
        if (w==-3) {
            return WidthDefault;
        } else if (w==-2) {
            return WidthByBlock;
        } else if (w==-1) {
            return WidthByLayer;
        } else if (w<3) {
            return Width00;
        } else if (w<8) {
            return Width01;
        } else if (w<12) {
            return Width02;
        } else if (w<14) {
            return Width03;
        } else if (w<17) {
            return Width04;
        } else if (w<19) {
            return Width05;
        } else if (w<23) {
            return Width06;
        } else if (w<28) {
            return Width07;
        } else if (w<33) {
            return Width08;
        } else if (w<38) {
            return Width09;
        } else if (w<46) {
            return Width10;
        } else if (w<52) {
            return Width11;
        } else if (w<57) {
            return Width12;
        } else if (w<66) {
            return Width13;
        } else if (w<76) {
            return Width14;
        } else if (w<86) {
            return Width15;
        } else if (w<96) {
            return Width16;
        } else if (w<104) {
            return Width17;
        } else if (w<114) {
            return Width18;
        } else if (w<131) {
            return Width19;
        } else if (w<150) {
            return Width20;
        } else if (w<180) {
            return Width21;
        } else if (w<206) {
            return Width22;
        } else {
            return Width23;
        }
    }

    /**
     * Enum of cursor types.
     */
    enum CursorType {
        ArrowCursor,          /**< ArrowCursor - standard arrow cursor. */
        UpArrowCursor,        /**< UpArrowCursor - upwards arrow. */
        CrossCursor,          /**< CrossCursor - crosshair. */
        WaitCursor,           /**< WaitCursor - hourglass/watch. */
        IbeamCursor,          /**< IbeamCursor - ibeam/text entry. */
        SizeVerCursor,        /**< SizeVerCursor - vertical resize. */
        SizeHorCursor,        /**< SizeHorCursor - horizontal resize. */
        SizeBDiagCursor,      /**< SizeBDiagCursor - diagonal resize (/). */
        SizeFDiagCursor,      /**< SizeFDiagCursor - diagonal resize (\). */
        SizeAllCursor,        /**< SizeAllCursor - all directions resize. */
        BlankCursor,          /**< BlankCursor - blank/invisible cursor. */
        SplitVCursor,         /**< SplitVCursor - vertical splitting. */
        SplitHCursor,         /**< SplitHCursor - horziontal splitting. */
        PointingHandCursor,   /**< PointingHandCursor - a pointing hand. */
        ForbiddenCursor,      /**< ForbiddenCursor - a slashed circle. */
        WhatsThisCursor,      /**< WhatsThisCursor - an arrow with a ?. */
        CadCursor,            /**< CadCursor - a bigger cross. */
        DelCursor,            /**< DelCursor - cursor for choosing entities */
        SelectCursor,         /**< SelectCursor - for selecting single entities */
        MagnifierCursor,      /**< MagnifierCursor - a magnifying glass. */
        MovingHandCursor      /**< Moving hand - a little flat hand. */
    };

    /**
     * Wrapper for Qt.
     */
    static Qt::CursorShape rsToQtCursorType(RS2::CursorType t) {
        switch (t) {
        case Qt::ArrowCursor:
            return Qt::ArrowCursor;
            break;
        case Qt::UpArrowCursor:
            return Qt::UpArrowCursor;
            break;
        case Qt::CrossCursor:
            return Qt::CrossCursor;
            break;
        case Qt::WaitCursor:
            return Qt::WaitCursor;
            break;
        case IbeamCursor:
            return Qt::IBeamCursor;
            break;
        case Qt::SizeVerCursor:
            return Qt::SizeVerCursor;
            break;
        case Qt::SizeHorCursor:
            return Qt::SizeHorCursor;
            break;
        case Qt::SizeBDiagCursor:
            return Qt::SizeBDiagCursor;
            break;
        case Qt::SizeFDiagCursor:
            return Qt::SizeFDiagCursor;
            break;
        case Qt::SizeAllCursor:
            return Qt::SizeAllCursor;
            break;
        case Qt::BlankCursor:
            return Qt::BlankCursor;
            break;
        case Qt::SplitVCursor:
            return Qt::SplitVCursor;
            break;
        case Qt::SplitHCursor:
            return Qt::SplitHCursor;
            break;
        case Qt::PointingHandCursor:
            return Qt::PointingHandCursor;
            break;
        case Qt::ForbiddenCursor:
            return Qt::ForbiddenCursor;
            break;
        case Qt::WhatsThisCursor:
            return Qt::WhatsThisCursor;
            break;
        default:
            return Qt::ArrowCursor;
            break;
        }
        return Qt::ArrowCursor;
    }

    /**
     * Paper formats.
     */
    enum PaperFormat {
        Custom, 
        Letter, 
        Legal, 
        Executive,
        A0, 
        A1,
        A2, 
        A3, 
        A4, 
        A5, 
        A6, 
        A7, 
        A8, 
        A9, 
        B0, 
        B1, 
        B2, 
        B3, 
        B4, 
        B5, 
        B6, 
        B7, 
        B8, 
        B9, 
        B10,
        C5E, 
        Comm10E,
        DLE, 
        Folio, 
        Tabloid, 
        NPageSize 
    };
    
    /**
     * Wrapper for Qt.
     */
    static QPrinter::PageSize rsToQtPaperFormat(RS2::PaperFormat f) {
        QPrinter::PageSize ret;
    
        switch (f) {
        case Custom:
            ret = QPrinter::Custom;
            break;
        case Letter:
            ret = QPrinter::Letter;
            break;
        case Legal:
            ret = QPrinter::Legal;
            break;
        case Executive:
            ret = QPrinter::Executive;
            break;
        case A0:
            ret = QPrinter::A0;
            break;
        case A1:
            ret = QPrinter::A1;
            break;
        case A2:
            ret = QPrinter::A2;
            break;
        case A3:
            ret = QPrinter::A3;
            break;
        default:
        case A4:
            ret = QPrinter::A4;
            break;
        case A5:
            ret = QPrinter::A5;
            break;
        case A6:
            ret = QPrinter::A6;
            break;
        case A7:
            ret = QPrinter::A7;
            break;
        case A8:
            ret = QPrinter::A8;
            break;
        case A9:
            ret = QPrinter::A9;
            break;
        case B0:
            ret = QPrinter::B0;
            break;
        case B1:
            ret = QPrinter::B1;
            break;
        case B2:
            ret = QPrinter::B2;
            break;
        case B3:
            ret = QPrinter::B3;
            break;
        case B4:
            ret = QPrinter::B4;
            break;
        case B5:
            ret = QPrinter::B5;
            break;
        case B6:
            ret = QPrinter::B6;
            break;
        case B7:
            ret = QPrinter::B7;
            break;
        case B8:
            ret = QPrinter::B8;
            break;
        case B9:
            ret = QPrinter::B9;
            break;
        case B10:
            ret = QPrinter::B10;
            break;
        case C5E:
            ret = QPrinter::C5E;
            break;
        case Comm10E:
            ret = QPrinter::Comm10E;
            break;
        case DLE:
            ret = QPrinter::DLE;
            break;
        case Folio:
            ret = QPrinter::Folio;
            break;
        case Tabloid:
            ret = QPrinter::Tabloid;
            break;
        case NPageSize:
            ret = QPrinter::NPageSize;
            break;
        }

        return ret;
    }

};
    
#endif
