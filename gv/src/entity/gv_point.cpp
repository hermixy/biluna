/****************************************************************************
** $Id: gv_point.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "gv_point.h"

#include "gv_graphicview.h"
#include "gv_painter.h"


const char* GV_Point::propertyX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* GV_Point::propertyY = QT_TRANSLATE_NOOP("QObject", "Position|Y");


/**
 * Default constructor.
 */
GV_Point::GV_Point(GV_EntityContainer* parentContainer,
                   const GV_PointData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {
    calculateBorders ();
}



void GV_Point::calculateBorders(bool /*visibleOnly*/) {
    minV = maxV = data.pos;
}



GV_VectorList GV_Point::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret(data.pos);
    return ret;
}



GV_Vector GV_Point::getNearestEndpoint(const GV_Vector& coord, double* dist) {

    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



GV_Vector GV_Point::getNearestPointOnEntity(const GV_Vector& coord,
        bool /*onEntity*/, double* dist, GV_Entity** entity) {
    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }
    if (entity!=NULL) {
        *entity = this;
    }
    return data.pos;
}



GV_Vector GV_Point::getNearestCenter(const GV_Vector& coord, double* dist) {

    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



GV_Vector GV_Point::getNearestMiddle(const GV_Vector& coord,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = data.pos.distanceTo(coord);
    }

    return data.pos;
}



GV_Vector GV_Point::getNearestDist(double /*distance*/,
                                   const GV_Vector& /*coord*/,
                                   double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



double GV_Point::getDistanceToPoint(const GV_Vector& coord,
                                    GV_Entity** entity,
                                    GV2::ResolveLevel /*level*/,
                                    double /*solidDist*/,
                                    bool /*visually*/) {
    if (entity!=NULL) {
        *entity = this;
    }
    return data.pos.distanceTo(coord);
}



void GV_Point::moveStartpoint(const GV_Vector& pos) {
    data.pos = pos;
    calculateBorders();
    update();
}



void GV_Point::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    if (ref.distanceTo(data.pos)<1.0e-4) {
        moveStartpoint(data.pos+offset);
    }
}



void GV_Point::move(const GV_Vector& offset) {
    data.pos.move(offset);
    calculateBorders();
    update();
}



void GV_Point::rotate(const GV_Vector& center, double angle) {
    data.pos.rotate(center, angle);
    calculateBorders();
    update();
}



void GV_Point::scale(const GV_Vector& center, const GV_Vector& factor) {
//    RB_DEBUG->print("GV_Point::scale: %f", factor.x);

    data.pos.scale(center, factor);
    calculateBorders();
    update();
}



void GV_Point::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.pos.mirror(axisPoint1, axisPoint2);
    calculateBorders();
    update();
}


void GV_Point::draw(GV_Painter* painter, GV_GraphicView* view, double /*patternOffset*/) {
    if (painter==NULL || view==NULL) {
        return;
    }

    painter->drawPoint(view->toGui(getPos()));
}



QVariant GV_Point::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_POINT_X) {
        return data.pos.x;
    } else if (name==GV_POINT_Y) {
        return data.pos.y;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Point::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_POINT_X;
        ret << GV_POINT_Y;
    }
    return ret;
}


void GV_Point::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_POINT_X) {
        data.pos.x = GV_Math::eval(value.toString());
    } else if (name==GV_POINT_Y) {
        data.pos.y = GV_Math::eval(value.toString());
    } else {
        GV_Entity::setProperty(name, value);
    }
    calculateBorders();
    update();
}
    

