/****************************************************************************
** $Id: gv_arc.cpp 7925 2008-01-17 18:01:07Z andrew $
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

#include "gv_arc.h"

#include "gv_constructionline.h"
#include "gv_linetypepattern.h"
#include "gv_information.h"
#include "gv_math.h"
#include "gv_graphicview.h"
#include "gv_painter.h"
#include "gv_polyline.h"


const char* GV_Arc::propertyCenterX = QT_TRANSLATE_NOOP("QObject", "Center|X");
const char* GV_Arc::propertyCenterY = QT_TRANSLATE_NOOP("QObject", "Center|Y");
const char* GV_Arc::propertyRadius = QT_TRANSLATE_NOOP("QObject", "Radius");
const char* GV_Arc::propertyStartangle = QT_TRANSLATE_NOOP("QObject", "Angles|Startangle");
const char* GV_Arc::propertyEndangle = QT_TRANSLATE_NOOP("QObject", "Angles|Endangle");
const char* GV_Arc::propertyReversed = QT_TRANSLATE_NOOP("QObject", "Reversed");


/**
 * Default constructor.
 */
GV_Arc::GV_Arc(GV_EntityContainer* parentContainer,
               const GV_ArcData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {
    calculateEndpoints();
    calculateBorders();
}



/**
 * Creates this arc from 3 given points which define the arc line.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 * @param p3 3rd point.
 */
bool GV_Arc::createFrom3P(const GV_Vector& p1, const GV_Vector& p2,
                          const GV_Vector& p3) {
    if (p1.distanceTo(p2)>GV_TOLERANCE &&
            p2.distanceTo(p3)>GV_TOLERANCE &&
            p3.distanceTo(p1)>GV_TOLERANCE) {

        // middle points between 3 points:
        GV_Vector mp1, mp2;
        GV_Vector dir1, dir2;
        double a1, a2;

        // intersection of two middle lines
        mp1 = (p1 + p2)/2.0;
        a1 = p1.angleTo(p2) + M_PI/2.0;
        dir1.setPolar(100.0, a1);
        mp2 = (p2 + p3)/2.0;
        a2 = p2.angleTo(p3) + M_PI/2.0;
        dir2.setPolar(100.0, a2);

        GV_ConstructionLineData d1(mp1, mp1 + dir1);
        GV_ConstructionLineData d2(mp2, mp2 + dir2);
        GV_ConstructionLine midLine1(NULL, d1);
        GV_ConstructionLine midLine2(NULL, d2);

        GV_VectorList sol =
            GV_Information::getIntersection(&midLine1, &midLine2);

        data.center = sol.get(0);
        data.radius = data.center.distanceTo(p3);
        data.angle1 = data.center.angleTo(p1);
        data.angle2 = data.center.angleTo(p3);
        data.reversed = GV_Math::isAngleBetween(data.center.angleTo(p2),
                                                data.angle1, data.angle2, true);

        if (sol.get(0).valid && data.radius<1.0e14 &&
                data.radius>GV_TOLERANCE) {
            calculateEndpoints();
            calculateBorders();
            return true;
        } else {
            RB_DEBUG->print("GV_Arc::createFrom3P(): "
                            "Cannot create an arc with inf radius.");
            return false;
        }
    } else {
        RB_DEBUG->print("GV_Arc::createFrom3P(): "
                        "Cannot create an arc with radius 0.0.");
        return false;
    }
}



/**
 * Creates an arc from its startpoint, endpoint, start direction (angle)
 * and radius.
 * 
 * @retval true Successfully created arc
 * @retval false Cannot creats arc (radius to small or endpoint to far away)
 */
bool GV_Arc::createFrom2PDirectionRadius(const GV_Vector& startPoint,
        const GV_Vector& endPoint,
        double direction1, double radius) {

    GV_Vector ortho;
    ortho.setPolar(radius, direction1 + M_PI/2.0);
    GV_Vector center1 = startPoint + ortho;
    GV_Vector center2 = startPoint - ortho;

    if (center1.distanceTo(endPoint) < center2.distanceTo(endPoint)) {
        data.center = center1;
    } else {
        data.center = center2;
    }

    data.radius = radius;
    data.angle1 = data.center.angleTo(startPoint);
    data.angle2 = data.center.angleTo(endPoint);
    data.reversed = false;

    double diff = GV_Math::correctAngle(getDirection1()-direction1);
    if (fabs(diff-M_PI)<1.0e-1) {
        data.reversed = true;
    }

    calculateEndpoints();
    calculateBorders();

    return true;
}



/**
 * Creates an arc from its startpoint, endpoint and bulge.
 */
bool GV_Arc::createFrom2PBulge(const GV_Vector& startPoint, const GV_Vector& endPoint,
                               double bulge) {
    data.reversed = (bulge<0.0);
    double alpha = atan(bulge)*4.0;

    GV_Vector middle = (startPoint+endPoint)/2.0;
    double dist = startPoint.distanceTo(endPoint)/2.0;

    // alpha can't be 0.0 at this point
    data.radius = fabs(dist / sin(alpha/2.0));

    double wu = fabs(GV_Math::pow(data.radius, 2.0) - GV_Math::pow(dist, 2.0));
    double h = sqrt(wu);
    double angle = startPoint.angleTo(endPoint);

    if (bulge>0.0) {
        angle+=M_PI/2.0;
    } else {
        angle-=M_PI/2.0;
    }

    if (fabs(alpha)>M_PI) {
        h*=-1.0;
    }

    data.center.setPolar(h, angle);
    data.center+=middle;
    data.angle1 = data.center.angleTo(startPoint);
    data.angle2 = data.center.angleTo(endPoint);

    calculateEndpoints();
    calculateBorders();

    return true;
}



/**
 * Recalculates the endpoints using the angles and the radius.
 */
void GV_Arc::calculateEndpoints() {
    startpoint.set(data.center.x + cos(data.angle1) * data.radius,
                   data.center.y + sin(data.angle1) * data.radius);
    endpoint.set(data.center.x + cos(data.angle2) * data.radius,
                 data.center.y + sin(data.angle2) * data.radius);
}


void GV_Arc::calculateBorders(bool /*visibleOnly*/) {

    double minX = std::min(startpoint.x, endpoint.x);
    double minY = std::min(startpoint.y, endpoint.y);
    double maxX = std::max(startpoint.x, endpoint.x);
    double maxY = std::max(startpoint.y, endpoint.y);
    
    if (getStartpoint().distanceTo(getEndpoint()) < 1.0e-6 && getRadius()>1.0e5) {
        minV.set(minX, minY);
        maxV.set(maxX, maxY);
        return;
    }

    double a1 = !isReversed() ? data.angle1 : data.angle2;
    double a2 = !isReversed() ? data.angle2 : data.angle1;

    // check for left limit:
    if ((a1<M_PI && a2>M_PI) ||
            (a1>a2-1.0e-12 && a2>M_PI) ||
            (a1>a2-1.0e-12 && a1<M_PI) ) {

        minX = std::min(data.center.x - data.radius, minX);
    }

    // check for right limit:
    if (a1 > a2-1.0e-12) {
        maxX = std::max(data.center.x + data.radius, maxX);
    }

    // check for bottom limit:
    if ((a1<(M_PI_2*3) && a2>(M_PI_2*3)) ||
            (a1>a2-1.0e-12    && a2>(M_PI_2*3)) ||
            (a1>a2-1.0e-12    && a1<(M_PI_2*3)) ) {

        minY = std::min(data.center.y - data.radius, minY);
    }

    // check for top limit:
    if ((a1<M_PI_2 && a2>M_PI_2) ||
            (a1>a2-1.0e-12   && a2>M_PI_2) ||
            (a1>a2-1.0e-12   && a1<M_PI_2) ) {

        maxY = std::max(data.center.y + data.radius, maxY);
    }

    minV.set(minX, minY);
    maxV.set(maxX, maxY);
}



GV_VectorList GV_Arc::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret(startpoint, endpoint, data.center);
    return ret;
}


