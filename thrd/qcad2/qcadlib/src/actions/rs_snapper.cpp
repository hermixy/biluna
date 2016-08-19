/****************************************************************************
** $Id: rs_snapper.cpp 9192 2008-02-24 18:29:47Z andrew $
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


#include "rs_snapper.h"

#include "rs_dialogfactory.h"
#include "rs_entitycontainer.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_grid.h"
#include "rs_information.h"
#include "rs_mouseevent.h"
#include "rs_painter.h"
#include "rs_settings.h"


RS_Pen RS_Snapper::snapperPen =
    RS_Pen(RS_Color(255,194,0),
           RS2::Width00,
           RS2::SolidLine);

/**
 * Constructor.
 */
RS_Snapper::RS_Snapper(RS_EntityContainer& container,
                       RS_GraphicView& graphicView) {
    this->container = &container;
    this->graphicView = &graphicView;
    finished = false;
    initSnapper();
}



/**
 * Destructor.
 */
RS_Snapper::~RS_Snapper() {}



/**
 * Initialize (called by all constructors)
 */
void RS_Snapper::initSnapper() {
    snapMode = graphicView->getDefaultSnapMode();
    snapRes = graphicView->getSnapRestriction();
    snapStatus = RS2::ActionSnapFree;
    keyEntity = NULL;
    snapSpot = RS_Vector(false);
    snapCoord = RS_Vector(false);
    visible = false;
    distance = 1.0;
    coordinate = RS_Vector(0,0);
    RS_SETTINGS->beginGroup("/Snap");
    snapRange = RS_SETTINGS->readNumEntry("/Range", 15);
    showAutoSnapAid = (bool)RS_SETTINGS->readNumEntry("/ShowAutoSnapAid", 1);
    RS_SETTINGS->endGroup();
    RS_SETTINGS->beginGroup("/Appearance");
    showCrosshairs = (bool)RS_SETTINGS->readNumEntry("/ShowCrosshairs", 1);
    RS_SETTINGS->endGroup();
    if (snapRange<2) {
        snapRange = 20;
    }
}

void RS_Snapper::finish() {
    hideSnapOptions();
    finished = true;
}


/**
 * Snap to a coordinate in the drawing using the current snap mode.
 *
 * @param e A mouse event.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapPoint(RS_MouseEvent* e) {
    snapSpot = RS_Vector(false);

    if (e==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Snapper::snapPoint: event is NULL");
        return snapSpot;
    }

    return snapPoint(e->x(), e->y());
}



/**
 * Snap to a coordinate in the drawing using the current snap mode.
 *
 * @param e A mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapPoint(int x, int y) {
    RS_DEBUG->print("RS_Snapper::snapPoint");

    snapSpot = RS_Vector(false);

    RS_Vector mouseCoord = graphicView->toGraph(x, y);
    RS_DEBUG->print("RS_Snapper::snapPoint: mouse coord is: %d/%d",
                    x, y);

    switch (snapMode) {

    case RS2::ActionSnapFree:
        snapSpot = snapFree(mouseCoord);
        break;

    case RS2::ActionSnapEndpoint:
        snapSpot = snapEndpoint(mouseCoord);
        break;

    case RS2::ActionSnapGrid:
        snapSpot = snapGrid(mouseCoord);
        break;

    case RS2::ActionSnapOnEntity:
        snapSpot = snapOnEntity(mouseCoord);
        break;

    case RS2::ActionSnapCenter:
        snapSpot = snapCenter(mouseCoord);
        break;

    case RS2::ActionSnapMiddle:
        snapSpot = snapMiddle(mouseCoord);
        break;

    case RS2::ActionSnapDist:
        snapSpot = snapDist(mouseCoord);
        break;

    case RS2::ActionSnapIntersection:
        snapSpot = snapIntersection(mouseCoord);
        break;
    
    case RS2::ActionSnapReference:
        snapSpot = snapReference(mouseCoord);
        break;
    
    case RS2::ActionSnapCoordinate:
        snapSpot = snapCoordinate();
        break;
    
    case RS2::ActionSnapCoordinatePolar:
        snapSpot = snapCoordinatePolar();
        break;
    
    case RS2::ActionSnapAuto:
        snapSpot = snapAuto(mouseCoord);
        break;

    default:
        break;
    }

    // handle snap restrictions that can be activated in addition
    //   to the ones above:
    switch (snapRes) {
    case RS2::ActionRestrictOrthogonal:
        snapCoord = restrictOrthogonal(snapSpot);
        break;
    case RS2::ActionRestrictHorizontal:
        snapCoord = restrictHorizontal(snapSpot);
        break;
    case RS2::ActionRestrictVertical:
        snapCoord = restrictVertical(snapSpot);
        break;
    default:
    case RS2::ActionRestrictNothing:
        snapCoord = snapSpot;
        break;
    }

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateCoordinateWidget(snapCoord,
                snapCoord - graphicView->getRelativeZero());
    }

    RS_DEBUG->print("RS_Snapper::snapPoint: OK");

    return snapCoord;
}



/**
 * Snaps to a free coordinate.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapFree(const RS_Vector& coord) {
    keyEntity = NULL;
    return coord;
}



/**
 * Snaps to the closest endpoint.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapEndpoint(const RS_Vector& coord) {
    RS_Vector vec(false);

    vec = container->getNearestEndpoint(coord,
                                        NULL/*, &keyEntity*/);
    return vec;
}



