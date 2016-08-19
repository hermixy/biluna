/****************************************************************************
** $Id: rs_solid.cpp 8991 2008-02-11 15:41:52Z andrew $
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


#include "rs_solid.h"

#include "rs_graphicview.h"
#include "rs_painter.h"



const char* RS_Solid::propertyPoint1X = QT_TRANSLATE_NOOP("QObject", "Point 1|X");
const char* RS_Solid::propertyPoint1Y = QT_TRANSLATE_NOOP("QObject", "Point 1|Y");
const char* RS_Solid::propertyPoint2X = QT_TRANSLATE_NOOP("QObject", "Point 2|X");
const char* RS_Solid::propertyPoint2Y = QT_TRANSLATE_NOOP("QObject", "Point 2|Y");
const char* RS_Solid::propertyPoint3X = QT_TRANSLATE_NOOP("QObject", "Point 3|X");
const char* RS_Solid::propertyPoint3Y = QT_TRANSLATE_NOOP("QObject", "Point 3|Y");
const char* RS_Solid::propertyPoint4X = QT_TRANSLATE_NOOP("QObject", "Point 4|X");
const char* RS_Solid::propertyPoint4Y = QT_TRANSLATE_NOOP("QObject", "Point 4|Y");



/**
 * Default constructor.
 */
RS_Solid::RS_Solid(RS_EntityContainer* parentContainer,
                   const RS_SolidData& d)
        : RS_Entity(parentContainer), RS_AtomicEntity(parentContainer), data(d) {
    calculateBorders();
}



/**
 * @return Corner number 'num'. 
 */
RS_Vector RS_Solid::getCorner(int num) const {
    if (num>=0 && num<4) {
        return data.corner[num];
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Solid::getCorner: Illegal corner requested from Solid");
        return RS_Vector(false);
    }
}
    
    
 
/**
 * Sets the given corner.
 */
void RS_Solid::setCorner(int num, const RS_Vector& v) {
    if (num>=0 && num<4) {
        data.corner[num] = v;
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Solid::setCorner: Illegal corner index");
    }
}



RS_VectorList RS_Solid::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret;
        
    ret << data.corner[0];
    ret << data.corner[1];
    ret << data.corner[2];
    if (!isTriangle()) {
        ret << data.corner[3];
    }

    return ret;
}

RS_Vector RS_Solid::getNearestRef(const RS_Vector& coord,
                                   double* dist,
                                   RS2::RefMode refMode) {

    return RS_AtomicEntity::getNearestRef(coord, dist, refMode);
}

RS_Vector RS_Solid::getNearestSelectedRef(const RS_Vector& coord,
        double* dist,
        RS2::RefMode refMode) {

    return RS_AtomicEntity::getNearestSelectedRef(coord, dist, refMode);
}




/**
 * @return The orientation (cw, ccw) of the solid.
 */
RS2::ContourOrientation RS_Solid::getOrientation() {
    double area = 0.0;
                
    area += data.corner[0].x * data.corner[1].y - 
            data.corner[0].y * data.corner[1].x;
    area += data.corner[1].x * data.corner[2].y - 
            data.corner[1].y * data.corner[2].x;
    if (isTriangle()){
        area += data.corner[2].x * data.corner[0].y - 
                data.corner[2].y * data.corner[0].x;
    }
    else {
        area += data.corner[2].x * data.corner[3].y - 
                data.corner[2].y * data.corner[3].x;
        area += data.corner[3].x * data.corner[0].y - 
                data.corner[3].y * data.corner[0].x;
    }

    if (area<0.0) {
        return RS2::ClockWise;
    }
    else {
        return RS2::CounterClockWise;
    }
}
    


/**
 * Changes the orientation of the solid.
 */
