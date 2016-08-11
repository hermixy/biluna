/****************************************************************************
** $Id: gv_ellipse.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "gv_ellipse.h"

#include "gv_application.h"
#include "gv_graphic.h"
#include "gv_graphicview.h"
#include "gv_painter.h"
#include "gv_information.h"
#include "gv_polyline.h"


const char* GV_Ellipse::propertyCenterX = QT_TRANSLATE_NOOP("QObject", "Center|X");
const char* GV_Ellipse::propertyCenterY = QT_TRANSLATE_NOOP("QObject", "Center|Y");
const char* GV_Ellipse::propertyMajorPX = QT_TRANSLATE_NOOP("QObject", "Major Point|X");
const char* GV_Ellipse::propertyMajorPY = QT_TRANSLATE_NOOP("QObject", "Major Point|Y");
const char* GV_Ellipse::propertyRatio = QT_TRANSLATE_NOOP("QObject", "Ratio");
const char* GV_Ellipse::propertyStartangle = QT_TRANSLATE_NOOP("QObject", "Angles|Startangle");
const char* GV_Ellipse::propertyEndangle = QT_TRANSLATE_NOOP("QObject", "Angles|Endangle");
const char* GV_Ellipse::propertyReversed = QT_TRANSLATE_NOOP("QObject", "Reversed");


/**
 * Constructor.
 */
GV_Ellipse::GV_Ellipse(GV_EntityContainer* parentContainer,
                       const GV_EllipseData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {

    calculateBorders();
}


/**
 * Calculates the boundary box of this ellipse.
 *
 * @todo Fix that - the algorithm used is really bad / slow.
 */
void GV_Ellipse::calculateBorders(bool /*visibleOnly*/) {
    RB_DEBUG->print("GV_Ellipse::calculateBorders");

    double radius1 = getMajorRadius();
    double radius2 = getMinorRadius();
    double angle = getAngle();
    double a1 = ((!isReversed()) ? data.angle1 : data.angle2);
    double a2 = ((!isReversed()) ? data.angle2 : data.angle1);
    GV_Vector startpoint = getStartpoint();
    GV_Vector endpoint = getEndpoint();

    double minX = std::min(startpoint.x, endpoint.x);
    double minY = std::min(startpoint.y, endpoint.y);
    double maxX = std::max(startpoint.x, endpoint.x);
    double maxY = std::max(startpoint.y, endpoint.y);

    // kind of a brute force. TODO: exact calculation
    GV_Vector vp;
    double a = a1;
    do {
        vp.set(data.center.x + radius1 * cos(a),
               data.center.y + radius2 * sin(a));
        vp.rotate(data.center, angle);

        minX = std::min(minX, vp.x);
        minY = std::min(minY, vp.y);
        maxX = std::max(maxX, vp.x);
        maxY = std::max(maxY, vp.y);

        a += 0.03;
    } while (GV_Math::isAngleBetween(GV_Math::correctAngle(a), a1, a2, false) && 
            a<4*M_PI);


    minV.set(minX, minY);
    maxV.set(maxX, maxY);
    RB_DEBUG->print("GV_Ellipse::calculateBorders: OK");
}



GV_VectorList GV_Ellipse::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
    if (!isFullEllipse()) {
        ret << getStartpoint()
            << getEndpoint();
    }
    ret << data.center
        << data.center+data.majorP
        << data.center-data.majorP
        << data.center+getMinorP()
        << data.center-getMinorP();
    return ret;
}



GV_Vector GV_Ellipse::getNearestEndpoint(const GV_Vector& coord, double* dist) {
    double dist1, dist2;
    GV_Vector nearerPoint;
    GV_Vector startpoint = getStartpoint();
    GV_Vector endpoint = getEndpoint();

    dist1 = startpoint.distanceTo(coord);
    dist2 = endpoint.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = endpoint;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = startpoint;
    }

    return nearerPoint;
}



