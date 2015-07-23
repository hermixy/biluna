/*****************************************************************
 * $Id: rb_settings.cpp 1680 2012-06-23 15:08:20Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_settings.h"
#include <iostream>

RB_Settings* RB_Settings::uniqueInstance = NULL;

RB_Settings::RB_Settings() {
    mInitialized = false;
    mCompanyKey = "";
    mAppKey = "";
    mGroup = "";
}

/**
 * Initialisation.
 * @param companyKey String that identifies the company. Must start
 *        with a "/". E.g. "/Red-Bag"
 * @param appKey String that identifies the application. Must start
 *        with a "/". E.g. "/DB"
 */
void RB_Settings::init(const RB_String& companyKey,
                       const RB_String& appKey) {

    mGroup = "";
    mAppKey = appKey;
    mCompanyKey = companyKey;

    //insertSearchPath(QSettings::Windows, mCompanyKey + mAppKey);
    //insertSearchPath(QSettings::Unix, "/usr/share/");
    mInitialized = true;
}

/**
 * Destructor
 */
RB_Settings::~RB_Settings() {}

/**
 * Begin group of settings
 * @param group group name
 */
void RB_Settings::beginGroup(const RB_String& group) {
    mGroup = group;
}

/**
 * End group of settings
 */
void RB_Settings::endGroup() {
    mGroup = "";
}

/**
 * Write integer entry to ini vile and cache
 * @param key key or id of value
 * @param value value of the key
 * @return true on success
 */
bool RB_Settings::writeEntry(const RB_String& key, int value) {
    RB_String s = RB_String("%1").arg(value);
    return writeEntry(key, s);
}

/**
 * Write double entry to ini file and to cache
 * @param key key or id of value
 * @param value value of the key
 * @return true on success
 */
bool RB_Settings::writeEntry(const RB_String& key, double value) {
    RB_String s = RB_String("%1").arg(value);
    return writeEntry(key, s);
}

/**
 * Write string entry to ini file and cache
 * @param key key or id of value
 * @param value value of the key
 * @return true on success
 */
bool RB_Settings::writeEntry(const RB_String& key, const RB_String& value) {
    //bool ret;
    QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
    //s.insertSearchPath(QSettings::Windows, mCompanyKey);

    //ret = s.writeEntry(QString("%1%2%3").arg(mAppKey).arg(group).arg(key), value);
    s.beginGroup(mGroup);
    s.setValue(key, value);
    s.endGroup();

	addToCache(key, value);
    return true;
}

/**
 * Get value of key from ini file
 * @param key key or id of value
 * @param defaultValue default value of the key
 * @return variant
 */
RB_Variant RB_Settings::value(const RB_String& key,
    const RB_Variant& defaultValue) {

    QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
    s.beginGroup(mGroup);
    RB_Variant ret = s.value(key, defaultValue);
    s.endGroup();
    return ret;
}
    
/**
 * Set value of key in ini file
 * @param key key or id of value
 * @param value value of the key
 */
void RB_Settings::setValue(const RB_String& key, const RB_Variant& value) {
    QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
    s.beginGroup(mGroup);
    s.setValue(key, value);
    s.endGroup();
}

/**
 * Clear all settings
 */
void RB_Settings::clear() {
    QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
    s.clear();
}

/**
 * @returns company key (name), as set in main.h
 */
RB_String RB_Settings::getCompanyKey() const {
    return mCompanyKey;
}

/**
 * @returns application key (name), as set in main.h
 */
RB_String RB_Settings::getAppKey() const {
    return mAppKey;
}

/**
 * Read string entry from cache or otherwise from ini file. If value was not
 * readable from cache it is now added to the cache
 * @param key key or id of value
 * @param def default value of the key
 * @param ok ok is set to true (variant can always be converted to a string)
 * @return string value
 */
RB_String RB_Settings::readEntry(const RB_String& key,
                                 const RB_String& def,
                                 bool* ok) {
    if (ok) *ok=true;

    // lookup:
    RB_String ret = readEntryCache(key);
    if (ret==RB_String::null) {
        QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
        //s.insertSearchPath(QSettings::Windows, mCompanyKey);

        s.beginGroup(mGroup);
        ret = s.value(key, def).toString();
        s.endGroup();
        //ret = s.readEntry(QString("%1%2%3").arg(mAppKey).arg(mGroup).arg(key),
		//	def, ok);
		
		addToCache(key, ret);
    }

    return ret;
}

/**
 * Read integer entry
 * @param key key or id of value
 * @param def default value of the key
 * @param ok If ok is non-null: *ok is set to true if the value could be
 *          converted to an int; otherwise *ok is set to false.
 * @return integer value
 */
int RB_Settings::readNumEntry(const RB_String& key, int def, bool* ok) {
    // lookup:
    RB_String sret = readEntryCache(key);
    if (sret==RB_String::null) {
    	int ret;
        QSettings s(QSettings::IniFormat, QSettings::UserScope, mCompanyKey, mAppKey);
        //s.insertSearchPath(QSettings::Windows, mCompanyKey);

        //ret = s.readNumEntry(QString("%1%2%3").arg(mAppKey).arg(mGroup).arg(key),
		//	def, ok);
        s.beginGroup(mGroup);
        ret = s.value(key, def).toInt(ok);
        s.endGroup();
		addToCache(key, RB_String("%1").arg(ret));
		return ret;
	}
	else {
        if (ok) {
            *ok=true;
        }
    	return sret.toInt();
	}
}

/**
 * Read string value from cache
 * @param key key or id of value
 * @return string value
 */
RB_String RB_Settings::readEntryCache(const RB_String& key) {
    RB_Hash<QString, QString>::const_iterator i = mCache.find(key);
    if (i == mCache.end()) {
		return RB_String::null;
	}
	else {
		return i.value();
	}
}

/**
 * Add string value to cache only
 * @param key key or id of value
 * @param value string value
 */
void RB_Settings::addToCache(const RB_String& key, const RB_String& value) {
    mCache[key] = value;
}
