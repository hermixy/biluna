/****************************************************************************
** $Id: gv_constructionline.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "gv_constructionline.h"

#include "rb_debug.h"



/**
 * Constructor.
 */
GV_ConstructionLine::GV_ConstructionLine(GV_EntityContainer* parentContainer,
        const GV_ConstructionLineData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {

    calculateBorders();
}



/**
 * Destructor.
 */
GV_ConstructionLine::~GV_ConstructionLine() {}



GV_Entity* GV_ConstructionLine::clone() {
    GV_ConstructionLine* c = new GV_ConstructionLine(parentContainer, data);
    c->copyPropertiesFrom(this);
    c->initId();
    return c;
}



void GV_ConstructionLine::calculateBorders(bool /*visibleOnly*/) {
    minV = GV_Vector::minimum(data.point1, data.point2);
    maxV = GV_Vector::maximum(data.point1, data.point2);
}



GV_Vector GV_ConstructionLine::getNearestEndpoint(const GV_Vector& coord,
        double* dist) {
    double dist1, dist2;
    GV_Vector* nearerPoint;

    dist1 = data.point1.distanceTo(coord);
    dist2 = data.point2.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = &data.point2;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = &data.point1;
    }

    return *nearerPoint;
}



GV_Vector GV_ConstructionLine::getNearestPointOnEntity(const GV_Vector& coord,
        bool /*onEntity*/, double* /*dist*/, GV_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    GV_Vector ae = data.point2-data.point1;
    GV_Vector ea = data.point1-data.point2;
    GV_Vector ap = coord-data.point1;
//    GV_Vector ep = coord-data.point2;
    
    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        return GV_Vector(false);
    }

    // Orthogonal projection from both sides:
    GV_Vector ba = ae * GV_Vector::dotP(ae, ap)
                   / (ae.magnitude()*ae.magnitude());
//    GV_Vector be = ea * GV_Vector::dotP(ea, ep)
//                   / (ea.magnitude()*ea.magnitude());

    return data.point1+ba;
}



GV_Vector GV_ConstructionLine::getNearestCenter(const GV_Vector& /*coord*/,
        double* dist) {

    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }

    return GV_Vector(false);
}



GV_Vector GV_ConstructionLine::getNearestMiddle(const GV_Vector& /*coord*/,
        double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}



GV_Vector GV_ConstructionLine::getNearestDist(double /*distance*/,
        const GV_Vector& /*coord*/,
        double* dist) {
    if (dist!=NULL) {
        *dist = GV_MAXDOUBLE;
    }
    return GV_Vector(false);
}


double GV_ConstructionLine::getDistanceToPoint(const GV_Vector& coord,
        GV_Entity** entity,
        GV2::ResolveLevel /*level*/, double /*solidDist*/, bool /*visually*/) {

    RB_DEBUG->print("GV_ConstructionLine::getDistanceToPoint");

    if (entity!=NULL) {
        *entity = this;
    }
    double dist = GV_MAXDOUBLE;
    GV_Vector ae = data.point2-data.point1;
    GV_Vector ea = data.point1-data.point2;
    GV_Vector ap = coord-data.point1;
//    GV_Vector ep = coord-data.point2;
    
    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        return dist;
    }

    // Orthogonal projection from both sides:
    GV_Vector ba = ae * GV_Vector::dotP(ae, ap) /
                   GV_Math::pow(ae.magnitude(), 2);
//    GV_Vector be = ea * GV_Vector::dotP(ea, ep) /
//                   GV_Math::pow(ea.magnitude(), 2);

//    RB_DEBUG->print("ba: %f", ba.magnitude());
//    RB_DEBUG->print("ae: %f", ae.magnitude());

    GV_Vector cp = GV_Vector::crossP(ap, ae);
    dist = cp.magnitude() / ae.magnitude();

    return dist;
}



void GV_ConstructionLine::move(const GV_Vector& offset) {
    data.point1.move(offset);
    data.point2.move(offset);
    //calculateBorders();
}



void GV_ConstructionLine::rotate(const GV_Vector& center, double angle) {
    data.point1.rotate(center, angle);
    data.point2.rotate(center, angle);
    //calculateBorders();
}



void GV_ConstructionLine::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.point1.scale(center, factor);
    data.point2.scale(center, factor);
    //calculateBorders();
}



void GV_ConstructionLine::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.point1.mirror(axisPoint1, axisPoint2);
    data.point2.mirror(axisPoint1, axisPoint2);
}

