/****************************************************************************
** $Id: rs_line.cpp 9188 2008-02-24 17:52:24Z andrew $
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


#include "rs_line.h"

#include "rs_debug.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_graphic.h"

    
const char* RS_Line::propertyStartpointX = QT_TRANSLATE_NOOP("QObject", "Startpoint|X");
const char* RS_Line::propertyStartpointY = QT_TRANSLATE_NOOP("QObject", "Startpoint|Y");
const char* RS_Line::propertyEndpointX = QT_TRANSLATE_NOOP("QObject", "Endpoint|X");
const char* RS_Line::propertyEndpointY = QT_TRANSLATE_NOOP("QObject", "Endpoint|Y");

/**
 * Constructor.
 */
RS_Line::RS_Line(RS_EntityContainer* parentContainer,
                 const RS_LineData& d)
        : RS_Entity(parentContainer), RS_AtomicEntity(parentContainer), data(d) {
    
    calculateBorders();
}



/**
 * Destructor.
 */
RS_Line::~RS_Line() {
}




RS_Entity* RS_Line::clone() {
    assert(rtti()==RS2::EntityLine);
    RS_Line* l = new RS_Line(parentContainer, data);
    l->copyPropertiesFrom(this);
    l->initId();
    return l;
}



void RS_Line::calculateBorders(bool /*visibleOnly*/) {
    minV = RS_Vector::minimum(data.startpoint, data.endpoint);
    maxV = RS_Vector::maximum(data.startpoint, data.endpoint);
}



RS_VectorList RS_Line::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret(data.startpoint, data.endpoint);
    return ret;
}


RS_Vector RS_Line::getNearestEndpoint(const RS_Vector& coord,
                                      double* dist) {
    double dist1, dist2;
    RS_Vector* nearerPoint;

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



RS_Vector RS_Line::getNearestPointOnEntity(const RS_Vector& coord,
        bool onEntity, double* dist, RS_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    RS_Vector ae = data.endpoint-data.startpoint;
    RS_Vector ea = data.startpoint-data.endpoint;
    RS_Vector ap = coord-data.startpoint;
    RS_Vector ep = coord-data.endpoint;

    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        if (dist!=NULL) {
            *dist = RS_MAXDOUBLE;
        }
        return RS_Vector(false);
    }

    // Orthogonal projection from both sides:
    RS_Vector ba = ae * RS_Vector::dotP(ae, ap)
                   / (ae.magnitude()*ae.magnitude());
    RS_Vector be = ea * RS_Vector::dotP(ea, ep)
                   / (ea.magnitude()*ea.magnitude());

    // Check if the projection is within this line:
    if (onEntity==true &&
            (ba.magnitude()>ae.magnitude() || be.magnitude()>ea.magnitude())) {
        //return getNearestEndpoint(coord, dist);
        if (dist!=NULL) {
            *dist = RS_MAXDOUBLE;
        }
        return RS_Vector(false);
    } else {
        if (dist!=NULL) {
            *dist = coord.distanceTo(data.startpoint+ba);
        }
        return data.startpoint+ba;
    }
}



RS_Vector RS_Line::getNearestCenter(const RS_Vector& coord,
                                    double* dist) {

    RS_Vector p = (data.startpoint + data.endpoint)/2.0;

    if (dist!=NULL) {
        *dist = p.distanceTo(coord);
    }

    return p;
}



RS_Vector RS_Line::getNearestMiddle(const RS_Vector& coord,
                                    double* dist) {
    return getNearestCenter(coord, dist);
}



