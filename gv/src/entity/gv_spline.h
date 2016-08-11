/****************************************************************************
** $Id: gv_spline.h 9362 2008-03-08 14:34:04Z andrew $
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


#ifndef GV_SPLINE_H
#define GV_SPLINE_H

#include "gv_entitycontainer.h"
#include "gv_valuelist.h"

#define GV_SPLINE_DEGREE   QObject::tr(GV_Spline::degree)
#define GV_SPLINE_CLOSED   QObject::tr(GV_Spline::closed)

/**
 * Holds the data that defines a line.
 */
class GV_SplineData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_SplineData() {}
    
    GV_SplineData(int degree, bool closed) {
        this->degree = degree;
        this->closed = closed;
    }

public:
    /** Degree of the spline (1, 2, 3) */
    int degree;
    /** Closed flag. */
    bool closed;
    /** Control points of the spline. */
    GV_ValueList<GV_Vector> controlPoints;
};


/**
 * Class for a spline entity.
 *
 * @author Andrew Mustun
 */
class GV_Spline : public GV_EntityContainer {
public:
    GV_Spline(GV_EntityContainer* parentContainer,
            const GV_SplineData& d);

    virtual GV_Entity* clone();

    virtual ~GV_Spline();

    /**    @return GV2::EntitySpline */
    virtual GV2::EntityType rtti() const {
        return GV2::EntitySpline;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntitySpline || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Spline");
    }
    /** @return false */
    virtual bool isEdge() const {
        return false;
    }

    /** @return Copy of data that defines the spline. */
    GV_SplineData getData() const {
        return data;
    }
    
    /** Sets the splines degree (1-3). */
    void setDegree(int deg) {
        if (deg>=1 && deg<=3) {
            data.degree = deg;
        }
    }

    /** @return Degree of this spline curve (1-3).*/
    int getDegree() {
        return data.degree;
    }

    /** @return 0. */
    int getNumberOfKnots() {
        return 0;
    }

    /** @return Number of control points. */
    int getNumberOfControlPoints() {
        return data.controlPoints.count();
    }

    /** 
     * @retval true if the spline is closed.
     * @retval false otherwise.
     */
    bool isClosed() {
        return data.closed;
    }

    /**
     * Sets the closed falg of this spline.
     */
    void setClosed(bool c) {
        data.closed = c;
        update();
    }

    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);


    void update();
    
    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    
    virtual void addControlPoint(const GV_Vector& v);
    virtual void removeLastControlPoint();

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, 
        const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, 
        double patternOffset=0.0);
    GV_ValueList<GV_Vector> getControlPoints();

    static void generateBasisFunctions(int c, double t, int npts, 
        int x[], double h[], double r[]);
    
    static void knotOpen(int num, int order, int knotVector[]);
    static void calculatePointOpen(int npts, int k, int p1,
        double b[], double h[], double p[]);
        
    static void knotClosed(int num, int order, int knotVector[]);
    static void calculatePointsClosed(int npts, int k, int p1,
        double b[], double h[], double p[]);

    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);
    
    virtual GV_PropertyAttributes getPropertyAttributes(
        const QString& propertyName);

protected:
    GV_SplineData data;
    
    static const char* degree;
    static const char* closed;
};

#endif
