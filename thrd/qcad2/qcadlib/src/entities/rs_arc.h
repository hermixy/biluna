/****************************************************************************
** $Id: rs_arc.h 7898 2008-01-15 17:26:19Z andrew $
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

#ifndef RS_ARC_H
#define RS_ARC_H

#include "rs_atomicentity.h"

class RS_Polyline;

#define RS_ARC_CENTER_X QObject::tr(RS_Arc::propertyCenterX)
#define RS_ARC_CENTER_Y QObject::tr(RS_Arc::propertyCenterY)
#define RS_ARC_RADIUS QObject::tr(RS_Arc::propertyRadius)
#define RS_ARC_STARTANGLE QObject::tr(RS_Arc::propertyStartangle)
#define RS_ARC_ENDANGLE QObject::tr(RS_Arc::propertyEndangle)
#define RS_ARC_REVERSED QObject::tr(RS_Arc::propertyReversed)

/**
 * Holds the data that defines an arc.
 */
class CAD_EXPORT RS_ArcData {
public:
    RS_ArcData() {}

    RS_ArcData(const RS_Vector& center,
               double radius,
               double angle1, double angle2,
               bool reversed) {

        this->center = center;
        this->radius = radius;
        this->angle1 = angle1;
        this->angle2 = angle2;
        this->reversed = reversed;
    }

    void reset() {
        center = RS_Vector(false);
        radius = 0.0;
        angle1 = 0.0;
        angle2 = 0.0;
        reversed = false;
    }

    bool isValid() {
        return (center.valid && radius>RS_TOLERANCE &&
                fabs(angle1-angle2)>RS_TOLERANCE_ANGLE);
    }

public:
    RS_Vector center;
    double radius;
    double angle1;
    double angle2;
    bool reversed;
};



/**
 * Class for an arc entity. All angles are in Rad.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Arc : public RS_AtomicEntity {
public:
    RS_Arc(RS_EntityContainer* parentContainer,
           const RS_ArcData& d);
    virtual ~RS_Arc() {}

    virtual RS_Entity* clone() {
        RS_Arc* a = new RS_Arc(parentContainer, data);
        a->copyPropertiesFrom(this);
        a->initId();
        return a;
    }

    /**    @return RS2::EntityArc */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityArc;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityArc || RS_Entity::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Arc");
    }
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }

    /** @return Copy of data that defines the arc. **/
    RS_ArcData getData() const {
        return data;
    }
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    /** Sets new arc parameters. **/
    void setData(RS_ArcData d) {
        data = d;
    }

    /** @return The center point (x) of this arc */
    RS_Vector getCenter() const {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const RS_Vector& c) {
        data.center = c;
    }
    
    /** @return The radius of this arc */
    double getRadius() const {
        return data.radius;
    }
    /** Sets new radius. */
    void setRadius(double r) {
        data.radius = fabs(r);
    }

    /** @return The start angle of this arc */
    double getAngle1() const {
        return data.angle1;
    }
    /** Sets new start angle. */
    void setAngle1(double a1) {
        data.angle1 = RS_Math::correctAngle(a1);
    }
    /** @return The end angle of this arc */
    double getAngle2() const {
        return data.angle2;
    }
    /** Sets new end angle. */
    void setAngle2(double a2) {
        data.angle2 = RS_Math::correctAngle(a2);
    }
    /** 
     * @return Direction 1. The angle at which the arc starts at 
     * the startpoint. 
     */
    double getDirection1() const {
        if (!data.reversed) {
            return RS_Math::correctAngle(data.angle1+M_PI/2.0);
        }
        else {
            return RS_Math::correctAngle(data.angle1-M_PI/2.0);
        }
    }
    /** 
     * @return Direction 2. The angle at which the arc starts at 
     * the endpoint.
     */
    double getDirection2() const {
        if (!data.reversed) {
            return RS_Math::correctAngle(data.angle2-M_PI/2.0);
        }
        else {
            return RS_Math::correctAngle(data.angle2+M_PI/2.0);
        }
    }

    /**
     * @retval true if the arc is reversed (clockwise), 
     * @retval false otherwise 
     */
    bool isReversed() const {
        return data.reversed;
    }
    /** sets the reversed status. */
    void setReversed(bool r) {
        data.reversed = r;
    }

    /** @return Start point of the entity. */
    virtual RS_Vector getStartpoint() const {
        return startpoint;
    }
    /** @return End point of the entity. */
    virtual RS_Vector getEndpoint() const {
        return endpoint;
    }
    virtual void moveStartpoint(const RS_Vector& pos);
    virtual void moveEndpoint(const RS_Vector& pos);
    
    virtual void trimStartpoint(const RS_Vector& pos);
    virtual void trimEndpoint(const RS_Vector& pos);
    
    virtual RS2::Ending getTrimPoint(const RS_Vector& coord, 
              const RS_Vector& trimPoint);

    virtual void reverse();

    RS_Vector getMiddlepoint() const;
    double getAngleLength(bool allowForZeroLength=false) const;
    virtual double getLength();
    double getBulge() const;
    
    bool createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                      const RS_Vector& p3);
    bool createFrom2PDirectionRadius(const RS_Vector& startPoint, const RS_Vector& endPoint,
        double direction1, double radius);
    bool createFrom2PBulge(const RS_Vector& startPoint, const RS_Vector& endPoint,
        double bulge);

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
    
    RS_Polyline* approximateWithLines(double segmentLength);
    
    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    virtual void calculateEndpoints();
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    
    static const char* propertyCenterX;
    static const char* propertyCenterY;
    static const char* propertyRadius;
    static const char* propertyStartangle;
    static const char* propertyEndangle;
    static const char* propertyReversed;

protected:
    RS_ArcData data;

    /**
     * Startpoint. This is redundant but stored for performance 
     * reasons.
     */
    RS_Vector startpoint;
    /**
     * Endpoint. This is redundant but stored for performance 
     * reasons.
     */
    RS_Vector endpoint;
    
};

#endif
