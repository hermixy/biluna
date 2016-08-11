/****************************************************************************
** $Id: gv_spline.cpp 9379 2008-03-10 20:47:45Z andrew $
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


#include "gv_spline.h"

#include "gv_application.h"
#include "rb_debug.h"
#include "gv_graphicview.h"
#include "gv_painter.h"
#include "gv_graphic.h"


const char* GV_Spline::degree = QT_TRANSLATE_NOOP("QObject", "Degree");
const char* GV_Spline::closed = QT_TRANSLATE_NOOP("QObject", "Closed");


/**
 * Constructor.
 */
GV_Spline::GV_Spline(GV_EntityContainer* parentContainer,
                     const GV_SplineData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {
    calculateBorders();
}



/**
 * Destructor.
 */
GV_Spline::~GV_Spline() {}




GV_Entity* GV_Spline::clone() {
    GV_Spline* l = new GV_Spline(parentContainer, data);
    l->copyPropertiesFrom(this);
    l->entities = entities;
    l->entities.setAutoDelete(entities.autoDelete());
    l->initId();
    l->detach();
    return l;
}



GV_VectorList GV_Spline::getRefPoints(GV2::RefMode /*refMode*/) {

    GV_VectorList ret;

    int i=0;
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it, ++i) {

        ret.set(i, (*it));
    }

    return ret;
}



GV_Vector GV_Spline::getNearestRef(const GV_Vector& coord,
                                   double* dist,
                                   GV2::RefMode refMode) {

    //return getRefPoints().getClosest(coord, dist);
    return GV_Entity::getNearestRef(coord, dist, refMode);
}



GV_Vector GV_Spline::getNearestSelectedRef(const GV_Vector& coord,
        double* dist,
        GV2::RefMode refMode) {

    //return getRefPoints().getClosest(coord, dist);
    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}


/**
 * Updates the internal polygon of this spline. Called when the 
 * spline or it's data, position, .. changes.
 */
void GV_Spline::update() {

    RB_DEBUG->print("GV_Spline::update");

    clear();

    if (isUndone()) {
        return;
    }

    if (data.degree<1 || data.degree>3) {
//        RB_DEBUG->print("GV_Spline::update: invalid degree: %d", data.degree);
        return;
    }

    if (data.controlPoints.count()<data.degree+1) {
        RB_DEBUG->print("GV_Spline::update: not enough control points");
        return;
    }

    resetBorders();

    GV_ValueList<GV_Vector> tControlPoints = data.controlPoints;

    if (data.closed) {
        for (int i=0; i<data.degree; ++i) {
            tControlPoints.append(data.controlPoints[i]);
        }
    }

    int i;
    int npts = tControlPoints.count();
    // order:
    int k = data.degree+1;
    // resolution:
    int p1 = getGraphicVariableInt("$SPLINESEGS", 8) * npts;

    double* b = new double[npts*3+1];
    double* h = new double[npts+1];
    double* p = new double[p1*3+1];

    GV_ValueList<GV_Vector>::iterator it;
    i = 1;
    for (it = tControlPoints.begin(); it!=tControlPoints.end(); ++it) {
        b[i] = (*it).x;
        b[i+1] = (*it).y;
        b[i+2] = 0.0;

//        RB_DEBUG->print("GV_Spline::update: b[%d]: %f/%f", i, b[i], b[i+1]);
        i+=3;
    }

    // set all homogeneous weighting factors to 1.0
    for (i=1; i <= npts; i++) {
        h[i] = 1.0;
    }

    for (i = 1; i <= 3*p1; i++) {
        p[i] = 0.0;
    }

    if (data.closed) {
        calculatePointsClosed(npts,k,p1,b,h,p);
    } else {
        calculatePointOpen(npts,k,p1,b,h,p);
    }

    GV_Vector prev(false);
    for (i = 1; i <= 3*p1; i=i+3) {
        if (prev.valid) {
            GV_Line* line = new GV_Line(this,
                                        GV_LineData(prev, GV_Vector(p[i], p[i+1])));
            line->setLayer(NULL);
            line->setPen(GV_Pen(GV2::FlagInvalid));
            if (isSelected()) {
                line->setSelected(true);
            }
            addEntity(line);
        }
        prev = GV_Vector(p[i], p[i+1]);

        minV = GV_Vector::minimum(prev, minV);
        maxV = GV_Vector::maximum(prev, maxV);
    }

    delete[] b;
    delete[] h;
    delete[] p;

    calculateBorders();
}



