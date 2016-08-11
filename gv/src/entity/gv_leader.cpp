/****************************************************************************
** $Id: gv_leader.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_leader.h"

#include "rb_debug.h"
#include "gv_line.h"
#include "gv_solid.h"


/**
 * Constructor.
 */
GV_Leader::GV_Leader(GV_EntityContainer* parentContainer)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer) {

    empty = true;
}


/**
 * Constructor.
 * @param d Leader data
 */
GV_Leader::GV_Leader(GV_EntityContainer* parentContainer,
                     const GV_LeaderData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {
    empty = true;
    arrowSize = -1.0;
}



/**
 * Destructor 
 */
GV_Leader::~GV_Leader() {}



double GV_Leader::getArrowSize() {
    if (arrowSize>0.0) {
        return arrowSize;
    }
    else {
        return getGraphicVariable("$DIMASZ", 2.5, 40);
    }
}



/**
 * Implementation of update. Updates the arrow.
 */
void GV_Leader::update() {

    // find and delete arrow:
    for (GV_Entity* e=firstEntity(); e!=NULL; e=nextEntity()) {
        if (e->rtti()==GV2::EntitySolid) {
            removeEntity(e);
            break;
        }
    }
    
    if (isUndone()) {
        setVisible(false);
        return;
    }
    else {
        setVisible(true);
    }

    GV_Entity* fe = firstEntity();
    if (fe!=NULL && fe->isAtomic()) {
        GV_Vector p1 = dynamic_cast<GV_AtomicEntity*>(fe)->getStartpoint();
        GV_Vector p2 = dynamic_cast<GV_AtomicEntity*>(fe)->getEndpoint();

        // first entity must be the line which gets the arrow:
        if (hasArrowHead()) {
            GV_Solid* s = new GV_Solid(this, GV_SolidData());
            s->shapeArrow(p1,
                          p2.angleTo(p1),
                          getArrowSize());
            s->setPen(GV_Pen(GV2::FlagInvalid));
            s->setLayer(NULL);
            GV_EntityContainer::addEntity(s);
        }
    }
}



/**
 * Adds a vertex from the endpoint of the last element or 
 * sets the startpoint to the point 'v'.
 *
 * The very first vertex added is the starting point.
 * 
 * @param v vertex coordinate
 *
 * @return Pointer to the entity that was addded or NULL if this
 *         was the first vertex added.
 */
GV_Entity* GV_Leader::addVertex(const GV_Vector& v) {

    GV_Entity* entity=NULL;
    static GV_Vector last = GV_Vector(false);

    if (empty) {
        last = v;
        empty = false;
    } else {
        // add line to the leader:
        entity = new GV_Line(this, GV_LineData(last, v));
        entity->setPen(GV_Pen(GV2::FlagInvalid));
        entity->setLayer(NULL);
        GV_EntityContainer::addEntity(entity);

        if (count()==1 && hasArrowHead()) {
            update();
        }

        last = v;
    }

    return entity;
}



/**
 * Reimplementation of the addEntity method for a normal container.
 * This reimplementation deletes the given entity!
 *
 * To add entities use addVertex() instead.
 */
void GV_Leader::addEntity(GV_Entity* entity) {
    RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Leader::addEntity:"
                    " should never be called");

    if (entity==NULL) {
        return;
    }
    delete entity;
}



GV_VectorList GV_Leader::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;

    int i=0;
    bool first = true;
    
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone), i++) {
        if (e->rtti()==GV2::EntityLine) {
            GV_AtomicEntity* ae = dynamic_cast<GV_AtomicEntity*>(e);
            if (first) {
                ret.set(0, ae->getStartpoint());
                i++;
                first = false;
            }
            ret.set(i, ae->getEndpoint());
        }
    }

    return ret;
}



GV_Vector GV_Leader::getNearestRef(const GV_Vector& coord,
                                   double* dist,
                                   GV2::RefMode refMode) {

    return GV_Entity::getNearestRef(coord, dist, refMode);
}



GV_Vector GV_Leader::getNearestSelectedRef(const GV_Vector& coord,
        double* dist,
        GV2::RefMode refMode) {

    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}



void GV_Leader::move(const GV_Vector& offset) {
    GV_EntityContainer::move(offset);
    update();
}



void GV_Leader::rotate(const GV_Vector& center, double angle) {
    GV_EntityContainer::rotate(center, angle);
    update();
}



void GV_Leader::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_EntityContainer::scale(center, factor);
    update();
}



void GV_Leader::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_EntityContainer::mirror(axisPoint1, axisPoint2);
    update();
}


void GV_Leader::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_EntityContainer::moveRef(ref, offset);
    update();
}


void GV_Leader::stretch(const GV_Vector& firstCorner,
                       const GV_Vector& secondCorner,
                       const GV_Vector& offset) {

    GV_EntityContainer::stretch(firstCorner, secondCorner, offset);
    update();
}