GV_Vector GV_Arc::getNearestEndpoint(const GV_Vector& coord, double* dist) {
    double dist1, dist2;
    GV_Vector* nearerPoint;

    dist1 = startpoint.distanceTo(coord);
    dist2 = endpoint.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = &endpoint;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = &startpoint;
    }

    return *nearerPoint;
}



GV_Vector GV_Arc::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity, double* dist, GV_Entity** entity) {

    GV_Vector vec(false);
    if (entity!=NULL) {
        *entity = this;
    }

    double angle = (coord-data.center).angle();
    if (onEntity==false || GV_Math::isAngleBetween(angle,
            data.angle1, data.angle2, isReversed())) {
        vec.setPolar(data.radius, angle);
        vec+=data.center;
    }
    if (dist!=NULL) {
        *dist = fabs((vec-data.center).magnitude()-data.radius);
    }

    return vec;
}



GV_Vector GV_Arc::getNearestCenter(const GV_Vector& coord,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



GV_Vector GV_Arc::getNearestMiddle(const GV_Vector& coord,
                                   double* dist) {

    GV_Vector ret = getMiddlepoint();

    if (dist!=NULL) {
        *dist = coord.distanceTo(ret);
    }
    return ret;
}



GV_Vector GV_Arc::getNearestDist(double distance,
                                 const GV_Vector& coord,
                                 double* dist) {

    if (data.radius<1.0e-6) {
        if (dist!=NULL) {
            *dist = GV_MAXDOUBLE;
        }
        return GV_Vector(false);
    }

    double a1, a2;
    GV_Vector p1, p2;
    double aDist = distance / data.radius;

    if (isReversed()) {
        a1 = data.angle1 - aDist;
        a2 = data.angle2 + aDist;
    } else {
        a1 = data.angle1 + aDist;
        a2 = data.angle2 - aDist;
    }

    p1.setPolar(data.radius, a1);
    p1 += data.center;
    p2.setPolar(data.radius, a2);
    p2 += data.center;

    double dist1, dist2;
    GV_Vector* nearerPoint;

    dist1 = p1.distanceTo(coord);
    dist2 = p2.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = &p2;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = &p1;
    }

    return *nearerPoint;
}




