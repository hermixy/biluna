/*****************************************************************
 * $Id: rb_system.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SYSTEM_H
#define RB_SYSTEM_H

#include <iostream>
#include <QDir>

#include "db_global.h"
#include "rb_debug.h"
#include "rb_string.h"
#include "rb_stringlist.h"

#define RB_SYSTEM RB_System::instance()

/**
 * Class for some system methods such as file and directory system operations.
 * Implemented as singleton. Use init to Initialize the class before you use it.
 * Example use: RB_SYSTEM::getHomeDir();
 * TODO: implement translation
 */
class DB_EXPORT RB_System {
	
public:
    /**
     * @return Instance to the unique system object.
     */
    static RB_System* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new RB_System();
        }
        return uniqueInstance;
    }

    void init(const RB_String& appName, const RB_String& appVersion, 
	          const RB_String& appDirName, const RB_String& appDir="");
//	void initLanguageList();

    bool checkInit();

    /**
     * @return Users home directory.
     */
    RB_String getHomeDir() {
        return QDir::homePath();
    }

    /**
     * @return Current directory.
     */
    RB_String getCurrentDir() {
        return QDir::currentPath();
    }

	/**
	 * @return Application directory.
	 */
	RB_String getAppDir() {
        return mAppDir;
	}

    /**
     * @return A list of absolute paths to all font files found.
     */
//    RB_StringList getFontList() {
//        RB_StringList ret = getFileList("fonts", "cxf");
//		return ret;
//    }
//	
//    /**
//     * @return A list of absolute paths to all hatch pattern files found.
//     */
//    RB_StringList getPatternList() {
//        RB_StringList ret = getFileList("patterns", "dxf");
//		return ret;
//    }
//
//    /**
//     * @return A list of absolute paths to all script files found.
//     */
//    RB_StringList getScriptList() {
//        RB_StringList ret = getFileList("scripts/python", "py");
//		return ret;
//    }
//	
//    /**
//     * @return A list of absolute paths to all machine configuration files found.
//     */
//    RB_StringList getMachineList() {
//        RB_StringList ret = getFileList("machines", "cxm");
//		return ret;
//    }
	
    /**
     * @return Absolute path to the documentation.
     */
    RB_String getDocPath() {
        RB_StringList lst = getDirectoryList("../doc");
		return lst.first();
    }

    /**
     * @return The application name.
     */
    RB_String getAppName() {
        return mAppName;
    }

    /**
     * @return The application version.
     */
    RB_String getAppVersion() {
        return mAppVersion;
    }

//    RB_StringList getFileList(const RB_String& subDirectory,
//                              const RB_String& fileExtension);
							  
    RB_StringList getDirectoryList(const RB_String& subDirectory);
				  
//	RB_StringList getLanguageList() {
//		return languageList;
//	}
//	
//	static RB_String languageToSymbol(const RB_String& lang);
//	static RB_String symbolToLanguage(const RB_String& symb);
//
//	static RB_String getEncoding(const RB_String& str);
//
//	void loadTranslation(const RB_String& lang, const RB_String& langCmd);
//
//    static bool test();
//
//	/** Returns ISO code for given locale. Needed for win32 to convert 
//	 from system encodings. */
//	static QString localeToISO(const QString& locale);
//
//
protected:
    RB_System() {
        initialized = false;
    }

    static RB_System* uniqueInstance;

    RB_String mAppName;
    RB_String mAppVersion;
    RB_String mAppDirName;
    RB_String mAppDir;
	
    //! List of available translations TODO: implement
//	RB_StringList languageList;
	
    bool initialized;
    
};

#endif // RB_System
