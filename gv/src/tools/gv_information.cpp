/****************************************************************************
** $Id: gv_information.cpp 9428 2008-03-19 08:30:36Z andrew $
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

#include "gv_information.h"

#include "gv_constructionline.h"
#include <stdlib.h>

/**
 * Default constructor.
 *
 * @param container The container to which we will add
 *        entities. Usually that's an GV_Graphic entity but
 *        it can also be a polyline, text, ...
 */
GV_Information::GV_Information(GV_EntityContainer& container) {
    this->container = &container;
}



/**
 * @return true: if the entity is a dimensioning enity.
 *         false: otherwise
 */
/*
bool GV_Information::isDimension(GV2::EntityType type) {
    if (type==GV2::EntityDimAligned ||
            type==GV2::EntityDimLinear ||
            type==GV2::EntityDimRadial ||
            type==GV2::EntityDimDiametric ||
            type==GV2::EntityDimAngular) {
        return true;
    } else {
        return false;
    }
}
*/



/**
 * @retval true the entity can be trimmed.
 * i.e. it is in a graphic or in a polyline.
 */
bool GV_Information::isTrimmable(GV_Entity* e) {
    if (e!=NULL) {
        if (e->getParentContainer()!=NULL) {
            if (e->getParentContainer()->rtti()==GV2::EntityPolyline) {
                return true;
            }
            else if (e->getParentContainer()->rtti()==GV2::EntityContainer ||
                      e->getParentContainer()->rtti()==GV2::EntityGraphic ||
                      e->getParentContainer()->rtti()==GV2::EntityBlock) {

                // normal entity:
                return true;
            }
        }
    }

    return false;
}


/**
 * @retval true the two entities can be trimmed to each other;
 * i.e. they are in a graphic or in the same polyline.
 */
bool GV_Information::isTrimmable(GV_Entity* e1, GV_Entity* e2) {
    if (e1!=NULL && e2!=NULL) {
        if (e1->getParentContainer()!=NULL && e2->getParentContainer()!=NULL) {
            if (e1->getParentContainer()->rtti()==GV2::EntityPolyline &&
               e2->getParentContainer()->rtti()==GV2::EntityPolyline &&
               e1->getParentContainer()==e2->getParentContainer()) {

                // in the same polyline
                GV_EntityContainer* pl = e1->getParentContainer();
                int idx1 = pl->findEntity(e1);
                int idx2 = pl->findEntity(e2);
//                RB_DEBUG->print("GV_Information::isTrimmable: "
//                    "idx1: %d, idx2: %d", idx1, idx2);
                if (abs(idx1-idx2)==1 || abs(idx1-idx2)==pl->count()-1) {
                    // directly following entities
                    RB_DEBUG->print("GV_Information::isTrimmable: true");
                    return true;
                }
                else {
                    // not directly following entities
                    RB_DEBUG->print("GV_Information::isTrimmable: false");
                    return false;
                }
            }
            else if ((e1->getParentContainer()->rtti()==GV2::EntityContainer ||
                      e1->getParentContainer()->rtti()==GV2::EntityGraphic ||
                      e1->getParentContainer()->rtti()==GV2::EntityBlock) &&
                     (e2->getParentContainer()->rtti()==GV2::EntityContainer ||
                      e2->getParentContainer()->rtti()==GV2::EntityGraphic ||
                      e2->getParentContainer()->rtti()==GV2::EntityBlock)) {

                // normal entities:
                return true;
            }
        }
        else {
            // independent entities with the same parent:
            return (e1->getParentContainer()==e2->getParentContainer());
        }
    }

    return false;
}


/**
 * Gets the nearest end point to the given coordinate.
 *
 * @param coord Coordinate (typically a mouse coordinate)
 *
 * @return the coordinate found or an invalid vector
 * if there are no elements at all in this graphics 
 * container.
 */
GV_Vector GV_Information::getNearestEndpoint(const GV_Vector& coord,
        double* dist) const {
    return container->getNearestEndpoint(coord, dist);
}


/**
 * Gets the nearest point to the given coordinate which is on an entity. 
 *
 * @param coord Coordinate (typically a mouse coordinate)
 * @param dist Pointer to a double which will contain the 
 *        measured distance after return or NULL
 * @param entity Pointer to a pointer which will point to the
 *        entity on which the point is or NULL
 *
 * @return the coordinate found or an invalid vector
 * if there are no elements at all in this graphics 
 * container.
 */