GV_Vector GV_Arc::getNearestDist(double distance,
                                 bool startp) {

    if (data.radius<1.0e-6) {
        return GV_Vector(false);
    }

    double a;
    GV_Vector p;
    double aDist = distance / data.radius;

    if (isReversed()) {
        if (startp) {
            a = data.angle1 - aDist;
        } else {
            a = data.angle2 + aDist;
        }
    } else {
        if (startp) {
            a = data.angle1 + aDist;
        } else {
            a = data.angle2 - aDist;
        }
    }

    p.setPolar(data.radius, a);
    p += data.center;

    return p;
}



double GV_Arc::getDistanceToPoint(const GV_Vector& coord,
                                  GV_Entity** entity,
                                  GV2::ResolveLevel,
                                  double, bool) {
    if (entity!=NULL) {
        *entity = this;
    }

    // check endpoints first:
    double dist = coord.distanceTo(getStartpoint());
    if (dist<1.0e-4) {
        return dist;
    }
    dist = coord.distanceTo(getEndpoint());
    if (dist<1.0e-4) {
        return dist;
    }

    if (GV_Math::isAngleBetween(data.center.angleTo(coord),
                                data.angle1, data.angle2,
                                isReversed())) {

        return fabs((coord-data.center).magnitude() - data.radius);
    } else {
        return GV_MAXDOUBLE;
    }
}



GV2::Side GV_Arc::getSideOfPoint(const GV_Vector& coord) {
    if (data.reversed) {
        if (data.center.distanceTo(coord) < data.radius) {
            return GV2::RightHand;
        }
        else {
            return GV2::LeftHand;
        }
    }
    else {
        if (data.center.distanceTo(coord) < data.radius) {
            return GV2::LeftHand;
        }
        else {
            return GV2::RightHand;
        }
    }
}



void GV_Arc::moveStartpoint(const GV_Vector& pos) {
    // polyline arcs: move point not angle:
    //if (parentContainer!=NULL && parentContainer->rtti()==GV2::EntityPolyline) {
        double bulge = getBulge();
        createFrom2PBulge(pos, getEndpoint(), bulge);
    //}

    // normal arc: move angle1
    /*else {
        data.angle1 = data.center.angleTo(pos);
        calculateEndpoints();
        calculateBorders();
    }*/
}



