/*****************************************************************
 * $Id: gv_snapper.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 7, 2008 9:05:54 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_snapper.h ?
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_SNAPPER_H
#define GV_SNAPPER_H


// #include "rs_entitycontainer.h"

#include "gv_coordinateevent.h"
#include "gv.h"

//#include "rs.h"
//#include "rs_mouseevent.h"
//#include "rs_coordinateevent.h"

//class RS_Entity;
//class RS_GraphicView;
//class RS_Vector;
//class RS_Preview;

class GV_Drawing;
class GV_GraphicsView;
class GV_Grid;
class RB_ObjectFactory;
class RVector;

/**
 * This class is used for snapping functions in a graphic view.
 * Actions are usually derrived from this base class if they need
 * to catch entities or snap to coordinates. Use the methods to
 * retrieve a graphic coordinate from a mouse coordinate.
 *
 * Possible snapping functions are described in GV2::ActionType.
 */
class GV_Snapper {
public:
    // GV_Snapper(RS_EntityContainer& container, RS_GraphicView& graphicView);
    GV_Snapper(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_Snapper();

    virtual GV_GraphicsView* getGraphicsView() { return mGraphicsView; }
    virtual GV_Drawing* getDrawing() { return mDrawing; }

    void initSnapper();

    virtual void coordinateEvent(GV_CoordinateEvent*) { }
#if 0
    /**
     * @return Pointer to the entity which was the key entity for the
     * last successful snapping action. If the snap mode is "end point"
     * the key entity is the entity whos end point was caught.
     * If the snap mode didn't require an entity (e.g. free, grid) this
     * method will return NULL.
     */
    RS_Entity* getKeyEntity() {
        return keyEntity;
    }
#endif
    /** Sets a new snap mode. */
    void setSnapMode(GV2::ActionType snapMode) {
        this->snapMode = snapMode;
    }

    GV2::ActionType getSnapMode() {
        return snapMode;
    }

    /** Sets a new snap restriction. */
    void setSnapRestriction(GV2::ActionType snapRes) {
        this->snapRes = snapRes;
    }

    /**
     * Sets the snap range in pixels for catchEntity().
     * @see catchEntity()
     */
    void setSnapRange(int r) {
        snapRange = r;
    }

    /**
     * Might be re-implemented to show a cursor / label / status label
     * that indicates the current snap mode.
     */
    virtual void setSnapStatus(GV2::ActionType t) {
        snapStatus = t;
    }

    virtual RVector snapPoint(double x, double y);

    RVector snapFree(const RVector& coord);
//    RS_Vector snapEndpoint(const RS_Vector& coord);
    RVector snapGrid(const RVector& coord);
//    RS_Vector snapOnEntity(const RS_Vector& coord);
//    RS_Vector snapCenter(const RS_Vector& coord);
//    RS_Vector snapMiddle(const RS_Vector& coord);
//    RS_Vector snapDist(const RS_Vector& coord);
//    RS_Vector snapIntersection(const RS_Vector& coord);
//    RS_Vector snapReference(const RS_Vector& coord);
//    RS_Vector snapCoordinate();
//    RS_Vector snapCoordinatePolar();
    RVector snapAuto(const RVector& coord);

//    RS_Vector restrictOrthogonal(const RS_Vector& coord);
//    RS_Vector restrictHorizontal(const RS_Vector& coord);
//    RS_Vector restrictVertical(const RS_Vector& coord);
//
//    RS_Entity* catchEntity(const RS_Vector& pos,
//                           RS2::ResolveLevel level=RS2::ResolveNone);
//    RS_Entity* catchEntity(RS_MouseEvent* e,
//                           RS2::ResolveLevel level=RS2::ResolveNone);

    virtual void suspend();
    virtual void resume();

    virtual void hideSnapOptions();
    virtual void showSnapOptions();

    RVector getSnapSpot() {
        return snapSpot;
    }
    RVector getSnapCoord() {
        return snapCoord;
    }

//    static RS_Pen getSnapperPen() {
//        return snapperPen;
//    }

    void setDistance(double d) {
        distance = d;
    }
    void setCoordinate(const RVector& c, bool relative, bool trigger);

protected:

//    RS_EntityContainer* container;
//    RS_GraphicView* graphicView;

//    RS_Entity* keyEntity; TODO etc.

    /** Pointer to the drawing */
    GV_Drawing* mDrawing;
    /** Pointer to the graphicsview. */
    GV_GraphicsView* mGraphicsView;

    RVector snapCoord;
    RVector snapSpot;

    GV2::ActionType snapMode;
    GV2::ActionType snapRes;

    GV2::ActionType snapStatus;
    /**
     * Snap distance for snapping to points with a
     * given distance from endpoints.
     */
    double distance;
    /**
     * Snap coordiante for snapping to points with
     * a given coordinate.
     */
    RVector coordinate;
    /**
     * Keeps track of the drawings in XOR mode.
     */
    bool visible;
    /**
     * Snap range for catching entities.
     */
    int snapRange;
    /**
     * Show large cross hairs.
     */
    bool showCrosshairs;
    bool showAutoSnapAid;
    bool finished;

//    static RS_Pen snapperPen; TODO: etc.

    RB_ObjectFactory* mObjectFactory;
};

#endif
