/****************************************************************************
** $Id: gv_dimlinear.h 7802 2008-01-03 10:30:18Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_DIMLINEAR_H
#define GV_DIMLINEAR_H

#include "gv_dimension.h"

#define GV_DIMLINEAR_DEFINITIONPOINT_X QObject::tr(GV_DimLinear::propertyDefinitionPointX)
#define GV_DIMLINEAR_DEFINITIONPOINT_Y QObject::tr(GV_DimLinear::propertyDefinitionPointY)
#define GV_DIMLINEAR_EXTENSIONPOINT1_X QObject::tr(GV_DimLinear::propertyExtensionPoint1X)
#define GV_DIMLINEAR_EXTENSIONPOINT1_Y QObject::tr(GV_DimLinear::propertyExtensionPoint1Y)
#define GV_DIMLINEAR_EXTENSIONPOINT2_X QObject::tr(GV_DimLinear::propertyExtensionPoint2X)
#define GV_DIMLINEAR_EXTENSIONPOINT2_Y QObject::tr(GV_DimLinear::propertyExtensionPoint2Y)
#define GV_DIMLINEAR_ANGLE QObject::tr(GV_DimLinear::propertyAngle)

/**
 * Holds the data that defines a linear dimension entity.
 */
class GV_DimLinearData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimLinearData() {}

    /**
     * Constructor with initialisation.
     *
     * @para extensionPoint1 Startpoint of the first extension line.
     * @para extensionPoint2 Startpoint of the second extension line.
     * @param angle Rotation angle in rad.
     * @param oblique Oblique angle in rad.
     */
    GV_DimLinearData(const GV_Vector& extensionPoint1,
                     const GV_Vector& extensionPoint2,
                     double angle, double oblique) {
        this->extensionPoint1 = extensionPoint1;
        this->extensionPoint2 = extensionPoint2;
        this->angle = angle;
        this->oblique = oblique;
    }

    friend class GV_DimLinear;
    friend class GV_ActionDimLinear;

public:
    /** Definition point. Startpoint of the first definition line. */
    GV_Vector extensionPoint1;
    /** Definition point. Startpoint of the second definition line. */
    GV_Vector extensionPoint2;
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
class GV_DimLinear : public GV_Dimension {
public:
    GV_DimLinear(GV_EntityContainer* parentContainer,
                 const GV_DimensionData& d,
                 const GV_DimLinearData& ed);
    virtual ~GV_DimLinear() {}

    virtual GV_Entity* clone() {
        GV_DimLinear* d = new GV_DimLinear(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /**    @return GV2::EntityDimLinear */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimLinear;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimLinear || GV_Dimension::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Linear Dimension");
    }

    /**
     * @return Copy of data that defines the linear dimension. 
     * @see getData()
     */
    GV_DimLinearData getEData() const {
        return edata;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    virtual QString getMeasuredLabel();

    virtual void update(bool autoText=false);

    GV_Vector getExtensionPoint1() {
        return edata.extensionPoint1;
    }

    GV_Vector getExtensionPoint2() {
        return edata.extensionPoint2;
    }

    double getAngle() {
        return edata.angle;
    }

    void setAngle(double a) {
        edata.angle = GV_Math::correctAngle(a);
    }

    double getOblique() {
        return edata.oblique;
    }

    virtual bool hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);

    static const char* propertyDefinitionPointX;
    static const char* propertyDefinitionPointY;
    static const char* propertyExtensionPoint1X;
    static const char* propertyExtensionPoint1Y;
    static const char* propertyExtensionPoint2X;
    static const char* propertyExtensionPoint2Y;
    static const char* propertyAngle;

protected:
    /** Extended data. */
    GV_DimLinearData edata;
};

#endif
