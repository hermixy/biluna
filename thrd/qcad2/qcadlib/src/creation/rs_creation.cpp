/****************************************************************************
** $Id: rs_creation.cpp 9417 2008-03-15 11:26:21Z andrew $
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


#include "rs_creation.h"

#include "rs_information.h"
#include "rs_fileinfo.h"
#include "rs_graphic.h"
#include "rs_constructionline.h"
#include "rs_graphicview.h"
#include "rs_modification.h"

/**
 * Default constructor.
 *
 * @param container The container to which we will add
 *        entities. Usually that's an RS_Graphic entity but
 *        it can also be a polyline, text, ...
 */
RS_Creation::RS_Creation(RS_EntityContainer* container,
                         RS_GraphicView* graphicView,
                         bool handleUndo) {
    this->container = container;
    this->graphicView = graphicView;
    this->handleUndo = handleUndo;
    if (container!=NULL) {
        graphic = container->getGraphic();
        document = container->getDocument();
    } else {
        graphic = NULL;
        document = NULL;
    }
}



/**
 * Creates an entity parallel to the given entity e through the given 
 * 'coord'.
 *
 * @param coord Coordinate to define the distance / side (typically a 
 *              mouse coordinate).
 * @param number Number of parallels.
 * @param e Original entity.
 *
 * @return Pointer to the first created parallel or NULL if no 
 *    parallel has been created.
 */
RS_Entity* RS_Creation::createParallelThrough(const RS_Vector& coord,
        int number,
        RS_Entity* e) {
    if (e==NULL) {
        return NULL;
    }

    double dist;

    if (e->rtti()==RS2::EntityLine) {
        RS_Line* l = dynamic_cast<RS_Line*>(e);
        RS_ConstructionLine cl(NULL,
                               RS_ConstructionLineData(l->getStartpoint(),
                                                       l->getEndpoint()));
        dist = cl.getDistanceToPoint(coord);
    } else {
        dist = e->getDistanceToPoint(coord);
    }

    if (dist<RS_MAXDOUBLE) {
        return createParallel(coord, dist, number, e);
    } else {
        return NULL;
    }
}



/**
 * Creates an entity parallel to the given entity e. 
 * Out of the 2 possible parallels, the one closest to
 * the given coordinate is returned.
 * Lines, Arcs and Circles can have parallels.
 *
 * @param coord Coordinate to define which parallel we want (typically a 
 *              mouse coordinate).
 * @param distance Distance of the parallel.
 * @param number Number of parallels.
 * @param e Original entity.
 *
 * @return Pointer to the first created parallel or NULL if no 
 *    parallel has been created.
 */
RS_Entity* RS_Creation::createParallel(const RS_Vector& coord,
                                       double distance, int number,
                                       RS_Entity* e) {
    if (e==NULL) {
        return NULL;
    }

    if (e->isOfType(RS2::EntityLine)) {
        return createParallelLine(coord, distance, number, dynamic_cast<RS_Line*>(e));
    } else if (e->isOfType(RS2::EntityArc)) {
        return createParallelArc(coord, distance, number, dynamic_cast<RS_Arc*>(e));
    } else if (e->isOfType(RS2::EntityCircle)) {
        return createParallelCircle(coord, distance, number, dynamic_cast<RS_Circle*>(e));
    }

    return NULL;
}



/**
 * Creates a line parallel to the given line e. 
 * Out of the 2 possible parallels, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which parallel we want (typically a 
 *              mouse coordinate).
 * @param distance Distance of the parallel.
 * @param number Number of parallels.
 * @param e Original entity.
 *
 * @return Pointer to the first created parallel or NULL if no 
 *    parallel has been created.
 */