GV_Vector GV_Information::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity,
        double* dist,
        GV_Entity** entity) const {

    return container->getNearestPointOnEntity(coord, onEntity, dist, entity);
}


/**
 * Gets the nearest entity to the given coordinate.
 *
 * @param coord Coordinate (typically a mouse coordinate)
 * @param dist Pointer to a double which will contain the 
 *             masured distance after return
 * @param level Level of resolving entities.
 *
 * @return the entity found or NULL if there are no elements 
 * at all in this graphics container.
 */
GV_Entity* GV_Information::getNearestEntity(const GV_Vector& coord,
        double* dist,
        GV2::ResolveLevel level) const {

    return container->getNearestEntity(coord, dist, level);
}



/**
 * Calculates the intersection point(s) between two entities.
 *
 * @param onEntities true: only return intersection points which are
 *                   on both entities.
 *                   false: return all intersection points.
 *
 * @todo support more entities
 *
 * @return All intersections of the two entities. The tangent flag in
 * GV_VectorList is set if one intersection is a tangent point.
 */
GV_VectorList GV_Information::getIntersection(GV_Entity* e1,
        GV_Entity* e2, bool onEntities) {

    //RB_DEBUG->print("GV_Information::getIntersection");
    
    GV_VectorList ret;
    double tol = 1.0e-4;

    if (e1==NULL || e2==NULL) {
        return ret;
    }

    // unsupported entities / entity combinations:
    if ((e1->rtti()==GV2::EntityEllipse && e2->rtti()==GV2::EntityEllipse) ||
            e1->rtti()==GV2::EntityText || e2->rtti()==GV2::EntityText ||
            e1->isOfType(GV2::EntityDimension) || e2->isOfType(GV2::EntityDimension)) {

        return ret;
    }

    // if the point has to be on the entities, rule out entities whos
    //   bounding rect does not overlap:
    if (onEntities) {
        RB_DEBUG->print("GV_Information::getIntersection: "
            "bounding rect does not overlap");
        GV_Vector min1 = e1->getMin();
        GV_Vector max1 = e1->getMax();
        GV_Vector min2 = e2->getMin();
        GV_Vector max2 = e2->getMax();
        if (min1.x>max2.x+1.0e-6 || min1.y>max2.y+1.0e-6 || max1.x<min2.x-1.0e-6 || max1.y<min2.y-1.0e-6) {
            return ret;
        }
    }

    // (only) one entity is an ellipse:
    if (e1->rtti()==GV2::EntityEllipse || e2->rtti()==GV2::EntityEllipse) {
        if (e2->rtti()==GV2::EntityEllipse) {
            GV_Entity* tmp = e1;
            e1 = e2;
            e2 = tmp;
        }
        if (e2->rtti()==GV2::EntityLine) {
            GV_Ellipse* ellipse = dynamic_cast<GV_Ellipse*>(e1);
            ret = getIntersectionLineEllipse(dynamic_cast<GV_Line*>(e2), ellipse);
            tol = 1.0e-1;
        } 
        
        // ellipse / arc, ellipse / ellipse: not supported:
        else {
            return ret;
        }
    } else {

        GV_Entity* te1 = e1;
        GV_Entity* te2 = e2;

        // entity copies - so we only have to deal with lines and arcs
        GV_Line l1(NULL,
                   GV_LineData(GV_Vector(0.0, 0.0), GV_Vector(0.0,0.0)));
        GV_Line l2(NULL,
                   GV_LineData(GV_Vector(0.0, 0.0), GV_Vector(0.0,0.0)));

        GV_Arc a1(NULL,
                  GV_ArcData(GV_Vector(0.0,0.0), 1.0, 0.0, 2*M_PI, false));
        GV_Arc a2(NULL,
                  GV_ArcData(GV_Vector(0.0,0.0), 1.0, 0.0, 2*M_PI, false));

        // convert construction lines to lines:
        if (e1->rtti()==GV2::EntityConstructionLine) {
            GV_ConstructionLine* cl = dynamic_cast<GV_ConstructionLine*>(e1);

            l1.setStartpoint(cl->getPoint1());
            l1.setEndpoint(cl->getPoint2());

            te1 = &l1;
        }
        if (e2->rtti()==GV2::EntityConstructionLine) {
            GV_ConstructionLine* cl = dynamic_cast<GV_ConstructionLine*>(e2);

            l2.setStartpoint(cl->getPoint1());
            l2.setEndpoint(cl->getPoint2());

            te2 = &l2;
        }


        // convert circles to arcs:
        if (e1->isOfType(GV2::EntityCircle)) {
            GV_Circle* c = dynamic_cast<GV_Circle*>(e1);

            GV_ArcData data(c->getCenter(), c->getRadius(), 0.0, 2*M_PI, false);
            a1.setData(data);

            te1 = &a1;
        }
        if (e2->isOfType(GV2::EntityCircle)) {
            GV_Circle* c = dynamic_cast<GV_Circle*>(e2);

            GV_ArcData data(c->getCenter(), c->getRadius(), 0.0, 2*M_PI, false);
            a2.setData(data);

            te2 = &a2;
        }


        // line / line:
        if (te1->isOfType(GV2::EntityLine) && te2->isOfType(GV2::EntityLine)) {
            GV_Line* line1 = dynamic_cast<GV_Line*>(te1);
            GV_Line* line2 = dynamic_cast<GV_Line*>(te2);

            ret = getIntersectionLineLine(line1, line2);
        }

        // line / arc:
        else if (te1->isOfType(GV2::EntityLine) && te2->isOfType(GV2::EntityArc)) {

            GV_Line* line = dynamic_cast<GV_Line*>(te1);
            GV_Arc* arc = dynamic_cast<GV_Arc*>(te2);

            ret = getIntersectionLineArc(line, arc);
        }

        // arc / line:
        else if (te1->isOfType(GV2::EntityArc) && te2->isOfType(GV2::EntityLine)) {

            GV_Arc* arc = dynamic_cast<GV_Arc*>(te1);
            GV_Line* line = dynamic_cast<GV_Line*>(te2);

            ret = getIntersectionLineArc(line, arc);
        }

        // arc / arc:
        else if (te1->isOfType(GV2::EntityArc) &&
                 te2->isOfType(GV2::EntityArc)) {

            GV_Arc* arc1 = dynamic_cast<GV_Arc*>(te1);
            GV_Arc* arc2 = dynamic_cast<GV_Arc*>(te2);

            ret = getIntersectionArcArc(arc1, arc2);
        } else {
            RB_DEBUG->print("GV_Information::getIntersection:: Unsupported entity type.");
        }
    }


    // Check all intersection points for being on entities:
    if (onEntities==true) {
        for (int i=0; i<ret.count(); ++i) {
            if (!e1->isPointOnEntity(ret.get(i), tol) ||
                !e2->isPointOnEntity(ret.get(i), tol)) {
                ret.set(i, GV_Vector(false));
            }
        }
    }

    int k=0;
    for (int i=0; i<ret.count(); ++i) {
        if (ret.get(i).valid) {
            ret.set(k, ret.get(i));
            k++;
        }
    }
    for (int i=k; i<4; ++i) {
        if (i<ret.count()) {
            ret.set(i, GV_Vector(false));
        }
    }
    
    //RB_DEBUG->print("GV_Information::getIntersection: OK");

    return ret;
}



