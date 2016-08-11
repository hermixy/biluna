/****************************************************************************
** $Id: gv_line.cpp 9188 2008-02-24 17:52:24Z andrew $
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


#include "gv_line.h"

#include "rb_debug.h"
#include "gv_graphicview.h"
#include "gv_painter.h"
#include "gv_graphic.h"

    
const char* GV_Line::propertyStartpointX = QT_TRANSLATE_NOOP("QObject", "Startpoint|X");
const char* GV_Line::propertyStartpointY = QT_TRANSLATE_NOOP("QObject", "Startpoint|Y");
const char* GV_Line::propertyEndpointX = QT_TRANSLATE_NOOP("QObject", "Endpoint|X");
const char* GV_Line::propertyEndpointY = QT_TRANSLATE_NOOP("QObject", "Endpoint|Y");

/**
 * Constructor.
 */
GV_Line::GV_Line(GV_EntityContainer* parentContainer,
                 const GV_LineData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {
    
    calculateBorders();
}



/**
 * Destructor.
 */
GV_Line::~GV_Line() {
}




GV_Entity* GV_Line::clone() {
    assert(rtti()==GV2::EntityLine);
    GV_Line* l = new GV_Line(parentContainer, data);
    l->copyPropertiesFrom(this);
    l->initId();
    return l;
}



void GV_Line::calculateBorders(bool /*visibleOnly*/) {
    minV = GV_Vector::minimum(data.startpoint, data.endpoint);
    maxV = GV_Vector::maximum(data.startpoint, data.endpoint);
}



GV_VectorList GV_Line::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret(data.startpoint, data.endpoint);
    return ret;
}


GV_Vector GV_Line::getNearestEndpoint(const GV_Vector& coord,
                                      double* dist) {
    double dist1, dist2;
    GV_Vector* nearerPoint;

    dist1 = data.startpoint.distanceTo(coord);
    dist2 = data.endpoint.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = &data.endpoint;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = &data.startpoint;
    }

    return *nearerPoint;
}



GV_Vector GV_Line::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity, double* dist, GV_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    GV_Vector ae = data.endpoint-data.startpoint;
    GV_Vector ea = data.startpoint-data.endpoint;
    GV_Vector ap = coord-data.startpoint;
    GV_Vector ep = coord-data.endpoint;

    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        if (dist!=NULL) {
            *dist = GV_MAXDOUBLE;
        }
        return GV_Vector(false);
    }

    // Orthogonal projection from both sides:
    GV_Vector ba = ae * GV_Vector::dotP(ae, ap)
                   / (ae.magnitude()*ae.magnitude());
    GV_Vector be = ea * GV_Vector::dotP(ea, ep)
                   / (ea.magnitude()*ea.magnitude());

    // Check if the projection is within this line:
    if (onEntity==true &&
            (ba.magnitude()>ae.magnitude() || be.magnitude()>ea.magnitude())) {
        //return getNearestEndpoint(coord, dist);
        if (dist!=NULL) {
            *dist = GV_MAXDOUBLE;
        }
        return GV_Vector(false);
    } else {
        if (dist!=NULL) {
            *dist = coord.distanceTo(data.startpoint+ba);
        }
        return data.startpoint+ba;
    }
}



GV_Vector GV_Line::getNearestCenter(const GV_Vector& coord,
                                    double* dist) {

    GV_Vector p = (data.startpoint + data.endpoint)/2.0;

    if (dist!=NULL) {
        *dist = p.distanceTo(coord);
    }

    return p;
}



GV_Vector GV_Line::getNearestMiddle(const GV_Vector& coord,
                                    double* dist) {
    return getNearestCenter(coord, dist);
}



