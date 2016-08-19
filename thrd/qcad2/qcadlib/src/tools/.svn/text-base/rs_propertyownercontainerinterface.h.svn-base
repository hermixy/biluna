/****************************************************************************
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


#ifndef RS_PROPERTYOWNERCONTAINERINTERFACE_H
#define RS_PROPERTYOWNERCONTAINERINTERFACE_H

#include "rs_propertyowner.h"

/**
 * Classes that implement this interface have to provide methods to
 * iterate through a list of property owner items and to change
 * properties of those items.
 *
 * @author Andrew Mustun
 */
class RS_PropertyOwnerContainerInterface {
public:
    RS_PropertyOwnerContainerInterface() {}
    virtual ~RS_PropertyOwnerContainerInterface() {}

    virtual RS_PropertyOwner* firstPropertyOwner() = 0;
    virtual RS_PropertyOwner* nextPropertyOwner() = 0;

    virtual void changeProperty(const RS_String& propertyName, 
        const RS_Variant& propertyValue) = 0;

    /**
     * Implementations can return a hash of key/variants pairs to 
     * indicate attributes about a certain property (such as
     * the preferred property editor (string, int, double, ...),
     * if a redraw of the graphic view is required if this property
     * changes, if this property affects other properties, 
     */
    virtual RS_PropertyAttributes getPropertyAttributesOfChildren(const RS_String& propertyName) {
        RS_PropertyAttributes ret;

        for (RS_PropertyOwner* p=firstPropertyOwner(); 
            p!=NULL; 
            p=nextPropertyOwner()) {

            RS_PropertyAttributes pa = 
                p->getPropertyAttributes(propertyName);

            ret.mixWith(pa);

            /*
            RS_HashIterator<RS_String,RS_Variant> i(attributeMap);
            while (i.hasNext()) {
                i.next();
                if (!ret.contains(i.key())) {
                    ret[i.key()] = i.value();
                }
            }
            */
        }

        return ret;
    }
};

#endif