/**
 * @return Intersection between two lines.
 */
GV_VectorList GV_Information::getIntersectionLineLine(GV_Line* e1,
        GV_Line* e2) {

    GV_VectorList ret;

    if (e1==NULL || e2==NULL) {
        return ret;
    }
            
    RB_DEBUG->print("GV_Information::getIntersectionLineLine");

    GV_Vector p1 = e1->getStartpoint();
    GV_Vector p2 = e1->getEndpoint();
    GV_Vector p3 = e2->getStartpoint();
    GV_Vector p4 = e2->getEndpoint();
    
//    RB_DEBUG->print("GV_Information::getIntersectionLineLine: %f/%f %f/%f - %f/%f %f/%f",
//        p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);

    double num = ((p4.x-p3.x)*(p1.y-p3.y) - (p4.y-p3.y)*(p1.x-p3.x));
    double div = ((p4.y-p3.y)*(p2.x-p1.x) - (p4.x-p3.x)*(p2.y-p1.y));
    
//    RB_DEBUG->print("GV_Information::getIntersectionLineLine: "
//        "div: %f (tolerance: %f)", div, GV_TOLERANCE);

    if (fabs(div)>1.0e-3) {
        double u = num / div;

        double xs = p1.x + u * (p2.x-p1.x);
        double ys = p1.y + u * (p2.y-p1.y);
        ret = GV_VectorList(GV_Vector(xs, ys));
    }

    // lines are parallel
    else {
        ret = GV_VectorList();
    }
    
    RB_DEBUG->print("GV_Information::getIntersectionLineLine: OK");

    return ret;
}