/**
 * Snaps to a grid point.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapGrid(const RS_Vector& coord) {

    RS_DEBUG->print("RS_Snapper::snapGrid begin");

    RS_Vector vec(false);
    double dist=0.0;

    RS_Grid* grid = graphicView->getGrid();

    if (grid!=NULL) {
        RS_Vector* pts = grid->getPoints();
        int closest = -1;
        dist = 32000.00;
        for (int i=0; i<grid->count(); ++i) {
            double d = pts[i].distanceTo(coord);
            if (d<dist) {
                closest = i;
                dist = d;
            }
        }
        if (closest>=0) {
            vec = pts[closest];
        }
    }
    keyEntity = NULL;

    RS_DEBUG->print("RS_Snapper::snapGrid end");

    return vec;
}



/**
 * Snaps to a point on an entity.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapOnEntity(const RS_Vector& coord) {

    RS_Vector vec(false);
    vec = container->getNearestPointOnEntity(coord, true, NULL, &keyEntity);
    return vec;
}



/**
 * Snaps to the closest center.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapCenter(const RS_Vector& coord) {
    RS_Vector vec(false);

    vec = container->getNearestCenter(coord, NULL);
    return vec;
}



/**
 * Snaps to the closest middle.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapMiddle(const RS_Vector& coord) {
    RS_Vector vec(false);

    vec = container->getNearestMiddle(coord, NULL);
    return vec;
}



/**
 * Snaps to the closest point with a given distance to the endpoint.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapDist(const RS_Vector& coord) {
    RS_Vector vec(false);

    vec = container->getNearestDist(distance,
                                    coord,
                                    NULL);
    return vec;
}



/**
 * Snaps to the closest intersection point.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapIntersection(const RS_Vector& coord) {
    RS_Vector vec(false);
    vec = container->getNearestIntersection(coord, NULL);
    return vec;
}



/**
 * Snaps to the closest entity reference point.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapReference(const RS_Vector& coord) {
    RS_Vector vec(false);
    vec = container->getNearestRef(coord, NULL);
    return vec;
}



/**
 * Snaps to a user defined coordinate independent of the mouse position.
 *
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapCoordinate() {
    return coordinate;
}


/**
 * Snaps to a user defined polar coordinate independent of the mouse 
 * position.
 *
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_Snapper::snapCoordinatePolar() {
    return coordinate;
}



/**
 * Snaps to the closest reference point or nothing.
 */
RS_Vector RS_Snapper::snapAuto(const RS_Vector& coord) {
    RS_Vector ret;
    /*
    RS_VectorList candidates1;
    RS_VectorList candidates2;
    */
    double maxDist = graphicView->toGraphDX(snapRange);

    ret = snapIntersection(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapIntersection;
        return ret;
    }
    
    ret = snapEndpoint(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapEndpoint;
        return ret;
    }
    
    ret = snapMiddle(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapMiddle;
        return ret;
    }
    
    ret = snapReference(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapReference;
        return ret;
    }
    
    ret = snapGrid(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapGrid;
        return ret;
    }
    
    ret = snapOnEntity(coord);
    if (ret.distanceTo(coord) < maxDist) {
        snapStatus = RS2::ActionSnapOnEntity;
        return ret;
    }

    snapStatus = RS2::ActionSnapFree;
    return coord;

    /*
    double minDist = RS_MAXDOUBLE;
    for (int i=0; i<candidates1.count(); ++i) {
        double dist = candidates1.get(i).distanceTo(coord);
        if (dist < minDist && dist < graphicView->toGraphDX(snapRange)) {
            minDist = dist;
            ret = candidates1.get(i);
        }
    }
    if (ret.valid) {
        return ret;
    }
    
    for (int i=0; i<candidates2.count(); ++i) {
        double dist = candidates2.get(i).distanceTo(coord);
        if (dist < minDist && dist < graphicView->toGraphDX(snapRange)) {
            minDist = dist;
            ret = candidates2.get(i);
        }
    }
    */
}



/**
 * 'Corrects' the given coordinates to 0, 90, 180, 270 degrees relative to
 * the current relative zero point.
 *
 * @param coord The uncorrected coordinates.
 * @return The corrected coordinates.
 */