void GV_Arc::moveEndpoint(const GV_Vector& pos) {
    // polyline arcs: move point not angle:
    //if (parentContainer!=NULL && parentContainer->rtti()==GV2::EntityPolyline) {
        double bulge = getBulge();
        createFrom2PBulge(getStartpoint(), pos, bulge);
    //}

    // normal arc: move angle1
    /*else {
        data.angle2 = data.center.angleTo(pos);
        calculateEndpoints();
        calculateBorders();
    }*/
}



void GV_Arc::trimStartpoint(const GV_Vector& pos) {
    data.angle1 = data.center.angleTo(pos);
    calculateEndpoints();
    calculateBorders();
}



void GV_Arc::trimEndpoint(const GV_Vector& pos) {
    data.angle2 = data.center.angleTo(pos);
    calculateEndpoints();
    calculateBorders();
}


GV2::Ending GV_Arc::getTrimPoint(const GV_Vector& coord,
                                 const GV_Vector& trimPoint) {

    double angEl = data.center.angleTo(trimPoint);
    double angM = data.center.angleTo(coord);

    if (GV_Math::getAngleDifference(angM, angEl)>M_PI) {
        if (data.reversed) {
            return GV2::EndingEnd;
        } else {
            return GV2::EndingStart;
        }
    } else {
        if (data.reversed) {
            return GV2::EndingStart;
        } else {
            return GV2::EndingEnd;
        }
    }
}


void GV_Arc::reverse() {
    double a = data.angle1;
    data.angle1 = data.angle2;
    data.angle2 = a;
    data.reversed = !data.reversed;
    calculateEndpoints();
    calculateBorders();
}
    
    
GV_Polyline* GV_Arc::approximateWithLines(double segmentLength) {
    GV_Polyline* polyline = new GV_Polyline(getParentContainer());
    polyline->setPen(getPen(false));
    polyline->setLayer(getLayer());

    // avoid a segment length of 0:
    if (segmentLength<1.0e-3) {
        segmentLength = 1.0e-3;
    }

    double a1 = getAngle1();
    double a2 = getAngle2();
    double aStep = segmentLength / data.radius;
    double a, cix, ciy;

    polyline->addVertex(getStartpoint());
    if(!data.reversed) {
        // Arc Counterclockwise:
        if(a1>a2-1.0e-10) {
            a2+=2*M_PI;
        }
        for(a=a1+aStep; a<=a2; a+=aStep) {
            cix = data.center.x + cos(a) * data.radius;
            ciy = data.center.y + sin(a) * data.radius;
            polyline->addVertex(GV_Vector(cix, ciy));
        }
    } else {
        // Arc Clockwise:
        if(a1<a2+1.0e-10) {
            a2-=2*M_PI;
        }
        for(a=a1-aStep; a>=a2; a-=aStep) {
            cix = data.center.x + cos(a) * data.radius;
            ciy = data.center.y + sin(a) * data.radius;
            polyline->addVertex(GV_Vector(cix, ciy));
        }
    }
    polyline->addVertex(getEndpoint());
    
    return polyline;
}


void GV_Arc::move(const GV_Vector& offset) {
    data.center.move(offset);
    calculateEndpoints();
    calculateBorders();
}



void GV_Arc::rotate(const GV_Vector& center, double angle) {
    data.center.rotate(center, angle);
    data.angle1 = GV_Math::correctAngle(data.angle1+angle);
    data.angle2 = GV_Math::correctAngle(data.angle2+angle);
    calculateEndpoints();
    calculateBorders();
}



void GV_Arc::scale(const GV_Vector& center, const GV_Vector& factor) {
    // negative scaling: mirroring
    if (factor.x<0.0) {
        mirror(data.center, data.center + GV_Vector(0.0, 1.0));
    }
    if (factor.y<0.0) {
        mirror(data.center, data.center + GV_Vector(1.0, 0.0));
    }

    data.center.scale(center, factor);
    data.radius *= factor.x;
    if (data.radius<0.0) {
        data.radius*=-1.0;
    }
    calculateEndpoints();
    calculateBorders();
}



