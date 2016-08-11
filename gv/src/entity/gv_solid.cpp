/****************************************************************************
** $Id: gv_solid.cpp 8991 2008-02-11 15:41:52Z andrew $
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


#include "gv_solid.h"

#include "gv_graphicview.h"
#include "gv_painter.h"



const char* GV_Solid::propertyPoint1X = QT_TRANSLATE_NOOP("QObject", "Point 1|X");
const char* GV_Solid::propertyPoint1Y = QT_TRANSLATE_NOOP("QObject", "Point 1|Y");
const char* GV_Solid::propertyPoint2X = QT_TRANSLATE_NOOP("QObject", "Point 2|X");
const char* GV_Solid::propertyPoint2Y = QT_TRANSLATE_NOOP("QObject", "Point 2|Y");
const char* GV_Solid::propertyPoint3X = QT_TRANSLATE_NOOP("QObject", "Point 3|X");
const char* GV_Solid::propertyPoint3Y = QT_TRANSLATE_NOOP("QObject", "Point 3|Y");
const char* GV_Solid::propertyPoint4X = QT_TRANSLATE_NOOP("QObject", "Point 4|X");
const char* GV_Solid::propertyPoint4Y = QT_TRANSLATE_NOOP("QObject", "Point 4|Y");



/**
 * Default constructor.
 */
GV_Solid::GV_Solid(GV_EntityContainer* parentContainer,
                   const GV_SolidData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {
    calculateBorders();
}



/**
 * @return Corner number 'num'. 
 */
GV_Vector GV_Solid::getCorner(int num) const {
    if (num>=0 && num<4) {
        return data.corner[num];
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Solid::getCorner: Illegal corner requested from Solid");
        return GV_Vector(false);
    }
}
    
    
 
/**
 * Sets the given corner.
 */
void GV_Solid::setCorner(int num, const GV_Vector& v) {
    if (num>=0 && num<4) {
        data.corner[num] = v;
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Solid::setCorner: Illegal corner index");
    }
}



GV_VectorList GV_Solid::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;
        
    ret << data.corner[0];
    ret << data.corner[1];
    ret << data.corner[2];
    if (!isTriangle()) {
        ret << data.corner[3];
    }

    return ret;
}

GV_Vector GV_Solid::getNearestRef(const GV_Vector& coord,
                                   double* dist,
                                   GV2::RefMode refMode) {

    return GV_AtomicEntity::getNearestRef(coord, dist, refMode);
}

GV_Vector GV_Solid::getNearestSelectedRef(const GV_Vector& coord,
        double* dist,
        GV2::RefMode refMode) {

    return GV_AtomicEntity::getNearestSelectedRef(coord, dist, refMode);
}




/**
 * @return The orientation (cw, ccw) of the solid.
 */
GV2::ContourOrientation GV_Solid::getOrientation() {
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
        return GV2::ClockWise;
    }
    else {
        return GV2::CounterClockWise;
    }
}
    


/**
 * Changes the orientation of the solid.
 */
void GV_Solid::reverse() {
    GV_Vector tmp;

//    RB_DEBUG->print("GV_Solid::reverse: "
//        "before: %f/%f - %f/%f - %f/%f",
//        data.corner[0].x,
//        data.corner[0].y,
//        data.corner[1].x,
//        data.corner[1].y,
//        data.corner[2].x,
//        data.corner[2].y
//        );

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
        RB_DEBUG->print("GV_Solid::reverse: not a triangle");
        tmp = data.corner[1];
        data.corner[1] = data.corner[3];
        data.corner[3] = tmp;
    }
    
//    RB_DEBUG->print("GV_Solid::reverse: "
//        "after: %f/%f - %f/%f - %f/%f",
//        data.corner[0].x,
//        data.corner[0].y,
//        data.corner[1].x,
//        data.corner[1].y,
//        data.corner[2].x,
//        data.corner[2].y
//        );
}



/**
 * Shapes this Solid into a standard arrow (used in dimensions).
 *
 * @param point The point the arrow points to.
 * @param angle Direction of the arrow.
 * @param arrowSize Size of arrow (length).
 */
void GV_Solid::shapeArrow(const GV_Vector& point,
                          double angle,
                          double arrowSize) {

    double cosv1, sinv1, cosv2, sinv2;
    double arrowSide = arrowSize/cos(0.165);

    cosv1 = cos(angle+0.165)*arrowSide;
    sinv1 = sin(angle+0.165)*arrowSide;
    cosv2 = cos(angle-0.165)*arrowSide;
    sinv2 = sin(angle-0.165)*arrowSide;

    data.corner[0] = point;
    data.corner[1] = GV_Vector(point.x - cosv1, point.y - sinv1);
    data.corner[2] = GV_Vector(point.x - cosv2, point.y - sinv2);
    data.corner[3] = GV_Vector(false);

    calculateBorders();
}



void GV_Solid::calculateBorders(bool /*visibleOnly*/) {
    resetBorders();

    for (int i=0; i<4; ++i) {
        if (data.corner[i].valid) {
            minV = GV_Vector::minimum(minV, data.corner[i]);
            maxV = GV_Vector::maximum(maxV, data.corner[i]);
        }
    }
}



