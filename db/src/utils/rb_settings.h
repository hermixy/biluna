/*****************************************************************
 * $Id: rb_settings.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SETTINGS_H
#define RB_SETTINGS_H

// #include <iostream>
#include <qglobal.h>
#include <QSettings>

#include "db_global.h"
#include "rb_string.h"
#include "rb_hash.h"

#define RB_SETTINGS RB_Settings::instance()
#define RB_Variant QVariant

/**
 * This class can store and reload settings from a 
 * configuration file or the windoze registry.
 * Please note that the Qt default implementation doesn't
 * work as one would expect. That's why this class overwrites
 * most of the default behaviour.
 * Example use: RB_SETTINGS::writeEntry(key, value);
 */
class DB_EXPORT RB_Settings {

public:
    /**
     * @return Instance to the unique settings object.
     */
    static RB_Settings* instance() {
        if (uniqueInstance==NULL) {
            uniqueInstance = new RB_Settings();
        }
        return uniqueInstance;
    }
    ~RB_Settings();

    void init(const RB_String& companyKey, const RB_String& appKey);

    void beginGroup(const RB_String& group);
    void endGroup();

    bool writeEntry(const RB_String& key, int value);
    bool writeEntry(const RB_String& key, double value);
    bool writeEntry(const RB_String& key, const RB_String& value);
    RB_String readEntry(const RB_String& key,
                        const RB_String& def = RB_String::null,
                        bool* ok = 0);
    int readNumEntry(const RB_String& key, int def=0, bool* ok=0);
	
    RB_String readEntryCache(const RB_String& key);
	void addToCache(const RB_String& key, const RB_String& value);

    RB_Variant value(const RB_String& key, 
        const RB_Variant& defaultValue = RB_Variant());
    void setValue(const RB_String& key, const RB_Variant& value);

    void clear();

    RB_String getCompanyKey() const;
    RB_String getAppKey() const;

protected:
    RB_Settings();

    static RB_Settings* uniqueInstance;
    RB_Hash<RB_String, RB_String> mCache;
    RB_String mCompanyKey;
    RB_String mAppKey;
    RB_String mGroup;
    bool mInitialized;
};

#endif // RB_SETTINGS_H