GV_Vector GV_Spline::getNearestEndpoint(const GV_Vector& coord,
                                        double* dist) {
    double minDist = GV_MAXDOUBLE;
    GV_Vector ret(false);

    if (!isClosed()) {
        GV_Vector startPoint = data.controlPoints[0];
        GV_Vector endPoint = data.controlPoints[data.controlPoints.count()-1];

        double d1 = startPoint.distanceTo(coord);
        double d2 = endPoint.distanceTo(coord);

        if (d1 < d2) {
            ret = startPoint;
            minDist = d1;
        }
        else {
            ret = endPoint;
            minDist = d2;
        }
    }

    if (dist!=NULL) {
        *dist = minDist;
    }

    return ret;
}



GV_Vector GV_Spline::getNearestCenter(const GV_Vector& /*coord*/,
                                      double* dist) {

    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }

    return GV_Vector(false);
}



GV_Vector GV_Spline::getNearestMiddle(const GV_Vector& /*coord*/,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }

    return GV_Vector(false);
}



GV_Vector GV_Spline::getNearestDist(double /*distance*/,
                                    const GV_Vector& /*coord*/,
                                    double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }

    return GV_Vector(false);
}



void GV_Spline::move(const GV_Vector& offset) {
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).move(offset);
    }

    update();
}



void GV_Spline::rotate(const GV_Vector& center, double angle) {
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).rotate(center, angle);
    }

    update();
}



void GV_Spline::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).scale(center, factor);
    }

    update();
}



void GV_Spline::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        (*it).mirror(axisPoint1, axisPoint2);
    }

    update();
}



void GV_Spline::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_ValueList<GV_Vector>::iterator it;
    for (it = data.controlPoints.begin();
            it!=data.controlPoints.end(); ++it) {

        if (ref.distanceTo(*it)<1.0e-4) {
            (*it).move(offset);
        }
    }

    update();
}




void GV_Spline::draw(GV_Painter* painter, GV_GraphicView* view,
                     double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

    GV_Entity* e = firstEntity(GV2::ResolveNone);
    double offset = 0.0;

    if (e!=NULL) {
        view->drawEntity(e);
        offset+=e->getLength();
    }

    for (GV_Entity* e=nextEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone)) {

        view->drawEntityPlain(e, -offset);
        offset+=e->getLength();
    }
}



/**
 * @return The reference points of the spline.
 */
GV_ValueList<GV_Vector> GV_Spline::getControlPoints() {
    return data.controlPoints;
}



/**
 * Appends the given point to the control points.
 */
void GV_Spline::addControlPoint(const GV_Vector& v) {
    data.controlPoints.append(v);
}



/**
 * Removes the control point that was last added.
 */
void GV_Spline::removeLastControlPoint() {
    data.controlPoints.pop_back();
}


/**
 * Generates B-Spline open knot vector with multiplicity 
 * equal to the order at the ends.
 */
void GV_Spline::knotOpen(int num, int order, int knotVector[]) {
    knotVector[1] = 0;
    for (int i = 2; i <= num + order; i++) {
        if ( (i > order) && (i < num + 2) ) {
            knotVector[i] = knotVector[i-1] + 1;
        } else {
            knotVector[i] = knotVector[i-1];
        }
    }
}



/**
 * Generates rational B-spline basis functions for an open knot vector.
 */