GV_Vector GV_Ellipse::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity, double* dist, GV_Entity** entity) {
    
    RB_DEBUG->print("GV_Ellipse::getNearestPointOnEntity");
    
    GV_Vector ret(false);
    
    double ang = getAngle();
    double dDistance = GV_MAXDOUBLE; 
    bool swap = false;
    bool majorSwap = false;

    GV_Vector normalized = (coord - data.center).rotate(-ang);

    // special case: coord in line with major axis:
    if (fabs(normalized.angle())<1.0e-6) {
        RB_DEBUG->print("GV_Ellipse::getNearestPointOnEntity: special case: 0.0 degrees");
        ret = GV_Vector(getMajorRadius(), 0.0);
        dDistance = ret.distanceTo(normalized);
    }
    
    else if (fabs(normalized.angle()-M_PI)<1.0e-6) {
        RB_DEBUG->print("GV_Ellipse::getNearestPointOnEntity: special case: 180.0 degrees");
        ret = GV_Vector(-getMajorRadius(), 0.0);
        dDistance = ret.distanceTo(normalized);
    }
    else {
        double dU = normalized.x;
        double dV = normalized.y;
        double dA = getMajorRadius();
        double dB = getMinorRadius();
        double dEpsilon = 1.0e-8;
        // iteration maximum
        int iMax = 32;
        //int riIFinal = 0;
        double rdX = 0.0;
        double rdY = 0.0;

        if (dA<dB) {
            double dum = dA;
            dA = dB;
            dB = dum;
            dum = dU;
            dU = dV;
            dV = dum;
            majorSwap = true;
        }

        if (dV<0.0) {
            dV*=-1.0;
            swap = true;
        }
        
        // initial guess 
        double dT = dB*(dV - dB); 
        
        // newton's method
        int i; 
        for (i = 0; i < iMax; i++) { 
            double dTpASqr = dT + dA*dA; 
            double dTpBSqr = dT + dB*dB; 
            double dInvTpASqr = 1.0/dTpASqr; 
            double dInvTpBSqr = 1.0/dTpBSqr; 
            double dXDivA = dA*dU*dInvTpASqr; 
            double dYDivB = dB*dV*dInvTpBSqr; 
            double dXDivASqr = dXDivA*dXDivA; 
            double dYDivBSqr = dYDivB*dYDivB; 
            double dF = dXDivASqr + dYDivBSqr - 1.0; 
            if ( fabs(dF) < dEpsilon ) { 
                // f(t0) is very close to zero:
                rdX = dXDivA*dA;
                rdY = dYDivB*dB; 
                //riIFinal = i; 
                break; 
            } 
            double dFDer = 2.0*(dXDivASqr*dInvTpASqr + dYDivBSqr*dInvTpBSqr); 
            
            double dRatio = dF/dFDer; 
            
            if ( fabs(dRatio) < dEpsilon ) { 
                // t1-t0 is very close to zero:
                rdX = dXDivA*dA; 
                rdY = dYDivB*dB; 
                //riIFinal = i; 
                break; 
            } 
            dT += dRatio; 
        } 

        if ( i == iMax ) { 
            // failed to converge:
            dDistance = GV_MAXDOUBLE;
        } 
        else {
            double dDelta0 = rdX - dU;
            double dDelta1 = rdY - dV; 
            dDistance = sqrt(dDelta0*dDelta0 + dDelta1*dDelta1); 
            ret = GV_Vector(rdX, rdY);
        }
    }
    
    if (ret.valid) {
        if (swap) {
            ret.y*=-1.0;
        }
        if (majorSwap) {
            double dum = ret.x;
            ret.x = ret.y;
            ret.y = dum;
        }
        ret = (ret.rotate(ang) + data.center);
        
        if (onEntity) {
            double a1 = data.center.angleTo(getStartpoint());
            double a2 = data.center.angleTo(getEndpoint());
            double a = data.center.angleTo(ret);
            if (!GV_Math::isAngleBetween(a, a1, a2, data.reversed)) {
                RB_DEBUG->print("GV_Ellipse::getNearestPointOnEntity: point not on entity");
                ret = GV_Vector(false);
            }
        }
    }

    if (dist!=NULL) {
        if (ret.valid) {
            *dist = dDistance;
        } else {
            *dist = GV_MAXDOUBLE;
        }
    }
    
    if (entity!=NULL) {
        if (ret.valid) {
            *entity = this;
        }
        else {
            *entity = NULL;
        }
    }
    
    return ret;
}