GV_Vector GV_Line::getNearestDist(double distance,
                                  const GV_Vector& coord,
                                  double* dist) {

    double a1 = getAngle1();

    GV_Vector dv;
    dv.setPolar(distance, a1);

    GV_Vector p1 = data.startpoint + dv;
    GV_Vector p2 = data.endpoint - dv;

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



GV_Vector GV_Line::getNearestDist(double distance,
                                  bool startp) {

    double a1 = getAngle1();

    GV_Vector dv;
    dv.setPolar(distance, a1);
    GV_Vector ret;

    if (startp) {
        ret = data.startpoint + dv;
    }
    else {
        ret = data.endpoint - dv;
    }

    return ret;

}


/*GV_Vector GV_Line::getNearestRef(const GV_Vector& coord,
                                 double* dist) {
    double d1, d2, d;
    GV_Vector p;
    GV_Vector p1 = getNearestEndpoint(coord, &d1);
    GV_Vector p2 = getNearestMiddle(coord, &d2);

    if (d1<d2) {
        d = d1;
        p = p1;
    } else {
        d = d2;
        p = p2;
    }

    if (dist!=NULL) {
        *dist = d;
    }

    return p;
}*/


double GV_Line::getDistanceToPoint(const GV_Vector& coord,
                                   GV_Entity** entity,
                                   GV2::ResolveLevel /*level*/,
                                   double /*solidDist*/,
                                   bool /*visually*/) {

    if (entity!=NULL) {
        *entity = this;
    }
    
    /*
    if (!coord.valid || !getStartpoint().valid || !getEndpoint().valid) {
        return GV_MAXDOUBLE;
    }
    */

    // check endpoints first:
    double dist = coord.distanceTo(getStartpoint());
    if (dist<1.0e-4) {
//        RB_DEBUG->print("GV_Line::getDistanceToPoint: to start point: %f", dist);
        return dist;
    }
    dist = coord.distanceTo(getEndpoint());
    if (dist<1.0e-4) {
//        RB_DEBUG->print("GV_Line::getDistanceToPoint: to end point: %f", dist);
        return dist;
    }

    dist = GV_MAXDOUBLE;
    GV_Vector ae = data.endpoint-data.startpoint;
    GV_Vector ea = data.startpoint-data.endpoint;
    GV_Vector ap = coord-data.startpoint;
    GV_Vector ep = coord-data.endpoint;

    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
//        RB_DEBUG->print("GV_Line::getDistanceToPoint: very small line: %f", dist);
        return dist;
    }

    // Orthogonal projection from both sides:
    GV_Vector ba = ae * GV_Vector::dotP(ae, ap) /
                   GV_Math::pow(ae.magnitude(), 2);
    GV_Vector be = ea * GV_Vector::dotP(ea, ep) /
                   GV_Math::pow(ea.magnitude(), 2);

    // Check if the projection is outside this line:
    if (ba.magnitude()>ae.magnitude() || be.magnitude()>ea.magnitude()) {
        // return distance to endpoint

        // for walls:
        getNearestEndpoint(coord, &dist);
//        RB_DEBUG->print("GV_Line::getDistanceToPoint (outside / endpoint): %f", dist);
        return dist;
        //#return GV_MAXDOUBLE;
    }

    GV_Vector cp = GV_Vector::crossP(ap, ae);
    dist = cp.magnitude() / ae.magnitude();
    
    return dist;
}



GV2::Side GV_Line::getSideOfPoint(const GV_Vector& coord) {
    return GV_Math::getSideOfPoint(data.startpoint, data.endpoint, coord);
}



void GV_Line::moveStartpoint(const GV_Vector& pos) {
    data.startpoint = pos;
    calculateBorders();
    update();
}



void GV_Line::moveEndpoint(const GV_Vector& pos) {
    data.endpoint = pos;
    calculateBorders();
    update();
}



GV2::Ending GV_Line::getTrimPoint(const GV_Vector& coord,
                                  const GV_Vector& trimPoint) {

    double angEl = getAngle1();
    double angM = trimPoint.angleTo(coord);
    double angDif = angEl-angM;

    if (angDif<0.0) {
        angDif*=-1.0;
    }
    if (angDif>M_PI) {
        angDif=2*M_PI-angDif;
    }

    if (angDif<M_PI/2.0) {
        return GV2::EndingStart;
    } else {
        return GV2::EndingEnd;
    }
}


