/****************************************************************************
** $Id: gv_entity.cpp 4580 2007-02-15 12:44:45Z andrew $
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


#include "gv_propertyowner.h"

#include "rb_debug.h"
#include "gv_application.h"

/**
 * Returns the property with the given name or 'def'.
 *
 * @param name Property name.
 * @param def Default value to return if the given property is undefined.
 */
QVariant GV_PropertyOwner::getProperty(
    const QString& name, const QVariant& def) {

    if (properties.contains(name)) {
        debugProperty("GV_PropertyOwner::getProperty", name, properties[name]);
        return properties[name];
    }
    else {
        return def;
    }
}



/**
 * @return Names of all known properties that are defined for this entity.
 */
QStringList GV_PropertyOwner::getPropertyNames(bool /*includeGeometry*/) {
    return properties.keys();
}


/**
 * Sets the property with the given name to the given value.
 * A previously existing property with the same name is overwritten.
 *
 * @param name Name of the property (unique for this entity).
 * @param value Initial value of the property.
 */
void GV_PropertyOwner::setProperty(const QString& name, const QVariant& value) {
    properties.insert(name, value);
}


/**
 * Deletes the property with the given name.
 */
void GV_PropertyOwner::deleteProperty(const QString& name) {
    properties.remove(name);
}
    

/**
 * @return True is the given property is available.
 */
bool GV_PropertyOwner::hasProperty(const QString& name) {
    return getPropertyNames(true).contains(name) || 
        GV_PropertyOwner::getPropertyNames(true).contains(name);
}
    
    
bool GV_PropertyOwner::variantToBool(const QVariant& value) {
    // needed for False to show up in translations:
    GV_Application::translate("QtBoolPropertyManager", "False");

    return (value.toString()==
        GV_Application::translate("QtBoolPropertyManager", "True") ||
        value.toString()=="True" || value.toString()=="true" ||
        value.toString()=="TRUE");
}


void GV_PropertyOwner::debugProperty(const QString& fn, 
    const QString& name, const QVariant& value) {

//    if (value.type()==QVariant::String) {
//        RB_DEBUG->print("%s: "
//            "property '%s' is a string with value '%s'",
//            (const char*)fn.toLatin1(),
//            (const char*)name.toLatin1(),
//            (const char*)value.toString().toLatin1());
//    }
//    else if (value.type()==QVariant::Bool) {
//        RB_DEBUG->print("%s: "
//            "property '%s' is a boolean with value '%d'",
//            (const char*)fn.toLatin1(),
//            (const char*)name.toLatin1(),
//            (int)value.toBool());
//    }
//    else if (value.type()==QVariant::Double) {
//        RB_DEBUG->print("%s: "
//            "property '%s' is a double with value '%f'",
//            (const char*)fn.toLatin1(),
//            (const char*)name.toLatin1(),
//            value.toDouble());
//    }
}