RS_Line* RS_Creation::createParallelLine(const RS_Vector& coord,
        double distance, int number,
        RS_Line* e) {

    if (e==NULL) {
        return NULL;
    }

    double ang = e->getAngle1() + M_PI/2.0;
    RS_Vector p1, p2;
    RS_LineData parallelData;
    RS_Line* ret = NULL;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->createPainter();
    }
    for (int num=1; num<=number; ++num) {

        // calculate 1st parallel:
        p1.setPolar(distance*num, ang);
        p1 += e->getStartpoint();
        p2.setPolar(distance*num, ang);
        p2 += e->getEndpoint();
        RS_Line parallel1(NULL, RS_LineData(p1, p2));

        // calculate 2nd parallel:
        p1.setPolar(distance*num, ang+M_PI);
        p1 += e->getStartpoint();
        p2.setPolar(distance*num, ang+M_PI);
        p2 += e->getEndpoint();
        RS_Line parallel2(NULL, RS_LineData(p1, p2));

        double dist1 = parallel1.getDistanceToPoint(coord);
        double dist2 = parallel2.getDistanceToPoint(coord);
        double minDist = std::min(dist1, dist2);

        if (minDist<RS_MAXDOUBLE) {
            if (dist1<dist2) {
                parallelData = parallel1.getData();
            } else {
                parallelData = parallel2.getData();
            }


            RS_Line* newLine = new RS_Line(container, parallelData);
            newLine->setLayerToActive();
            newLine->setPenToActive();
            if (ret==NULL) {
                ret = newLine;
            }
            if (container!=NULL) {
                container->addEntity(newLine);
            }
            if (document!=NULL && handleUndo) {
                document->addUndoable(newLine);
                //document->endUndoCycle();
            }
            if (graphicView!=NULL) {
                graphicView->drawEntity(newLine);
            }
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    return ret;
}



/**
 * Creates a arc parallel to the given arc e. 
 * Out of the 2 possible parallels, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which parallel we want (typically a 
 *              mouse coordinate).
 * @param distance Distance of the parallel.
 * @param number Number of parallels.
 * @param e Original entity.
 *
 * @return Pointer to the first created parallel or NULL if no 
 *    parallel has been created.
 */
RS_Arc* RS_Creation::createParallelArc(const RS_Vector& coord,
                                       double distance, int number,
                                       RS_Arc* e) {

    if (e==NULL) {
        return NULL;
    }

    RS_ArcData parallelData;
    RS_Arc* ret = NULL;

    bool inside = (e->getCenter().distanceTo(coord) < e->getRadius());

    if (inside) {
        distance *= -1;
    }
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
    }
    for (int num=1; num<=number; ++num) {

        // calculate parallel:
        bool ok = true;
        RS_Arc parallel1(NULL, e->getData());
        parallel1.setRadius(e->getRadius() + distance*num);
        if (parallel1.getRadius()<0.0) {
            parallel1.setRadius(RS_MAXDOUBLE);
            ok = false;
        }

        // calculate 2nd parallel:
        //RS_Arc parallel2(NULL, e->getData());
        //parallel2.setRadius(e->getRadius()+distance*num);

        //double dist1 = parallel1.getDistanceToPoint(coord);
        //double dist2 = parallel2.getDistanceToPoint(coord);
        //double minDist = min(dist1, dist2);

        //if (minDist<RS_MAXDOUBLE) {
        if (ok==true) {
            //if (dist1<dist2) {
            parallelData = parallel1.getData();
            //} else {
            //    parallelData = parallel2.getData();
            //}

            RS_Arc* newArc = new RS_Arc(container, parallelData);
            newArc->setLayerToActive();
            newArc->setPenToActive();
            if (ret==NULL) {
                ret = newArc;
            }
            if (container!=NULL) {
                container->addEntity(newArc);
            }
            if (document!=NULL && handleUndo) {
                document->addUndoable(newArc);
            }
            if (graphicView!=NULL) {
                graphicView->drawEntity(newArc);
            }
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }
    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    return ret;
}



/**
 * Creates a circle parallel to the given circle e. 
 * Out of the 2 possible parallels, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which parallel we want (typically a 
 *              mouse coordinate).
 * @param distance Distance of the parallel.
 * @param number Number of parallels.
 * @param e Original entity.
 *
 * @return Pointer to the first created parallel or NULL if no 
 *    parallel has been created.
 */
RS_Circle* RS_Creation::createParallelCircle(const RS_Vector& coord,
        double distance, int number,
        RS_Circle* e) {

    if (e==NULL) {
        return NULL;
    }

    RS_CircleData parallelData;
    RS_Circle* ret = NULL;

    bool inside = (e->getCenter().distanceTo(coord) < e->getRadius());

    if (inside) {
        distance *= -1;
    }
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    for (int num=1; num<=number; ++num) {

        // calculate parallel:
        bool ok = true;
        RS_Circle parallel1(NULL, e->getData());
        parallel1.setRadius(e->getRadius() + distance*num);
        if (parallel1.getRadius()<0.0) {
            parallel1.setRadius(RS_MAXDOUBLE);
            ok = false;
        }

        if (ok==true) {
            parallelData = parallel1.getData();

            RS_Circle* newCircle = new RS_Circle(container, parallelData);
            newCircle->setLayerToActive();
            newCircle->setPenToActive();
            if (ret==NULL) {
                ret = newCircle;
            }
            if (container!=NULL) {
                container->addEntity(newCircle);
            }
            if (document!=NULL && handleUndo) {
                document->addUndoable(newCircle);
            }
            if (graphicView!=NULL) {
                graphicView->drawEntity(newCircle);
            }
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }
    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }
    return ret;
}



/**
 * Creates a bisecting line of the angle between the entities 
 * e1 and e2. Out of the 4 possible bisectors, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which bisector we want (typically a 
 *              mouse coordinate).
 * @param length Length of the bisecting line.
 * @param num Number of bisectors
 * @param l1 First line.
 * @param l2 Second line.
 *
 * @return Pointer to the first bisector created or NULL if no bisectors
 *   were created.
 */
RS_Line* RS_Creation::createBisector(const RS_Vector& coord1,
                                     const RS_Vector& coord2,
                                     double length,
                                     int num,
                                     RS_Line* l1,
                                     RS_Line* l2) {

    RS_VectorList sol;

    // check given entities:
    if (l1==NULL || l2==NULL ||
            l1->rtti()!=RS2::EntityLine || l2->rtti()!=RS2::EntityLine) {
        return NULL;
    }

    // intersection between entities:
    sol = RS_Information::getIntersection(l1, l2, false);
    RS_Vector inters = sol.get(0);
    if (inters.valid==false) {
        return NULL;
    }

    double angle1 = inters.angleTo(l1->getNearestPointOnEntity(coord1));
    double angle2 = inters.angleTo(l2->getNearestPointOnEntity(coord2));
    double angleDiff = RS_Math::getAngleDifference(angle1, angle2);
    if (angleDiff>M_PI) {
        angleDiff = angleDiff - 2*M_PI;
    }
    RS_Line* ret = NULL;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    for (int n=1; n<=num; ++n) {

        double angle = angle1 +
                       (angleDiff / (num+1) * n);

        RS_LineData d;
        RS_Vector v;

        RS_Vector c;
        v.setPolar(length, angle);
        d = RS_LineData(inters, inters + v);

        RS_Line* newLine = new RS_Line(container, d);
        if (container!=NULL) {
            newLine->setLayerToActive();
            newLine->setPenToActive();
            container->addEntity(newLine);
        }
        if (document!=NULL && handleUndo) {
            document->addUndoable(newLine);
        }
        if (graphicView!=NULL) {
            graphicView->drawEntity(newLine);
        }
        if (ret==NULL) {
            ret = newLine;
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }
    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    return ret;
}



/**
 * Creates a tangent between a given point and a circle or arc.
 * Out of the 2 possible tangents, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which tangent we want (typically a 
 *              mouse coordinate).
 * @param point Point.
 * @param circle Circle, arc or ellipse entity.
 */
RS_Line* RS_Creation::createTangent1(const RS_Vector& coord,
                                     const RS_Vector& point,
                                     RS_Entity* circle) {
    RS_Line* ret = NULL;
    RS_Vector circleCenter;

    // check given entities:
    if (circle==NULL || !point.valid ||
            (circle->rtti()!=RS2::EntityArc && circle->rtti()!=RS2::EntityCircle
             && circle->rtti()!=RS2::EntityEllipse)) {

        return NULL;
    }

    if (circle->rtti()==RS2::EntityCircle) {
        circleCenter = dynamic_cast<RS_Circle*>(circle)->getCenter();
    } else if (circle->rtti()==RS2::EntityArc) {
        circleCenter = dynamic_cast<RS_Arc*>(circle)->getCenter();
    } else if (circle->rtti()==RS2::EntityEllipse) {
        circleCenter = dynamic_cast<RS_Ellipse*>(circle)->getCenter();
    }

    // the two tangent points:
    RS_VectorList sol;

    // calculate tangent points for arcs / circles:
    if (circle->rtti()!=RS2::EntityEllipse) {
        // create temp. thales circle:
        RS_Vector tCenter = (point + circleCenter)/2.0;
        double tRadius = point.distanceTo(tCenter);

        RS_Circle tmp(NULL, RS_CircleData(tCenter, tRadius));

        // get the two intersection points which are the tangent points:
        sol = RS_Information::getIntersection(&tmp, circle, false);
    }

    // calculate tangent points for ellipses:
    else {
        RS_Ellipse* el = dynamic_cast<RS_Ellipse*>(circle);
        //sol.alloc(2);
        //sol.set(0, circleCenter);
        //sol.set(1, circleCenter);


        double a = el->getMajorRadius();     // the length of the major axis / 2
        double b = el->getMinorRadius();     // the length of the minor axis / 2

        // rotate and move point:
        RS_Vector point2 = point;
        point2.move(-el->getCenter());
        point2.rotate(-el->getAngle());

        double xp = point2.x;             // coordinates of the given point
        double yp = point2.y;

        double xt1;                      // Tangent point 1
        double yt1;
        double xt2;                      // Tangent point 2
        double yt2;

        double a2 = a * a;
        double b2 = b * b;
        double d = a2 / b2 * yp / xp;
        double e = a2 / xp;
        double af = b2 * d * d + a2;
        double bf = -b2 * d * e * 2.0;
        double cf = b2 * e * e - a2 * b2;
        double t = sqrt(bf * bf - af * cf * 4.0);
        yt1 = (t - bf) / (af * 2.0);
        xt1 = e - d * yt1;
        yt2 = (-t - bf) / (af * 2.0);
        xt2 = e - d * yt2;

        RS_Vector s1 = RS_Vector(xt1, yt1);
        RS_Vector s2 = RS_Vector(xt2, yt2);

        s1.rotate(el->getAngle());
        s1.move(el->getCenter());

        s2.rotate(el->getAngle());
        s2.move(el->getCenter());

        sol.set(0, s1);
        sol.set(1, s2);
    }

    if (!sol.get(0).valid || !sol.get(1).valid) {
        return NULL;
    }

    // create all possible tangents:
    RS_Line* poss[2];

    RS_LineData d;

    d = RS_LineData(sol.get(0), point);
    poss[0] = new RS_Line(NULL, d);
    d = RS_LineData(sol.get(1), point);
    poss[1] = new RS_Line(NULL, d);

    // find closest tangent:
    double minDist = RS_MAXDOUBLE;
    double dist;
    int idx = -1;
    for (int i=0; i<2; ++i) {
        dist = poss[i]->getDistanceToPoint(coord);
        if (dist<minDist) {
            minDist = dist;
            idx = i;
        }
    }

    // create the closest tangent:
    if (idx!=-1) {
        RS_LineData d = poss[idx]->getData();

        for (int i=0; i<2; ++i) {
            delete poss[i];
        }

        if (document!=NULL && handleUndo) {
            document->startUndoCycle();
        }

        ret = new RS_Line(container, d);
        ret->setLayerToActive();
        ret->setPenToActive();
        if (container!=NULL) {
            container->addEntity(ret);
        }
        if (document!=NULL && handleUndo) {
            document->addUndoable(ret);
            document->endUndoCycle();
        }
        if (graphicView!=NULL) {
            graphicView->createPainter();
            graphicView->drawEntity(ret);
            graphicView->restore();
        }
    } else {
        ret = NULL;
    }

    return ret;
}



/**
 * Creates a tangent between two circles or arcs.
 * Out of the 4 possible tangents, the one closest to
 * the given coordinate is returned.
 *
 * @param coord Coordinate to define which tangent we want (typically a 
 *              mouse coordinate).
 * @param circle1 1st circle or arc entity.
 * @param circle2 2nd circle or arc entity.
 */
RS_Line* RS_Creation::createTangent2(const RS_Vector& coord,
                                     RS_Entity* circle1,
                                     RS_Entity* circle2) {
    RS_Line* ret = NULL;
    RS_Vector circleCenter1;
    RS_Vector circleCenter2;
    double circleRadius1 = 0.0;
    double circleRadius2 = 0.0;

    // check given entities:
    if (circle1==NULL || circle2==NULL ||
            (circle1->rtti()!=RS2::EntityArc &&
             circle1->rtti()!=RS2::EntityCircle) ||
            (circle2->rtti()!=RS2::EntityArc &&
             circle2->rtti()!=RS2::EntityCircle) ) {

        return NULL;
    }

    if (circle1->rtti()==RS2::EntityCircle) {
        circleCenter1 = (dynamic_cast<RS_Circle*>(circle1))->getCenter();
        circleRadius1 = (dynamic_cast<RS_Circle*>(circle1))->getRadius();
    } else if (circle1->rtti()==RS2::EntityArc) {
        circleCenter1 = (dynamic_cast<RS_Arc*>(circle1))->getCenter();
        circleRadius1 = (dynamic_cast<RS_Arc*>(circle1))->getRadius();
    }

    if (circle2->rtti()==RS2::EntityCircle) {
        circleCenter2 = (dynamic_cast<RS_Circle*>(circle2))->getCenter();
        circleRadius2 = (dynamic_cast<RS_Circle*>(circle2))->getRadius();
    } else if (circle2->rtti()==RS2::EntityArc) {
        circleCenter2 = (dynamic_cast<RS_Arc*>(circle2))->getCenter();
        circleRadius2 = (dynamic_cast<RS_Arc*>(circle2))->getRadius();
    }

    // create all possible tangents:
    RS_Line* poss[4];
    for (int i=0; i<4; ++i) {
        poss[i] = NULL;
    }

    RS_LineData d;

    double angle1 = circleCenter1.angleTo(circleCenter2);
    double dist1 = circleCenter1.distanceTo(circleCenter2);

    if (dist1>1.0e-6) {
        // outer tangents:
        double dist2 = circleRadius2 - circleRadius1;
        if (dist1>dist2) {
            double angle2 = asin(dist2/dist1);
            double angt1 = angle1 + angle2 + M_PI/2.0;
            double angt2 = angle1 - angle2 - M_PI/2.0;
            RS_Vector offs1;
            RS_Vector offs2;

            offs1.setPolar(circleRadius1, angt1);
            offs2.setPolar(circleRadius2, angt1);

            d = RS_LineData(circleCenter1 + offs1,
                            circleCenter2 + offs2);
            poss[0] = new RS_Line(NULL, d);


            offs1.setPolar(circleRadius1, angt2);
            offs2.setPolar(circleRadius2, angt2);

            d = RS_LineData(circleCenter1 + offs1,
                            circleCenter2 + offs2);
            poss[1] = new RS_Line(NULL, d);
        }

        // inner tangents:
        double dist3 = circleRadius2 + circleRadius1;
        if (dist1>dist3) {
            double angle3 = asin(dist3/dist1);
            double angt3 = angle1 + angle3 + M_PI/2.0;
            double angt4 = angle1 - angle3 - M_PI/2.0;
            RS_Vector offs1;
            RS_Vector offs2;

            offs1.setPolar(circleRadius1, angt3);
            offs2.setPolar(circleRadius2, angt3);

            d = RS_LineData(circleCenter1 - offs1,
                            circleCenter2 + offs2);
            poss[2] = new RS_Line(NULL, d);


            offs1.setPolar(circleRadius1, angt4);
            offs2.setPolar(circleRadius2, angt4);

            d = RS_LineData(circleCenter1 - offs1,
                            circleCenter2 + offs2);
            poss[3] = new RS_Line(NULL, d);
        }

    }

    // find closest tangent:
    double minDist = RS_MAXDOUBLE;
    double dist;
    int idx = -1;
    for (int i=0; i<4; ++i) {
        if (poss[i]!=NULL) {
            dist = poss[i]->getDistanceToPoint(coord);
            if (dist<minDist) {
                minDist = dist;
                idx = i;
            }
        }
    }

    if (idx!=-1) {
        RS_LineData d = poss[idx]->getData();
        for (int i=0; i<4; ++i) {
            if (poss[i]!=NULL) {
                delete poss[i];
            }
        }

        if (document!=NULL && handleUndo) {
            document->startUndoCycle();
        }

        ret = new RS_Line(container, d);
        ret->setLayerToActive();
        ret->setPenToActive();
        if (container!=NULL) {
            container->addEntity(ret);
        }
        if (document!=NULL && handleUndo) {
            document->addUndoable(ret);
            document->endUndoCycle();
        }
        if (graphicView!=NULL) {
            graphicView->createPainter();
            graphicView->drawEntity(ret);
            graphicView->restore();
        }
    } else {
        ret = NULL;
    }

    return ret;
}


/**
 * Creates a line with a relative angle to the given entity.
 *
 * @param coord Coordinate to define the point where the line should end.
 *              (typically a mouse coordinate).
 * @param entity Pointer to basis entity. The angle is relative to the 
 *               angle of this entity.
 * @param angle Angle of the line relative to the angle of the basis entity.
 * @param length Length of the line we're creating.
 */
RS_Line* RS_Creation::createLineRelAngle(const RS_Vector& coord,
        RS_Entity* entity,
        double angle,
        double length) {

    // check given entity / coord:
    if (entity==NULL || !coord.valid ||
            (entity->rtti()!=RS2::EntityArc && entity->rtti()!=RS2::EntityCircle
             && entity->rtti()!=RS2::EntityLine)) {

        return NULL;
    }

    double a1=0.0;

    switch (entity->rtti()) {
    case RS2::EntityLine:
        a1 = dynamic_cast<RS_Line*>(entity)->getAngle1();
        break;
    case RS2::EntityArc:
        a1 = dynamic_cast<RS_Arc*>(entity)->getCenter().angleTo(coord) + M_PI/2.0;
        break;
    case RS2::EntityCircle:
        a1 = dynamic_cast<RS_Circle*>(entity)->getCenter().angleTo(coord) + M_PI/2.0;
        break;
    default:
        // never reached
        break;
    }

    a1 += angle;

    RS_Vector v1;
    v1.setPolar(length/2.0, a1);
    //RS_ConstructionLineData(coord-v1, coord+v1);
    RS_LineData d(coord-v1, coord+v1);
    RS_Line* ret;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    ret = new RS_Line(container, d);
    ret->setLayerToActive();
    ret->setPenToActive();
    if (container!=NULL) {
        container->addEntity(ret);
    }
    if (document!=NULL && handleUndo) {
        document->addUndoable(ret);
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
        graphicView->drawEntity(ret);
        graphicView->restore();
    }

    return ret;
}


/**
 * Creates a polygon with 'number' edges.
 *
 * @param center Center of the polygon.
 * @param corner The first corner of the polygon
 * @param number Number of edges / corners.
 */
RS_Line* RS_Creation::createPolygon(const RS_Vector& center,
                                    const RS_Vector& corner,
                                    int number) {

    // check given coords / number:
    if (!center.valid || !corner.valid || number<3) {
        return NULL;
    }

    RS_Line* ret = NULL;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    RS_Vector c1(false);
    RS_Vector c2 = corner;
    RS_Line* line;

    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    for (int n=1; n<=number; ++n) {
        c1 = c2;
        c2 = c2.rotate(center, (M_PI*2)/number);

        line = new RS_Line(container, RS_LineData(c1, c2));
        line->setLayerToActive();
        line->setPenToActive();

        if (ret==NULL) {
            ret = line;
        }

        if (container!=NULL) {
            container->addEntity(line);
        }
        if (document!=NULL && handleUndo) {
            document->addUndoable(line);
        }
        if (graphicView!=NULL) {
            graphicView->drawEntity(line);
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    return ret;
}



/**
 * Creates a polygon with 'number' edges.
 *
 * @param corner1 The first corner of the polygon.
 * @param corner2 The second corner of the polygon.
 * @param number Number of edges / corners.
 */
RS_Line* RS_Creation::createPolygon2(const RS_Vector& corner1,
                                     const RS_Vector& corner2,
                                     int number) {

    // check given coords / number:
    if (!corner1.valid || !corner2.valid || number<3) {
        return NULL;
    }

    RS_Line* ret = NULL;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    double len = corner1.distanceTo(corner2);
    double ang1 = corner1.angleTo(corner2);
    double ang = ang1;

    RS_Vector c1(false);
    RS_Vector c2 = corner1;
    RS_Vector edge;
    RS_Line* line;
    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    for (int n=1; n<=number; ++n) {
        c1 = c2;
        edge.setPolar(len, ang);
        c2 = c1 + edge;

        line = new RS_Line(container, RS_LineData(c1, c2));
        line->setLayerToActive();
        line->setPenToActive();

        if (ret==NULL) {
            ret = line;
        }

        if (container!=NULL) {
            container->addEntity(line);
        }
        if (document!=NULL && handleUndo) {
            document->addUndoable(line);
        }
        if (graphicView!=NULL) {
            graphicView->drawEntity(line);
        }

        // more accurate than incrementing the angle:
        ang = ang1 + (2*M_PI)/number*n;
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    return ret;
}



/**
 * Creates an insert with the given data.
 *
 * @param data Insert data (position, block name, ..)
 */
RS_Insert* RS_Creation::createInsert(RS_InsertData& data) {

    RS_DEBUG->print("RS_Creation::createInsert");

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    RS_Insert* ins = new RS_Insert(container, data);
    // inserts are also on layers
    ins->setLayerToActive();
    ins->setPenToActive();

    if (container!=NULL) {
        container->addEntity(ins);
    }
    if (document!=NULL && handleUndo) {
        document->addUndoable(ins);
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
        graphicView->drawEntity(ins);
        graphicView->restore();
    }

    RS_DEBUG->print("RS_Creation::createInsert: OK");

    return ins;
}



/**
 * Creates an image with the given data.
 */
RS_Image* RS_Creation::createImage(RS_ImageData& data) {

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    RS_Image* img = new RS_Image(container, data);
    img->setLayerToActive();
    img->setPenToActive();
    img->update();

    if (container!=NULL) {
        container->addEntity(img);
    }
    if (document!=NULL && handleUndo) {
        document->addUndoable(img);
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
        graphicView->drawEntity(img);
        graphicView->restore();
    }

    return img;
}


/**
 * Creates a new block from the currently selected entitiies.
 *
 * @param referencePoint Reference point for the block.
 * @param name Block name
 * @param remove true: remove existing entities, false: don't touch entities
 */
RS_Block* RS_Creation::createBlock(const RS_BlockData& data,
                                   const RS_Vector& referencePoint,
                                   const bool remove
                                      ) {

    // start undo cycle for the container if we're deleting the existing entities
    if (remove && document!=NULL) {
        document->startUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    RS_Block* block =
        new RS_Block(container,
                     RS_BlockData(data.name, data.basePoint, data.frozen));

    // copy entities into a block
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        //for (uint i=0; i<container->count(); ++i) {
        //RS_Entity* e = container->entityAt(i);

        if (e!=NULL && e->isSelected()) {

            // delete / redraw entity in graphic view:
            if (remove) {
                if (graphicView!=NULL) {
                    graphicView->deleteEntity(e);
                }
                e->setSelected(false);
            }
            else {
                if (graphicView!=NULL) {
                    graphicView->deleteEntity(e);
                }
                e->setSelected(false);
                if (graphicView!=NULL) {
                    graphicView->drawEntity(e);
                }
            }

            // add entity to block:
            RS_Entity* c = e->clone();
            c->move(-referencePoint);
            block->addEntity(c);

            if (remove) {
                //container->removeEntity(e);
                //i=0;
                e->changeUndoState();
                if (document!=NULL) {
                    document->addUndoable(e);
                }
            }
        }
    }
    if (graphicView!=NULL) {
        graphicView->restore();
    }

    if (remove && document!=NULL) {
        document->endUndoCycle();
    }

    if (graphic!=NULL) {
        graphic->addBlock(block);
    }

    return block;
}



/**
 * Inserts a library item from the given path into the drawing.
 */
RS_Insert* RS_Creation::createLibraryInsert(const RS_LibraryInsertData& data) {

    RS_DEBUG->print("RS_Creation::createLibraryInsert");

    RS_Graphic g;
    if (!g.open(data.file, RS2::FormatUnknown)) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Creation::createLibraryInsert: Cannot open file: %s");
        return NULL;
    }

    // unit conversion:
    if (graphic!=NULL) {
        double uf = RS_Units::convert(1.0, g.getUnit(),
                                      graphic->getUnit());
        g.scale(RS_Vector(0.0, 0.0), RS_Vector(uf, uf));
    }

    RS_String s;
    s = RS_FileInfo(data.file).baseName();

    RS_Modification m(*container, graphicView);
    m.paste(
        RS_PasteData(
            data.insertionPoint,
            data.factor, data.angle, true,
            s, false, false, false),
        &g);

    RS_DEBUG->print("RS_Creation::createLibraryInsert: OK");

    return NULL;
}



/**
 * Creates an equidistant to an existing polyline.
 *
 * @param side Side of equidistant (left or right).
 * @param distance Distance of the equidistant polyline to the 
 *                 original one.
 * @param number Number of equidistants.
 * @param pl Original polyline.
 *
 * @return Pointer to the first created polyline or NULL if no 
 *    polyline has been created.
 */
RS_Polyline* RS_Creation::createEquidistant(RS2::Side side,
        double distance,
        int number,
        RS_Polyline* pl,
        bool useArcsAtCorners) {

    RS_DEBUG->print("RS_Creation::createEquidistant");

    if (pl==NULL) {
        return NULL;
    }

    bool isClosed = pl->isClosed() ||
                    pl->getStartpoint().distanceTo(pl->getEndpoint())<RS_TOLERANCE;
    
    RS_DEBUG->print("RS_Creation::createEquidistant: isClosed: %d", (int)isClosed);

    bool isTriangle = pl->count()==3 && isClosed;

    RS_DEBUG->print("RS_Creation::createEquidistant: prepare");
    RS_EntityContainer* p;
    p = preparePolylineForEquidistant(pl);

    if (p==NULL) {
        return NULL;
    }

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->createPainter();
    }

    RS_Polyline* firstEquidistant = NULL;

    for (int k=1; k<=number; ++k) {
        double dist = distance*k;

        // intersection of first with last parallel:
        RS_Vector firstIntersection(false);
        RS_EntityContainer parallels(NULL, false);
        RS_Polyline* equidistant = new RS_Polyline(container, RS_PolylineData());
        if (k==1) {
            firstEquidistant = equidistant;
        }
        bool first = true;
        RS_AtomicEntity* ori[3];   // current entity and next entity of the original
        RS_AtomicEntity* para[3];  // parallel for current entity and next entity
        para[0] = NULL;
        para[1] = NULL;
        para[2] = NULL;
        RS_DEBUG->print("RS_Creation::createEquidistant: find first entity with parallel");
        RS_AtomicEntity* firstOri = findEntityWithParallel(
                                        p, p->firstEntity(), side, dist);
        RS_DEBUG->print("RS_Creation::createEquidistant: find last entity with parallel");
        RS_AtomicEntity* lastOri = findReverseEntityWithParallel(
                                       p, p->lastEntity(), side, dist);

        //RS_DEBUG->print("RS_Creation::createEquidistant: firstOri:");
        //RS_DEBUG->printEntity(firstOri);
        //RS_DEBUG->print("RS_Creation::createEquidistant: lastOri:");
        //RS_DEBUG->printEntity(lastOri);

        p->findEntity(firstOri);
        ori[0] = firstOri;

        /*
        RS_DEBUG->print("ori[0]:");
        RS_DEBUG->printEntity(ori[0]);

        RS_DEBUG->print("current:");
        RS_DEBUG->printEntity(p->currentEntity());
        */

        do {
            /*
            RS_DEBUG->print("\n\nprocessing entity (ori[0]):");
            RS_DEBUG->printEntity(ori[0]);
            */
                                
            bool isCorner = false;
            if (ori[0]!=NULL && ori[0]->rtti()==RS2::EntityArc) {
                isCorner = (fabs(((RS_Arc*)ori[0])->getData().radius) < 1.0e-6);
                RS_DEBUG->print("RS_Creation::createEquidistant: "
                    "isCorner: %d (radius: %f)", 
                    (int)isCorner, fabs(((RS_Arc*)ori[0])->getData().radius));
            }

            RS_DEBUG->print("RS_Creation::createEquidistant: ori[1]: find next entity with parallel");
            ori[1] = findEntityWithParallel(p, p->nextEntity(), side, dist);

            /*
            RS_DEBUG->print("ori[1]:");
            RS_DEBUG->printEntity(ori[1]);
            */

            //RS_DEBUG->print("ori[2]: find next entity with parallel");
            ori[2] = findEntityWithParallel(p, p->nextEntity(), side, dist);
            //RS_DEBUG->print("ori[2]:");
            //RS_DEBUG->printEntity(ori[2]);
            if (ori[2]!=NULL) {
                p->prevEntity();
            }
            bool dropped = false;

            // create parallels for current and next entity of original:
            for (int i=0; i<3; ++i) {
                if (para[i]!=NULL) {
                    delete para[i];
                    para[i] = NULL;
                }

                if (ori[i]!=NULL) {
                    RS_DEBUG->print(RS_Debug::D_TESTING,
                                    "creating parallel %d..", i);
                    para[i] = createEquidistantParallel(
                                  ori[i], side, dist);
                }
            }


            if (para[0]!=NULL) {
                // begin equidistant:
                if (first) {
                    // check intersection of first segment with last one
                    RS_AtomicEntity* lastPara = createEquidistantParallel(
                                                    lastOri, side, dist);

                    if (lastPara!=NULL) {
                        RS_VectorList vs;
                        vs = RS_Information::getIntersection(para[0], lastPara, true);
                        if (vs.hasValid() && isClosed) {
                            firstIntersection = vs.get(0);
                            RS_DEBUG->print(RS_Debug::D_TESTING,
                                            "creating 1st point of equidistant (intersection with last): %f/%f",
                                            firstIntersection.x, firstIntersection.y);
                            equidistant->addVertex(firstIntersection);
                        } else {
                            RS_DEBUG->print(RS_Debug::D_TESTING,
                                            "creating 1st point of equidistant: %f/%f",
                                            para[0]->getStartpoint().x, para[0]->getStartpoint().y);
                            equidistant->addVertex(para[0]->getStartpoint());
                        }
                        first = false;
                        delete lastPara;
                    }
                }

                // extend equidistant...
                if (para[1]!=NULL) {
                    RS_VectorList vs, vs1, vs2;
                    vs1 = RS_Information::getIntersection(para[0], para[1], true);
                    if (para[2]!=NULL && !isTriangle) {
                        vs2 = RS_Information::getIntersection(para[0], para[2], true);
                    }
                    RS_Vector closest1 = vs1.getClosest(equidistant->getEndpoint());
                    RS_Vector closest2 = vs2.getClosest(equidistant->getEndpoint());
                    vs = RS_VectorList(closest1, closest2);

                    // ... to next intersection:
                    if (vs.hasValid()) {
                        int closestIdx = -1;
                        RS_Vector is = vs.getClosest(equidistant->getEndpoint(),
                                                     NULL, &closestIdx);

                        RS_DEBUG->print(RS_Debug::D_TESTING,
                                        "closest intersection: idx: %d: %f/%f",
                                        closestIdx, is.x, is.y);

                        // drop next entity because the entity after the next
                        //  one intersects closer with the current entity.
                        if (closestIdx==1 ||
                                closest1.distanceTo(closest2)<RS_TOLERANCE) {

                            RS_DEBUG->print(RS_Debug::D_TESTING,
                                            "dropping entity %d (closer intersection found) "
                                            "and moving on to the "
                                            "one after the next", ori[1]->getId());
                            dropped = true;
                        }

                        if (equidistant->getEndpoint().distanceTo(is)
                                > RS_TOLERANCE) {

                            RS_DEBUG->print(RS_Debug::D_TESTING,
                                            "adding point to equidistant: %f/%f",
                                            is.x, is.y);
                            double bulge = 0.0;
                            if (para[0]->rtti()==RS2::EntityArc) {
                                RS_ArcData arcData = ((RS_Arc*)para[0])->getData();
                                arcData.angle1 = arcData.center.angleTo(equidistant->getEndpoint());
                                arcData.angle2 = arcData.center.angleTo(is);
                                RS_Arc arc(NULL, arcData);
                                bulge = arc.getBulge();
                            }
                            extendEquidistant(equidistant, 
                                is, 
                                bulge, 
                                isCorner && !useArcsAtCorners);
                        } else {
                            RS_DEBUG->print(RS_Debug::D_TESTING,
                                            "dropping entity %d (would add zero entity) "
                                            "and moving on to the "
                                            "one after the next", ori[1]->getId());
                        }
                    }

                    // ... to end of entity and around corner (arc):
                    else {
                        RS_DEBUG->print(RS_Debug::D_TESTING,
                                        "adding round corner to equidistant: %f/%f - %f/%f",
                                        para[0]->getEndpoint().x, para[0]->getEndpoint().y,
                                        para[1]->getStartpoint().x, para[1]->getStartpoint().y);
                        double bulge = 0.0;
                        if (para[0]->rtti()==RS2::EntityArc) {
                            RS_ArcData arcData = ((RS_Arc*)para[0])->getData();
                            arcData.angle1 = arcData.center.angleTo(equidistant->getEndpoint());
                            RS_Arc arc(NULL, arcData);
                            bulge = arc.getBulge();
                        }
                        extendEquidistant(equidistant, 
                            para[0]->getEndpoint(), 
                            bulge, 
                            isCorner && !useArcsAtCorners);
                    }
                }

                // para[1] is NULL, end equidistant:
                else {
                    RS_DEBUG->print("handle end of equidistant");
                    double bulge = 0.0;
                    if (para[0]->rtti()==RS2::EntityArc) {
                        RS_DEBUG->print("handle end of equidistant: arc");
                        RS_ArcData arcData = ((RS_Arc*)para[0])->getData();
                        arcData.angle1 = arcData.center.angleTo(equidistant->getEndpoint());
                        if (isClosed && firstIntersection.valid) {
                            arcData.angle2 = arcData.center.angleTo(firstIntersection);
                        }
                        RS_Arc arc(NULL, arcData);
                        bulge = arc.getBulge();
                    }
                    
                    if (isClosed && firstIntersection.valid) {
                        RS_DEBUG->print("handle end of equidistant: close to first intersection");
                        RS_Vector lastVertex = firstIntersection;
                        if (!useArcsAtCorners) {
                            lastVertex = equidistant->getStartpoint();
                        }
                        extendEquidistant(equidistant, 
                            lastVertex, 
                            bulge,
                            isCorner && !useArcsAtCorners);
                    }
                    else {
                        RS_DEBUG->print("handle end of equidistant: extend to end of parallel");
                        extendEquidistant(equidistant, 
                            para[0]->getEndpoint(), 
                            bulge,
                            isCorner && !useArcsAtCorners);
                    }
                }
            }

            if (!dropped) {
                ori[0] = ori[1];
            } else {
                ori[0] = ori[2];
                p->nextEntity();
            }
        } while(ori[0]!=NULL);

        RS_DEBUG->print(RS_Debug::D_TESTING,
                        "iteration done");

        RS_DEBUG->print(RS_Debug::D_TESTING,
                        "cleaning up..");
        for (int i=0; i<2; ++i) {
            if (para[i]!=NULL) {
                delete para[i];
                para[i] = NULL;
            }
        }
        RS_DEBUG->print(RS_Debug::D_TESTING,
                        "cleaning up: OK");

        if (container!=NULL) {
            container->addEntity(equidistant);
        }


        if (document!=NULL && handleUndo) {
            document->addUndoable(equidistant);
        }


        if (graphicView!=NULL) {
            graphicView->drawEntity(equidistant);
        }
    }

    delete p;
    p = NULL;

    if (graphicView!=NULL) {
        graphicView->restore();
    }

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }


    RS_DEBUG->print(RS_Debug::D_TESTING,
                    "returning");

    // TODO: cut equidistant where it intersects with itself

    return firstEquidistant;
}



/**
 * Creates an equidistant to an existing polyline.
 *
 * @param coord Coordinate to define on which side the equidistant
 *              should be created (typically a mouse coordinate).
 * @param distance Distance of the equidistant polyline to the 
 *                 original one.
 * @param number Number of equidistants.
 * @param pl Original polyline.
 *
 * @return Pointer to the first created polyline or NULL if no 
 *    polyline has been created.
 */
RS_Polyline* RS_Creation::createEquidistant(const RS_Vector& coord,
        double distance,
        int number,
        RS_Polyline* pl,
        bool useArcsAtCorners) {

    RS_DEBUG->print("RS_Creation::createEquidistant");

    if (pl==NULL) {
        return NULL;
    }

    // find entity closest to the given coordinate to determine
    //  on which side the equidistant will be:
    RS_Entity* closest = pl->getNearestEntity(coord);
    if (closest==NULL) {
        return NULL;
    }

    RS2::Side side = closest->getSideOfPoint(coord);
    return createEquidistant(side, distance, number, pl, useArcsAtCorners);
}



/**
 * Prepares the given polyline for creating an equidistant. Arcs segments 
 * with zero radius are added at the corners. These segments allow an easy 
 * expansion for equidistant polylines.
 */
RS_EntityContainer* RS_Creation::preparePolylineForEquidistant(
    RS_Polyline* pl) {

    RS_DEBUG->print("RS_Creation::preparePolylineForEquidistant");

    if (pl==NULL) {
        return NULL;
    }

    bool isClosed = pl->isClosed() ||
                    pl->getStartpoint().distanceTo(pl->getEndpoint())<RS_TOLERANCE;
    
    RS_DEBUG->print("RS_Creation::preparePolylineForEquidistant: "
        "isClosed: %d", (int)isClosed);

    RS_EntityContainer* ret = new RS_EntityContainer(pl->getParentContainer());

    RS_AtomicEntity* last = NULL;
    if (isClosed && pl->lastEntity()!=NULL && pl->lastEntity()->isAtomic()) {
        last = dynamic_cast<RS_AtomicEntity*>(pl->lastEntity());
    }

    for (RS_Entity* e = pl->firstEntity(); e!=NULL; e = pl->nextEntity()) {
        if (e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            if (last!=NULL) {
                double lastDir =
                    RS_Math::correctAngle(last->getDirection2() + M_PI);
                double thisDir = ae->getDirection1();

                RS_DEBUG->print("  lastDir: %f", lastDir);
                RS_DEBUG->print("  thisDir: %f", thisDir);

                // check if we have a corner that is not smooth here:
                if (fabs(lastDir-thisDir) > 1.0e-4) {
                    RS_DEBUG->print("  corner..");

                    RS2::Side s = last->getSideOfPoint(ae->getNearestDist(ae->getLength()/100.0, true));

                    bool rev;
                    double deltaAngle;
                    if (s==RS2::RightHand) {
                        deltaAngle = M_PI/2;
                        rev=true;
                    } else {
                        deltaAngle = -M_PI/2;
                        rev=false;
                    }
                    RS_DEBUG->print("  rev: %d", (bool)rev);
                    RS_DEBUG->print("  deltaAngle: %f", deltaAngle);

                    // create zero radius arc:
                    RS_Arc* a = new RS_Arc(ret,
                                           RS_ArcData(ae->getStartpoint(),
                                                      0.0,
                                                      RS_Math::correctAngle(lastDir+deltaAngle),
                                                      RS_Math::correctAngle(thisDir+deltaAngle),
                                                      rev));

                    RS_DEBUG->print("  a->getDirection1(): %f", a->getDirection1());
                    RS_DEBUG->print(RS_Debug::D_TESTING,
                                    "  RS_Math::getAngleDifference(a->getDirection1(), lastDir): %f",
                                    RS_Math::getAngleDifference(a->getDirection1(), lastDir));

                    ret->addEntity(a);

                    //RS_DEBUG->printEntity(a, "  ");
                }
            }

            last = ae;
            ret->addEntity(e->clone());
        }
    }

    RS_DEBUG->print("RS_Creation::preparePolylineForEquidistant: OK");

    return ret;
}



/**
 * Helper function for equidistant creation.
 */
RS_AtomicEntity* RS_Creation::createEquidistantParallel(RS_AtomicEntity* ori,
        RS2::Side side, double distance) {
    RS_AtomicEntity* para = NULL;

    RS_DEBUG->print("  RS_Creation::createEquidistantParallel");

    if (ori!=NULL) {
        switch (ori->rtti()) {
        case RS2::EntityLine: {
                RS_Line* l = dynamic_cast<RS_Line*>(ori);
                //RS_DEBUG->printEntity(l,
                //                      "    creating parallel for line: ");

                double sideAngle = l->getDirection1();
                if (side==RS2::RightHand) {
                    sideAngle-=M_PI/2;
                } else {
                    sideAngle+=M_PI/2;
                }
                sideAngle = RS_Math::correctAngle(sideAngle);
                RS_Vector sideVector;
                sideVector.setPolar(distance, sideAngle);

                para = new RS_Line(NULL,
                                   RS_LineData(l->getStartpoint()+sideVector,
                                               l->getEndpoint()+sideVector));

                RS_DEBUG->print(RS_Debug::D_TESTING,
                                "    parallel %f/%f - %f/%f",
                                para->getStartpoint().x, para->getStartpoint().y,
                                para->getEndpoint().x, para->getEndpoint().y);

            }
            break;

        case RS2::EntityArc: {
                RS_Arc* a = (RS_Arc*)ori;
                //RS_DEBUG->printEntity(a,
                //                      "    creating parallel for arc: ");

                RS_ArcData paraData = a->getData();
                if ((side==RS2::RightHand && !a->isReversed()) ||
                        (side==RS2::LeftHand && a->isReversed()) ) {
                    paraData.radius+=distance;
                } else {
                    paraData.radius-=distance;
                }
                //if (fabs(paraData.radius)>1.0e-6) {
                // note that the radius might be negative, something we don't want
                // and avoid here:
                if (paraData.radius>1.0e-6) {
                    para = new RS_Arc(NULL, paraData);
                    ((RS_Arc*)para)->calculateEndpoints();
                    // r5194:
                    if (para->getStartpoint().distanceTo(para->getEndpoint())<1.0e-5) {
                        delete para;
                        para = NULL;
                    }
                }
            }
            break;

        default:
            break;
        }
    }

    //RS_DEBUG->print("    parallel is:");
    //RS_DEBUG->printEntity(para, "    ");

    return para;
}



/**
 * Helper function for equidistant creation. 
 * Extends the given equidistant to the given position with the 
 * given bulge. No extension is done if the segment to add would
 * have zero length.
 *
 * @param sharpCornerInsteadOfArc: Form sharp corner instead of creating
 *  an arc with the given bulge.
 */
void RS_Creation::extendEquidistant(RS_Polyline* ed,
                                    const RS_Vector& pos,
                                    double bulge,
                                    bool sharpCornerInsteadOfArc) {

    RS_DEBUG->print(RS_Debug::D_TESTING,
                    "RS_Creation::extendEquidistant: bulge: %f", bulge);
    RS_DEBUG->print("RS_Creation::extendEquidistant: "
        "sharpCornerInsteadOfArc: %d", (int)sharpCornerInsteadOfArc);

    if (ed==NULL) {
        return;
    }
        
    if (ed->getEndpoint().distanceTo(pos)<RS_TOLERANCE ||
        fabs(bulge)>1.0e6) {

        return;
    }
    
    if (!sharpCornerInsteadOfArc) {
        ed->setNextBulge(bulge);
        ed->addVertex(pos);
    }
    else {
        // if we were to create an arc around the corner, it would look like this:
        RS_Arc arc(NULL, RS_ArcData());
        arc.createFrom2PBulge(ed->getEndpoint(), pos, bulge);

        // direction at endpoints (tangents):
        double direction1 = arc.getDirection1();
        double direction2 = arc.getDirection2();
        RS_Vector vDirection1;
        RS_Vector vDirection2;
        vDirection1.setPolar(arc.getRadius()*2, direction1);
        vDirection2.setPolar(arc.getRadius()*2, direction2);

        // create tangent lines:
        RS_Line line1(NULL, 
            RS_LineData(ed->getEndpoint(), 
                ed->getEndpoint()+vDirection1));
        RS_Line line2(NULL, 
            RS_LineData(pos, 
                pos+vDirection2));
        
        // intersection of tangents:
        RS_VectorList sp = RS_Information::getIntersectionLineLine(&line1, &line2);
        if (sp.count()>0) {
            RS_DEBUG->print("RS_Creation::extendEquidistant: "
                "intersection of tangents: %f/%f", 
                sp.get(0).x, sp.get(0).y);

            // 20080315:
            if (ed->lastEntity()!=NULL && ed->lastEntity()->rtti()==RS2::EntityLine) {
                ed->removeLastVertex();
            }
            ed->setNextBulge(0.0);
            ed->addVertex(sp.at(0));
        }
        else {
            ed->setNextBulge(bulge);
            ed->addVertex(pos);
        }
    }
}



/**
 * Finds the next entity in the given polyline that has a valid
 * parallel with the given parameters. Search starts with
 * the given start entity (inclusive).
 */
RS_AtomicEntity* RS_Creation::findEntityWithParallel(
    RS_EntityContainer* pl,
    RS_Entity* start,
    RS2::Side side, double distance,
    bool reversed) {

    RS_AtomicEntity* ret = NULL;

    if (pl==NULL || start==NULL) {
        return ret;
    }

    RS_DEBUG->print("RS_Creation::findEntityWithParallel");
    //RS_DEBUG->print("  start:");
    //RS_DEBUG->printEntity(start, "  ");

    pl->findEntity(start);
    RS_Entity* current = pl->currentEntity();

    RS_AtomicEntity* para = NULL;
    bool done = false;
    do {
        //RS_DEBUG->print("  current:");
        //RS_DEBUG->printEntity(current, "  ");

        if (current->isAtomic()) {
            para = createEquidistantParallel(
                       dynamic_cast<RS_AtomicEntity*>(current),
                       side, distance);
            if (para!=NULL) {
                delete para;
                ret = dynamic_cast<RS_AtomicEntity*>(current);
                RS_DEBUG->print("  entity found");
                done = true;
            }
        }
        if (!done) {
            if (!reversed) {
                current = pl->nextEntity();
            } else {
                current = pl->prevEntity();
            }
        }
    } while(!done && current!=NULL);

    return ret;
}



/**
 * Finds the next entity in the given polyline that has a valid
 * parallel with the given parameters. Search starts with
 * the given start entity (inclusive).
 */
RS_AtomicEntity* RS_Creation::findReverseEntityWithParallel(
    RS_EntityContainer* pl,
    RS_Entity* start,
    RS2::Side side, double distance) {

    return findEntityWithParallel(pl, start, side, distance, true);
}
