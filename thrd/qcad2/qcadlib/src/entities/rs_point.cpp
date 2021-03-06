/****************************************************************************
** $Id: rs_point.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "rs_point.h"

#include "rs_graphicview.h"
#include "rs_painter.h"


const char* RS_Point::propertyX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* RS_Point::propertyY = QT_TRANSLATE_NOOP("QObject", "Position|Y");


/**
 * Default constructor.
 */
RS_Point::RS_Point(RS_EntityContainer* parentContainer,
                   const RS_PointData& d)
        : RS_Entity(parentContainer), RS_AtomicEntity(parentContainer), data(d) {
    calculateBorders ();
}



void RS_Point::calculateBorders(bool /*visibleOnly*/) {
    minV = maxV = data.pos;
}



RS_VectorList RS_Point::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret(data.pos);
    return ret;
}



RS_Vector RS_Point::getNearestEndpoint(const RS_Vector& coord, double* dist) {

    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



RS_Vector RS_Point::getNearestPointOnEntity(const RS_Vector& coord,
        bool /*onEntity*/, double* dist, RS_Entity** entity) {
    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }
    if (entity!=NULL) {
        *entity = this;
    }
    return data.pos;
}



RS_Vector RS_Point::getNearestCenter(const RS_Vector& coord, double* dist) {

    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



RS_Vector RS_Point::getNearestMiddle(const RS_Vector& coord,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



RS_Vector RS_Point::getNearestDist(double /*distance*/,
                                   const RS_Vector& /*coord*/,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



double RS_Point::getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity,
                                    RS2::ResolveLevel /*level*/,
                                    double /*solidDist*/,
                                    bool /*visually*/) {
    if (entity!=NULL) {
        *entity = this;
    }
    return data.pos.distanceTo(coord);
}



void RS_Point::moveStartpoint(const RS_Vector& pos) {
    data.pos = pos;
    calculateBorders();
    update();
}



void RS_Point::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    if (ref.distanceTo(data.pos)<1.0e-4) {
        moveStartpoint(data.pos+offset);
    }
}



void RS_Point::move(const RS_Vector& offset) {
    data.pos.move(offset);
    calculateBorders();
    update();
}



void RS_Point::rotate(const RS_Vector& center, double angle) {
    data.pos.rotate(center, angle);
    calculateBorders();
    update();
}



void RS_Point::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_DEBUG->print("RS_Point::scale: %f", factor.x);

    data.pos.scale(center, factor);
    calculateBorders();
    update();
}



void RS_Point::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.pos.mirror(axisPoint1, axisPoint2);
    calculateBorders();
    update();
}


void RS_Point::draw(RS_Painter* painter, RS_GraphicView* view, double /*patternOffset*/) {
    if (painter==NULL || view==NULL) {
        return;
    }

    painter->drawPoint(view->toGui(getPos()));
}



RS_Variant RS_Point::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_POINT_X) {
        return data.pos.x;
    } else if (name==RS_POINT_Y) {
        return data.pos.y;
    } else {
        return RS_Entity::getProperty(name, def);
    }
}



RS_StringList RS_Point::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_POINT_X;
        ret << RS_POINT_Y;
    }
    return ret;
}


void RS_Point::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_POINT_X) {
        data.pos.x = RS_Math::eval(value.toString());
    } else if (name==RS_POINT_Y) {
        data.pos.y = RS_Math::eval(value.toString());
    } else {
        RS_Entity::setProperty(name, value);
    }
    calculateBorders();
    update();
}
    