/**
 * @param tolerance Tolerance.
 *
 * @retval true if the given point is on this entity.
 * @retval false otherwise
 */
bool GV_Ellipse::isPointOnEntity(const GV_Vector& coord,
                                double tolerance) {
    double dist = getDistanceToPoint(coord, NULL, GV2::ResolveNone);
    return (dist<=tolerance);
}



GV_Vector GV_Ellipse::getNearestCenter(const GV_Vector& coord,
                                       double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



/**
 * @todo Implement this.
 */
GV_Vector GV_Ellipse::getNearestMiddle(const GV_Vector& /*coord*/,
                                       double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



GV_Vector GV_Ellipse::getNearestDist(double /*distance*/,
                                     const GV_Vector& /*coord*/,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



double GV_Ellipse::getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity,
                                      GV2::ResolveLevel, 
                                      double /*solidDist*/,
                                      bool /*visually*/) {
    double dist = GV_MAXDOUBLE;
    getNearestPointOnEntity(coord, true, &dist, entity);

    return dist;

}



/**
 * Approximates the ellipse with arcs.
 *
 * @param segments Number of arc segments per quarter.
 *
 * @return Polyline with arcs as segments or NULL. It is the caller's 
 *         responsibility to delete the polyline.
 */
GV_Polyline* GV_Ellipse::approximateWithArcs(int segments) {
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: stored start angle: %f", getAngle1());

    if (segments<=0) {
//        RB_DEBUG->print(RB_Debug::D_WARNING,
//            "GV_Ellipse::approximateWithArcs: "
//            "invalid number of segments: %d",
//            segments);
        return NULL;
    }
    
    if (getMajorRadius()<1.0e-6 || getMinorRadius()<1.0e-6) {
//        RB_DEBUG->print(RB_Debug::D_WARNING,
//            "GV_Ellipse::approximateWithArcs: "
//            "invalid ellipse major / minor: %f / %f",
//            getMajorRadius(), getMinorRadius());
        return NULL;
    }

    // special case: circular
    if (fabs(getMajorRadius()-getMinorRadius())<1.0e-6) {
        GV_Polyline* polyline = new GV_Polyline(NULL);
        polyline->setPen(getPen(false));
        polyline->setLayer(getLayer());

        GV_Arc arc(NULL, GV_ArcData(getCenter(), 
                            getMajorRadius(), 
                            getAngle1(),
                            getAngle2(),
                            isReversed()));
        polyline->addVertex(arc.getStartpoint(), arc.getBulge());
        polyline->addVertex(arc.getEndpoint());
    
        return polyline;
    }

    double majorRadius = getMajorRadius();
    double minorRadius = getMinorRadius();
    
    GV_Vector* rakPoint = new GV_Vector[segments+1];
    GV_Vector* rakCenter = new GV_Vector[segments];
    double* rafRadius = new double[segments];

    // intermediate ellipse quantities
    double majorRadiusSquare = majorRadius*majorRadius;
    double minorRadiusSquare = minorRadius*minorRadius;
    double minorTimesMajor = majorRadius*minorRadius;
    double invMinorMinusMajor;

    if (fabs(minorRadiusSquare-majorRadiusSquare)<1.0e-6) {
        invMinorMinusMajor = 1.0;
    }
    else {
        invMinorMinusMajor = 1.0/(minorRadiusSquare-majorRadiusSquare);
    }
    
    // segment points on ellipse in first quadrant. points generated ccw
    rakPoint[0] = GV_Vector(majorRadius,0.0);
    rakPoint[segments] = GV_Vector(0.0,minorRadius);

    // curvature at end points, store curvature for computing arcs
    double k0 = majorRadius/minorRadiusSquare;
    double k1 = minorRadius/majorRadiusSquare;
    
    // select ellipse points based on curvature properties
    double invSegments = 1.0/segments;
    int i;
    for (i = 1; i < segments; i++) {
        // curvature at new point is weighted average of curvature at ends
        double w1 = i*invSegments;
        double w0 = 1.0 - w1;
        double k = w0*k0 + w1*k1;
        // compute point having this curvature
        double tmp = GV_Math::pow(minorTimesMajor/k,2.0/3.0);
        rakPoint[i].x = majorRadius*sqrt(fabs((tmp-majorRadiusSquare)*invMinorMinusMajor));
        rakPoint[i].y = minorRadius*sqrt(fabs((tmp-minorRadiusSquare)*invMinorMinusMajor));
    }
    
    // compute arc at (a,0)
    GV_Circle circle(NULL, GV_CircleData());
    circle.createFrom3P(GV_Vector(rakPoint[1].x,-rakPoint[1].y),
        rakPoint[0], rakPoint[1]);
    rakCenter[0] = circle.getCenter();
    rafRadius[0] = circle.getRadius();
    
    // compute arc at (0,b)
    int last = segments-1;
    circle.createFrom3P(GV_Vector(-rakPoint[last].x,rakPoint[last].y),
        rakPoint[segments], rakPoint[last]);
    rakCenter[last] = circle.getCenter();
    rafRadius[last] = circle.getRadius();
    
    // compute arcs at intermediate points between (a,0) and (0,b)
    int iM, iP;
    for (iM = 0, i = 1, iP = 2; i < last; iM++, i++, iP++) {
        circle.createFrom3P(rakPoint[iM], rakPoint[i], rakPoint[iP]);
        rakCenter[i] = circle.getCenter();
        rafRadius[i] = circle.getRadius();
    }
    
    // create polyline from the arc data:
    GV_Polyline* polyline = new GV_Polyline(NULL);
    polyline->setPen(getPen(false));
    polyline->setLayer(getLayer());

    GV_Arc* arc = NULL;
    double limitAngle1 = GV_Math::correctAngle(
        getCenter().angleTo(getStartpoint()) - getAngle());
    double limitAngle2 = GV_Math::correctAngle(
        getCenter().angleTo(getEndpoint()) - getAngle());

    GV_Vector normalizedStartpoint = getStartpoint();
    normalizedStartpoint.move(-getCenter());
    normalizedStartpoint.rotate(-getAngle());
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: normalizedStartpoint: %f/%f",
//        normalizedStartpoint.x, normalizedStartpoint.y);
    
    GV_Vector normalizedEndpoint = getEndpoint();
    normalizedEndpoint.move(-getCenter());
    normalizedEndpoint.rotate(-getAngle());

//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: isFullEllipse: %d", isFullEllipse());
    
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: limitAngle1: %f", limitAngle1);
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: limitAngle2: %f", limitAngle2);

    // find out start and end quadrant:
    int startQuadrant = 1;
    int endQuadrant = 4;
    if (!isFullEllipse()) {
        if (data.angle1 < M_PI/2.0 - 1.0e-6) {
            startQuadrant = 1;
        } else if (data.angle1 < M_PI - 1.0e-6) {
            startQuadrant = 2;
        } else if (data.angle1 < M_PI/2.0*3.0 - 1.0e-6) {
            startQuadrant = 3;
        } else {
            startQuadrant = 4;
        }
        
        if (data.angle2 < M_PI/2.0 + 1.0e-6) {
            endQuadrant = 1;
        } else if (data.angle2 < M_PI + 1.0e-6) {
            endQuadrant = 2;
        } else if (data.angle2 < M_PI/2.0*3.0 + 1.0e-6) {
            endQuadrant = 3;
        } else {
            endQuadrant = 4;
        }
    }
    
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: startQuadrant: %d", startQuadrant);
//    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: endQuadrant: %d", endQuadrant);

    int quadrant = startQuadrant;
    bool done = false;
    bool adding = false;
    int counter = 0;
    do {
        counter++;
//        RB_DEBUG->print("GV_Ellipse::approximateWithArcs: quadrant: %d", quadrant);

        int startIndex;
        int endIndex;
        int step;
        if (quadrant==1 || quadrant==3) {
            startIndex = 0;
            endIndex = segments;
            step = 1;
        }
        else {
            startIndex = segments-1;
            endIndex = -1;
            step = -1;
        }
    
        for (i=startIndex; i!=endIndex && !done; i+=step) {
            if (quadrant==1) {
                arc = new GV_Arc(polyline, 
                    GV_ArcData(rakCenter[i], rafRadius[i], 
                        0.0, 2*M_PI, false));
                arc->trimStartpoint(rakPoint[i]);
                arc->trimEndpoint(rakPoint[i+1]);
            }
            else if (quadrant==2) {
                arc = new GV_Arc(polyline, 
                    GV_ArcData(GV_Vector(-rakCenter[i].x, rakCenter[i].y), 
                        rafRadius[i], 0.0, 2*M_PI, false));
                arc->trimStartpoint(GV_Vector(-rakPoint[i+1].x, rakPoint[i+1].y));
                arc->trimEndpoint(GV_Vector(-rakPoint[i].x, rakPoint[i].y));
            }
            else if (quadrant==3) {
                arc = new GV_Arc(polyline, 
                    GV_ArcData(-rakCenter[i], rafRadius[i], 
                        0.0, 2*M_PI, false));
                arc->trimStartpoint(-rakPoint[i]);
                arc->trimEndpoint(-rakPoint[i+1]);
            }
            else if (quadrant==4) {
                arc = new GV_Arc(polyline, 
                    GV_ArcData(GV_Vector(rakCenter[i].x, -rakCenter[i].y), 
                        rafRadius[i], 0.0, 2*M_PI, false));
                arc->trimStartpoint(GV_Vector(rakPoint[i+1].x, -rakPoint[i+1].y));
                arc->trimEndpoint(GV_Vector(rakPoint[i].x, -rakPoint[i].y));
            }
            
            if (!isFullEllipse()) {
                if (arc!=NULL && arc->getStartpoint().distanceTo(arc->getEndpoint())>1.0e-6) {
                    // does the segment start on ellipse arc?
                    bool startInside = GV_Math::isAngleBetween(arc->getStartpoint().angle(),
                        limitAngle1, limitAngle2, isReversed());
                    // does the segment end on ellipse arc?
                    bool endInside = GV_Math::isAngleBetween(arc->getEndpoint().angle(),
                        limitAngle1, limitAngle2+1.0e-6, isReversed());

                    if (adding && startInside && endInside && 
                        GV_Math::isAngleBetween(limitAngle2,
                            arc->getStartpoint().angle(), arc->getEndpoint().angle(), false)) {
                        
                        endInside = false;
                    }
                    
                    if (!adding && startInside && endInside && 
                        GV_Math::isAngleBetween(limitAngle1,
                            arc->getStartpoint().angle(), arc->getEndpoint().angle(), false)) {
                        
                        startInside = false;
                    }
                        
//                    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: "
//                        "startInside: %d", (int)startInside);
//                    RB_DEBUG->print("GV_Ellipse::approximateWithArcs: "
//                        "endInside: %d", (int)endInside);

                    if (startInside && endInside) {
                        // segment starts exactly at start of ellipse arc:
                        if (fabs(arc->getStartpoint().angle()-limitAngle1)<1.0e-6) {
                            adding = true;
                        }
                        // segment ends exactly at end of ellipse arc:
                        if (fabs(arc->getEndpoint().angle()-limitAngle2)<1.0e-6) {
                            done = true;
                        }
                        if (adding) {
                            polyline->addVertex(arc->getStartpoint(), arc->getBulge());
                        }
                    }
                    else if (endInside) {
                        RB_DEBUG->print("GV_Ellipse::approximateWithArcs: trimming startpoint");
                        arc->trimStartpoint(normalizedStartpoint);
                        polyline->addVertex(arc->getStartpoint(), arc->getBulge());
                        adding = true;
                    }
                    else if (startInside) {
                        if (adding) {
                            RB_DEBUG->print("GV_Ellipse::approximateWithArcs: trimming endpoint");
                            arc->trimEndpoint(normalizedEndpoint);
                            polyline->addVertex(arc->getStartpoint(), arc->getBulge());
                            done = true;
                        }
                    }
                }
            }
            else {
                if (arc!=NULL) {
                    polyline->addVertex(arc->getStartpoint(), arc->getBulge());
                }
            }
            
            if (arc!=NULL) {
                delete arc;
                arc = NULL;
            }
        }

        quadrant++;
        if (quadrant>4) {
            if (isFullEllipse()) {
                done = true;
            }
            quadrant=1;
        }
    } while(!done && counter<10);
    
    // close polyline
    if (normalizedEndpoint.distanceTo(polyline->getEndpoint())>1.0e-6) {
        polyline->addVertex(normalizedEndpoint);
    }

    // move to real position of ellipse:
    polyline->rotate(GV_Vector(0.0, 0.0), getAngle());
    polyline->move(getCenter());

    delete[] rakPoint;
    delete[] rakCenter;
    delete[] rafRadius;

    return polyline;

}



void GV_Ellipse::move(const GV_Vector& offset) {
    data.center.move(offset);
    //calculateEndpoints();
    calculateBorders();
}



void GV_Ellipse::rotate(const GV_Vector& center, double angle) {
    data.center.rotate(center, angle);
    data.majorP.rotate(angle);
    //calculateEndpoints();
    calculateBorders();
}



void GV_Ellipse::moveStartpoint(const GV_Vector& pos) {
    data.angle1 = getEllipseAngle(pos);
    //data.angle1 = data.center.angleTo(pos);
    //calculateEndpoints();
    calculateBorders();
}



void GV_Ellipse::moveEndpoint(const GV_Vector& pos) {
    data.angle2 = getEllipseAngle(pos);
    //data.angle2 = data.center.angleTo(pos);
    //calculateEndpoints();
    calculateBorders();
}


GV2::Ending GV_Ellipse::getTrimPoint(const GV_Vector& coord, 
        const GV_Vector& trimPoint) {
    
    double angEl = getEllipseAngle(trimPoint);
    double angM = getEllipseAngle(coord);

    if (GV_Math::getAngleDifference(angM, angEl)>M_PI) {
        return GV2::EndingStart;
    }
    else {
        return GV2::EndingEnd;
    }
}

double GV_Ellipse::getEllipseAngle(const GV_Vector& pos) {
    GV_Vector m = pos;
    m.rotate(data.center, -data.majorP.angle());
    GV_Vector v = m-data.center;
    v.scale(GV_Vector(1.0, 1.0/data.ratio));
    return v.angle(); 
}



void GV_Ellipse::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.center.scale(center, factor);
    data.majorP.scale(factor);
    //calculateEndpoints();
    calculateBorders();
}


/**
 * @todo deal with angles correctly
 */
void GV_Ellipse::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_Vector mp = data.center + data.majorP;

    data.center.mirror(axisPoint1, axisPoint2);
    mp.mirror(axisPoint1, axisPoint2);

    data.majorP = mp - data.center;

    if (!isFullEllipse()) {
        double a = axisPoint1.angleTo(axisPoint2);
        double a1 = data.angle1;
        double a2 = data.angle2;

        GV_Vector vec;
        vec.setPolar(1.0, a1);
        vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
        data.angle2 = GV_Math::correctAngle(vec.angle() - 2.0*a);

        vec.setPolar(1.0, a2);
        vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
        data.angle1 = GV_Math::correctAngle(vec.angle() - 2.0*a);

        //data.reversed = (!data.reversed);
    }

    //calculateEndpoints();
    calculateBorders();
}



