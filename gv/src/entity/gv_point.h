/****************************************************************************
** $Id: gv_point.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef GV_POINT_H
#define GV_POINT_H

#include "gv_atomicentity.h"


#define GV_POINT_X   QObject::tr(GV_Point::propertyX)
#define GV_POINT_Y   QObject::tr(GV_Point::propertyY)


/**
 * Holds the data that defines a point.
 */
class GV_PointData {
public:
    GV_PointData() : pos(false) {
    }
    
    GV_PointData(const GV_Vector& pos) {
        this->pos = pos;
    }

    GV_Vector pos;
};



/**
 * Class for a point entity.
 *
 * @author Andrew Mustun
 */
class GV_Point : public virtual GV_AtomicEntity {
public:
    GV_Point(GV_EntityContainer* parentContainer,
             const GV_PointData& d);

    virtual GV_Entity* clone() {
        assert(rtti()==GV2::EntityPoint);
        GV_Point* p = new GV_Point(parentContainer, data);
        p->copyPropertiesFrom(this);
        p->initId();
        return p;
    }

    /**    @return GV_ENTITY_POINT */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityPoint;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityPoint || GV_AtomicEntity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Point");
    }
    
    /** 
     * @return Start point of the entity. 
     */
    virtual GV_Vector getStartpoint() const {
        return data.pos;
    }
    /** 
     * @return End point of the entity. 
     */
    virtual GV_Vector getEndpoint() const {
        return data.pos;
    }

    virtual void moveStartpoint(const GV_Vector& pos);

    /** @return Copy of data that defines the point. */
    GV_PointData getData() const {
        return data;
    }

    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    /** @return Position of the point */
    GV_Vector getPos() {
        return data.pos;
    }

    /** Sets a new position for this point. */
    void setPos(const GV_Vector& pos) {
        moveStartpoint(pos);
    }

    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity = true, double* dist = NULL, GV_Entity** entity = NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector& ) {
        return GV2::NoSide;
    }
    
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
protected:
    static const char* propertyX;
    static const char* propertyY;
    
    GV_PointData data;
};

#endif
