/****************************************************************************
** $Id: rs_dimaligned.h 7802 2008-01-03 10:30:18Z andrew $
**
** Copyright (C) 2008 RibbonSoft. All rights reserved.
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


#ifndef RS_DIMORDINATE_H
#define RS_DIMORDINATE_H

#include "rs_dimension.h"


#define RS_DIMORDINATE_DEFINITIONPOINT1_X QObject::tr(RS_DimOrdinate::propertyDefPoint1X)
#define RS_DIMORDINATE_DEFINITIONPOINT1_Y QObject::tr(RS_DimOrdinate::propertyDefPoint1Y)
#define RS_DIMORDINATE_DEFINITIONPOINT2_X QObject::tr(RS_DimOrdinate::propertyDefPoint2X)
#define RS_DIMORDINATE_DEFINITIONPOINT2_Y QObject::tr(RS_DimOrdinate::propertyDefPoint2Y)
#define RS_DIMORDINATE_TYPE QObject::tr(RS_DimOrdinate::propertyType)

#define RS_DIMORDINATE_TYPE_X  QObject::tr(RS_DimOrdinate::valueTypeX)
#define RS_DIMORDINATE_TYPE_Y  QObject::tr(RS_DimOrdinate::valueTypeY)

/**
 * Holds the data that defines an aligned dimension entity.
 */
class RS_DimOrdinateData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_DimOrdinateData() {}

    /**
     * Constructor with initialisation.
     *
     * @para extensionPoint1 Definition point. Startpoint of the 
     *         leader.
     * @para extensionPoint2 Definition point. Endpoint of the 
     *         leader.
     */
    RS_DimOrdinateData(const RS_Vector& defPoint1,
                      const RS_Vector& defPoint2,
                      bool xType) {
        this->defPoint1 = defPoint1;
        this->defPoint2 = defPoint2;
        this->xType = xType;
    }

public:
    /** Definition point 1. */
    RS_Vector defPoint1;
    /** Definition point 2. */
    RS_Vector defPoint2;
    /** True: measures x-value, false: y-value */
    bool xType;
};



/**
 * Class for aligned dimension entities.
 *
 * @author Andrew Mustun
 */
class RS_DimOrdinate : public RS_Dimension {
public:
    RS_DimOrdinate(RS_EntityContainer* parentContainer,
                  const RS_DimensionData& d,
                  const RS_DimOrdinateData& ed);
    virtual ~RS_DimOrdinate() {}

    virtual RS_Entity* clone() {
        RS_DimOrdinate* d = new RS_DimOrdinate(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /** @return RS2::EntityDimOrdinate */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDimOrdinate;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityDimOrdinate || RS_Dimension::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Ordinate Dimension");
    }

    /**
     * @return Copy of data that defines the aligned dimension. 
     * @see getData()
     */
    RS_DimOrdinateData getEData() const {
        return edata;
    }

    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    virtual RS_String getMeasuredLabel();

    virtual void update(bool autoText=false);

    RS_Vector getDefinitionPoint1() {
        return edata.defPoint1;
    }

    RS_Vector getDefinitionPoint2() {
        return edata.defPoint2;
    }

    bool isXType() {
        return edata.xType;
    }
    
    virtual bool hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2);

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    virtual RS_PropertyAttributes getPropertyAttributes(
        const RS_String& propertyName);

    static const char* propertyDefPoint1X;
    static const char* propertyDefPoint1Y;
    static const char* propertyDefPoint2X;
    static const char* propertyDefPoint2Y;
    static const char* propertyType;
    
    static const char* valueTypeX;
    static const char* valueTypeY;

protected:
    /** Extended data. */
    RS_DimOrdinateData edata;
};

#endif
