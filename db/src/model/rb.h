/*****************************************************************
 * $Id: rb.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Line Designation Table (LDT) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_H
#define RB_H

#include <cmath>
#include <Qt>
#include "db_global.h"
#include "rb_string.h"

// The magic number PI
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


/**
 * Class namespace for various enums along with some simple
 * wrapper methods for converting the enums to the Qt
 * counterparts.
 */
class DB_EXPORT RB2 {
public:
    const static int HIDDENCOLUMNS = 7;
    const static int MODELPREFIX_TO_MODELTYPE_MULTIPLIER = 1000; // thousand
    const static int PERSPECTIVE_TO_MODELPREFIX_MULTIPLIER = 10; // hundred

    static int modelTypeToPerspective(int modelType) {
        return (int) std::floor(modelType / (MODELPREFIX_TO_MODELTYPE_MULTIPLIER
                                             * PERSPECTIVE_TO_MODELPREFIX_MULTIPLIER));
    }

    static RB_String getCbToLeStyle() {
        return RB_String("QComboBox::drop-down "
                         "{border-top-right-radius: 3px; "
                         "border-bottom-right-radius: 3px;}");
    }

    /**
     * Action types used by complex actions to create working models,
     * naming is: "Action"_context_action e.g. Action_Ldt_FluidCheck (without
     * the underscores)
     */
    enum ActionType {
        ActionNone,        			/**< Invalid action id. */
        ActionLdtFluidCheck,			/**< Check item fluid data action */
        ActionLdtFluidUpdate,			/**< Update item fluid data action */
        ActionLdtInsulationCheck,		/**< Check  item insulation data action */
        ActionLdtInsulationUpdate,		/**< Update item insulation data action */
        ActionLdtPaintCheck,			/**< Check item paint data action */
        ActionLdtPaintUpdate,			/**< Update item paint data action */
        ActionLdtPedCheck,			/**< Check item PED data action */
        ActionLdtPedUpdate,			/**< Update item PED data action */
        ActionLdtPipeClassCheck,		/**< Check item pipe class data action */
        ActionLdtPipeClassUpdate,		/**< Update item pipe class data action */
        ActionDefault				/**< Default- or all actions */
    };

    /**
     * Alignment types such as right center top but also financial amount
     * which is right aligned and with only two decimals
     */
    enum AlignType {
        AlignBottom = 1<<0,             /**< Data to be aligned at the bottom */
        AlignCenter = 1<<1,             /**< Data to be aligned at the horizontal center */
        AlignFinancialAmount = 1<<2,    /**< Data to be shown as financial amount, center right and precision=2 */
        AlignLeft = 1<<3,               /**< Data to be aligned at the right */
        AlignMiddle = 1<<4,             /**< Data to be aligned at the vertical middle */
        AlignRight = 1<<5,              /**< Data to be aligned at the right */
        AlignTop  = 1<<6
    };

    /**
     * Separator for comma separated files (CSV), comma, semicolon or tab
     */
    enum CsvSeparator {
    	CsvComma,
    	CsvSemicolon,
    	CsvTab
    };
    
    /**
     * Database driver types, Qt 4.3.3 possible drivers:
     *     	QDB2    	IBM DB2
     *     	QIBASE    	Borland InterBase Driver
     *     	QMYSQL    	MySQL Driver
     *     	QOCI    	Oracle Call Interface Driver
     *     	QODBC    	ODBC Driver (includes Microsoft SQL Server)
     *     	QPSQL    	PostgreSQL Driver
     *     	QSQLITE    	SQLite version 3 or above
     *     	QSQLITE2   	SQLite version 2
     *     	QTDS    	Sybase Adaptive Server
     * TODO: not used anymore?
     */
    enum DatabaseDriver {
    	DatabaseNone,					/**< Invalid database driver */
    	DatabaseAccess,					/**< Microsoft Access (local uses ODBC) */
    	DabaseDb2,						/**< IBM DB2 driver */
    	DatabaseIbase,					/**< Borland Interbase Driver */
    	DatabaseMysql,					/**< MySQL database driver */
    	DatabaseOci,					/**< Oracle Call Interface driver */
    	DatabaseOdbc,					/**< ODBC driver including SQL server */
    	DatabasePsql,					/**< PostgreSQL driver */
    	DatabaseSqlite,					/**< SQLite 3 or above internal application driver */
    	DatabaseSqlite2,				/**< SQLite 2 driver */
    	DatabaseTds,					/**< Sybase Adaptive server driver */
    	DatabaseDefault					/**< All or default database driver */
    };
    
