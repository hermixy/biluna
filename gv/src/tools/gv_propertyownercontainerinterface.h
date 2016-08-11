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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_PROPERTYOWNERCONTAINERINTERFACE_H
#define GV_PROPERTYOWNERCONTAINERINTERFACE_H

#include "gv_propertyowner.h"

/**
 * Classes that implement this interface have to provide methods to
 * iterate through a list of property owner items and to change
 * properties of those items.
 *
 * @author Andrew Mustun
 */
class GV_PropertyOwnerContainerInterface {
public:
    GV_PropertyOwnerContainerInterface() {}
    virtual ~GV_PropertyOwnerContainerInterface() {}

    virtual GV_PropertyOwner* firstPropertyOwner() = 0;
    virtual GV_PropertyOwner* nextPropertyOwner() = 0;

    virtual void changeProperty(const QString& propertyName, 
        const QVariant& propertyValue) = 0;

    /**
     * Implementations can return a hash of key/variants pairs to 
     * indicate attributes about a certain property (such as
     * the preferred property editor (string, int, double, ...),
     * if a redraw of the graphic view is required if this property
     * changes, if this property affects other properties, 
     */
    virtual GV_PropertyAttributes getPropertyAttributesOfChildren(const QString& propertyName) {
        GV_PropertyAttributes ret;

        for (GV_PropertyOwner* p=firstPropertyOwner(); 
            p!=NULL; 
            p=nextPropertyOwner()) {

            GV_PropertyAttributes pa = 
                p->getPropertyAttributes(propertyName);

            ret.mixWith(pa);

            /*
            GV_HashIterator<QString,QVariant> i(attributeMap);
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
