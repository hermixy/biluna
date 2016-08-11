/****************************************************************************
** $Id: gv_system.h 9244 2008-02-28 18:37:58Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_SYSTEM_H
#define GV_SYSTEM_H

#include <iostream>

#include "rb_debug.h"
#include "gv_dir.h"
#include <QString>
#include <QStringList>
//Added by qt3to4:
#include <QString>

#define GV_SYSTEM GV_System::instance()

/**
 * Class for some system methods such as file system operations.
 * Implemented as singleton. Use init to Initialize the class
 * before you use it.
 *
 * @author Andrew Mustun
 */
class GV_System {
protected:
    GV_System() {
        initialized = false;
    }

public:
    /**
     * @return Instance to the unique system object.
     */
    static GV_System* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new GV_System();
        }
        return uniqueInstance;
    }

    void init(const QString& appName, const QString& appVersion, 
              const QString& appDirName, const QString& appDir="");
    void initLanguageList();

    bool checkInit();
    bool createHomePath(const QString& p);

    static QString getEnvironmentVariable(
        const QString& name, const QString& def=QString());

    /**
     * @return Users home directory.
     */
    static QString getHomeDir() {
        return GV_Dir::homePath();
    }

    /**
     * @return Current directory.
     */
    QString getCurrentDir() {
        return GV_Dir::currentPath();
    }

    /**
     * @return Application directory.
     */
    QString getAppDir() {
        return appDir;
    }

    /**
     * @return A list of absolute paths to all font files found.
     */
    QStringList getFontList() {
        QStringList ret = getFileList("fonts", "cxf");
        return ret;
    }
    
    /**
     * @return A list of absolute paths to all hatch pattern files found.
     */
    QStringList getPatternList() {
        QStringList ret = getFileList("patterns", "dxf");
        return ret;
    }

    /**
     * @return A list of absolute paths to all script files found.
     */
    QStringList getScriptList() {
        QStringList ret = getFileList("scripts/qsa", "qs");
        return ret;
    }
    
    /**
     * @return A list of absolute paths to all machine configuration files found.
     */
    QStringList getMachineList() {
        QStringList ret = getFileList("machines", "cxm");
        return ret;
    }
    
    /**
     * @return Absolute path to the documentation.
     */
    QString getDocPath() {
        QStringList lst = getDirectoryList("doc");
        return lst.first();
    }

    /**
     * @return The application name.
     */
    QString getAppName() {
        return appName;
    }

    /**
     * @return The application version.
     */
    QString getAppVersion() {
        return appVersion;
    }

    QStringList getFileList(const QString& subDirectory,
                              const QString& fileExtension);
                              
    QStringList getDirectoryList(const QString& subDirectory);
                              
    QStringList getLanguageList() {
        return languageList;
    }
    
    static QString languageToSymbol(const QString& lang);
    static QString symbolToLanguage(const QString& symb);

    static QString getEncoding(const QString& str);

    void loadTranslation(const QString& lang/*, const QString& langCmd*/);

    static bool test();

    /** Returns ISO code for given locale. Needed for win32 to convert 
     from system encodings. */
    static QString localeToISO(const QString& locale);

    static QString fixCrippledWindowsPath(const QString& crippled);

protected:
    static GV_System* uniqueInstance;

    QString appName;
    QString appVersion;
    QString appDirName;
    QString appDir;
    QString projDir;
    QString projDirName;
    
    //! List of available translations
    QStringList languageList;
    
    bool initialized;
};

#endif

