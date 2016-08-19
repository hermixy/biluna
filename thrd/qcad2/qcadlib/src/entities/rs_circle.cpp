/****************************************************************************
** $Id: rs_circle.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "rs_circle.h"

//#include <values.h>

#include "rs_constructionline.h"
#include "rs_information.h"
#include "rs_graphicview.h"
#include "rs_painter.h"



const char* RS_Circle::propertyCenterX = QT_TRANSLATE_NOOP("QObject", "Center|X");
const char* RS_Circle::propertyCenterY = QT_TRANSLATE_NOOP("QObject", "Center|Y");
const char* RS_Circle::propertyRadius = QT_TRANSLATE_NOOP("QObject", "Radius");


/**
 * Default constructor.
 */
RS_Circle::RS_Circle(RS_EntityContainer* parentContainer,
                     const RS_CircleData& d)
        : RS_Entity(parentContainer), RS_AtomicEntity(parentContainer), data(d) {
    calculateBorders();
}



void RS_Circle::calculateBorders(bool /*visibleOnly*/) {
    RS_Vector r(data.radius,data.radius,0.0);
    minV = data.center - r;
    maxV = data.center + r;
}



/**
 * @return Angle length in rad.
 */
double RS_Circle::getAngleLength() const {
    return 2*M_PI;
}



/**
 * @return Length of the circle which is the circumference.
 */
double RS_Circle::getLength() {
    return 2*M_PI*data.radius;
}


/**
 * Creates this circle from a center point and a radius.
 *
 * @param c Center.
 * @param r Radius
 */
bool RS_Circle::createFromCR(const RS_Vector& c, double r) {
    if (fabs(r)>RS_TOLERANCE) {
        data.radius = fabs(r);
        data.center = c;
        return true;
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFromCR(): "
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
bool RS_Circle::createFrom2P(const RS_Vector& p1, const RS_Vector& p2) {
    if (p1.distanceTo(p2)>RS_TOLERANCE) {
        data.radius = p1.distanceTo(p2)/2.0;
        data.center = p1 + (p2-p1)/2.0;
        return true;
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom2P(): "
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
bool RS_Circle::createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                             const RS_Vector& p3) {
    if (p1.distanceTo(p2)>RS_TOLERANCE &&
            p2.distanceTo(p3)>RS_TOLERANCE &&
            p3.distanceTo(p1)>RS_TOLERANCE) {

        // middle points between 3 points:
        RS_Vector mp1, mp2;
        RS_Vector dir1, dir2;
        double a1, a2;

        // intersection of two middle lines
        mp1 = (p1 + p2)/2.0;
        a1 = p1.angleTo(p2) + M_PI/2.0;
        dir1.setPolar(100.0, a1);
        mp2 = (p2 + p3)/2.0;
        a2 = p2.angleTo(p3) + M_PI/2.0;
        dir2.setPolar(100.0, a2);

        RS_ConstructionLineData d1(mp1, mp1 + dir1);
        RS_ConstructionLineData d2(mp2, mp2 + dir2);
        RS_ConstructionLine midLine1(NULL, d1);
        RS_ConstructionLine midLine2(NULL, d2);

        RS_VectorList sol =
            RS_Information::getIntersection(&midLine1, &midLine2);

        data.center = sol.get(0);
        data.radius = data.center.distanceTo(p3);

        if (sol.get(0).valid && data.radius<1.0e14 && data.radius>RS_TOLERANCE) {
            return true;
        } else {
            RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
                            "Cannot create a circle with inf radius.");
            return false;
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



RS_VectorList RS_Circle::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_Vector v1(data.radius, 0.0);
    RS_Vector v2(0.0, data.radius);
    
    RS_VectorList ret(data.center, 
        data.center+v1, data.center+v2,
        data.center-v1, data.center-v2);
    return ret;
}


/**
 * @return Always an invalid vector.
 */
RS_Vector RS_Circle::getNearestEndpoint(const RS_Vector& /*coord*/, double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_Circle::getNearestPointOnEntity(const RS_Vector& coord,
        bool /*onEntity*/, double* dist, RS_Entity** entity) {

    RS_Vector vec(false);
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



RS_Vector RS_Circle::getNearestCenter(const RS_Vector& coord,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



RS_Vector RS_Circle::getNearestMiddle(const RS_Vector& /*coord*/,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_Circle::getNearestDist(double /*distance*/,
                                    const RS_Vector& /*coord*/,
                                    double* dist) {

    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}

RS_Vector RS_Circle::getNearestDist(double /*distance*/,
                                  bool /*startp*/) {

    return RS_Vector(false);
}



double RS_Circle::getDistanceToPoint(const RS_Vector& coord,
                                     RS_Entity** entity,
                                     RS2::ResolveLevel, double,
                                     bool) {
    if (entity!=NULL) {
        *entity = this;
    }

    return fabs((coord-data.center).magnitude() - data.radius);
}



RS2::Side RS_Circle::getSideOfPoint(const RS_Vector& coord) {
    if (data.center.distanceTo(coord) < data.radius) {
        return RS2::LeftHand;
    }
    else {
        return RS2::RightHand;
    }
}



void RS_Circle::move(const RS_Vector& offset) {
    data.center.move(offset);
    calculateBorders();
}



void RS_Circle::rotate(const RS_Vector& center, double angle) {
    data.center.rotate(center, angle);
    calculateBorders();
}



void RS_Circle::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.center.scale(center, factor);
    data.radius *= factor.x;
    calculateBorders();
}



void RS_Circle::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.center.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}


void RS_Circle::draw(RS_Painter* painter, RS_GraphicView* view, 
    double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }


    // simple style-less lines
    if (getPen().getLineType()==RS2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==RS2::ModePreview) {

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
        RS_LineTypePattern* pat;
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
        RS_Pen pen = painter->getPen();
        pen.setLineType(RS2::SolidLine);
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
        RS_Vector cp = view->toGui(getCenter());
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



void RS_Circle::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_Vector v1(data.radius, 0.0);
    RS_Vector v2(0.0, data.radius);
    
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


RS_Variant RS_Circle::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_CIRCLE_CENTER_X) {
        return data.center.x;
    } else if (name==RS_CIRCLE_CENTER_Y) {
        return data.center.y;
    } else if (name==RS_CIRCLE_RADIUS) {
        return data.radius;
    } else {
        return RS_Entity::getProperty(name, def);
    }
}



RS_StringList RS_Circle::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret 
        << RS_CIRCLE_RADIUS
        << RS_CIRCLE_CENTER_X
        << RS_CIRCLE_CENTER_Y;
    }
    return ret;
}


void RS_Circle::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_CIRCLE_CENTER_X) {
        data.center.x = RS_Math::eval(value.toString());
        calculateBorders();
    } else if (name==RS_CIRCLE_CENTER_Y) {
        data.center.y = RS_Math::eval(value.toString());
        calculateBorders();
    } else if (name==RS_CIRCLE_RADIUS) {
        setRadius(RS_Math::eval(value.toString()));
        calculateBorders();
    } else {
        RS_Entity::setProperty(name, value);
    }
}
