/*****************************************************************
 * $Id: gv_snapper.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 7, 2008 9:06:21 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_snapper.cpp ?
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_snapper.h"
#if 0
#include "rs_dialogfactory.h"
#include "rs_entitycontainer.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_grid.h"
#include "rs_information.h"
#include "rs_mouseevent.h"
#include "rs_painter.h"
#include "rs_settings.h"


RS_Pen GV_Snapper::snapperPen =
    RS_Pen(RS_Color(255,194,0),
           RS2::Width00,
           RS2::SolidLine);
#endif

#include "gv_dialogfactory.h"
#include "gv_drawing.h"
#include "gv_graphicsview.h"
#include "gv_grid.h"
#include "rb_debug.h"
#include "rb_settings.h"


/**
 * Constructor.
 */
GV_Snapper::GV_Snapper(GV_Drawing& dwg, GV_GraphicsView& gv) {
//    this->container = &container; TODO: etc.
    this->mDrawing = &dwg;
    this->mGraphicsView = &gv;
    this->mObjectFactory = dwg.getFactory();

//    finished = false;
    initSnapper();
}



/**
 * Destructor.
 */
GV_Snapper::~GV_Snapper() {
    RB_DEBUG->print("GV_Snapper::~GV_Snapper() OK");
}



/**
 * Initialize (called by all constructors)
 */
void GV_Snapper::initSnapper() {
    snapMode = mDrawing->getDefaultSnapMode();
    snapRes = mDrawing->getSnapRestriction();

    snapStatus = GV2::ActionSnapFree;
//    keyEntity = NULL;
    snapSpot = RVector();
    snapCoord = RVector();
    visible = false;
    distance = 1.0;
    coordinate = RVector(0,0);
    RB_SETTINGS->beginGroup("/Snap");
    snapRange = RB_SETTINGS->readNumEntry("/Range", 15);
    showAutoSnapAid = (bool)RB_SETTINGS->readNumEntry("/ShowAutoSnapAid", 1);
    RB_SETTINGS->endGroup();
    RB_SETTINGS->beginGroup("/Appearance");
    showCrosshairs = (bool)RB_SETTINGS->readNumEntry("/ShowCrosshairs", 1);
    RB_SETTINGS->endGroup();

    if (snapRange < 2) {
        snapRange = 20;
    }
}

/**
 * Snap to a coordinate in the drawing using the current snap mode.
 *
 * @param e A mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RVector GV_Snapper::snapPoint(double x, double y) {
    RB_DEBUG->print("GV_Snapper::snapPoint");

    snapSpot = RVector();

    // RS_Vector mouseCoord = graphicView->toGraph(x, y);
    RVector mouseCoord(x, y);
    RB_DEBUG->print("GV_Snapper::snapPoint: mouse coord is: %d/%d", x, y);

    switch (snapMode) {

    case GV2::ActionSnapFree:
        snapSpot = snapFree(mouseCoord);
        break;
/*
    case GV2::ActionSnapEndpoint:
        snapSpot = snapEndpoint(mouseCoord);
        break;
*/
    case GV2::ActionSnapGrid:
        snapSpot = snapGrid(mouseCoord);
        break;
/*
    case GV2::ActionSnapOnEntity:
        snapSpot = snapOnEntity(mouseCoord);
        break;

    case GV2::ActionSnapCenter:
        snapSpot = snapCenter(mouseCoord);
        break;

    case GV2::ActionSnapMiddle:
        snapSpot = snapMiddle(mouseCoord);
        break;

    case GV2::ActionSnapDist:
        snapSpot = snapDist(mouseCoord);
        break;

    case GV2::ActionSnapIntersection:
        snapSpot = snapIntersection(mouseCoord);
        break;

    case GV2::ActionSnapReference:
        snapSpot = snapReference(mouseCoord);
        break;

    case GV2::ActionSnapCoordinate:
        snapSpot = snapCoordinate();
        break;

    case GV2::ActionSnapCoordinatePolar:
        snapSpot = snapCoordinatePolar();
        break;
*/
    case GV2::ActionSnapAuto:
        snapSpot = snapAuto(mouseCoord);
        break;

    default:
        break;
    }
