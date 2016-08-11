/****************************************************************************
** $Id: gv_ellipse.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef GV_ELLIPSE_H
#define GV_ELLIPSE_H

#include "gv_atomicentity.h"

class GV_Polyline;

#define GV_ELLIPSE_CENTER_X QObject::tr(GV_Ellipse::propertyCenterX)
#define GV_ELLIPSE_CENTER_Y QObject::tr(GV_Ellipse::propertyCenterY)
#define GV_ELLIPSE_MAJORP_X QObject::tr(GV_Ellipse::propertyMajorPX)
#define GV_ELLIPSE_MAJORP_Y QObject::tr(GV_Ellipse::propertyMajorPY)
#define GV_ELLIPSE_RATIO QObject::tr(GV_Ellipse::propertyRatio)
#define GV_ELLIPSE_STARTANGLE QObject::tr(GV_Ellipse::propertyStartangle)
#define GV_ELLIPSE_ENDANGLE QObject::tr(GV_Ellipse::propertyEndangle)
#define GV_ELLIPSE_REVERSED QObject::tr(GV_Ellipse::propertyReversed)

/**
 * Holds the data that defines an ellipse.
 */
class GV_EllipseData {
public:
    GV_EllipseData(const GV_Vector& center,
                   const GV_Vector& majorP,
                   double ratio,
                   double angle1, double angle2,
                   bool reversed) {

        this->center = center;
        this->majorP = majorP;
        this->ratio = ratio;
        this->angle1 = angle1;
        this->angle2 = angle2;
        this->reversed = reversed;
    }

    friend class GV_Ellipse;

private:
    //! Ellipse center
    GV_Vector center;
    //! Endpoint of major axis relative to center.
    GV_Vector majorP;
    //! Ratio of minor axis to major axis.
    double ratio;
    //! Start angle
    double angle1;
    //! End angle
    double angle2;
    //! Reversed (cw) flag
    bool reversed;
};




/**
 * Class for an ellipse entity. All angles are in Rad.
 *
 * @author Andrew Mustun
 */
class GV_Ellipse : public GV_AtomicEntity {
public:
    GV_Ellipse(GV_EntityContainer* parentContainer,
               const GV_EllipseData& d);
    virtual ~GV_Ellipse() {}

    virtual GV_Entity* clone() {
        GV_Ellipse* e = new GV_Ellipse(parentContainer, data);
        e->copyPropertiesFrom(this);
        e->initId();
        return e;
    }

    /**    @return GV2::EntityEllipse */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityEllipse;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityEllipse || GV_AtomicEntity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Ellipse");
    }


    /** 
     * @return Start point of the entity. 
     */
    virtual GV_Vector getStartpoint() const {
        GV_Vector p;
        p.set(data.center.x + cos(data.angle1) * getMajorRadius(),
              data.center.y + sin(data.angle1) * getMinorRadius());
        p.rotate(data.center, getAngle());
        return p;
    }
    /** 
     * @return End point of the entity. 
     */
    virtual GV_Vector getEndpoint() const {
        GV_Vector p;
        p.set(data.center.x + cos(data.angle2) * getMajorRadius(),
              data.center.y + sin(data.angle2) * getMinorRadius());
        p.rotate(data.center, getAngle());
        return p;
    }
    
    virtual void moveStartpoint(const GV_Vector& pos);
    virtual void moveEndpoint(const GV_Vector& pos);

    virtual GV2::Ending getTrimPoint(const GV_Vector& coord, 
              const GV_Vector& trimPoint);

    double getEllipseAngle(const GV_Vector& pos);

    /** @return Copy of data that defines the ellipse. **/
    GV_EllipseData getData() {
        return data;
    }
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

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

    bool isFullEllipse() {
        return (GV_Math::correctAngle(data.angle1)<1.0e-6 && 
            GV_Math::correctAngle(data.angle2)>2*M_PI-1.0e-6) ||
            (fabs(GV_Math::correctAngle(data.angle1) - 
                  GV_Math::correctAngle(data.angle2))<1.0e-4);
    }

    /** @return The rotation angle of this ellipse */
    double getAngle() const {
        return data.majorP.angle();
    }

    /** @return The start angle of this arc */
    double getAngle1() {
        return data.angle1;
    }
    /** Sets new start angle. */
    void setAngle1(double a1) {
        data.angle1 = a1;
    }
    /** @return The end angle of this arc */
    double getAngle2() {
        return data.angle2;
    }
    /** Sets new end angle. */
    void setAngle2(double a2) {
        data.angle2 = a2;
    }


    /** @return The center point (x) of this arc */
    GV_Vector getCenter() {
        return data.center;
    }
    /** Sets new center. */
    void setCenter(const GV_Vector& c) {
        data.center = c;
    }

    /** @return The endpoint of the major axis (relative to center). */
    GV_Vector getMajorP() {
        return data.majorP;
    }
    /** Sets new major point (relative to center). */
    void setMajorP(const GV_Vector& p) {
        data.majorP = p;
    }
    
    /** @return The endpoint of the minor axis (relative to center). */
    GV_Vector getMinorP() {
        double angle = GV_Math::correctAngle(getAngle() + M_PI/2.0);
        GV_Vector ret;
        ret.setPolar(getMinorRadius(), angle);
        return ret;
    }

    void setMinorP(const GV_Vector& p) {
        double angle = GV_Math::correctAngle(p.angle() - M_PI/2.0);
        data.majorP.setPolar(getMajorRadius(), angle);
        setRatio(p.magnitude() / getMajorRadius());
    }

    /** @return The ratio of minor to major axis */
    double getRatio() {
        return data.ratio;
    }
    /** Sets new ratio. */
    void setRatio(double r) {
        data.ratio = r;
    }


    /**
     * @return Angle length in rad.
     */
    virtual double getAngleLength() const {
        double ret = 0.0;
        
        if (isReversed()) {
            if (data.angle1<data.angle2) {
                ret = data.angle1+2*M_PI-data.angle2;
            } else {
                ret = data.angle1-data.angle2;
            }
        } else {
            if (data.angle2<data.angle1) {
                ret = data.angle2+2*M_PI-data.angle1;
            } else {
                ret = data.angle2-data.angle1;
            }
        }

        // full ellipse:
        if (fabs(ret)<1.0e-6) {
            ret = 2*M_PI;
        }
        
        return ret;
    }

    /** @return The major radius of this ellipse. Same as getRadius() */
    double getMajorRadius() const {
        return data.majorP.magnitude();
    }

    /** @return The minor radius of this ellipse */
    double getMinorRadius() const {
        return data.majorP.magnitude()*data.ratio;
    }

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
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual bool isPointOnEntity(const GV_Vector& coord,
                                 double tolerance=GV_TOLERANCE);
    virtual GV2::Side getSideOfPoint(const GV_Vector& ) {
        // TODO: implement
        return GV2::NoSide;
    }

    GV_Polyline* approximateWithArcs(int segments);

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    //virtual void calculateEndpoints();
    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    static const char* propertyCenterX;
    static const char* propertyCenterY;
    static const char* propertyMajorPX;
    static const char* propertyMajorPY;
    static const char* propertyRatio;
    static const char* propertyStartangle;
    static const char* propertyEndangle;
    static const char* propertyReversed;

protected:
    GV_EllipseData data;
};

#endif
