/****************************************************************************
** $Id: rs_dimradial.h 7802 2008-01-03 10:30:18Z andrew $
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


#ifndef RS_DIMRADIAL_H
#define RS_DIMRADIAL_H

#include "rs_dimension.h"


#define RS_DIMRADIAL_DEFINITIONPOINT1_X QObject::tr(RS_DimRadial::propertyDefinitionPoint1X)
#define RS_DIMRADIAL_DEFINITIONPOINT1_Y QObject::tr(RS_DimRadial::propertyDefinitionPoint1Y)
#define RS_DIMRADIAL_DEFINITIONPOINT2_X QObject::tr(RS_DimRadial::propertyDefinitionPoint2X)
#define RS_DIMRADIAL_DEFINITIONPOINT2_Y QObject::tr(RS_DimRadial::propertyDefinitionPoint2Y)
#define RS_DIMRADIAL_LEADER QObject::tr(RS_DimRadial::propertyLeader)

/**
 * Holds the data that defines a radial dimension entity.
 */
class CAD_EXPORT RS_DimRadialData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_DimRadialData() {}

    /**
     * Constructor with initialisation.
     *
     * @param definitionPoint Definition point of the radial dimension. 
     * @param leader Leader length.
     */
    RS_DimRadialData(const RS_Vector& definitionPoint,
                     double leader) {
        this->definitionPoint = definitionPoint;
        this->leader = leader;
    }

    friend class RS_DimRadial;

public:
    /** Definition point. */
    RS_Vector definitionPoint;
    /** Leader length. */
    double leader;
};



/**
 * Class for radial dimension entities.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_DimRadial : public RS_Dimension {
public:
    RS_DimRadial(RS_EntityContainer* parentContainer,
                 const RS_DimensionData& d,
                 const RS_DimRadialData& ed);
    virtual ~RS_DimRadial() {}

    virtual RS_Entity* clone() {
        RS_DimRadial* d = new RS_DimRadial(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        return d;
    }

    /** @return RS2::EntityDimRadial */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityDimRadial;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityDimRadial || RS_Dimension::isOfType(t));
    }

    /**
     * @return Copy of data that defines the radial dimension. 
     * @see getData()
     */
    RS_DimRadialData getEData() const {
        return edata;
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    virtual RS_String getMeasuredLabel();

    virtual void update(bool autoText=false);

    RS_Vector getDefinitionPoint() {
        return edata.definitionPoint;
    }
    double getLeader() {
        return edata.leader;
    }

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);

    static const char* propertyDefinitionPoint1X;
    static const char* propertyDefinitionPoint1Y;
    static const char* propertyDefinitionPoint2X;
    static const char* propertyDefinitionPoint2Y;
    static const char* propertyLeader;

protected:
    /** Extended data. */
    RS_DimRadialData edata;
};

#endif