/*
    // handle snap restrictions that can be activated in addition
    //   to the ones above:
    switch (snapRes) {
    case GV2::ActionRestrictOrthogonal:
        snapCoord = restrictOrthogonal(snapSpot);
        break;
    case GV2::ActionRestrictHorizontal:
        snapCoord = restrictHorizontal(snapSpot);
        break;
    case GV2::ActionRestrictVertical:
        snapCoord = restrictVertical(snapSpot);
        break;
    default:
    case GV2::ActionRestrictNothing:
*/        snapCoord = snapSpot; /*
        break;
    }

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateCoordinateWidget(snapCoord,
                snapCoord - graphicView->getRelativeZero());
    }

    RS_DEBUG->print("GV_Snapper::snapPoint: OK");
*/
    return snapCoord;
}



/**
 * Snaps to a free coordinate.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RVector GV_Snapper::snapFree(const RVector& coord) {
    // keyEntity = NULL;
    return coord;
}

#if 0

/**
 * Snaps to the closest endpoint.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector GV_Snapper::snapEndpoint(const RS_Vector& coord) {
    RS_Vector vec(false);

    vec = container->getNearestEndpoint(coord,
                                        NULL/*, &keyEntity*/);
    return vec;
}
#endif


/**
 * Snaps to a grid point.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RVector GV_Snapper::snapGrid(const RVector& coord) {

    RB_DEBUG->print("GV_Snapper::snapGrid begin");

    RVector vec;
    double dist=0.0;

    GV_Grid* grid = mDrawing->getGrid();

    if (grid!=NULL) {
        RVector* pts = grid->getPoints();
        int closest = -1;
        dist = 32000.00;

        for (int i = 0; i < grid->count(); ++i) {
            double d = pts[i].getDistanceTo(coord);
            if (d < dist) {
                closest = i;
                dist = d;
            }
        }

        if (closest >= 0) {
            vec = pts[closest];
        }
    }

//    keyEntity = NULL;
    RB_DEBUG->print("GV_Snapper::snapGrid end");
    return vec;
}

#if 0

/**
 * Snaps to a point on an entity.
 *
 * @param coord The mouse coordinate.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector GV_Snapper::snapOnEntity(const RS_Vector& coord) {

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
RS_Vector GV_Snapper::snapCenter(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::snapMiddle(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::snapDist(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::snapIntersection(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::snapReference(const RS_Vector& coord) {
    RS_Vector vec(false);
    vec = container->getNearestRef(coord, NULL);
    return vec;
}



/**
 * Snaps to a user defined coordinate independent of the mouse position.
 *
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector GV_Snapper::snapCoordinate() {
    return coordinate;
}


/**
 * Snaps to a user defined polar coordinate independent of the mouse
 * position.
 *
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector GV_Snapper::snapCoordinatePolar() {
    return coordinate;
}

#endif


/**
 * Snaps to the closest reference point or nothing.
 */