void RS_Solid::reverse() {
    RS_Vector tmp;

    RS_DEBUG->print("RS_Solid::reverse: "
        "before: %f/%f - %f/%f - %f/%f",
        data.corner[0].x,
        data.corner[0].y,
        data.corner[1].x,
        data.corner[1].y,
        data.corner[2].x,
        data.corner[2].y
        );

    if (isTriangle()) {
        tmp = data.corner[2];
        data.corner[2] = data.corner[1];
        if (data.corner[3].valid) {
            data.corner[3] = data.corner[1];
        }
        else {
            data.corner[3] = data.corner[1];
            data.corner[3].valid = false;
        }
        data.corner[1] = tmp;
    }
    else {
        RS_DEBUG->print("RS_Solid::reverse: not a triangle");
        tmp = data.corner[1];
        data.corner[1] = data.corner[3];
        data.corner[3] = tmp;
    }
    
    RS_DEBUG->print("RS_Solid::reverse: "
        "after: %f/%f - %f/%f - %f/%f",
        data.corner[0].x,
        data.corner[0].y,
        data.corner[1].x,
        data.corner[1].y,
        data.corner[2].x,
        data.corner[2].y
        );
}



/**
 * Shapes this Solid into a standard arrow (used in dimensions).
 *
 * @param point The point the arrow points to.
 * @param angle Direction of the arrow.
 * @param arrowSize Size of arrow (length).
 */
void RS_Solid::shapeArrow(const RS_Vector& point,
                          double angle,
                          double arrowSize) {

    double cosv1, sinv1, cosv2, sinv2;
    double arrowSide = arrowSize/cos(0.165);

    cosv1 = cos(angle+0.165)*arrowSide;
    sinv1 = sin(angle+0.165)*arrowSide;
    cosv2 = cos(angle-0.165)*arrowSide;
    sinv2 = sin(angle-0.165)*arrowSide;

    data.corner[0] = point;
    data.corner[1] = RS_Vector(point.x - cosv1, point.y - sinv1);
    data.corner[2] = RS_Vector(point.x - cosv2, point.y - sinv2);
    data.corner[3] = RS_Vector(false);

    calculateBorders();
}



void RS_Solid::calculateBorders(bool /*visibleOnly*/) {
    resetBorders();

    for (int i=0; i<4; ++i) {
        if (data.corner[i].valid) {
            minV = RS_Vector::minimum(minV, data.corner[i]);
            maxV = RS_Vector::maximum(maxV, data.corner[i]);
        }
    }
}



RS_Vector RS_Solid::getNearestEndpoint(const RS_Vector& coord, double* dist) {

    double minDist = RS_MAXDOUBLE;
    double curDist;
    RS_Vector ret;

    for (int i=0; i<4; ++i) {
        if (data.corner[i].valid) {
            curDist = data.corner[i].distanceTo(coord);
            if (curDist<minDist) {
                ret = data.corner[i];
                minDist = curDist;
            }
        }
    }

    if (dist!=NULL) {
        *dist = minDist;
    }

    return ret;
}



/**
 * @todo Implement this.
 */
RS_Vector RS_Solid::getNearestPointOnEntity(const RS_Vector& /*coord*/,
        bool /*onEntity*/, double* /*dist*/, RS_Entity** /*entity*/) {

    RS_Vector ret(false);
    return ret;
}



RS_Vector RS_Solid::getNearestCenter(const RS_Vector& /*coord*/,
                                     double* dist) {

    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }

    return RS_Vector(false);
}