    /**
     * Level of discipline detail data to be handled or shown. For example
     * for LDT_Line
     */
    enum DataDetailLevel {
    	DataDetailNone,					/**< Invalid or no details */
    	DataDetailBasic,				/**< Basic or minimum data */
    	DataDetailDefault				/**< Default- or full data details */
    };

//    /**
//     * Dialog types, example = 103XXX:
//     * - 10 = DB,
//     * - 3 = dialog,
//     * - XXX = sequence number
//     * 103100 is starting number leaving 103000 to 103099 for test/debug cases
//     * Refer also WidgetType and DialogType
//     */
//    enum DialogType {
//        // start number from 103000
//        DialogTest = 103000,                   /**< Test Dialog */
//        DialogBaseTest,                        /**< Test of base dialogs */
//        // start numbering from 103100
//        DialogNone = 103100,                   /**< Invalid dialog */
//        DialogBaseEditSort,                    /**< Base table dialog with sorting */
//        DialogBaseEditUpDown,                  /**< Base table dialog with up/down */
//        DialogBaseSelectionOnly,               /**< Base table dialog for selection only */
//        DialogDbBrowser,                       /**< Database browser dialog */
//        DialogDbConnection,                    /**< Connect to database dialog */
//        DialogDbMapping,                       /**< Execute (database) mapping dialog */
//        DialogProject,                         /**< Select project from database dialog */
//        DialogSelectMapping,                   /**< Select mapping dialog */
//        DialogDefault                          /**< Default- or all dialogs */
//    };

    /**
     * Delegate editor types such as QLineEdit, QDateEdit and QSpinBox
     * Used in model in RB2::EditorRole
     */
    enum EditorRole {
        EditorDefault,
        EditorCustom,
        EditorDate,
        EditorDouble,
        EditorInteger,
        EditorLine,
        EditorPassword,
        EditorTime,
        EditorNone
    };

    /**
     * Encryption and decryption type
     */
    enum EncryptionType {
    	DecryptionRsa,			/**< RSA decryption of license file */
    	DecryptionSymmetric,	/**< Symmetric (password) decryption */
    	EncryptionRsa,			/**< RSA enecryption of license file */
    	EncryptionSymmetric		/**< Symmetric (password) encryption */
    };
    
    /**
     * File types. Used by file dialogs. Note: the extension might not
     * be enough to distinguish file types.
     */
    enum FileType {
        FileUnknown,       	/**< Unknown / unsupported format. */
//		FileDXF1,          	/**< QCad 1 compatibility DXF format. */
//		FileDXF,           	/**< DXF format. 2000. */
//		FileDXF12,         	/**< DXF format. R12. */
//		FileCXF,           	/**< CAM Expert Font format. */
//		FileCAM            	/**< CAM Expert CAM format (NC, CNC, D, ..) */
        FileSVG,			/**< Scalable Vector Graphics file format */
        FileXML				/**< eXtended Markup Language file format */
    };
    
    /**
     * Object flags, maximum 16 bits?
     */
    enum Flags {
//        FlagUndone      = 1<<0,     /**< Flag for Undoables. */
//        FlagVisible     = 1<<1,     /**< Entity Visibility. */
//        FlagByLayer     = 1<<2,     /**< Entity attribute (e.g. color) is defined by layer. */
//        FlagByBlock     = 1<<3,     /**< Entity attribute (e.g. color) defined by block. */
//        FlagFrozen      = 1<<4,     /**< Layer frozen. */
//        FlagDefFrozen   = 1<<5,     /**< Layer frozen by default. */
//        FlagLocked      = 1<<6,     /**< Layer locked. */
//        FlagInvalid     = 1<<7,     /**< Used for invalid pens. */
//        FlagSelected    = 1<<8,     /**< Entity in current selection. */
//        FlagClosed      = 1<<9,     /**< Polyline closed? */
//        FlagTemp        = 1<<10,    /**< Flag for temporary entities (e.g. hatch) */
//        FlagProcessed   = 1<<11,    /**< Flag for processed entities (optcontour) */
//        FlagSelected1   = 1<<12,    /**< Startpoint selected */
//        FlagSelected2   = 1<<13,    /**< Endpoint selected */
//        FlagHighlighted = 1<<14,    /**< Entity is highlighted temporarily (as a user action feedback) */
//        FlagSelectionVisible = 1<<15,   /**< Entity is selected. Selection is made visible to user. */
//        FlagRemove = 1<<16,         /**< Entity is flagged for removal. */
        //FlagOverride = 1<<17      /**< Used for override pens */

