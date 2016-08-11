/****************************************************************************
** $Id: gv_entity.h 4573 2007-02-13 20:50:49Z andrew $
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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


#ifndef GV_PROPERTYOWNER_H
#define GV_PROPERTYOWNER_H

#include <QVariant>
#include "gv_hash.h"
#include <QString>
#include <QStringList>


/**
 * A property ID. Convenience wrapper for |-separated strings
 * that are used as property IDs.
 */
class GV_PropertyId {
public:
    GV_PropertyId(const QString& str) {
        QStringList nameList = str.split('|');
        if (nameList.count()>1) {
            groupTitle = nameList[0];
            propertyTitle = nameList[nameList.count()-1];
        }
        else {
            groupTitle = "";
            propertyTitle = nameList[0];
        }
    }

public:
    QString groupTitle;
    QString propertyTitle;
};


/**
 * Structure to transfer attributes about property types.
 * For example a property 'Side' might have the two choices
 * 'Left' and 'Right'.
 */
class GV_PropertyAttributes {
public:
    GV_PropertyAttributes() {
        affectsOtherProperties = false;
        isVisible = true;
        isAngle = false;
    }

    void mixWith(const GV_PropertyAttributes& other) {
        affectsOtherProperties = other.affectsOtherProperties|affectsOtherProperties;
        isVisible = other.isVisible|isVisible;
        for (int i=0; i<choices.count(); ++i) {
            if (!other.choices.contains(choices.at(i))) {
                choices.removeAll(choices.at(i));
            }
            
            GV_HashIterator<QString,QVariant> it(other.userAttributes);
            while (it.hasNext()) {
                it.next();
                if (!userAttributes.contains(it.key())) {
                    userAttributes[it.key()] = it.value();
                }
            }
        }
    }
    
public:
    QStringList choices;
    bool affectsOtherProperties;
    GV_Hash<QString,QVariant> userAttributes;
    bool isVisible;
    bool isAngle;
    // TODO: limits, decimals, ...
};



/**
 * Base class for all property owners (entities, layers, ...).
 *
 * @author Andrew Mustun
 */
class GV_PropertyOwner {
public:
    GV_PropertyOwner() {}
    virtual ~GV_PropertyOwner() {}
    
    virtual void copyPropertiesFrom(GV_PropertyOwner* po) {
        properties = po->properties;
    }

    virtual QVariant getProperty(const QString& name,
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    /*
    virtual QStringList getPropertyChoices(const QString& / *name* /) {
        return QStringList();
    }
    */
    virtual GV_PropertyAttributes getPropertyAttributes(const QString& /*name*/) {
        return GV_PropertyAttributes();
    }
    /*
    virtual GV_Hash<QString,QVariant> getPropertyAttributes(const QString& / *name* /) {
        return GV_Hash<QString,QVariant>();
    }
    */
    /*
    virtual QVariant::Type getPropertyType(const QString& name) {
        return QVariant::String;
    }
    */
    virtual void setProperty(const QString& name, const QVariant& value);
    virtual void deleteProperty(const QString& name);
    virtual bool hasProperty(const QString& name);
    virtual bool isSelectedForPropertyEditing() {
        return true;
    }

    static bool variantToBool(const QVariant& value);
    static void debugProperty(const QString& fn, 
        const QString& name, const QVariant& value);

protected:
    //! Hash of properties.
    GV_Hash<QString, QVariant> properties;
};

#endif
