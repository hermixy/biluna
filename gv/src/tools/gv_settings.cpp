/****************************************************************************
** $Id: gv_settings.cpp 5739 2007-07-02 14:22:01Z andrew $
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


#include "gv_settings.h"
#include <iostream>

GV_Settings* GV_Settings::uniqueInstance = NULL;


GV_Settings::GV_Settings() {
    initialized = false;
    companyKey = "";
    appKey = "";
    group = "";
}

/**
 * Initialisation.
 *
 * @param companyKey String that identifies the company. Must start
 *        with a "/". E.g. "/RibbonSoft"
 * @param appKey String that identifies the application. Must start
 *        with a "/". E.g. "/QCad2"
 */
void GV_Settings::init(const QString& companyKey,
                       const QString& appKey) {

    group = "";
    this->appKey = appKey;
    this->companyKey = companyKey;

    initialized = true;
}


/**
 * Destructor
 */
GV_Settings::~GV_Settings() {}



void GV_Settings::beginGroup(const QString& group) {
    this->group = group;
}

void GV_Settings::endGroup() {
    this->group = "";
}

bool GV_Settings::writeEntry(const QString& key, int value) {
    QString s = QString("%1").arg(value);
    return writeEntry(key, s);
}

bool GV_Settings::writeEntry(const QString& key, double value) {
    QString s = QString("%1").arg(value);
    return writeEntry(key, s);
}

bool GV_Settings::writeEntry(const QString& key, const QString& value) {
    QSettings s(QSettings::IniFormat, QSettings::UserScope, companyKey, appKey);

    s.beginGroup(group);
    s.setValue(key, value);
    s.endGroup();

    addToCache(key, value);
    return true;

}

QVariant GV_Settings::value(const QString& key, 
    const QVariant& defaultValue) {

    QSettings s(QSettings::IniFormat, QSettings::UserScope, companyKey, appKey);
    s.beginGroup(group);
    QVariant ret = s.value(key, defaultValue);
    s.endGroup();
    return ret;
}
    
void GV_Settings::setValue(const QString& key, const QVariant& value) {
    QSettings s(QSettings::IniFormat, QSettings::UserScope, companyKey, appKey);
    s.beginGroup(group);
    s.setValue(key, value);
    s.endGroup();
}

QString GV_Settings::readEntry(const QString& key,
                                 const QString& def,
                                 bool* ok) {
    if (ok!=NULL) {
        *ok=true;
    }

    // lookup:
    QString ret = readEntryCache(key);
    if (ret==QString::null) {
        QSettings s(QSettings::IniFormat, QSettings::UserScope, companyKey, appKey);

        s.beginGroup(group);
        ret = s.value(key, def).toString();
        s.endGroup();
        
        addToCache(key, ret);
    }

    return ret;
}



double GV_Settings::readDoubleEntry(const QString& key,
                                 double def,
                                 bool* ok) {
    if (ok!=NULL) {
        *ok=true;
    }

    QString strDef;
    strDef = QString("%1").arg(def);

    QString ret = readEntry(key, strDef, ok);

    if (ok==NULL || *ok==true) {
        return ret.toDouble(ok);
    }
    else {
        return def;
    }
}



int GV_Settings::readNumEntry(const QString& key, int def, bool* ok) {
    // lookup:
    QString sret = readEntryCache(key);
    if (sret==QString::null) {
        int ret;
        QSettings s(QSettings::IniFormat, QSettings::UserScope, companyKey, appKey);
        s.beginGroup(group);
        ret = s.value(key, def).toInt(ok);
        s.endGroup();
        addToCache(key, QString("%1").arg(ret));
        return ret;
    }
    else {
        if (ok) {
            *ok=true;
        }
        return sret.toInt();
    }
}


QString GV_Settings::readEntryCache(const QString& key) {
    GV_Hash<QString, QString>::const_iterator i  = cache.find(group + key);
    if (i==cache.end()) {
        return QString::null;
    }
    else {
        return i.value();
    }
}


void GV_Settings::addToCache(const QString& key, const QString& value) {
    cache[group + key] = value;
}