        FlagIsDirty = 1<<0,         /**< Object data is changed */
        FlagFromDatabase = 1<<1,    /**< Object created from database */
        FlagIsDeleted = 1<<2,       /**< Object is deleted, used for update of database */
        FlagChildrenCreated = 1<<3  /**< Object children (if exist) have been created, used for treeView */
    };

    /**
     * Insulation material or all materials or 
     */
    enum Insulated {
        InsulateNone,        			/**< Invalid insulation id. */
        InsulateNo,	        			/**< Not insulated */
        InsulateYes,					/**< Insulated */
        InsulateDefault					/**< Insulated or not insulated */
    };
    

    /**
     * Material of construction
     */
    enum LicenseType {
        LicenseNone,        			/**< Invalid license */
        LicenseEvaluation,				/**< Valid evaluation license */
        LicenseInProgress,				/**< User license in progress */
        LicenseLocalUser,				/**< Valid local user license */
        LicenseNetworkUser,				/**< Valid network user license */
        LicenseDefault					/**< Default- or all licenses */
    };

    /**
     * Material of construction
     */
    enum MaterialType {
        MaterialNone,        			/**< Invalid action id. */
        MaterialCS,						/**< Carbon Steel */
        MaterialSS,						/**< Stainless Steel */
        MaterialDefault					/**< Default- or all material */
    };
    
    enum MemberType {
    	MemberNone,        				/**< Invalid member type */
        MemberByteArray,                /**< Byte array such as an image */
        MemberChar10,        			/**< 10 characters, currency code */
        MemberChar20,        			/**< 20 characters, reference number, telephone number */
        MemberChar40,        			/**< 40 characters, uuids and comboboxes, _id fields */
        MemberChar125,        			/**< 125 characters, usually line edits, email address */
        MemberChar165,        			/**< 165 characters, usually _idx field line edits */
        MemberChar255,        			/**< 255 characters, usually for text edits, short description */
        MemberChar2500,        			/**< 2500 characters, usually text edits */
        MemberDateTime,        			/**< Date time member type */
    	MemberDouble,        			/**< Double member type */
        MemberInteger,     				/**< Integer member type, can be _id field for fixed relation */
    	MemberString,        			/**< String, usually for larger text */
        MemberUInteger,    				/**< Unsigned integer member type */
    	MemberDefault        			/**< Default- or all member types */
    };
    
//    /**
//     * Model manager types, example = 101XXX:
//     * - 10 = DB,
//     * - 1 = model,
//     * - XXX = sequence number
//     * 101100 is starting number leaving 101000 to 101099 for test/debug cases
//     * Refer also WidgetType and DialogType
//     */
//    enum ModelType {
//        // start numbering from 101000
//        ModelTest = 101000,             /**< Table model */
//        ModelTestChild,                 /**< Child table model */
//        ModelTestDialog,                /**< Dialog model */
//        ModelTestTree,                  /**< Tree model */
//        ModelTestTreeChild,             /**< Child Tree model */
//        // start numbering from 101100
//        ModelNone = 101100,             /**< Invalid model */
//        ModelProject,                   /**< Project model */
//        ModelSelectMapping,             /**< Select mapping model */
//        ModelDefault                    /**< Default- or all models */
//    };

    /**
     * Permission to part of the database and applications
     */
    enum PermissionType {
        PermissionNone          = 1<<0, /**< No- or invalid permission/access */
        PermissionRead          = 1<<1,	/**< User read permission, R in CRUDX */
        PermissionUpdate        = 1<<2, /**< User has permission to update data, U in CRUDX */
        PermissionCreate        = 1<<3, /**< User has permission to create data, C in CRUDX */
        PermissionDelete        = 1<<4, /**< User has permission to delete data, D in CRUDX */
        PermissionExecute       = 1<<5, /**< User has permission to execute operation, X in CRUDX */
        PermissionAdmin         = 1<<6,	/**< General db adminstrator premission */
        PermissionSuperUser     = 1<<7,	/**< Unlimited db operations for developers */
        PermissionDefault	    = 1<<8	/**< Default- or all permissions */
    };
    
