/****************************************************************************
** $Id: gv_arc.h 7898 2008-01-15 17:26:19Z andrew $
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

#ifndef GV_ARC_H
#define GV_ARC_H

#include "gv_atomicentity.h"

class GV_Polyline;

#define GV_ARC_CENTER_X QObject::tr(GV_Arc::propertyCenterX)
#define GV_ARC_CENTER_Y QObject::tr(GV_Arc::propertyCenterY)
#define GV_ARC_RADIUS QObject::tr(GV_Arc::propertyRadius)
#define GV_ARC_STARTANGLE QObject::tr(GV_Arc::propertyStartangle)
#define GV_ARC_ENDANGLE QObject::tr(GV_Arc::propertyEndangle)
#define GV_ARC_REVERSED QObject::tr(GV_Arc::propertyReversed)

/**
 * Holds the data that defines an arc.
 */
class GV_ArcData {
public:
    GV_ArcData() {}

    GV_ArcData(const GV_Vector& center,
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
        center = GV_Vector(false);
        radius = 0.0;
        angle1 = 0.0;
        angle2 = 0.0;
        reversed = false;
    }

    bool isValid() {
        return (center.valid && radius>GV_TOLERANCE &&
                fabs(angle1-angle2)>GV_TOLERANCE_ANGLE);
    }

public:
    GV_Vector center;
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
class GV_Arc : public GV_AtomicEntity {
public:
    GV_Arc(GV_EntityContainer* parentContainer,
           const GV_ArcData& d);
    virtual ~GV_Arc() {}

    virtual GV_Entity* clone() {
        GV_Arc* a = new GV_Arc(parentContainer, data);
        a->copyPropertiesFrom(this);
        a->initId();
        return a;
    }

    /**    @return GV2::EntityArc */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityArc;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityArc || GV_Entity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Arc");
    }
    /** @return true */
    virtual bool isEdge() const {
        return true;
    }

    /** @return Copy of data that defines the arc. **/
    GV_ArcData getData() const {
        return data;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    /** Sets new arc parameters. **/
    void setData(GV_ArcData d) {
        data = d;
    }

    /** @return The center point (x) of this arc */
    GV_Vector getCenter() const {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const GV_Vector& c) {
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
        data.angle1 = GV_Math::correctAngle(a1);
    }
    /** @return The end angle of this arc */
    double getAngle2() const {
        return data.angle2;
    }
    /** Sets new end angle. */
    void setAngle2(double a2) {
        data.angle2 = GV_Math::correctAngle(a2);
    }
    /** 
     * @return Direction 1. The angle at which the arc starts at 
     * the startpoint. 
     */
    double getDirection1() const {
        if (!data.reversed) {
            return GV_Math::correctAngle(data.angle1+M_PI/2.0);
        }
        else {
            return GV_Math::correctAngle(data.angle1-M_PI/2.0);
        }
    }
    /** 
     * @return Direction 2. The angle at which the arc starts at 
     * the endpoint.
     */
    double getDirection2() const {
        if (!data.reversed) {
            return GV_Math::correctAngle(data.angle2-M_PI/2.0);
        }
        else {
            return GV_Math::correctAngle(data.angle2+M_PI/2.0);
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
    virtual GV_Vector getStartpoint() const {
        return startpoint;
    }
    /** @return End point of the entity. */
    virtual GV_Vector getEndpoint() const {
        return endpoint;
    }
    virtual void moveStartpoint(const GV_Vector& pos);
    virtual void moveEndpoint(const GV_Vector& pos);
    
    virtual void trimStartpoint(const GV_Vector& pos);
    virtual void trimEndpoint(const GV_Vector& pos);
    
    virtual GV2::Ending getTrimPoint(const GV_Vector& coord,
              const GV_Vector& trimPoint);

    virtual void reverse();

    GV_Vector getMiddlepoint() const;
    double getAngleLength(bool allowForZeroLength=false) const;
    virtual double getLength();
    double getBulge() const;
    
    bool createFrom3P(const GV_Vector& p1, const GV_Vector& p2,
                      const GV_Vector& p3);
    bool createFrom2PDirectionRadius(const GV_Vector& startPoint, const GV_Vector& endPoint,
        double direction1, double radius);
    bool createFrom2PBulge(const GV_Vector& startPoint, const GV_Vector& endPoint,
        double bulge);

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
    
    GV_Polyline* approximateWithLines(double segmentLength);
    
    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    virtual void calculateEndpoints();
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name,
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    static const char* propertyCenterX;
    static const char* propertyCenterY;
    static const char* propertyRadius;
    static const char* propertyStartangle;
    static const char* propertyEndangle;
    static const char* propertyReversed;

protected:
    GV_ArcData data;

    /**
     * Startpoint. This is redundant but stored for performance 
     * reasons.
     */
    GV_Vector startpoint;
    /**
     * Endpoint. This is redundant but stored for performance 
     * reasons.
     */
    GV_Vector endpoint;
    
};

#endif
