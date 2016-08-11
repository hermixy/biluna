/****************************************************************************
** $Id: gv_settings.h 5739 2007-07-02 14:22:01Z andrew $
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


#ifndef GV_SETTINGS_H
#define GV_SETTINGS_H

#include <iostream>
#include <qglobal.h>

#if QT_VERSION>=0x030000
#include <qsettings.h>
#endif

#include <QString>
#include "gv_hash.h"

#define GV_SETTINGS GV_Settings::instance()
#define QVariant QVariant

/**
 * This class can store and reload settings from a 
 * configuration file or the windoze registry.
 * Please note that the Qt default implementation doesn't
 * work as one would expect. That's why this class overwrites
 * most of the default behaviour.
 * 
 */
class GV_Settings {
public:
    GV_Settings();

public:
    /**
     * @return Instance to the unique settings object.
     */
    static GV_Settings* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new GV_Settings();
        }
        return uniqueInstance;
    }

    /**
     * Initialize the system.
     *
     * @param appName Application name
     * @param appDirName Application directory name used for 
     *     subdirectories in /usr, /etc ~/.
     */
    void init(const QString& companyKey, const QString& appKey);

    void beginGroup(const QString& group);
    void endGroup();

    bool writeEntry(const QString& key, int value);
    bool writeEntry(const QString& key, double value);
    bool writeEntry(const QString& key, const QString& value);
    QString readEntry(const QString& key,
                        const QString& def = QString::null,
                        bool* ok = 0);
    double readDoubleEntry(const QString& key,
                        double def = 0.0,
                        bool* ok = 0);
    int readNumEntry(const QString& key, int def=0, bool* ok=0);
    
    QString readEntryCache(const QString& key);
    void addToCache(const QString& key, const QString& value);

    QVariant value(const QString& key, 
        const QVariant& defaultValue = QVariant());
    void setValue(const QString& key, const QVariant& value);

public:
    ~GV_Settings();

protected:
    static GV_Settings* uniqueInstance;

    GV_Hash<QString, QString> cache;
    QString companyKey;
    QString appKey;
    QString group;
    bool initialized;
};

#endif