/**
 * @return One or two intersection points between given entities.
 */
GV_VectorList GV_Information::getIntersectionLineArc(GV_Line* line,
        GV_Arc* arc) {

    GV_VectorList ret;

    if (line==NULL || arc==NULL) {
        return ret;
    }

    double dist=0.0;
    GV_Vector nearest;
    nearest = line->getNearestPointOnEntity(arc->getCenter(), false, &dist);

    // special case: arc touches line (tangent):
    if (fabs(dist - arc->getRadius()) < 1.0e-4) {
        ret = GV_VectorList(nearest);
        ret.setTangent(true);
        return ret;
    }

    GV_Vector p = line->getStartpoint();
    GV_Vector d = line->getEndpoint() - line->getStartpoint();
    if (d.magnitude()<1.0e-6) {
        return ret;
    }

    GV_Vector c = arc->getCenter();
    double r = arc->getRadius();
    GV_Vector delta = p - c;

    // root term:
    double term = GV_Math::pow(GV_Vector::dotP(d, delta), 2.0)
                  - GV_Math::pow(d.magnitude(), 2.0)
                  * (GV_Math::pow(delta.magnitude(), 2.0) - GV_Math::pow(r, 2.0));

    // no intersection:
    if (term<0.0) {
        GV_VectorList s;
        ret = s;
    }

    // one or two intersections:
    else {
        double t1 = (- GV_Vector::dotP(d, delta) + sqrt(term))
                    / GV_Math::pow(d.magnitude(), 2.0);
        double t2;
        bool tangent = false;

        // only one intersection:
        if (fabs(term)<GV_TOLERANCE) {
            t2 = t1;
            tangent = true;
        }

        // two intersections
        else {
            t2 = (-GV_Vector::dotP(d, delta) - sqrt(term))
                 / GV_Math::pow(d.magnitude(), 2.0);
        }

        GV_Vector sol1;
        GV_Vector sol2(false);

        sol1 = p + d * t1;

        if (!tangent) {
            sol2 = p + d * t2;
        }

        ret = GV_VectorList(sol1, sol2);
        ret.setTangent(tangent);
    }

    return ret;
}



/**
 * @return One or two intersection points between given entities.
 */
GV_VectorList GV_Information::getIntersectionArcArc(GV_Arc* e1,
        GV_Arc* e2) {

    GV_VectorList ret;

    if (e1==NULL || e2==NULL) {
        return ret;
    }

    GV_Vector c1 = e1->getCenter();
    GV_Vector c2 = e2->getCenter();

    double r1 = e1->getRadius();
    double r2 = e2->getRadius();

    GV_Vector u = c2 - c1;

    // concentric
    if (u.magnitude()<1.0e-6) {
        return ret;
    }

    GV_Vector v = GV_Vector(u.y, -u.x);

    double s, t1, t2, term;

    s = 1.0/2.0 * ((r1*r1 - r2*r2)/(GV_Math::pow(u.magnitude(), 2.0)) + 1.0);

    term = (r1*r1)/(GV_Math::pow(u.magnitude(), 2.0)) - s*s;

    // no intersection:
    if (term<0.0) {
        ret = GV_VectorList();
    }

    // one or two intersections:
    else {
        t1 = sqrt(term);
        t2 = -sqrt(term);
        bool tangent = false;

        GV_Vector sol1 = c1 + u*s + v*t1;
        GV_Vector sol2 = c1 + u*s + v*t2;

        if (sol1.distanceTo(sol2)<1.0e-4) {
            sol2 = GV_Vector(false);
            ret = GV_VectorList(sol1);
            tangent = true;
        } else {
            ret = GV_VectorList(sol1, sol2);
        }

        ret.setTangent(tangent);
    }

    return ret;
}



/**
 * @return One or two intersection points between given entities.
 */
