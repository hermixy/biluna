/****************************************************************************
** $Id: rs_dimlinear.h 7802 2008-01-03 10:30:18Z andrew $
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
** not clear to you.
**
**********************************************************************/


#ifndef RS_DIMLINEAR_H
#define RS_DIMLINEAR_H

#include "rs_dimension.h"

#define RS_DIMLINEAR_DEFINITIONPOINT_X QObject::tr(RS_DimLinear::propertyDefinitionPointX)
#define RS_DIMLINEAR_DEFINITIONPOINT_Y QObject::tr(RS_DimLinear::propertyDefinitionPointY)
#define RS_DIMLINEAR_EXTENSIONPOINT1_X QObject::tr(RS_DimLinear::propertyExtensionPoint1X)
#define RS_DIMLINEAR_EXTENSIONPOINT1_Y QObject::tr(RS_DimLinear::propertyExtensionPoint1Y)
#define RS_DIMLINEAR_EXTENSIONPOINT2_X QObject::tr(RS_DimLinear::propertyExtensionPoint2X)
#define RS_DIMLINEAR_EXTENSIONPOINT2_Y QObject::tr(RS_DimLinear::propertyExtensionPoint2Y)
#define RS_DIMLINEAR_ANGLE QObject::tr(RS_DimLinear::propertyAngle)

/**
 * Holds the data that defines a linear dimension entity.
 */
class RS_DimLinearData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_DimLinearData() {}

    /**
     * Constructor with initialisation.
     *
     * @para extensionPoint1 Startpoint of the first extension line.
     * @para extensionPoint2 Startpoint of the second extension line.
     * @param angle Rotation angle in rad.
     * @param oblique Oblique angle in rad.
     */
    RS_DimLinearData(const RS_Vector& extensionPoint1,
                     const RS_Vector& extensionPoint2,
                     double angle, double oblique) {
        this->extensionPoint1 = extensionPoint1;
        this->extensionPoint2 = extensionPoint2;
        this->angle = angle;
        this->oblique = oblique;
    }

    friend class RS_DimLinear;
    friend class RS_ActionDimLinear;

public:
    /** Definition point. Startpoint of the first definition line. */
    RS_Vector extensionPoint1;
    /** Definition point. Startpoint of the second definition line. */
    RS_Vector extensionPoint2;
    /** Rotation angle in rad. */
    double angle;
    /** Oblique angle in rad. */
    double oblique;
};



/**
 * Class for aligned dimension entities.
 *
 * @author Andrew Mustun
 */
class RS_DimLinear : public RS_Dimension {
public:
    RS_DimLinear(RS_EntityContainer* parentContainer,
                 const RS_DimensionData& d,
                 const RS_DimLinearData& ed);
    virtual ~RS_DimLinear() {}

    virtual RS_Entity* clone() {
        RS_DimLinear* d = new RS_DimLinear(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /**    @return RS2::EntityDimLinear */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDimLinear;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityDimLinear || RS_Dimension::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Linear Dimension");
    }

    /**
     * @return Copy of data that defines the linear dimension. 
     * @see getData()
     */
    RS_DimLinearData getEData() const {
        return edata;
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    virtual RS_String getMeasuredLabel();

    virtual void update(bool autoText=false);

    RS_Vector getExtensionPoint1() {
        return edata.extensionPoint1;
    }

    RS_Vector getExtensionPoint2() {
        return edata.extensionPoint2;
    }

    double getAngle() {
        return edata.angle;
    }

    void setAngle(double a) {
        edata.angle = RS_Math::correctAngle(a);
    }

    double getOblique() {
        return edata.oblique;
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

    static const char* propertyDefinitionPointX;
    static const char* propertyDefinitionPointY;
    static const char* propertyExtensionPoint1X;
    static const char* propertyExtensionPoint1Y;
    static const char* propertyExtensionPoint2X;
    static const char* propertyExtensionPoint2Y;
    static const char* propertyAngle;

protected:
    /** Extended data. */
    RS_DimLinearData edata;
};

#endif
