/****************************************************************************
** $Id: rs_entity.cpp 4580 2007-02-15 12:44:45Z andrew $
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
** not clear to you.
**
**********************************************************************/


#include "rs_propertyowner.h"

#include "rs_debug.h"
#include "rs_application.h"

/**
 * Returns the property with the given name or 'def'.
 *
 * @param name Property name.
 * @param def Default value to return if the given property is undefined.
 */
RS_Variant RS_PropertyOwner::getProperty(
    const RS_String& name, const RS_Variant& def) {

    if (properties.contains(name)) {
        debugProperty("RS_PropertyOwner::getProperty", name, properties[name]);
        return properties[name];
    }
    else {
        return def;
    }
}



/**
 * @return Names of all known properties that are defined for this entity.
 */
RS_StringList RS_PropertyOwner::getPropertyNames(bool /*includeGeometry*/) {
    return properties.keys();
}


/**
 * Sets the property with the given name to the given value.
 * A previously existing property with the same name is overwritten.
 *
 * @param name Name of the property (unique for this entity).
 * @param value Initial value of the property.
 */
void RS_PropertyOwner::setProperty(const RS_String& name, const RS_Variant& value) {
    properties.insert(name, value);
}


/**
 * Deletes the property with the given name.
 */
void RS_PropertyOwner::deleteProperty(const RS_String& name) {
    properties.remove(name);
}
    

/**
 * @return True is the given property is available.
 */
bool RS_PropertyOwner::hasProperty(const RS_String& name) {
    return getPropertyNames(true).contains(name) || 
        RS_PropertyOwner::getPropertyNames(true).contains(name);
}
    
    
bool RS_PropertyOwner::variantToBool(const RS_Variant& value) {
    // needed for False to show up in translations:
    RS_Application::translate("QtBoolPropertyManager", "False");

    return (value.toString()==
        RS_Application::translate("QtBoolPropertyManager", "True") ||
        value.toString()=="True" || value.toString()=="true" ||
        value.toString()=="TRUE");
}


void RS_PropertyOwner::debugProperty(const RS_String& fn, 
    const RS_String& name, const RS_Variant& value) {

    if (value.type()==RS_Variant::String) {
        RS_DEBUG->print("%s: "
            "property '%s' is a string with value '%s'", 
            (const char*)fn.toLatin1(),
            (const char*)name.toLatin1(),
            (const char*)value.toString().toLatin1());
    }
    else if (value.type()==RS_Variant::Bool) {
        RS_DEBUG->print("%s: "
            "property '%s' is a boolean with value '%d'", 
            (const char*)fn.toLatin1(),
            (const char*)name.toLatin1(),
            (int)value.toBool());
    }
    else if (value.type()==RS_Variant::Double) {
        RS_DEBUG->print("%s: "
            "property '%s' is a double with value '%f'", 
            (const char*)fn.toLatin1(),
            (const char*)name.toLatin1(),
            value.toDouble());
    }
}