void GV_Line::reverse() {
    GV_Vector v = data.startpoint;
    data.startpoint = data.endpoint;
    data.endpoint = v;
    update();
}



bool GV_Line::hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2) {
    if (data.startpoint.isInWindow(v1, v2) ||
            data.endpoint.isInWindow(v1, v2)) {
        return true;
    } else {
        return false;
    }
}


void GV_Line::move(const GV_Vector& offset) {
    data.startpoint.move(offset);
    data.endpoint.move(offset);
    calculateBorders();
    update();
}



void GV_Line::rotate(const GV_Vector& center, double angle) {
    data.startpoint.rotate(center, angle);
    data.endpoint.rotate(center, angle);
    calculateBorders();
    update();
}



void GV_Line::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.startpoint.scale(center, factor);
    data.endpoint.scale(center, factor);
    calculateBorders();
    update();
}



void GV_Line::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.startpoint.mirror(axisPoint1, axisPoint2);
    data.endpoint.mirror(axisPoint1, axisPoint2);
    calculateBorders();
    update();
}


/**
 * Stretches the given range of the entity by the given offset.
 */
void GV_Line::stretch(const GV_Vector& firstCorner,
                      const GV_Vector& secondCorner,
                      const GV_Vector& offset) {

    bool moved = false;

    if (getStartpoint().isInWindow(firstCorner,
                                   secondCorner)) {
        data.startpoint = data.startpoint+offset;
        moved = true;
    }
    if (getEndpoint().isInWindow(firstCorner,
                                 secondCorner)) {
        data.endpoint = data.endpoint+offset;
        moved = true;
    }
    
    if (moved) {
        calculateBorders();
        update();
    }
}



void GV_Line::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    bool moved = false;

    if (ref.distanceTo(data.startpoint)<1.0e-4) {
        data.startpoint = data.startpoint+offset;
        moved = true;
    }
    if (ref.distanceTo(data.endpoint)<1.0e-4) {
        data.endpoint = data.endpoint+offset;
        moved = true;
    }

    if (moved) {
        calculateBorders();
        update();
    }
}

    

void GV_Line::isoProject(GV2::IsoProjectionType type, double /*segmentLength*/) {
    data.startpoint.isoProject(type);
    data.endpoint.isoProject(type);
    calculateBorders();
    update();
}



