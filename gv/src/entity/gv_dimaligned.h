/****************************************************************************
** $Id: gv_dimaligned.h 7816 2008-01-04 16:57:10Z andrew $
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


#ifndef GV_DIMALIGNED_H
#define GV_DIMALIGNED_H

#include "gv_dimension.h"


#define GV_DIMALIGNED_EXTENSIONPOINT1_X QObject::tr(GV_DimAligned::propertyExtensionPoint1X)
#define GV_DIMALIGNED_EXTENSIONPOINT1_Y QObject::tr(GV_DimAligned::propertyExtensionPoint1Y)
#define GV_DIMALIGNED_EXTENSIONPOINT2_X QObject::tr(GV_DimAligned::propertyExtensionPoint2X)
#define GV_DIMALIGNED_EXTENSIONPOINT2_Y QObject::tr(GV_DimAligned::propertyExtensionPoint2Y)

/**
 * Holds the data that defines an aligned dimension entity.
 */
class GV_DimAlignedData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_DimAlignedData() {}

    /**
     * Constructor with initialisation.
     *
     * @para extensionPoint1 Definition point. Startpoint of the 
     *         first extension line.
     * @para extensionPoint2 Definition point. Startpoint of the 
     *         second extension line.
     */
    GV_DimAlignedData(const GV_Vector& extensionPoint1,
                      const GV_Vector& extensionPoint2) {
        this->extensionPoint1 = extensionPoint1;
        this->extensionPoint2 = extensionPoint2;
    }

public:
    /** Definition point. Startpoint of the first extension line. */
    GV_Vector extensionPoint1;
    /** Definition point. Startpoint of the second extension line. */
    GV_Vector extensionPoint2;
};



/**
 * Class for aligned dimension entities.
 *
 * @author Andrew Mustun
 */
class GV_DimAligned : public GV_Dimension {
public:
    GV_DimAligned(GV_EntityContainer* parentContainer,
                  const GV_DimensionData& d,
                  const GV_DimAlignedData& ed);
    virtual ~GV_DimAligned() {}

    virtual GV_Entity* clone() {
        GV_DimAligned* d = new GV_DimAligned(parentContainer, data, edata);
        d->copyPropertiesFrom(this);
        d->entities = entities;
        d->entities.setAutoDelete(entities.autoDelete());
        d->autoTextPos = autoTextPos;
        d->initId();
        d->detach();
        d->refDefinitionPoint1 = refDefinitionPoint1;
        d->refDefinitionPoint2 = refDefinitionPoint2;
        return d;
    }

    /** @return GV2::EntityDimAligned */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimAligned;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimAligned || GV_Dimension::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Aligned Dimension");
    }

    /**
     * @return Copy of data that defines the aligned dimension. 
     * @see getData()
     */
    GV_DimAlignedData getEData() const {
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

    static const char* propertyExtensionPoint1X;
    static const char* propertyExtensionPoint1Y;
    static const char* propertyExtensionPoint2X;
    static const char* propertyExtensionPoint2Y;

protected:
    /** Extended data. */
    GV_DimAlignedData edata;
    GV_Vector refDefinitionPoint1;
    GV_Vector refDefinitionPoint2;
};

#endif