RS_Vector RS_Snapper::restrictOrthogonal(const RS_Vector& coord) {
    RS_Vector rz = graphicView->getRelativeZero();
    RS_Vector ret(coord);

    RS_Vector retx = RS_Vector(rz.x, ret.y);
    RS_Vector rety = RS_Vector(ret.x, rz.y);

    if (retx.distanceTo(ret) > rety.distanceTo(ret)) {
        ret = rety;
    } else {
        ret = retx;
    }

    return ret;
}



/**
 * 'Corrects' the given coordinates to 0, 180 degrees relative to
 * the current relative zero point.
 *
 * @param coord The uncorrected coordinates.
 * @return The corrected coordinates.
 */
RS_Vector RS_Snapper::restrictHorizontal(const RS_Vector& coord) {
    RS_Vector rz = graphicView->getRelativeZero();
    RS_Vector ret = RS_Vector(coord.x, rz.y);
    return ret;
}


/**
 * 'Corrects' the given coordinates to 90, 270 degrees relative to
 * the current relative zero point.
 *
 * @param coord The uncorrected coordinates.
 * @return The corrected coordinates.
 */
RS_Vector RS_Snapper::restrictVertical(const RS_Vector& coord) {
    RS_Vector rz = graphicView->getRelativeZero();
    RS_Vector ret = RS_Vector(rz.x, coord.y);
    return ret;
}


/**
 * Catches an entity which is close to the given position 'pos'.
 *
 * @param pos A graphic coordinate.
 * @param level The level of resolving for iterating through the entity
 *        container
 * @return Pointer to the entity or NULL.
 */
RS_Entity* RS_Snapper::catchEntity(const RS_Vector& pos,
                                   RS2::ResolveLevel level) {

    RS_DEBUG->print("RS_Snapper::catchEntity");

    // set default distance for points inside solids
    double dist = graphicView->toGraphDX(snapRange)*0.9;

    RS_Entity* entity = container->getNearestEntity(pos, &dist, level, true);

    int idx = -1;
    if (entity!=NULL && entity->getParentContainer()!=NULL) {
        idx = entity->getParentContainer()->findEntity(entity);
    }

    if (entity!=NULL && dist<=graphicView->toGraphDX(snapRange)) {
        // highlight:
        RS_DEBUG->print("RS_Snapper::catchEntity: found: %d", idx);
        return entity;
    } else {
        RS_DEBUG->print("RS_Snapper::catchEntity: not found. dist: %f", dist);
        return NULL;
    }
    RS_DEBUG->print("RS_Snapper::catchEntity: OK");
}



/**
 * Catches an entity which is close to the mouse cursor.
 *
 * @param e A mouse event.
 * @param level The level of resolving for iterating through the entity
 *        container
 * @return Pointer to the entity or NULL.
 */
RS_Entity* RS_Snapper::catchEntity(RS_MouseEvent* e,
                                   RS2::ResolveLevel level) {

    return catchEntity(
               RS_Vector(graphicView->toGraphX(e->x()),
                         graphicView->toGraphY(e->y())),
               level);
}



/**
 * Hides the snapper options. Default implementation does nothing.
 */
void RS_Snapper::hideSnapOptions() {
    RS_DEBUG->print("RS_Snapper::hideSnapOptions");
    if (RS_DIALOGFACTORY!=NULL) {
        switch (snapMode) {
        case RS2::ActionSnapDist:
        case RS2::ActionSnapCoordinate:
        case RS2::ActionSnapCoordinatePolar:
            RS_DIALOGFACTORY->requestSnapOptions(this, false);
            break;
        
        default:
            break;
        }
    }
    RS_DEBUG->print("RS_Snapper::hideSnapOptions: OK");
}



/**
 * Shows the snapper options. Default implementation does nothing.
 */
void RS_Snapper::showSnapOptions() {
    RS_DEBUG->print("RS_Snapper::showSnapOptions");
    if (RS_DIALOGFACTORY!=NULL) {
        switch (snapMode) {
        case RS2::ActionSnapDist:
        case RS2::ActionSnapCoordinate:
        case RS2::ActionSnapCoordinatePolar:
            RS_DIALOGFACTORY->requestSnapOptions(this, true);
            break;

        default:
            break;
        }
    }
}


/**
 * Suspends this snapper while another action takes place.
 */
void RS_Snapper::suspend() {
    snapSpot = snapCoord = RS_Vector(false);
}

/**
 * Resumes this snapper after it has been suspended.
 */
void RS_Snapper::resume() {
}



void RS_Snapper::setCoordinate(const RS_Vector& c, bool relative, bool trigger) {
    coordinate = c;
    if (relative) {
        coordinate += graphicView->getRelativeZero();
    }
    if (trigger) {
        //RS_CoordinateEvent e(coordinate);
        //coordinateEvent(&e);
        graphicView->emulateMousePressEvent();
        graphicView->emulateMouseReleaseEvent();
    }
    else {
        graphicView->emulateMouseMoveEvent();
    }
}