GV_VectorList GV_Information::getIntersectionLineEllipse(GV_Line* line,
        GV_Ellipse* ellipse) {

    GV_VectorList ret;

    if (line==NULL || ellipse==NULL) {
        return ret;
    }

    // rotate into normal position:
    double ang = ellipse->getAngle();

    double rx = ellipse->getMajorRadius();
    double ry = ellipse->getMinorRadius();
    GV_Vector center = ellipse->getCenter();
    GV_Vector a1 = line->getStartpoint().rotate(center, -ang);
    GV_Vector a2 = line->getEndpoint().rotate(center, -ang);
    GV_Vector origin = a1;
    GV_Vector dir = a2-a1;
    GV_Vector diff = origin - center;
    GV_Vector mDir = GV_Vector(dir.x/(rx*rx), dir.y/(ry*ry));
    GV_Vector mDiff = GV_Vector(diff.x/(rx*rx), diff.y/(ry*ry));

    double a = GV_Vector::dotP(dir, mDir);
    double b = GV_Vector::dotP(dir, mDiff);
    double c = GV_Vector::dotP(diff, mDiff) - 1.0;
    double d = b*b - a*c;

    if (d < 0) {
        RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: outside 0");
    } else if ( d > 0 ) {
        double root = sqrt(d);
        double t_a = (-b - root) / a;
        double t_b = (-b + root) / a;

        /*if ( (t_a < 0 || 1 < t_a) && (t_b < 0 || 1 < t_b) ) {
            if ( (t_a < 0 && t_b < 0) || (t_a > 1 && t_b > 1) ) {
                RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: outside 1");
            }
            else {
                RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: inside 1");
            }
        } else {*/
            RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: intersection 1");
            GV_Vector ret1(false);
            GV_Vector ret2(false);
            //if ( 0 <= t_a && t_a <= 1 ) { 
                //RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: 0<=t_a<=1");
                ret1 = a1.lerp(a2, t_a);
//                RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: ret1: %f/%f", ret1.x, ret1.y);
            //}
            //if ( 0 <= t_b && t_b <= 1 ) {
                //RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: 0<=t_b<=1");
                ret2 = a1.lerp(a2, t_b);
//                RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: ret2: %f/%f", ret2.x, ret2.y);
            //}
            if (ret1.valid && ret2.valid) {
                ret = GV_VectorList(ret1, ret2);
            }
            else {
                if (ret1.valid) {
                    ret = GV_VectorList(ret1);
                }
                if (ret2.valid) {
                    ret = GV_VectorList(ret2);
                }
            }
        //}
    } else {
        double t = -b/a;
        if ( 0 <= t && t <= 1 ) {
            RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: 0<=t<=1");
            RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: intersection 2");
            ret = GV_VectorList(a1.lerp(a2, t));
//            RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: ret1: %f/%f", ret.get(0).x, ret.get(0).y);
        } else {
            RB_DEBUG->print("GV_Information::getIntersectionLineEllipse: outside 2");
        }
    }

    ret.rotate(center, ang);
    return ret;



    /*
    GV_Arc* arc = new GV_Arc(NULL,
                             GV_ArcData(ellipse->getCenter(),
                                        ellipse->getMajorRadius(),
                                        ellipse->getAngle1(),
                                        ellipse->getAngle2(),
                                        false));
    GV_Line* other = (GV_Line*)line->clone();
    double angle = ellipse->getAngle();
    //double ratio = ellipse->getRatio();

    // rotate entities:
    other->rotate(ellipse->getCenter(), -angle);
    other->scale(ellipse->getCenter(), GV_Vector(1.0, 1.0/ellipse->getRatio()));

    ret = getIntersectionLineArc(other, arc);

    ret.scale(ellipse->getCenter(), GV_Vector(1.0, ellipse->getRatio()));
    ret.rotate(ellipse->getCenter(), angle);

    delete arc;
    delete other;

    return ret;
    */
}


/**
 * Checks if the given coordinate is inside the given contour.
 *
 * @param point Coordinate to check.
 * @param contour One or more entities which shape a contour.
 *         If the given contour is not closed, the result is undefined.
 *         The entities don't need to be in a specific order.
 * @param onContour Will be set to true if the given point it exactly
 *         on the contour.
 */
