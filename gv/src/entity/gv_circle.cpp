/****************************************************************************
** $Id: gv_circle.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "gv_circle.h"

//#include <values.h>

#include "gv_constructionline.h"
#include "gv_information.h"
#include "gv_graphicview.h"
#include "gv_painter.h"



const char* GV_Circle::propertyCenterX = QT_TRANSLATE_NOOP("QObject", "Center|X");
const char* GV_Circle::propertyCenterY = QT_TRANSLATE_NOOP("QObject", "Center|Y");
const char* GV_Circle::propertyRadius = QT_TRANSLATE_NOOP("QObject", "Radius");


/**
 * Default constructor.
 */
GV_Circle::GV_Circle(GV_EntityContainer* parentContainer,
                     const GV_CircleData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {
    calculateBorders();
}



void GV_Circle::calculateBorders(bool /*visibleOnly*/) {
    GV_Vector r(data.radius,data.radius,0.0);
    minV = data.center - r;
    maxV = data.center + r;
}



/**
 * @return Angle length in rad.
 */
double GV_Circle::getAngleLength() const {
    return 2*M_PI;
}



/**
 * @return Length of the circle which is the circumference.
 */
double GV_Circle::getLength() {
    return 2*M_PI*data.radius;
}


/**
 * Creates this circle from a center point and a radius.
 *
 * @param c Center.
 * @param r Radius
 */
bool GV_Circle::createFromCR(const GV_Vector& c, double r) {
    if (fabs(r)>GV_TOLERANCE) {
        data.radius = fabs(r);
        data.center = c;
        return true;
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Circle::createFromCR(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



/**
 * Creates this circle from two opposite points.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 */
bool GV_Circle::createFrom2P(const GV_Vector& p1, const GV_Vector& p2) {
    if (p1.distanceTo(p2)>GV_TOLERANCE) {
        data.radius = p1.distanceTo(p2)/2.0;
        data.center = p1 + (p2-p1)/2.0;
        return true;
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Circle::createFrom2P(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



/**
 * Creates this circle from 3 given points which define the circle line.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 * @param p3 3rd point.
 */
bool GV_Circle::createFrom3P(const GV_Vector& p1, const GV_Vector& p2,
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

        if (sol.get(0).valid && data.radius<1.0e14 && data.radius>GV_TOLERANCE) {
            return true;
        } else {
            RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Circle::createFrom3P(): "
                            "Cannot create a circle with inf radius.");
            return false;
        }
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Circle::createFrom3P(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



GV_VectorList GV_Circle::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_Vector v1(data.radius, 0.0);
    GV_Vector v2(0.0, data.radius);
    
    GV_VectorList ret(data.center, 
        data.center+v1, data.center+v2,
        data.center-v1, data.center-v2);
    return ret;
}


/**
 * @return Always an invalid vector.
 */
GV_Vector GV_Circle::getNearestEndpoint(const GV_Vector& /*coord*/, double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



GV_Vector GV_Circle::getNearestPointOnEntity(const GV_Vector& coord,
        bool /*onEntity*/, double* dist, GV_Entity** entity) {

    GV_Vector vec(false);
    if (entity!=NULL) {
        *entity = this;
    }

    double angle = (coord-data.center).angle();
    vec.setPolar(data.radius, angle);
    vec+=data.center;

    if (dist!=NULL) {
        *dist = fabs((vec-data.center).magnitude()-data.radius);
    }

    return vec;
}



GV_Vector GV_Circle::getNearestCenter(const GV_Vector& coord,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



GV_Vector GV_Circle::getNearestMiddle(const GV_Vector& /*coord*/,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



GV_Vector GV_Circle::getNearestDist(double /*distance*/,
                                    const GV_Vector& /*coord*/,
                                    double* dist) {

    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}

GV_Vector GV_Circle::getNearestDist(double /*distance*/,
                                  bool /*startp*/) {

    return GV_Vector(false);
}



double GV_Circle::getDistanceToPoint(const GV_Vector& coord,
                                     GV_Entity** entity,
                                     GV2::ResolveLevel, double,
                                     bool) {
    if (entity!=NULL) {
        *entity = this;
    }

    return fabs((coord-data.center).magnitude() - data.radius);
}



GV2::Side GV_Circle::getSideOfPoint(const GV_Vector& coord) {
    if (data.center.distanceTo(coord) < data.radius) {
        return GV2::LeftHand;
    }
    else {
        return GV2::RightHand;
    }
}



void GV_Circle::move(const GV_Vector& offset) {
    data.center.move(offset);
    calculateBorders();
}



void GV_Circle::rotate(const GV_Vector& center, double angle) {
    data.center.rotate(center, angle);
    calculateBorders();
}



void GV_Circle::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.center.scale(center, factor);
    data.radius *= factor.x;
    calculateBorders();
}



void GV_Circle::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.center.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}


void GV_Circle::draw(GV_Painter* painter, GV_GraphicView* view, 
    double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }


    // simple style-less lines
    if (getPen().getLineType()==GV2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==GV2::ModePreview) {

        painter->drawArc(view->toGui(getCenter()),
                         getRadius() * view->getFactor().x,
                         0.0, 2*M_PI,
                         false);
    } else {
        double styleFactor = getStyleFactor(view);
        if (styleFactor<0.0) {
            painter->drawArc(view->toGui(getCenter()),
                         getRadius() * view->getFactor().x,
                         0.0, 2*M_PI,
                         false);
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

        double* da;     // array of distances in x.
        int i;          // index counter

        double length = getAngleLength();

        // create pattern:
        da = new double[pat->num];

        for (i=0; i<pat->num; ++i) {
            da[i] = fabs(pat->pattern[i] * styleFactor)/getRadius();
        }

        double tot=0.0;
        i=0;
        bool done = false;
        double curA = 0.0;
        //double cx = getCenter().x * factor.x + offsetX;
        //double cy = - a->getCenter().y * factor.y + getHeight() - offsetY;
        GV_Vector cp = view->toGui(getCenter());
        double r = getRadius() * view->getFactor().x;

        do {
            if (pat->pattern[i] * styleFactor > 0.0) {
                if (tot+fabs(da[i])<length) {
                    painter->drawArc(cp, r,
                                     curA,
                                     curA + da[i],
                                     false);
                } else {
                    painter->drawArc(cp, r,
                                     curA,
                                     2*M_PI,
                                     false);
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
}



void GV_Circle::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_Vector v1(data.radius, 0.0);
    GV_Vector v2(0.0, data.radius);
    
    if (ref.distanceTo(data.center + v1)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center + v1 + offset);
    } else if (ref.distanceTo(data.center + v2)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center + v2 + offset);
    } else if (ref.distanceTo(data.center - v1)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center - v1 + offset);
    } else if (ref.distanceTo(data.center - v2)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center - v2 + offset);
    } else if (ref.distanceTo(data.center)<1.0e-4) {
        data.center += offset;
    } 
    calculateBorders();
}


QVariant GV_Circle::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_CIRCLE_CENTER_X) {
        return data.center.x;
    } else if (name==GV_CIRCLE_CENTER_Y) {
        return data.center.y;
    } else if (name==GV_CIRCLE_RADIUS) {
        return data.radius;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Circle::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret 
        << GV_CIRCLE_RADIUS
        << GV_CIRCLE_CENTER_X
        << GV_CIRCLE_CENTER_Y;
    }
    return ret;
}


void GV_Circle::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_CIRCLE_CENTER_X) {
        data.center.x = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_CIRCLE_CENTER_Y) {
        data.center.y = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_CIRCLE_RADIUS) {
        setRadius(GV_Math::eval(value.toString()));
        calculateBorders();
    } else {
        GV_Entity::setProperty(name, value);
    }
}