void GV_Ellipse::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_Vector startpoint = getStartpoint();
    GV_Vector endpoint = getEndpoint();
    
    if (!isFullEllipse()) {
        if (ref.distanceTo(startpoint)<1.0e-4) {
            moveStartpoint(startpoint+offset);
        }
        if (ref.distanceTo(endpoint)<1.0e-4) {
            moveEndpoint(endpoint+offset);
        }
    }

    if (ref.distanceTo(data.center+data.majorP)<1.0e-4) {
        double mr = getMinorRadius();
        data.majorP += offset;
        setRatio(mr / getMajorRadius());
    }
    if (ref.distanceTo(data.center-data.majorP)<1.0e-4) {
        double minorRadius = getMinorRadius();
        data.majorP -= offset;
        setRatio(minorRadius / getMajorRadius());
    }

    if (ref.distanceTo(data.center+getMinorP())<1.0e-4) {
        //double majorRadius = getMajorRadius();
        setMinorP(getMinorP() + offset);
    }
    if (ref.distanceTo(data.center-getMinorP())<1.0e-4) {
        //double majorRadius = getMajorRadius();
        setMinorP(getMinorP() - offset);
    }

    if (ref.distanceTo(data.center)<1.0e-4) {
        data.center += offset;
    }

//    RB_DEBUG->print("GV_Ellipse::moveRef: angle1: %f", data.angle1);
//    RB_DEBUG->print("GV_Ellipse::moveRef: angle2: %f", data.angle2);
}



