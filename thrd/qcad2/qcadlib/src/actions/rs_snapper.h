/****************************************************************************
** $Id: rs_snapper.h 7382 2007-11-29 16:37:31Z andrew $
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


#ifndef RS_SNAPPER_H
#define RS_SNAPPER_H

#include "rs_entitycontainer.h"

#include "rs.h"
#include "rs_mouseevent.h"
#include "rs_coordinateevent.h"

class RS_Entity;
class RS_GraphicView;
class RS_Vector;
class RS_Preview;

/**
 * This class is used for snapping functions in a graphic view. 
 * Actions are usually derrived from this base class if they need
 * to catch entities or snap to coordinates. Use the methods to
 * retrieve a graphic coordinate from a mouse coordinate.
 *
 * Possible snapping functions are described in RS2::ActionType.
 *
 * @author Andrew Mustun
 */
class RS_Snapper {
public:
    RS_Snapper(RS_EntityContainer& container, RS_GraphicView& graphicView);
    virtual ~RS_Snapper();

    void initSnapper();
    void finish();
    
    virtual void coordinateEvent(RS_CoordinateEvent*) { }

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

    /** Sets a new snap mode. */
    void setSnapMode(RS2::ActionType snapMode) {
        this->snapMode = snapMode;
    }
    RS2::ActionType getSnapMode() {
        return snapMode;
    }
    /** Sets a new snap restriction. */
    void setSnapRestriction(RS2::ActionType snapRes) {
        this->snapRes = snapRes;
    }

    /** 
     * Sets the snap range in pixels for catchEntity().
     *
     * @see catchEntity()
     */
    void setSnapRange(int r) {
        snapRange = r;
    }

    /**
     * Might be re-implemented to show a cursor / label / status label
     * that indicates the current snap mode.
     */
    virtual void setSnapStatus(RS2::ActionType t) {
        snapStatus = t;
    }

    virtual RS_Vector snapPoint(RS_MouseEvent* e);
    virtual RS_Vector snapPoint(int x, int y);

    RS_Vector snapFree(const RS_Vector& coord);
    RS_Vector snapEndpoint(const RS_Vector& coord);
    RS_Vector snapGrid(const RS_Vector& coord);
    RS_Vector snapOnEntity(const RS_Vector& coord);
    RS_Vector snapCenter(const RS_Vector& coord);
    RS_Vector snapMiddle(const RS_Vector& coord);
    RS_Vector snapDist(const RS_Vector& coord);
    RS_Vector snapIntersection(const RS_Vector& coord);
    RS_Vector snapReference(const RS_Vector& coord);
    RS_Vector snapCoordinate();
    RS_Vector snapCoordinatePolar();
    RS_Vector snapAuto(const RS_Vector& coord);
    
    RS_Vector restrictOrthogonal(const RS_Vector& coord);
    RS_Vector restrictHorizontal(const RS_Vector& coord);
    RS_Vector restrictVertical(const RS_Vector& coord);

    RS_Entity* catchEntity(const RS_Vector& pos,
                           RS2::ResolveLevel level=RS2::ResolveNone);
    RS_Entity* catchEntity(RS_MouseEvent* e,
                           RS2::ResolveLevel level=RS2::ResolveNone);

    virtual void suspend();
    virtual void resume();

    virtual void hideSnapOptions();
    virtual void showSnapOptions();

    RS_Vector getSnapSpot() {
        return snapSpot;
    }
    RS_Vector getSnapCoord() {
        return snapCoord;
    }
    
    static RS_Pen getSnapperPen() {
        return snapperPen;
    }

    void setDistance(double d) {
        distance = d;
    }
    void setCoordinate(const RS_Vector& c, bool relative, bool trigger);

protected:
    RS_EntityContainer* container;
    RS_GraphicView* graphicView;
    RS_Entity* keyEntity;
    RS_Vector snapCoord;
    RS_Vector snapSpot;
    RS2::ActionType snapMode;
    RS2::ActionType snapRes;
    RS2::ActionType snapStatus;
    /**
     * Snap distance for snapping to points with a 
     * given distance from endpoints.
     */
    double distance;
    /**
     * Snap coordiante for snapping to points with 
     * a given coordinate.
     */
    RS_Vector coordinate;
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
    
    static RS_Pen snapperPen;
};

#endif