RVector GV_Snapper::snapAuto(const RVector& coord) {
    RVector ret;
    /*
    RS_VectorList candidates1;
    RS_VectorList candidates2;
    */
    double maxDist = 100.0;
//    double maxDist = graphicView->toGraphDX(snapRange);
//
//    ret = snapIntersection(coord);
//    if (ret.distanceTo(coord) < maxDist) {
//        snapStatus = GV2::ActionSnapIntersection;
//        return ret;
//    }
//
//    ret = snapEndpoint(coord);
//    if (ret.distanceTo(coord) < maxDist) {
//        snapStatus = GV2::ActionSnapEndpoint;
//        return ret;
//    }
//
//    ret = snapMiddle(coord);
//    if (ret.distanceTo(coord) < maxDist) {
//        snapStatus = GV2::ActionSnapMiddle;
//        return ret;
//    }
//
//    ret = snapReference(coord);
//    if (ret.distanceTo(coord) < maxDist) {
//        snapStatus = GV2::ActionSnapReference;
//        return ret;
//    }

    ret = snapGrid(coord);
    if (ret.getDistanceTo(coord) < maxDist) {
        snapStatus = GV2::ActionSnapGrid;
        return ret;
    }

//    ret = snapOnEntity(coord);
//    if (ret.distanceTo(coord) < maxDist) {
//        snapStatus = GV2::ActionSnapOnEntity;
//        return ret;
//    }

    snapStatus = GV2::ActionSnapFree;
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

#if 0

/**
 * 'Corrects' the given coordinates to 0, 90, 180, 270 degrees relative to
 * the current relative zero point.
 *
 * @param coord The uncorrected coordinates.
 * @return The corrected coordinates.
 */
RS_Vector GV_Snapper::restrictOrthogonal(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::restrictHorizontal(const RS_Vector& coord) {
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
RS_Vector GV_Snapper::restrictVertical(const RS_Vector& coord) {
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
RS_Entity* GV_Snapper::catchEntity(const RS_Vector& pos,
                                   GV2::ResolveLevel level) {

    RS_DEBUG->print("GV_Snapper::catchEntity");

    // set default distance for points inside solids
    double dist = graphicView->toGraphDX(snapRange)*0.9;

    RS_Entity* entity = container->getNearestEntity(pos, &dist, level, true);

    int idx = -1;
    if (entity!=NULL && entity->getParentContainer()!=NULL) {
        idx = entity->getParentContainer()->findEntity(entity);
    }

    if (entity!=NULL && dist<=graphicView->toGraphDX(snapRange)) {
        // highlight:
        RS_DEBUG->print("GV_Snapper::catchEntity: found: %d", idx);
        return entity;
    } else {
        RS_DEBUG->print("GV_Snapper::catchEntity: not found. dist: %f", dist);
        return NULL;
    }
    RS_DEBUG->print("GV_Snapper::catchEntity: OK");
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
                                   GV2::ResolveLevel level) {

    return catchEntity(
               RS_Vector(graphicView->toGraphX(e->x()),
                         graphicView->toGraphY(e->y())),
               level);
}

#endif

/**
 * Hides the snapper options. Default implementation does nothing.
 */
void GV_Snapper::hideSnapOptions() {
    RB_DEBUG->print("GV_Snapper::hideSnapOptions");

    if (GV_DIALOGFACTORY!=NULL) {
        switch (snapMode) {
        case GV2::ActionSnapDist:
        case GV2::ActionSnapCoordinate:
        case GV2::ActionSnapCoordinatePolar:
//            GV_DIALOGFACTORY->requestSnapOptions(this, false);
            break;
        default:
            break;
        }
    }

    RB_DEBUG->print("GV_Snapper::hideSnapOptions: OK");
}



/**
 * Shows the snapper options. Default implementation does nothing.
 */
void GV_Snapper::showSnapOptions() {
    RB_DEBUG->print("GV_Snapper::showSnapOptions");

    if (GV_DIALOGFACTORY!=NULL) {
        switch (snapMode) {
        case GV2::ActionSnapDist:
        case GV2::ActionSnapCoordinate:
        case GV2::ActionSnapCoordinatePolar:
//            GV_DIALOGFACTORY->requestSnapOptions(this, true);
            break;

        default:
            break;
        }
    }
}


/**
 * Suspends this snapper while another action takes place.
 */
void GV_Snapper::suspend() {
    snapSpot = snapCoord = RVector();
}

/**
 * Resumes this snapper after it has been suspended.
 */
void GV_Snapper::resume() { }



void GV_Snapper::setCoordinate(const RVector& c, bool relative, bool trigger) {
//    coordinate = c; // TODO: etc.
//    if (relative) {
//        coordinate += graphicView->getRelativeZero();
//    }
//    if (trigger) {
//        //RS_CoordinateEvent e(coordinate);
//        //coordinateEvent(&e);
//        graphicView->emulateMousePressEvent();
//        graphicView->emulateMouseReleaseEvent();
//    }
//    else {
//        graphicView->emulateMouseMoveEvent();
//    }
}



