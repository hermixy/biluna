/****************************************************************************
** $Id: rs_entity.h 4573 2007-02-13 20:50:49Z andrew $
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


#ifndef RS_PROPERTYOWNER_H
#define RS_PROPERTYOWNER_H

#include "rs_variant.h"
#include "rs_hash.h"
#include "rs_string.h"
#include "rs_stringlist.h"


/**
 * A property ID. Convenience wrapper for |-separated strings
 * that are used as property IDs.
 */
class CAD_EXPORT RS_PropertyId {
public:
    RS_PropertyId(const RS_String& str) {
        RS_StringList nameList = str.split('|');
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
    RS_String groupTitle;
    RS_String propertyTitle;
};


/**
 * Structure to transfer attributes about property types.
 * For example a property 'Side' might have the two choices
 * 'Left' and 'Right'.
 */
class CAD_EXPORT RS_PropertyAttributes {
public:
    RS_PropertyAttributes() {
        affectsOtherProperties = false;
        isVisible = true;
        isAngle = false;
    }

    void mixWith(const RS_PropertyAttributes& other) {
        affectsOtherProperties = other.affectsOtherProperties|affectsOtherProperties;
        isVisible = other.isVisible|isVisible;
        for (int i=0; i<choices.count(); ++i) {
            if (!other.choices.contains(choices.at(i))) {
                choices.removeAll(choices.at(i));
            }
            
            RS_HashIterator<RS_String,RS_Variant> it(other.userAttributes);
            while (it.hasNext()) {
                it.next();
                if (!userAttributes.contains(it.key())) {
                    userAttributes[it.key()] = it.value();
                }
            }
        }
    }
    
public:
    RS_StringList choices;
    bool affectsOtherProperties;
    RS_Hash<RS_String,RS_Variant> userAttributes;
    bool isVisible;
    bool isAngle;
    // TODO: limits, decimals, ...
};



/**
 * Base class for all property owners (entities, layers, ...).
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_PropertyOwner {
public:
    RS_PropertyOwner() {}
    virtual ~RS_PropertyOwner() {}
    
    virtual void copyPropertiesFrom(RS_PropertyOwner* po) {
        properties = po->properties;
    }

    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    /*
    virtual RS_StringList getPropertyChoices(const RS_String& / *name* /) {
        return RS_StringList();
    }
    */
    virtual RS_PropertyAttributes getPropertyAttributes(const RS_String& /*name*/) {
        return RS_PropertyAttributes();
    }
    /*
    virtual RS_Hash<RS_String,RS_Variant> getPropertyAttributes(const RS_String& / *name* /) {
        return RS_Hash<RS_String,RS_Variant>();
    }
    */
    /*
    virtual RS_Variant::Type getPropertyType(const RS_String& name) {
        return RS_Variant::String;
    }
    */
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    virtual void deleteProperty(const RS_String& name);
    virtual bool hasProperty(const RS_String& name);
    virtual bool isSelectedForPropertyEditing() {
        return true;
    }

    static bool variantToBool(const RS_Variant& value);
    static void debugProperty(const RS_String& fn, 
        const RS_String& name, const RS_Variant& value);

protected:
    //! Hash of properties.
    RS_Hash<RS_String, RS_Variant> properties;
};

#endif
