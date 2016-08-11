/****************************************************************************
** $Id: gv_dimangular.h 7802 2008-01-03 10:30:18Z andrew $
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


#ifndef GV_DIMANGULAR_H
#define GV_DIMANGULAR_H

#include "gv_dimension.h"

#define GV_DIMANGULAR_DEFINITIONPOINT_X QObject::tr(GV_DimAngular::propertyDefinitionPointX)
#define GV_DIMANGULAR_DEFINITIONPOINT_Y QObject::tr(GV_DimAngular::propertyDefinitionPointY)
#define GV_DIMANGULAR_DEFINITIONPOINT1_X QObject::tr(GV_DimAngular::propertyDefinitionPoint1X)
#define GV_DIMANGULAR_DEFINITIONPOINT1_Y QObject::tr(GV_DimAngular::propertyDefinitionPoint1Y)
#define GV_DIMANGULAR_DEFINITIONPOINT2_X QObject::tr(GV_DimAngular::propertyDefinitionPoint2X)
#define GV_DIMANGULAR_DEFINITIONPOINT2_Y QObject::tr(GV_DimAngular::propertyDefinitionPoint2Y)
#define GV_DIMANGULAR_DEFINITIONPOINT3_X QObject::tr(GV_DimAngular::propertyDefinitionPoint3X)
#define GV_DIMANGULAR_DEFINITIONPOINT3_Y QObject::tr(GV_DimAngular::propertyDefinitionPoint3Y)
#define GV_DIMANGULAR_DEFINITIONPOINT4_X QObject::tr(GV_DimAngular::propertyDefinitionPoint4X)
#define GV_DIMANGULAR_DEFINITIONPOINT4_Y QObject::tr(GV_DimAngular::propertyDefinitionPoint4Y)

/**
 * Holds the data that defines a angular dimension entity.
 */
class GV_DimAngularData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimAngularData() {}

    /**
     * Constructor with initialisation.
     *
     * @param definitionPoint Definition point of the angular dimension. 
     * @param leader Leader length.
     */
    GV_DimAngularData(const GV_Vector& definitionPoint1,
                      const GV_Vector& definitionPoint2,
                      const GV_Vector& definitionPoint3,
                      const GV_Vector& definitionPoint4) {
        this->definitionPoint1 = definitionPoint1;
        this->definitionPoint2 = definitionPoint2;
        this->definitionPoint3 = definitionPoint3;
        this->definitionPoint4 = definitionPoint4;
    }

public:
    /** Definition point 1. */
    GV_Vector definitionPoint1;
    /** Definition point 2. */
    GV_Vector definitionPoint2;
    /** Definition point 3. */
    GV_Vector definitionPoint3;
    /** Definition point 4. */
    GV_Vector definitionPoint4;
};



/**
 * Class for angular dimension entities.
 *
 * @author Andrew Mustun
 */
class GV_DimAngular : public GV_Dimension {
public:
    GV_DimAngular(GV_EntityContainer* parentContainer,
                 const GV_DimensionData& d,
                 const GV_DimAngularData& ed);
    virtual ~GV_DimAngular() {}

    virtual GV_Entity* clone() {
        GV_DimAngular* d = new GV_DimAngular(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /** @return GV2::EntityDimAngular */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimAngular;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimAngular || GV_Dimension::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Angular Dimension");
    }

    /**
     * @return Copy of data that defines the angular dimension. 
     * @see getData()
     */
    GV_DimAngularData getEData() const {
        return edata;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    virtual QString getMeasuredLabel();
    double getAngle();
    GV_Vector getCenter();
    bool getAngles(double& ang1, double& ang2, bool& reversed,
        GV_Vector& p1, GV_Vector& p2);

    virtual void update(bool autoText=false);

    GV_Vector getDefinitionPoint1() {
        return edata.definitionPoint1;
    }
    GV_Vector getDefinitionPoint2() {
        return edata.definitionPoint2;
    }
    GV_Vector getDefinitionPoint3() {
        return edata.definitionPoint3;
    }
    GV_Vector getDefinitionPoint4() {
        return edata.definitionPoint4;
    }

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);

    static const char* propertyDefinitionPointX;
    static const char* propertyDefinitionPointY;
    static const char* propertyDefinitionPoint1X;
    static const char* propertyDefinitionPoint1Y;
    static const char* propertyDefinitionPoint2X;
    static const char* propertyDefinitionPoint2Y;
    static const char* propertyDefinitionPoint3X;
    static const char* propertyDefinitionPoint3Y;
    static const char* propertyDefinitionPoint4X;
    static const char* propertyDefinitionPoint4Y;

protected:
    /** Extended data. */
    GV_DimAngularData edata;
};

#endif