    /**
     * Perspectives of the Biluna system, such as accounting,
     * the various engineering discplines and asset management.
     * The last capital letters after 'Perspective' correspond
     * with the perpective code, for example 'ACC' is perpective code
     * for accounting.
     * last number used is 30
     */
    enum PerspectiveType {
        PerspectiveNone = 0,                /**< Invalid context */
        PerspectiveACC = 11,                /**< Accounting */
        PerspectiveDB = 10,                 /**< Base and database */
        PerspectiveCRM = 27,                /**< Customer Relation Management */
        PerspectiveCAD = 12,                /**< Computer Aided Design */
        PerspectiveECC = 13,                /**< Estimate and Cost Control */
        PerspectiveENG = 23,                /**< General engineering tools */
        PerspectiveEQL = 14,                /**< Equipment List */
        PerspectiveGV = 24,                 /**< Graphics views */
        PerspectiveJOB = 15,                /**< Planning, schedule and task */
        PerspectiveLDT = 16,                /**< Line Designation Table */
        PerspectiveMAT = 17,                /**< Material Management */
        PerspectiveMRP = 30,                /**< Material Requirements Planning or Manufacturing Resource Planning */
        PerspectivePCALC = 19,              /**< Plant calculation */
        PerspectivePCC = 18,                /**< Pipe Class Component */
        PerspectivePENG = 25,               /**< Plant Engineering 2D P&ID, LDT EQL */
        PerspectivePNID = 20,               /**< Piping and Instrument Diagram */
        PerspectiveRPT = 21,                /**< Report management */
        PerspectiveSAIL = 28,               /**< Sailing navigation */
        PerspectiveSCAN = 26,               /**< Scan, questionaire or interview */
        PerspectiveSRM = 29,                /**< Supplier Relation Management */
        PerspectiveVES = 22,                /**< Vessel Exchanger Software */
        PerspectiveTEST = 99,               /**< Testing only */
        PerspectiveDefault = 100            /**< Default- or all contexts */
    };

    /**
     * Project status in a database, the status of a project
     * overrides the permissions of the users but not the admin and the super user
     */
    enum ProjectStatus {
        ProjectNone = -4,           /**< No- or invalid project. */
        ProjectDeleted = -3,        /**< The project is deleted/hidden for all users */
        ProjectHidden = -2,         /**< The project is hidden for all users, except admin and superuser when creating a new project*/
        ProjectLocked = -1,          /**< The project is locked or the data is frozen */
        ProjectLive = 0,            /**< The project is live, editable and is default/normal status*/
        ProjectTest = 1,            /**< The project is used for testing */
        ProjectDefault = 2          /**< Default- or all project status */
    };

    /**
     * Enum of levels of resolving when cloning or traversing 
     * an object tree (model). A level is an object plus his containers
     */
    enum ResolveLevel {
        ResolveNone = 0,	        /**< children are not resolved */
        ResolveOne = 2,				/**< Resolve one level */
        ResolveTwo = 4,				/**< Resolve two levels */
        ResolveThree = 6,			/**< Resolve three levels */
        ResolveFour = 8,			/**< Resolve four levels */
        ResolveFive = 10,			/**< Resolve five levels */
        ResolveSix = 12,			/**< Resolve six levels */
        ResolveSeven = 14,			/**< Resolve seven levels */
        ResolveEight = 16,			/**< Resolve eight levels */
        ResolveNine = 18,			/**< Resolve nine levels */
        ResolveTen = 20,			/**< Resolve ten levels */
        ResolveAll = 100         	/**< all objects and their children are resolved */
	};

    /**
     * Additional roles based on Qt::UserRole
     */
    enum RoleType {
        RoleOrigData = Qt::UserRole + 1,    /**< Relational table to show original data such as id*/
        RoleEditor                          /**< Type of delegate editor to be shown, RB2::EditorRole */
    };

    /** 
     * Selection of objects such as LDT_Line
     */
    enum Selection {
    	SelectNone,        		/**< Invalid selection id. */
        SelectAll,              /**< All items relevant */
        Selected,               /**< Only the selected items are relevant */
    	SelectRevised			/**< Only the revised items are relevant */
    };
    
	/**
     * Sort order of columns or vertical direction, used with database models
	 */
    enum SortOrderType {
        SortOrderNone,          /**< No sort order or undefined */
        SortOrderAscending,     /**< Ascending sort order */
        SortOrderDescending,    /**< Descending sort order */
        SortOrderDefault        /**< Default sort order */
    };
	
    /**
     * Status of record in database,
     * statustype > 100 are not to be shown in GUI
     */
    enum StatusType {
        StatusDefault = 0,      /**< Default status, visible and editable in GUI. */
        StatusLocked = 11,      /**< The record is on locked and can not be changed. */
        StatusInvisible = 101,  /**< The record is set to invisible. */
        StatusDeleted = 102     /**< Record can not be used anymore. */
    };