RS_Vector RS_Line::getNearestDist(double distance,
                                  const RS_Vector& coord,
                                  double* dist) {

    double a1 = getAngle1();

    RS_Vector dv;
    dv.setPolar(distance, a1);

    RS_Vector p1 = data.startpoint + dv;
    RS_Vector p2 = data.endpoint - dv;

    double dist1, dist2;
    RS_Vector* nearerPoint;

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



RS_Vector RS_Line::getNearestDist(double distance,
                                  bool startp) {

    double a1 = getAngle1();

    RS_Vector dv;
    dv.setPolar(distance, a1);
    RS_Vector ret;

    if (startp) {
        ret = data.startpoint + dv;
    }
    else {
        ret = data.endpoint - dv;
    }

    return ret;

}


/*RS_Vector RS_Line::getNearestRef(const RS_Vector& coord,
                                 double* dist) {
    double d1, d2, d;
    RS_Vector p;
    RS_Vector p1 = getNearestEndpoint(coord, &d1);
    RS_Vector p2 = getNearestMiddle(coord, &d2);

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


double RS_Line::getDistanceToPoint(const RS_Vector& coord,
                                   RS_Entity** entity,
                                   RS2::ResolveLevel /*level*/,
                                   double /*solidDist*/,
                                   bool /*visually*/) {

    if (entity!=NULL) {
        *entity = this;
    }
    
    /*
    if (!coord.valid || !getStartpoint().valid || !getEndpoint().valid) {
        return RS_MAXDOUBLE;
    }
    */

    // check endpoints first:
    double dist = coord.distanceTo(getStartpoint());
    if (dist<1.0e-4) {
        RS_DEBUG->print("RS_Line::getDistanceToPoint: to start point: %f", dist);
        return dist;
    }
    dist = coord.distanceTo(getEndpoint());
    if (dist<1.0e-4) {
        RS_DEBUG->print("RS_Line::getDistanceToPoint: to end point: %f", dist);
        return dist;
    }

    dist = RS_MAXDOUBLE;
    RS_Vector ae = data.endpoint-data.startpoint;
    RS_Vector ea = data.startpoint-data.endpoint;
    RS_Vector ap = coord-data.startpoint;
    RS_Vector ep = coord-data.endpoint;

    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        RS_DEBUG->print("RS_Line::getDistanceToPoint: very small line: %f", dist);
        return dist;
    }

    // Orthogonal projection from both sides:
    RS_Vector ba = ae * RS_Vector::dotP(ae, ap) /
                   RS_Math::pow(ae.magnitude(), 2);
    RS_Vector be = ea * RS_Vector::dotP(ea, ep) /
                   RS_Math::pow(ea.magnitude(), 2);

    // Check if the projection is outside this line:
    if (ba.magnitude()>ae.magnitude() || be.magnitude()>ea.magnitude()) {
        // return distance to endpoint

        // for walls:
        getNearestEndpoint(coord, &dist);
        RS_DEBUG->print("RS_Line::getDistanceToPoint (outside / endpoint): %f", dist);
        return dist;
        //#return RS_MAXDOUBLE;
    }

    RS_Vector cp = RS_Vector::crossP(ap, ae);
    dist = cp.magnitude() / ae.magnitude();
    
    return dist;
}



RS2::Side RS_Line::getSideOfPoint(const RS_Vector& coord) {
    return RS_Math::getSideOfPoint(data.startpoint, data.endpoint, coord);
}



void RS_Line::moveStartpoint(const RS_Vector& pos) {
    data.startpoint = pos;
    calculateBorders();
    update();
}



void RS_Line::moveEndpoint(const RS_Vector& pos) {
    data.endpoint = pos;
    calculateBorders();
    update();
}



RS2::Ending RS_Line::getTrimPoint(const RS_Vector& coord,
                                  const RS_Vector& trimPoint) {

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
        return RS2::EndingStart;
    } else {
        return RS2::EndingEnd;
    }
}


void RS_Line::reverse() {
    RS_Vector v = data.startpoint;
    data.startpoint = data.endpoint;
    data.endpoint = v;
    update();
}



bool RS_Line::hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2) {
    if (data.startpoint.isInWindow(v1, v2) ||
            data.endpoint.isInWindow(v1, v2)) {
        return true;
    } else {
        return false;
    }
}


void RS_Line::move(const RS_Vector& offset) {
    data.startpoint.move(offset);
    data.endpoint.move(offset);
    calculateBorders();
    update();
}



void RS_Line::rotate(const RS_Vector& center, double angle) {
    data.startpoint.rotate(center, angle);
    data.endpoint.rotate(center, angle);
    calculateBorders();
    update();
}



void RS_Line::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.startpoint.scale(center, factor);
    data.endpoint.scale(center, factor);
    calculateBorders();
    update();
}



void RS_Line::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.startpoint.mirror(axisPoint1, axisPoint2);
    data.endpoint.mirror(axisPoint1, axisPoint2);
    calculateBorders();
    update();
}