void GV_Spline::generateBasisFunctions(int c, double t, int npts,
                       int x[], double h[], double r[]) {

    int nplusc;
    int i,k;
    double d,e;
    double sum;
    //double temp[36];

    nplusc = npts + c;

    double* temp = new double[nplusc+1];

    // calculate the first order nonrational basis functions n[i]
    for (i = 1; i<= nplusc-1; i++) {
        if (( t >= x[i]) && (t < x[i+1]))
            temp[i] = 1;
        else
            temp[i] = 0;
    }

    /* calculate the higher order nonrational basis functions */

    for (k = 2; k <= c; k++) {
        for (i = 1; i <= nplusc-k; i++) {
            // if the lower order basis function is zero skip the calculation
            if (temp[i] != 0)
                d = ((t-x[i])*temp[i])/(x[i+k-1]-x[i]);
            else
                d = 0;
            // if the lower order basis function is zero skip the calculation
            if (temp[i+1] != 0)
                e = ((x[i+k]-t)*temp[i+1])/(x[i+k]-x[i+1]);
            else
                e = 0;

            temp[i] = d + e;
        }
    }

    // pick up last point
    if (t == (double)x[nplusc]) {
        temp[npts] = 1;
    }

    // calculate sum for denominator of rational basis functions
    sum = 0.;
    for (i = 1; i <= npts; i++) {
        sum = sum + temp[i]*h[i];
    }

    // form rational basis functions and put in r vector
    for (i = 1; i <= npts; i++) {
        if (sum != 0) {
            r[i] = (temp[i]*h[i])/(sum);
        } else
            r[i] = 0;
    }

    delete[] temp;
}


/**
 * generates a rational B-spline curve using a uniform open knot vector.
 */
void GV_Spline::calculatePointOpen(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    int nplusc;

    double step;
    double t;
    double temp;

    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    // zero and redimension the knot vector and the basis array
    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    // generate the uniform open knot vector
    knotOpen(npts,k,x);

    icount = 0;

    // calculate the points on the rational B-spline curve
    t = 0;
    step = ((double)x[nplusc])/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        // generate the basis function for this value of t
        generateBasisFunctions(k,t,npts,x,h,nbasis);

        // generate a point on the curve
        for (j = 1; j <= 3; j++) {
            jcount = j;
            p[icount+j] = 0.;

            // Do local matrix multiplication
            for (i = 1; i <= npts; i++) {
                temp = nbasis[i]*b[jcount];
                p[icount + j] = p[icount + j] + temp;
                jcount = jcount + 3;
            }
        }
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}


void GV_Spline::knotClosed(int num, int order, int knotVector[]) {
    int nplusc,i;
//    int nplus2;

    nplusc = num + order;
//    nplus2 = num + 2;

    knotVector[1] = 0;
    for (i = 2; i <= nplusc; i++) {
        knotVector[i] = i-1;
    }
}



/**
 * generates a rational B-spline curve using a uniform closed knot vector.
 */
void GV_Spline::calculatePointsClosed(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    int nplusc;

    double step;
    double t;
    double temp;


    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    // redimension the knot vector and the basis array
    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    // generate the uniform periodic knot vector
    knotClosed(npts,k,x);
    icount = 0;

    // calculate the points on the rational B-spline curve
    t = k-1;
    step = ((double)((npts)-(k-1)))/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        // generate the basis function for this value of t
        generateBasisFunctions(k,t,npts,x,h,nbasis);
        // generate a point on the curve:
        for (j = 1; j <= 3; j++) {
            jcount = j;
            p[icount+j] = 0.;

            // local matrix multiplication
            for (i = 1; i <= npts; i++) {
                temp = nbasis[i]*b[jcount];
                p[icount + j] = p[icount + j] + temp;
                jcount = jcount + 3;
            }
        }
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}


QVariant GV_Spline::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_SPLINE_DEGREE) {
        return getDegree();
    } else if (name==GV_SPLINE_CLOSED) {
        return isClosed();
    } else {
        return GV_EntityContainer::getProperty(name, def);
    }
}



QStringList GV_Spline::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_EntityContainer::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_SPLINE_DEGREE;
        ret << GV_SPLINE_CLOSED;
    }
    return ret;
}


void GV_Spline::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_SPLINE_DEGREE) {
        setDegree(value.toInt());
    } else if (name==GV_SPLINE_CLOSED) {
        setClosed(variantToBool(value));
    } else {
        GV_EntityContainer::setProperty(name, value);
    }
    update();
}


/**
 * Limits the choices for the spline degree property.
 */
GV_PropertyAttributes GV_Spline::getPropertyAttributes(const QString& propertyName) {
    GV_PropertyAttributes ret = 
        GV_EntityContainer::getPropertyAttributes(propertyName);

    if (propertyName==GV_SPLINE_DEGREE) {
        ret.choices << "2";
        ret.choices << "3";
    }

    return ret;
}