    /**
     * Valid types
     * - 0 = valid false,
     * - 1 = valid true
     * - X = other number not yet used but could be 'maybe'
     */
    enum ValidType {
        ValidFalse = 0,            /**< Disable the action */
        ValidTrue = 1              /**< Enable the action */
    };

//    /**
//     * Widget/dialog manager types, example = 102XXX:
//     * - 10 = DB,
//     * - 2 = widget,
//     * - XXX = sequence number
//     * 102100 is starting number leaving 102000 to 102099 for test/debug cases
//     * Refer also ModelType and DialogType
//     */
//    enum WidgetType {
//        // start numbering from 102000
//        WidgetTestDialog = 102000,      /**< Dialog widget */
//        WidgetTestTable,                /**< Table widget */
//        WidgetTestTableChild,           /**< Child table widget with table parent */
//        WidgetTestText,                 /**< Text widget */
//        WidgetTestTree,                 /**< Tree widget */
//        WidgetTestTreeChild,            /**< Child table widget with tree parent */
//        // start numbering from 102100
//        WidgetNone = 102100,            /**< Invalid model */
//        WidgetCommand,                  /**< Command widget */
//        WidgetDefault                   /**< Default- or all models */
//    };

    /**
     * Map enum for driver to actual driver name: e.g. "QODBC"
     */
    static RB_String enumToDriverName(RB2::DatabaseDriver driverEnum) {
        switch (driverEnum) {
            case RB2::DatabaseAccess:
                return "QODBC";
                break;
            case RB2::DabaseDb2:
                return "QDB2";
                break;
            case RB2::DatabaseIbase:
                return "QIBASE";
                break;
            case RB2::DatabaseMysql:
                return "QMYSQL";
                break;
            case RB2::DatabaseOci:
                return "QOCI";
                break;
            case RB2::DatabaseOdbc:
                return "QODBC";
                break;
            case RB2::DatabasePsql:
                return "QPSQL";
                break;
            case RB2::DatabaseSqlite:
                return "QSQLITE";
                break;
            case RB2::DatabaseSqlite2:
                return "QSQLITE2";
                break;
            case RB2::DatabaseTds:
                return "QTDS";
                break;
            default:
                return "None";
                break;
        }
    }

    /**
     * Map actual driver name: e.g. "QODBC" to enum e.g.: RB2::DatabaseMysql
     */
    static RB2::DatabaseDriver driverNameToEnum(const RB_String& dName) {
        if (dName == "QODBC") return RB2::DatabaseAccess;
        else if (dName == "QDB2") return RB2::DabaseDb2;
        else if (dName == "QIBASE") return RB2::DatabaseIbase;
        else if (dName == "QMYSQL") return RB2::DatabaseMysql;
        else if (dName == "QOCI") return RB2::DatabaseOci;
        else if (dName == "QODBC") return RB2::DatabaseOdbc;
        else if (dName == "QPSQL") return RB2::DatabasePsql;
        else if (dName == "QSQLITE") return RB2::DatabaseSqlite;
        else if (dName == "QSQLITE2") return RB2::DatabaseSqlite2;
        else if (dName == "QTDS") return RB2::DatabaseTds;
        return RB2::DatabaseNone;
    }

    static RB_String memberTypeToString(MemberType type) {
        if (type == MemberNone) return RB_String("MemberNone");
        else if (type == MemberByteArray) return RB_String("MemberByteArray");
        else if (type == MemberChar10) return RB_String("MemberChar10");
        else if (type == MemberChar20) return RB_String("MemberChar20");
        else if (type == MemberChar40) return RB_String("MemberChar40");
        else if (type == MemberChar125) return RB_String("MemberChar125");
        else if (type == MemberChar165) return RB_String("MemberChar165");
        else if (type == MemberChar255) return RB_String("MemberChar255");
        else if (type == MemberChar2500) return RB_String("MemberChar2500");
        else if (type == MemberDateTime) return RB_String("MemberDateTime");
        else if (type == MemberDouble) return RB_String("MemberDouble");
        else if (type == MemberInteger) return RB_String("MemberInteger");
        else if (type == MemberString) return RB_String("MemberString");
        else if (type == MemberUInteger) return RB_String("MemberUInteger");
        else if (type == MemberDefault) return RB_String("MemberDefault");
        return "ERROR";
    }

	/**
	 * Convert pipe sizes from NPS Nominal Pipe Size to DN Nominal Diameter
     * \todo move to PerspectivePENG
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
     * \todo move to PerspectivePENG
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

#endif