/**
 * Stretches the given range of the entity by the given offset.
 */
void RS_Line::stretch(const RS_Vector& firstCorner,
                      const RS_Vector& secondCorner,
                      const RS_Vector& offset) {

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



void RS_Line::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
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

    

void RS_Line::isoProject(RS2::IsoProjectionType type, double /*segmentLength*/) {
    data.startpoint.isoProject(type);
    data.endpoint.isoProject(type);
    calculateBorders();
    update();
}



void RS_Line::draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset) {
    if (painter==NULL || view==NULL || !getStartpoint().valid || !getEndpoint().valid) {
        RS_DEBUG->print("RS_Line::draw: painter or view is NULL");
        return;
    }
    
    // for debugging min / max
    //painter->drawRect(view->toGui(getMin()), view->toGui(getMax()));
        
    if (getPen().getLineType()==RS2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==RS2::ModePreview) {

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
        RS_LineTypePattern* pat;
        if (isSelected()) {
            pat = &patternSelected;
        } else {
            pat = view->getPattern(getPen().getLineType());
        }
        if (pat==NULL) {
            RS_DEBUG->print(RS_Debug::D_WARNING,
                "RS_Line::draw: Invalid line pattern");
            return;
        }
            
        // Pen to draw pattern is always solid:
        RS_Pen pen = painter->getPen();

        pen.setLineType(RS2::SolidLine);
        painter->setPen(pen);

        // index counter
        int i;

        // line data:
        double length = getLength();
        double angle = getAngle1();
        
        // pattern segment length:
        double patternSegmentLength = 0.0;

        // create pattern:
        RS_Vector* dp = new RS_Vector[pat->num];

        for (i=0; i<pat->num; ++i) {
            dp[i] = RS_Vector(cos(angle) * fabs(pat->pattern[i] * styleFactor),
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
        RS_Vector patternOffsetVec;
        patternOffsetVec.setPolar(patternOffset, angle);
        
        double tot=patternOffset;
        i=0;
        bool done = false;
        bool cutStartpoint, cutEndpoint, drop;
        RS_Vector curP=getStartpoint()+patternOffsetVec;
        
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
                    RS_Vector p1 = curP;
                    RS_Vector p2 = curP + dp[i];
                    
                    if (cutStartpoint) {
                        p1 = getStartpoint();
                    }
                    if (cutEndpoint) {
                        p2 = getEndpoint();
                    }

                    RS_Vector g1 = view->toGui(p1);
                    RS_Vector g2 = view->toGui(p2);
                    
                    if (g1.isInWindow(RS_Vector(0,0), 
                         RS_Vector(view->getWidth(), view->getHeight())) ||
                        g2.isInWindow(RS_Vector(0,0), 
                         RS_Vector(view->getWidth(), view->getHeight()))) {
                         
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



RS_Variant RS_Line::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_LINE_STARTPOINT_X) {
        return data.startpoint.x;
    } else if (name==RS_LINE_STARTPOINT_Y) {
        return data.startpoint.y;
    } else if (name==RS_LINE_ENDPOINT_X) {
        return data.endpoint.x;
    } else if (name==RS_LINE_ENDPOINT_Y) {
        return data.endpoint.y;
    } else {
        return RS_Entity::getProperty(name, def);
    }
}



RS_StringList RS_Line::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_Entity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_LINE_STARTPOINT_X;
        ret << RS_LINE_STARTPOINT_Y;
        ret << RS_LINE_ENDPOINT_X;
        ret << RS_LINE_ENDPOINT_Y;
    }
    return ret;
}


void RS_Line::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_LINE_STARTPOINT_X) {
        data.startpoint.x = RS_Math::eval(value.toString());
    } else if (name==RS_LINE_STARTPOINT_Y) {
        data.startpoint.y = RS_Math::eval(value.toString());
    } else if (name==RS_LINE_ENDPOINT_X) {
        data.endpoint.x = RS_Math::eval(value.toString());
    } else if (name==RS_LINE_ENDPOINT_Y) {
        data.endpoint.y = RS_Math::eval(value.toString());
    } else {
        RS_Entity::setProperty(name, value);
    }
    calculateBorders();
    update();
}
    