GV_Vector GV_Solid::getNearestEndpoint(const GV_Vector& coord, double* dist) {

    double minDist = GV_MAXDOUBLE;
    double curDist;
    GV_Vector ret;

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
GV_Vector GV_Solid::getNearestPointOnEntity(const GV_Vector& /*coord*/,
        bool /*onEntity*/, double* /*dist*/, GV_Entity** /*entity*/) {

    GV_Vector ret(false);
    return ret;
}



GV_Vector GV_Solid::getNearestCenter(const GV_Vector& /*coord*/,
                                     double* dist) {

    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }

    return GV_Vector(false);
}



GV_Vector GV_Solid::getNearestMiddle(const GV_Vector& /*coord*/,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



GV_Vector GV_Solid::getNearestDist(double /*distance*/,
                                   const GV_Vector& /*coord*/,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



/**
 * @return Distance from one of the boundry lines of this solid to given point.
 */
double GV_Solid::getDistanceToPoint(const GV_Vector& coord,
                                    GV_Entity** /*entity*/,
                                    GV2::ResolveLevel level,
                                    double solidDist,
                                    bool /*visually*/) {
    double minDist = GV_MAXDOUBLE;

    for (int i=0; i<4; ++i) {
        GV_Line l(NULL, GV_LineData(data.corner[i], data.corner[(i+1)%4]));
        double dist = l.getDistanceToPoint(coord, NULL, level, solidDist);
        if (dist<minDist) {
            minDist = dist;
        }
    }

    return minDist;
}



void GV_Solid::move(const GV_Vector& offset) {
    for (int i=0; i<4; ++i) {
        data.corner[i].move(offset);
    }
    calculateBorders();
}



void GV_Solid::rotate(const GV_Vector& center, double angle) {
    for (int i=0; i<4; ++i) {
        data.corner[i].rotate(center, angle);
    }
    calculateBorders();
}



void GV_Solid::scale(const GV_Vector& center, const GV_Vector& factor) {
    for (int i=0; i<4; ++i) {
        data.corner[i].scale(center, factor);
    }
    calculateBorders();
}



void GV_Solid::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    for (int i=0; i<4; ++i) {
        data.corner[i].mirror(axisPoint1, axisPoint2);
    }
    calculateBorders();
}



void GV_Solid::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_AtomicEntity::moveRef(ref, offset);
    for (int i=0; i<4; i++) {
        if (ref.distanceTo(data.corner[i])<1.0e-4) {
           data.corner[i].move(offset);
        }
    }
}



void GV_Solid::stretch(const GV_Vector& firstCorner,
                                 const GV_Vector& secondCorner,
                                 const GV_Vector& offset) {

    for (int i=0; i<4; i++) {
        if (data.corner[i].isInWindow(firstCorner, secondCorner)) {
           data.corner[i].move(offset);
        }
    }
    
    GV_AtomicEntity::stretch(firstCorner, secondCorner, offset);
}



void GV_Solid::isoProject(GV2::IsoProjectionType type, double /*segmentLength*/) {
    for (int i=0; i<4; ++i) {
        data.corner[i].isoProject(type);
    }
    calculateBorders();
    update();
}


void GV_Solid::draw(GV_Painter* painter, GV_GraphicView* view, 
    double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

//    GV_SolidData d = getData();
    if (isTriangle()) {
        painter->fillTriangle(view->toGui(getCorner(0)),
                              view->toGui(getCorner(1)),
                              view->toGui(getCorner(2)));
    }
}




QVariant GV_Solid::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_SOLID_POINT1_X) {
        return data.corner[0].x;
    } else if (name==GV_SOLID_POINT1_Y) {
        return data.corner[0].y;
    } else if (name==GV_SOLID_POINT2_X) {
        return data.corner[1].x;
    } else if (name==GV_SOLID_POINT2_Y) {
        return data.corner[1].y;
    } else if (name==GV_SOLID_POINT3_X) {
        return data.corner[2].x;
    } else if (name==GV_SOLID_POINT3_Y) {
        return data.corner[2].y;
    } else if (name==GV_SOLID_POINT4_X) {
        return data.corner[3].x;
    } else if (name==GV_SOLID_POINT4_Y) {
        return data.corner[3].y;
    } else {
        return GV_AtomicEntity::getProperty(name, def);
    }
}



QStringList GV_Solid::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_AtomicEntity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_SOLID_POINT1_X
            << GV_SOLID_POINT1_Y
            << GV_SOLID_POINT2_X
            << GV_SOLID_POINT2_Y
            << GV_SOLID_POINT3_X
            << GV_SOLID_POINT3_Y
            << GV_SOLID_POINT4_X
            << GV_SOLID_POINT4_Y;
    }
    return ret;
}


void GV_Solid::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_SOLID_POINT1_X) {
        data.corner[0].x = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT1_Y) {
        data.corner[0].y = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT2_X) {
        data.corner[1].x = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT2_Y) {
        data.corner[1].y = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT3_X) {
        data.corner[2].x = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT3_Y) {
        data.corner[2].y = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT4_X) {
        data.corner[3].x = GV_Math::eval(value.toString());
    } else if (name==GV_SOLID_POINT4_Y) {
        data.corner[3].y = GV_Math::eval(value.toString());
    } else {
        GV_AtomicEntity::setProperty(name, value);
    }
    calculateBorders();
    update();
}