RS_Vector RS_Solid::getNearestMiddle(const RS_Vector& /*coord*/,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_Solid::getNearestDist(double /*distance*/,
                                   const RS_Vector& /*coord*/,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



/**
 * @return Distance from one of the boundry lines of this solid to given point.
 */
double RS_Solid::getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** /*entity*/,
                                    RS2::ResolveLevel level,
                                    double solidDist,
                                    bool /*visually*/) {
    double minDist = RS_MAXDOUBLE;

    for (int i=0; i<4; ++i) {
        RS_Line l(NULL, RS_LineData(data.corner[i], data.corner[(i+1)%4]));
        double dist = l.getDistanceToPoint(coord, NULL, level, solidDist);
        if (dist<minDist) {
            minDist = dist;
        }
    }

    return minDist;
}



void RS_Solid::move(const RS_Vector& offset) {
    for (int i=0; i<4; ++i) {
        data.corner[i].move(offset);
    }
    calculateBorders();
}



void RS_Solid::rotate(const RS_Vector& center, double angle) {
    for (int i=0; i<4; ++i) {
        data.corner[i].rotate(center, angle);
    }
    calculateBorders();
}



void RS_Solid::scale(const RS_Vector& center, const RS_Vector& factor) {
    for (int i=0; i<4; ++i) {
        data.corner[i].scale(center, factor);
    }
    calculateBorders();
}



void RS_Solid::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    for (int i=0; i<4; ++i) {
        data.corner[i].mirror(axisPoint1, axisPoint2);
    }
    calculateBorders();
}



void RS_Solid::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_AtomicEntity::moveRef(ref, offset);
    for (int i=0; i<4; i++) {
        if (ref.distanceTo(data.corner[i])<1.0e-4) {
           data.corner[i].move(offset);
        }
    }
}



void RS_Solid::stretch(const RS_Vector& firstCorner,
                                 const RS_Vector& secondCorner,
                                 const RS_Vector& offset) {

    for (int i=0; i<4; i++) {
        if (data.corner[i].isInWindow(firstCorner, secondCorner)) {
           data.corner[i].move(offset);
        }
    }
    
    RS_AtomicEntity::stretch(firstCorner, secondCorner, offset);
}



void RS_Solid::isoProject(RS2::IsoProjectionType type, double /*segmentLength*/) {
    for (int i=0; i<4; ++i) {
        data.corner[i].isoProject(type);
    }
    calculateBorders();
    update();
}


void RS_Solid::draw(RS_Painter* painter, RS_GraphicView* view, 
    double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

    RS_SolidData d = getData();
    if (isTriangle()) {
        painter->fillTriangle(view->toGui(getCorner(0)),
                              view->toGui(getCorner(1)),
                              view->toGui(getCorner(2)));
    }
}




RS_Variant RS_Solid::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_SOLID_POINT1_X) {
        return data.corner[0].x;
    } else if (name==RS_SOLID_POINT1_Y) {
        return data.corner[0].y;
    } else if (name==RS_SOLID_POINT2_X) {
        return data.corner[1].x;
    } else if (name==RS_SOLID_POINT2_Y) {
        return data.corner[1].y;
    } else if (name==RS_SOLID_POINT3_X) {
        return data.corner[2].x;
    } else if (name==RS_SOLID_POINT3_Y) {
        return data.corner[2].y;
    } else if (name==RS_SOLID_POINT4_X) {
        return data.corner[3].x;
    } else if (name==RS_SOLID_POINT4_Y) {
        return data.corner[3].y;
    } else {
        return RS_AtomicEntity::getProperty(name, def);
    }
}



RS_StringList RS_Solid::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_AtomicEntity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_SOLID_POINT1_X
            << RS_SOLID_POINT1_Y
            << RS_SOLID_POINT2_X
            << RS_SOLID_POINT2_Y
            << RS_SOLID_POINT3_X
            << RS_SOLID_POINT3_Y
            << RS_SOLID_POINT4_X
            << RS_SOLID_POINT4_Y;
    }
    return ret;
}


void RS_Solid::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_SOLID_POINT1_X) {
        data.corner[0].x = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT1_Y) {
        data.corner[0].y = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT2_X) {
        data.corner[1].x = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT2_Y) {
        data.corner[1].y = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT3_X) {
        data.corner[2].x = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT3_Y) {
        data.corner[2].y = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT4_X) {
        data.corner[3].x = RS_Math::eval(value.toString());
    } else if (name==RS_SOLID_POINT4_Y) {
        data.corner[3].y = RS_Math::eval(value.toString());
    } else {
        RS_AtomicEntity::setProperty(name, value);
    }
    calculateBorders();
    update();
}