void GV_Arc::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.center.mirror(axisPoint1, axisPoint2);
    data.reversed = (!data.reversed);
    /*
    startpoint.mirror(axisPoint1, axisPoint2);
    endpoint.mirror(axisPoint1, axisPoint2);

    data.angle1 = data.center.angleTo(startpoint);
    data.angle2 = data.center.angleTo(endpoint);
    */

    GV_Vector vec;
    vec.setPolar(1.0, data.angle1);
    vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle1 = vec.angle();

    vec.setPolar(1.0, data.angle2);
    vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle2 = vec.angle();

    calculateEndpoints();
    calculateBorders();
}



void GV_Arc::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    if (ref.distanceTo(startpoint)<1.0e-4) {
        moveStartpoint(startpoint+offset);
    }
    if (ref.distanceTo(endpoint)<1.0e-4) {
        moveEndpoint(endpoint+offset);
    }
    if (ref.distanceTo(data.center)<1.0e-4) {
        move(offset);
    } 
}



void GV_Arc::stretch(const GV_Vector& firstCorner,
                      const GV_Vector& secondCorner,
                      const GV_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
    else {
        if (getStartpoint().isInWindow(firstCorner,
                                       secondCorner)) {
            moveStartpoint(getStartpoint() + offset);
        }
        if (getEndpoint().isInWindow(firstCorner,
                                     secondCorner)) {
            moveEndpoint(getEndpoint() + offset);
        }
    }
}



void GV_Arc::draw(GV_Painter* painter, GV_GraphicView* view,
                  double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

    if (getStartpoint().distanceTo(getEndpoint()) < 1.0e-6 && getRadius()>1.0e5) {
        return;
    }

    /*
    RB_DEBUG->print("GV_Arc::draw: radius: %f", getRadius());
    
    RB_DEBUG->print("GV_Arc::draw: start: %f/%f", 
        getStartpoint().x, getStartpoint().y);
    RB_DEBUG->print("GV_Arc::draw: end: %f/%f", 
        getEndpoint().x, getEndpoint().y);
    
    RB_DEBUG->print("GV_Arc::draw: angle1: %f", getAngle1());
    RB_DEBUG->print("GV_Arc::draw: angle2: %f", getAngle2());
    */

    //double styleFactor = getStyleFactor();

    // simple style-less lines
    if (getPen().getLineType()==GV2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==GV2::ModePreview) {

        painter->drawArc(view->toGui(getCenter()),
                         getRadius() * view->getFactor().x,
                         getAngle1(), getAngle2(),
                         isReversed());
    } else {
        double styleFactor = getStyleFactor(view);
        if (styleFactor<0.0) {
            painter->drawArc(view->toGui(getCenter()),
                         getRadius() * view->getFactor().x,
                         getAngle1(), getAngle2(),
                         isReversed());
            return;
        }

        // Pattern:
        GV_LineTypePattern* pat;
        if (isSelected()) {
            pat = &patternSelected;
        } else {
            pat = view->getPattern(getPen().getLineType());
        }

        if (pat==NULL) {
            return;
        }

        if (getRadius()<1.0e-6) {
            return;
        }

        // Pen to draw pattern is always solid:
        GV_Pen pen = painter->getPen();
        pen.setLineType(GV2::SolidLine);
        painter->setPen(pen);

        double a1;
        double a2;
        if (data.reversed) {
            a2 = getAngle1();
            a1 = getAngle2();
        } else {
            a1 = getAngle1();
            a2 = getAngle2();
        }

        double* da;     // array of distances in x.
        int i;          // index counter

        double length = getAngleLength();

        // create scaled pattern:
        da = new double[pat->num];

        for (i=0; i<pat->num; ++i) {
            da[i] = fabs(pat->pattern[i] * styleFactor) / getRadius();
        }

        double tot=0.0;
        i=0;
        bool done = false;
        double curA = a1;
        //double cx = getCenter().x * factor.x + offsetX;
        //double cy = - a->getCenter().y * factor.y + getHeight() - offsetY;
        GV_Vector cp = view->toGui(getCenter());
        double r = getRadius() * view->getFactor().x;

        do {
            if (pat->pattern[i] > 0.0) {
                if (!view->isPrinting() && da[i]*r<2) {
                    painter->drawPoint(cp + 
                        GV_Vector(r * cos(curA), -r * sin(curA)));
                }
                else if (tot+da[i]<length) {
                    painter->drawArc(cp, r,
                                     curA,
                                     curA + da[i],
                                     false);
                } else {
                    painter->drawArc(cp, r,
                                     curA,
                                     a2,
                                     false);
                }
            }
            curA+=da[i];
            tot+=da[i];
            done=tot>length;

            i++;
            if (i>=pat->num) {
                i=0;
            }
        } while(!done);

        delete[] da;
    }
}



