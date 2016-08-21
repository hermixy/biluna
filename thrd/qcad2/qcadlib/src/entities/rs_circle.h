/****************************************************************************
** $Id: rs_circle.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef RS_CIRCLE_H
#define RS_CIRCLE_H

#include "rs_atomicentity.h"


#define RS_CIRCLE_CENTER_X QObject::tr(RS_Circle::propertyCenterX)
#define RS_CIRCLE_CENTER_Y QObject::tr(RS_Circle::propertyCenterY)
#define RS_CIRCLE_RADIUS QObject::tr(RS_Circle::propertyRadius)


/**
 * Holds the data that defines a circle.
 */
class CAD_EXPORT RS_CircleData {
public:
    RS_CircleData() {}

    RS_CircleData(const RS_Vector& center,
                  double radius) {

        this->center = center;
        this->radius = radius;
    }

    void reset() {
        center = RS_Vector(false);
        radius = 0.0;
    }

    bool isValid() {
        return (center.valid && radius>RS_TOLERANCE);
    }

public:
    RS_Vector center;
    double radius;
};



/**
 * Class for a circle entity.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Circle : public RS_AtomicEntity {
public:
    RS_Circle (RS_EntityContainer* parentContainer,
               const RS_CircleData& d);
    virtual ~RS_Circle() {}

    virtual RS_Entity* clone() {
        RS_Circle* c = new RS_Circle(parentContainer, data);
        c->copyPropertiesFrom(this);
        c->initId();
        return c;
    }

    /**    @return RS2::EntityCircle */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityCircle;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityCircle || RS_AtomicEntity::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Circle");
    }
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }

    /** @return Copy of data that defines the circle. **/
    RS_CircleData getData() {
        return data;
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    virtual RS_Vector getStartpoint() const {
        return data.center + RS_Vector(data.radius, 0.0);
    }
    virtual RS_Vector getEndpoint() const {
        return data.center + RS_Vector(data.radius, 0.0);
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
    RS_Vector getCenter() {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const RS_Vector& c) {
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

    bool createFromCR(const RS_Vector& c, double r);
    bool createFrom2P(const RS_Vector& p1, const RS_Vector& p2);
    bool createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                      const RS_Vector& p3);

    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity = true, double* dist = NULL, RS_Entity** entity=NULL);
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     bool startp);
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector& coord);

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    
    static const char* propertyCenterX;
    static const char* propertyCenterY;
    static const char* propertyRadius;

protected:
    RS_CircleData data;
};

#endif
