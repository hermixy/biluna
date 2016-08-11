/****************************************************************************
** $Id: gv_dimaligned.h 7802 2008-01-03 10:30:18Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_DIMORDINATE_H
#define GV_DIMORDINATE_H

#include "gv_dimension.h"


#define GV_DIMORDINATE_DEFINITIONPOINT1_X QObject::tr(GV_DimOrdinate::propertyDefPoint1X)
#define GV_DIMORDINATE_DEFINITIONPOINT1_Y QObject::tr(GV_DimOrdinate::propertyDefPoint1Y)
#define GV_DIMORDINATE_DEFINITIONPOINT2_X QObject::tr(GV_DimOrdinate::propertyDefPoint2X)
#define GV_DIMORDINATE_DEFINITIONPOINT2_Y QObject::tr(GV_DimOrdinate::propertyDefPoint2Y)
#define GV_DIMORDINATE_TYPE QObject::tr(GV_DimOrdinate::propertyType)

#define GV_DIMORDINATE_TYPE_X  QObject::tr(GV_DimOrdinate::valueTypeX)
#define GV_DIMORDINATE_TYPE_Y  QObject::tr(GV_DimOrdinate::valueTypeY)

/**
 * Holds the data that defines an aligned dimension entity.
 */
class GV_DimOrdinateData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimOrdinateData() {}

    /**
     * Constructor with initialisation.
     *
     * @para extensionPoint1 Definition point. Startpoint of the 
     *         leader.
     * @para extensionPoint2 Definition point. Endpoint of the 
     *         leader.
     */
    GV_DimOrdinateData(const GV_Vector& defPoint1,
                      const GV_Vector& defPoint2,
                      bool xType) {
        this->defPoint1 = defPoint1;
        this->defPoint2 = defPoint2;
        this->xType = xType;
    }

public:
    /** Definition point 1. */
    GV_Vector defPoint1;
    /** Definition point 2. */
    GV_Vector defPoint2;
    /** True: measures x-value, false: y-value */
    bool xType;
};



/**
 * Class for aligned dimension entities.
 *
 * @author Andrew Mustun
 */
class GV_DimOrdinate : public GV_Dimension {
public:
    GV_DimOrdinate(GV_EntityContainer* parentContainer,
                  const GV_DimensionData& d,
                  const GV_DimOrdinateData& ed);
    virtual ~GV_DimOrdinate() {}

    virtual GV_Entity* clone() {
        GV_DimOrdinate* d = new GV_DimOrdinate(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /** @return GV2::EntityDimOrdinate */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimOrdinate;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimOrdinate || GV_Dimension::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Ordinate Dimension");
    }

    /**
     * @return Copy of data that defines the aligned dimension. 
     * @see getData()
     */
    GV_DimOrdinateData getEData() const {
        return edata;
    }

    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    virtual QString getMeasuredLabel();

    virtual void update(bool autoText=false);

    GV_Vector getDefinitionPoint1() {
        return edata.defPoint1;
    }

    GV_Vector getDefinitionPoint2() {
        return edata.defPoint2;
    }

    bool isXType() {
        return edata.xType;
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
    virtual GV_PropertyAttributes getPropertyAttributes(
        const QString& propertyName);

    static const char* propertyDefPoint1X;
    static const char* propertyDefPoint1Y;
    static const char* propertyDefPoint2X;
    static const char* propertyDefPoint2Y;
    static const char* propertyType;
    
    static const char* valueTypeX;
    static const char* valueTypeY;

protected:
    /** Extended data. */
    GV_DimOrdinateData edata;
};

#endif