void GV_Ellipse::draw(GV_Painter* painter, GV_GraphicView* view, double /*patternOffset*/) {
    RB_DEBUG->print("GV_Ellipse::draw");

    if (painter==NULL || view==NULL || getMajorRadius()<1.0e-6 ||
        getMinorRadius()<1.0e-6) {
        return;
    }

    if (getPen().getLineType()==GV2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==GV2::ModePreview) {

        RB_DEBUG->print("GV_Ellipse::draw: Qt method");
        
        painter->drawEllipse(view->toGui(getCenter()),
                             getMajorRadius() * view->getFactor().x,
                             getMinorRadius() * view->getFactor().x,
                             getAngle(),
                             GV_Math::correctAngle(getAngle1()), 
                             GV_Math::correctAngle(getAngle2()),
                             isReversed());
    } else {
        double styleFactor = getStyleFactor(view);
        if (styleFactor<0.0) {
            RB_DEBUG->print("GV_Ellipse::draw: Qt method (style factor)");
            painter->drawEllipse(view->toGui(getCenter()),
                             getMajorRadius() * view->getFactor().x,
                             getMinorRadius() * view->getFactor().x,
                             getAngle(),
                             GV_Math::correctAngle(getAngle1()), 
                             GV_Math::correctAngle(getAngle2()),
                             isReversed());
            return;
        }
            
        RB_DEBUG->print("GV_Ellipse::draw: QCad method");
        
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

        // Pen to draw pattern is always solid:
        GV_Pen pen = painter->getPen();
        pen.setLineType(GV2::SolidLine);
        painter->setPen(pen);
        
        double a1;
        double a2;
        if (data.reversed) {
            RB_DEBUG->print("GV_Ellipse::draw: reversed");
            a2 = getAngle1();
            a1 = getAngle2();
        } else {
            RB_DEBUG->print("GV_Ellipse::draw: not reversed");
            a1 = getAngle1();
            a2 = getAngle2();
        }

        if (a1>a2) {
            a1-=M_PI*2;
        }
            
//        RB_DEBUG->print("GV_Ellipse::draw: a1: %f, a2: %f", a1, a2);

        double* da;     // array of distances in x.
        int i;          // index counter

        double length = getAngleLength();
//        RB_DEBUG->print("GV_Ellipse::draw: length: %f", length);

        // create pattern:
        da = new double[pat->num];

        double tot=0.0;
        i=0;
        bool done = false;
        double curA = a1;
        double curR;
        GV_Vector cp = view->toGui(getCenter());
        double r1 = getMajorRadius() * view->getFactor().x;
        double r2 = getMinorRadius() * view->getFactor().x;

        do {
            curR = sqrt(GV_Math::pow(getMinorRadius()*cos(curA), 2.0)
                        + GV_Math::pow(getMajorRadius()*sin(curA), 2.0));

            if (curR>1.0e-6) {
                da[i] = fabs(pat->pattern[i] * styleFactor) / curR;
                if (pat->pattern[i] * styleFactor > 0.0) {

                    if (tot+fabs(da[i])<length) {
//                        RB_DEBUG->print("GV_Ellipse::draw: from %f to %f", curA, curA + da[i]);
                        painter->drawEllipse(cp,
                                             r1, r2,
                                             getAngle(),
                                             curA,
                                             curA + da[i],
                                             false);
                    } else {
//                        RB_DEBUG->print("GV_Ellipse::draw: from %f to %f", curA, a2);
                        painter->drawEllipse(cp,
                                             r1, r2,
                                             getAngle(),
                                             curA,
                                             a2,
                                             false);
                    }
                }
            }
            curA+=da[i];
            tot+=fabs(da[i]);
            done=tot>length;

            i++;
            if (i>=pat->num) {
                i=0;
            }
        } while(!done);

        delete[] da;
    }
    
    RB_DEBUG->print("GV_Ellipse::draw: OK");
}



