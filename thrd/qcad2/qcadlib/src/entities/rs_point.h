/****************************************************************************
** $Id: rs_point.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef RS_POINT_H
#define RS_POINT_H

#include "rs_atomicentity.h"


#define RS_POINT_X   QObject::tr(RS_Point::propertyX)
#define RS_POINT_Y   QObject::tr(RS_Point::propertyY)


/**
 * Holds the data that defines a point.
 */
class RS_PointData {
public:
    RS_PointData() : pos(false) {
    }
    
    RS_PointData(const RS_Vector& pos) {
        this->pos = pos;
    }

    RS_Vector pos;
};



/**
 * Class for a point entity.
 *
 * @author Andrew Mustun
 */
class RS_Point : public virtual RS_AtomicEntity {
public:
    RS_Point(RS_EntityContainer* parentContainer,
             const RS_PointData& d);

    virtual RS_Entity* clone() {
        assert(rtti()==RS2::EntityPoint);
        RS_Point* p = new RS_Point(parentContainer, data);
        p->copyPropertiesFrom(this);
        p->initId();
        return p;
    }

    /**    @return RS_ENTITY_POINT */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityPoint;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityPoint || RS_AtomicEntity::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Point");
    }
    
    /** 
     * @return Start point of the entity. 
     */
    virtual RS_Vector getStartpoint() const {
        return data.pos;
    }
    /** 
     * @return End point of the entity. 
     */
    virtual RS_Vector getEndpoint() const {
        return data.pos;
    }

    virtual void moveStartpoint(const RS_Vector& pos);

    /** @return Copy of data that defines the point. */
    RS_PointData getData() const {
        return data;
    }

    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    /** @return Position of the point */
    RS_Vector getPos() {
        return data.pos;
    }

    /** Sets a new position for this point. */
    void setPos(const RS_Vector& pos) {
        moveStartpoint(pos);
    }

    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity = true, double* dist = NULL, RS_Entity** entity = NULL);
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector& ) {
        return RS2::NoSide;
    }
    
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    
protected:
    static const char* propertyX;
    static const char* propertyY;
    
    RS_PointData data;
};

#endif