bool GV_Information::isPointInsideContour(const GV_Vector& point,
        GV_EntityContainer* contour, bool* onContour) {

    if (contour==NULL) {
        RB_DEBUG->warning("GV_Information::isPointInsideContour: contour is NULL");
        return false;
    }

    if (point.x < contour->getMin().x || point.x > contour->getMax().x ||
            point.y < contour->getMin().y || point.y > contour->getMax().y) {
        return false;
    }

    double width = contour->getSize().x+1.0;

    bool sure;
    int counter;
    int tries = 0;
    double rayAngle = 0.0;
    do {
        sure = true;

        // create ray:
        GV_Vector v;
        v.setPolar(width*10.0, rayAngle);
        GV_Line ray(NULL, GV_LineData(point, point+v));
        counter = 0;
        GV_VectorList sol;

        if (onContour!=NULL) {
            *onContour = false;
        }

        for (GV_Entity* e = contour->firstEntity(GV2::ResolveAll);
                e!=NULL;
                e = contour->nextEntity(GV2::ResolveAll)) {

            // intersection(s) from ray with contour entity:
            sol = GV_Information::getIntersection(&ray, e, true);

            for (int i=0; i<=1; ++i) {
                GV_Vector p = sol.get(i);

                if (p.valid) {
                    // point is on the contour itself
                    if (p.distanceTo(point)<1.0e-5) {
                        if (onContour!=NULL) {
                            *onContour = true;
                        }
                    } else {
                        if (e->isOfType(GV2::EntityLine)) {
                            GV_Line* line = dynamic_cast<GV_Line*>(e);

                            // ray goes through startpoint of line:
                            if (p.distanceTo(line->getStartpoint())<1.0e-4) {
                                if (GV_Math::correctAngle(line->getAngle1())<M_PI) {
                                    counter++;
                                    sure = false;
                                }
                            }

                            // ray goes through endpoint of line:
                            else if (p.distanceTo(line->getEndpoint())<1.0e-4) {
                                if (GV_Math::correctAngle(line->getAngle2())<M_PI) {
                                    counter++;
                                    sure = false;
                                }
                            }
                            // ray goes through the line:


                            else {
                                counter++;
                            }
                        } else if (e->isOfType(GV2::EntityArc)) {
                            GV_Arc* arc = dynamic_cast<GV_Arc*>(e);

                            if (p.distanceTo(arc->getStartpoint())<1.0e-4) {
                                double dir = arc->getDirection1();
                                if ((dir<M_PI && dir>=1.0e-5) ||
                                        ((dir>2*M_PI-1.0e-5 || dir<1.0e-5) &&
                                         arc->getCenter().y>p.y)) {
                                    counter++;
                                    sure = false;
                                }
                            }
                            else if (p.distanceTo(arc->getEndpoint())<1.0e-4) {
                                double dir = arc->getDirection2();
                                if ((dir<M_PI && dir>=1.0e-5) ||
                                        ((dir>2*M_PI-1.0e-5 || dir<1.0e-5) &&
                                         arc->getCenter().y>p.y)) {
                                    counter++;
                                    sure = false;
                                }
                            } else {
                                counter++;
                            }
                        } else if (e->isOfType(GV2::EntityCircle)) {
                            // tangent:
                            if (i==0 && sol.get(1).valid==false) {
                                if (!sol.isTangent()) {
                                    counter++;
                                } else {
                                    sure = false;
                                }
                            } else if (i==1 || sol.get(1).valid==true) {
                                counter++;
                            }
                        }
                    }
                }
            }
        }

        rayAngle+=0.02;
        tries++;
    }
    while (!sure && rayAngle<2*M_PI && tries<6);

    return ((counter%2)==1);
}
    
    
    
bool GV_Information::isOverlapping(const GV_Vector& r1v1, const GV_Vector& r1v2,
        const GV_Vector& r2v1, const GV_Vector& r2v2) {
        
    GV_Vector min1 = GV_Vector::minimum(r1v1, r1v2);
    GV_Vector max1 = GV_Vector::maximum(r1v1, r1v2);
    GV_Vector min2 = GV_Vector::minimum(r2v1, r2v2);
    GV_Vector max2 = GV_Vector::maximum(r2v1, r2v2);

    return !(min1.x>max2.x+1.0e-6 || 
             min1.y>max2.y+1.0e-6 || 
             max1.x<min2.x-1.0e-6 || 
             max1.y<min2.y-1.0e-6);
}
    
