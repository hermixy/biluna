/****************************************************************************
** $Id: gv_circle.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef GV_CIRCLE_H
#define GV_CIRCLE_H

#include "gv_atomicentity.h"


#define GV_CIRCLE_CENTER_X QObject::tr(GV_Circle::propertyCenterX)
#define GV_CIRCLE_CENTER_Y QObject::tr(GV_Circle::propertyCenterY)
#define GV_CIRCLE_RADIUS QObject::tr(GV_Circle::propertyRadius)


/**
 * Holds the data that defines a circle.
 */
class GV_CircleData {
public:
    GV_CircleData() {}

    GV_CircleData(const GV_Vector& center,
                  double radius) {

        this->center = center;
        this->radius = radius;
    }

    void reset() {
        center = GV_Vector(false);
        radius = 0.0;
    }

    bool isValid() {
        return (center.valid && radius>GV_TOLERANCE);
    }

public:
    GV_Vector center;
    double radius;
};



/**
 * Class for a circle entity.
 *
 * @author Andrew Mustun
 */
class GV_Circle : public GV_AtomicEntity {
public:
    GV_Circle (GV_EntityContainer* parentContainer,
               const GV_CircleData& d);
    virtual ~GV_Circle() {}

    virtual GV_Entity* clone() {
        GV_Circle* c = new GV_Circle(parentContainer, data);
        c->copyPropertiesFrom(this);
        c->initId();
        return c;
    }

    /**    @return GV2::EntityCircle */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityCircle;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityCircle || GV_AtomicEntity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Circle");
    }
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }

    /** @return Copy of data that defines the circle. **/
    GV_CircleData getData() {
        return data;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    virtual GV_Vector getStartpoint() const {
        return data.center + GV_Vector(data.radius, 0.0);
    }
    virtual GV_Vector getEndpoint() const {
        return data.center + GV_Vector(data.radius, 0.0);
    }
    /** 
     * @return Direction 1. The angle at which the arc starts at 
     * the startpoint. 
     */
    double getDirection1() const {
        return M_PI/2.0;
    }
    /** 
     * @return Direction 2. The angle at which the arc starts at 
     * the endpoint.
     */
    double getDirection2() const {
        return M_PI/2.0*3.0;
    }

    /** @return The center point (x) of this arc */
    GV_Vector getCenter() {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const GV_Vector& c) {
        data.center = c;
    }
    /** @return The radius of this arc */
    double getRadius() {
        return data.radius;
    }
    /** Sets new radius. */
    void setRadius(double r) {
        data.radius = r;
    }
    double getAngleLength() const;
    virtual double getLength();

    bool createFromCR(const GV_Vector& c, double r);
    bool createFrom2P(const GV_Vector& p1, const GV_Vector& p2);
    bool createFrom3P(const GV_Vector& p1, const GV_Vector& p2,
                      const GV_Vector& p3);

    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity = true, double* dist = NULL, GV_Entity** entity=NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     bool startp);
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector& coord);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    static const char* propertyCenterX;
    static const char* propertyCenterY;
    static const char* propertyRadius;

protected:
    GV_CircleData data;
};

#endif