/**
 * @return Middle point of the entity. 
 */
GV_Vector GV_Arc::getMiddlepoint() const {
    double a;
    GV_Vector ret;

    if (isReversed()) {
        a = data.angle1 - getAngleLength()/2.0;
    } else {
        a = data.angle1 + getAngleLength()/2.0;
    }
    ret.setPolar(data.radius, a);
    ret+=data.center;

    return ret;
}



/**
 * @return Angle length in rad.
 *
 * @param allowForZeroLength: Allow for zero length result if start 
 *    and end angle are the same. Otherwise 2*PI will be returned for
 *    such an arc, assuming it is a full circle.
 */
double GV_Arc::getAngleLength(bool allowForZeroLength) const {
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

    // full circle:
    if (!allowForZeroLength && fabs(ret)<1.0e-6) {
        ret = 2*M_PI;
    }
    
    return ret;
}



/**
 * @return Length of the arc.
 */
double GV_Arc::getLength() {
    return getAngleLength()*data.radius;
}



/**
 * Gets the arc's bulge (tangens of angle length divided by 4).
 */
double GV_Arc::getBulge() const {
    double bulge = tan(fabs(getAngleLength())/4.0);
    if (isReversed()) {
        bulge*=-1;
    }
    return bulge;
}



QVariant GV_Arc::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_ARC_CENTER_X) {
        return data.center.x;
    } else if (name==GV_ARC_CENTER_Y) {
        return data.center.y;
    } else if (name==GV_ARC_RADIUS) {
        return data.radius;
    } else if (name==GV_ARC_STARTANGLE) {
        return GV_Math::rad2deg(data.angle1);
    } else if (name==GV_ARC_ENDANGLE) {
        return GV_Math::rad2deg(data.angle2);
    } else if (name==GV_ARC_REVERSED) {
        return data.reversed;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Arc::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret 
        << GV_ARC_CENTER_X
        << GV_ARC_CENTER_Y
        << GV_ARC_RADIUS
        << GV_ARC_STARTANGLE
        << GV_ARC_ENDANGLE
        << GV_ARC_REVERSED;
    }
    return ret;
}


void GV_Arc::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_ARC_CENTER_X) {
        data.center.x = GV_Math::eval(value.toString());
        calculateBorders();
        calculateEndpoints();
    } else if (name==GV_ARC_CENTER_Y) {
        data.center.y = GV_Math::eval(value.toString());
        calculateBorders();
        calculateEndpoints();
    } else if (name==GV_ARC_RADIUS) {
        setRadius(GV_Math::eval(value.toString()));
        calculateBorders();
        calculateEndpoints();
    } else if (name==GV_ARC_STARTANGLE) {
        setAngle1(GV_Math::deg2rad(GV_Math::eval(value.toString())));
        calculateBorders();
        calculateEndpoints();
    } else if (name==GV_ARC_ENDANGLE) {
        setAngle2(GV_Math::deg2rad(GV_Math::eval(value.toString())));
        calculateBorders();
        calculateEndpoints();
    } else if (name==GV_ARC_REVERSED) {
        data.reversed = variantToBool(value);
        calculateBorders();
        calculateEndpoints();
    } else {
        GV_Entity::setProperty(name, value);
    }
}
    
    
