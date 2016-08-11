/****************************************************************************
** $Id: gv_dimradial.h 7802 2008-01-03 10:30:18Z andrew $
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


#ifndef GV_DIMRADIAL_H
#define GV_DIMRADIAL_H

#include "gv_dimension.h"


#define GV_DIMRADIAL_DEFINITIONPOINT1_X QObject::tr(GV_DimRadial::propertyDefinitionPoint1X)
#define GV_DIMRADIAL_DEFINITIONPOINT1_Y QObject::tr(GV_DimRadial::propertyDefinitionPoint1Y)
#define GV_DIMRADIAL_DEFINITIONPOINT2_X QObject::tr(GV_DimRadial::propertyDefinitionPoint2X)
#define GV_DIMRADIAL_DEFINITIONPOINT2_Y QObject::tr(GV_DimRadial::propertyDefinitionPoint2Y)
#define GV_DIMRADIAL_LEADER QObject::tr(GV_DimRadial::propertyLeader)

/**
 * Holds the data that defines a radial dimension entity.
 */
class GV_DimRadialData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimRadialData() {}

    /**
     * Constructor with initialisation.
     *
     * @param definitionPoint Definition point of the radial dimension. 
     * @param leader Leader length.
     */
    GV_DimRadialData(const GV_Vector& definitionPoint,
                     double leader) {
        this->definitionPoint = definitionPoint;
        this->leader = leader;
    }

    friend class GV_DimRadial;

public:
    /** Definition point. */
    GV_Vector definitionPoint;
    /** Leader length. */
    double leader;
};



/**
 * Class for radial dimension entities.
 *
 * @author Andrew Mustun
 */
class GV_DimRadial : public GV_Dimension {
public:
    GV_DimRadial(GV_EntityContainer* parentContainer,
                 const GV_DimensionData& d,
                 const GV_DimRadialData& ed);
    virtual ~GV_DimRadial() {}

    virtual GV_Entity* clone() {
        GV_DimRadial* d = new GV_DimRadial(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /** @return GV2::EntityDimRadial */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimRadial;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimRadial || GV_Dimension::isOfType(t));
    }

    /**
     * @return Copy of data that defines the radial dimension. 
     * @see getData()
     */
    GV_DimRadialData getEData() const {
        return edata;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    virtual QString getMeasuredLabel();

    virtual void update(bool autoText=false);

    GV_Vector getDefinitionPoint() {
        return edata.definitionPoint;
    }
    double getLeader() {
        return edata.leader;
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

    static const char* propertyDefinitionPoint1X;
    static const char* propertyDefinitionPoint1Y;
    static const char* propertyDefinitionPoint2X;
    static const char* propertyDefinitionPoint2Y;
    static const char* propertyLeader;

protected:
    /** Extended data. */
    GV_DimRadialData edata;
};

#endif