QVariant GV_Ellipse::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_ELLIPSE_CENTER_X) {
        return data.center.x;
    } else if (name==GV_ELLIPSE_CENTER_Y) {
        return data.center.y;
    } else if (name==GV_ELLIPSE_MAJORP_X) {
        return data.majorP.x;
    } else if (name==GV_ELLIPSE_MAJORP_Y) {
        return data.majorP.y;
    } else if (name==GV_ELLIPSE_RATIO) {
        return data.ratio;
    } else if (name==GV_ELLIPSE_STARTANGLE) {
        return GV_Math::rad2deg(data.angle1);
    } else if (name==GV_ELLIPSE_ENDANGLE) {
        return GV_Math::rad2deg(data.angle2);
    } else if (name==GV_ELLIPSE_REVERSED) {
        return data.reversed;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Ellipse::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret 
        << GV_ELLIPSE_CENTER_X
        << GV_ELLIPSE_CENTER_Y
        << GV_ELLIPSE_MAJORP_X
        << GV_ELLIPSE_MAJORP_Y
        << GV_ELLIPSE_RATIO
        << GV_ELLIPSE_STARTANGLE
        << GV_ELLIPSE_ENDANGLE
        << GV_ELLIPSE_REVERSED;
    }
    return ret;
}


void GV_Ellipse::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_ELLIPSE_CENTER_X) {
        data.center.x = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_ELLIPSE_CENTER_Y) {
        data.center.y = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_ELLIPSE_MAJORP_X) {
        data.majorP.x = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_ELLIPSE_MAJORP_Y) {
        data.majorP.y = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_ELLIPSE_RATIO) {
        setRatio(GV_Math::eval(value.toString()));
        calculateBorders();
    } else if (name==GV_ELLIPSE_STARTANGLE) {
        setAngle1(GV_Math::deg2rad(GV_Math::eval(value.toString())));
        calculateBorders();
    } else if (name==GV_ELLIPSE_ENDANGLE) {
        setAngle2(GV_Math::deg2rad(GV_Math::eval(value.toString())));
        calculateBorders();
    } else if (name==GV_ELLIPSE_REVERSED) {
        data.reversed = variantToBool(value);
        calculateBorders();
    } else {
        GV_Entity::setProperty(name, value);
    }
}
    
    