void GV_Line::draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset) {
    if (painter==NULL || view==NULL || !getStartpoint().valid || !getEndpoint().valid) {
        RB_DEBUG->print("GV_Line::draw: painter or view is NULL");
        return;
    }
    
    // for debugging min / max
    //painter->drawRect(view->toGui(getMin()), view->toGui(getMax()));
        
    if (getPen().getLineType()==GV2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==GV2::ModePreview) {

        painter->drawLine(view->toGui(getStartpoint()),
                          view->toGui(getEndpoint()));
    } else {
        double styleFactor = getStyleFactor(view);
        if (styleFactor<0.0) {
            painter->drawLine(view->toGui(getStartpoint()),
                          view->toGui(getEndpoint()));
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
            RB_DEBUG->print(RB_Debug::D_WARNING,
                "GV_Line::draw: Invalid line pattern");
            return;
        }
            
        // Pen to draw pattern is always solid:
        GV_Pen pen = painter->getPen();

        pen.setLineType(GV2::SolidLine);
        painter->setPen(pen);

        // index counter
        int i;

        // line data:
        double length = getLength();
        double angle = getAngle1();
        
        // pattern segment length:
        double patternSegmentLength = 0.0;

        // create pattern:
        GV_Vector* dp = new GV_Vector[pat->num];

        for (i=0; i<pat->num; ++i) {
            dp[i] = GV_Vector(cos(angle) * fabs(pat->pattern[i] * styleFactor),
                              sin(angle) * fabs(pat->pattern[i] * styleFactor));

            patternSegmentLength += fabs(pat->pattern[i] * styleFactor);
        }

        // handle pattern offset:
        int m;
        if (patternOffset<0.0) {
            m = (int)ceil(patternOffset / patternSegmentLength);
        }
        else {
            m = (int)floor(patternOffset / patternSegmentLength);
        }
        
        patternOffset -= (m*patternSegmentLength);
        GV_Vector patternOffsetVec;
        patternOffsetVec.setPolar(patternOffset, angle);
        
        double tot=patternOffset;
        i=0;
        bool done = false;
        bool cutStartpoint, cutEndpoint, drop;
        GV_Vector curP=getStartpoint()+patternOffsetVec;
        
        do {
            cutStartpoint = false;
            cutEndpoint = false;
            drop = false;
            
            // line segment (otherwise space segment)
            if (pat->pattern[i]>0.0) {
                // drop the whole pattern segment line:
                if (tot+pat->pattern[i]*styleFactor < 0.0) {
                    drop = true;
                }
                else {
                    // trim startpoint of pattern segment line to line startpoint
                    if (tot < 0.0) {
                        cutStartpoint = true;
                    }
                    // trim endpoint of pattern segment line to line endpoint
                    if (tot+pat->pattern[i]*styleFactor > length) {
                        cutEndpoint = true;
                    }
                }

                if (drop) {
                    // do nothing
                }
                else {
                    GV_Vector p1 = curP;
                    GV_Vector p2 = curP + dp[i];
                    
                    if (cutStartpoint) {
                        p1 = getStartpoint();
                    }
                    if (cutEndpoint) {
                        p2 = getEndpoint();
                    }

                    GV_Vector g1 = view->toGui(p1);
                    GV_Vector g2 = view->toGui(p2);
                    
                    if (g1.isInWindow(GV_Vector(0,0), 
                         GV_Vector(view->getWidth(), view->getHeight())) ||
                        g2.isInWindow(GV_Vector(0,0), 
                         GV_Vector(view->getWidth(), view->getHeight()))) {
                         
                        painter->drawLine(view->toGui(p1),
                                      view->toGui(p2));
                    }
                }
            }
            curP+=dp[i];
            tot+=fabs(pat->pattern[i]*styleFactor);
            done=tot>length;

            i++;
            if (i>=pat->num) {
                i=0;
            }
        } while(!done);

        delete[] dp;

    }
}



QVariant GV_Line::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_LINE_STARTPOINT_X) {
        return data.startpoint.x;
    } else if (name==GV_LINE_STARTPOINT_Y) {
        return data.startpoint.y;
    } else if (name==GV_LINE_ENDPOINT_X) {
        return data.endpoint.x;
    } else if (name==GV_LINE_ENDPOINT_Y) {
        return data.endpoint.y;
    } else {
        return GV_Entity::getProperty(name, def);
    }
}



QStringList GV_Line::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_LINE_STARTPOINT_X;
        ret << GV_LINE_STARTPOINT_Y;
        ret << GV_LINE_ENDPOINT_X;
        ret << GV_LINE_ENDPOINT_Y;
    }
    return ret;
}


void GV_Line::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_LINE_STARTPOINT_X) {
        data.startpoint.x = GV_Math::eval(value.toString());
    } else if (name==GV_LINE_STARTPOINT_Y) {
        data.startpoint.y = GV_Math::eval(value.toString());
    } else if (name==GV_LINE_ENDPOINT_X) {
        data.endpoint.x = GV_Math::eval(value.toString());
    } else if (name==GV_LINE_ENDPOINT_Y) {
        data.endpoint.y = GV_Math::eval(value.toString());
    } else {
        GV_Entity::setProperty(name, value);
    }
    calculateBorders();
    update();
}
    

