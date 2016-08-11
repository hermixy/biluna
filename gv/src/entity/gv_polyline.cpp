/****************************************************************************
** $Id: gv_polyline.cpp 7930 2008-01-17 21:23:04Z andrew $
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


#include "gv_polyline.h"

#include "rb_debug.h"
#include "gv_line.h"
#include "gv_arc.h"
#include "gv_graphicview.h"


/**
 * Constructor.
 */
GV_Polyline::GV_Polyline(GV_EntityContainer* parentContainer)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer) {

    closingEntity = NULL;
    nextBulge = 0.0;
}


/**
 * Constructor.
 * @param d Polyline data
 */
GV_Polyline::GV_Polyline(GV_EntityContainer* parentContainer,
                         const GV_PolylineData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {

    closingEntity = NULL;
    nextBulge = 0.0;
    calculateBorders();
}


/**
 * Destructor 
 */
GV_Polyline::~GV_Polyline() {
    RB_DEBUG->print("GV_Polyline::~GV_Polyline");
    RB_DEBUG->print("GV_Polyline::~GV_Polyline: OK");
}


/**
 * Validates the polyline.
 *
 * @retval true: Polyline is valid.
 * @retval false: Polyline is not valid (entities not connected, ...).
 */
bool GV_Polyline::validate() {
    bool ret = true;
    GV_Vector pt = getStartpoint();

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone)) {
        if (e->isAtomic()) {
            GV_AtomicEntity* ae = dynamic_cast<GV_AtomicEntity*>(e);
            if (pt.distanceTo(ae->getStartpoint())<1.0e-3) {
                pt = ae->getEndpoint();
            }
            else {
                ret = false;
                break;
            }
        }
        else {
            ret = false;
            break;
        }
    }

    return ret;
}


/**
 * Overrides the implementation of GV_EntityContainer.
 * Does nothing, since polylines are already 'optimized' by definition.
 * 
 * @retval true all contours were closed
 * @retval false at least one contour is not closed
 */
bool GV_Polyline::optimizeContours() {
    RB_DEBUG->print("GV_Polyline::optimizeContours");
    return (isClosed() || getStartpoint().distanceTo(getEndpoint())<1.04-6);
}


/**
 * Removes the last vertex of this polyline.
 */
void GV_Polyline::removeLastVertex() {
    GV_Entity* last = lastEntity();
    if (last!=NULL) {
        removeEntity(last);
        last = lastEntity();
        if (last!=NULL) {
            if (last->isAtomic()) {
                data.endpoint = dynamic_cast<GV_AtomicEntity*>(last)->getEndpoint();
            }
            else {
                RB_DEBUG->print(RB_Debug::D_WARNING, 
                    "GV_Polyline::removeLastVertex: "
                     "polyline contains non-atomic entity");
            }
        }
        else {
            data.endpoint = data.startpoint;
        }
    }
    else {
        data.startpoint.valid = false;
    }
}


/**
 * Adds a vertex from the endpoint of the last segment or
 * from the startpoint of the first segment to 'v' or
 * sets the startpoint to the point 'v'.
 *
 * The very first vertex added with this method is the startpoint.
 * 
 * @param v vertex coordinate to be added
 * @param bulge The bulge of the _next_ vertex or 0 for a line segment 
 *        (see DXF documentation)
 * @param prepend true: prepend at start instead of append at end
 *
 * @return Pointer to the entity that was addded or NULL if this
 *         was the first vertex added.
 */
GV_Entity* GV_Polyline::addVertex(const GV_Vector& v, double bulge, bool prepend) {

    GV_Entity* entity=NULL;
    //static double nextBulge = 0.0;

    // very first vertex:
    if (!data.startpoint.valid) {
        data.startpoint = data.endpoint = v;
        nextBulge = bulge;
    }

    // consequent vertices:
    else {
        // add entity to the polyline:
        entity = createVertex(v, nextBulge, prepend);
        if (entity!=NULL) {
            if (prepend==false) {
                GV_EntityContainer::addEntity(entity);
                data.endpoint = v;
            }
            else {
                GV_EntityContainer::insertEntity(0, entity);
                data.startpoint = v;
            }
        }
        nextBulge = bulge;
        endPolyline();
    }
    //data.endpoint = v;

    return entity;
}



/**
 * Creates a vertex from the endpoint of the last element or 
 * sets the startpoint to the point 'v'.
 *
 * The very first vertex added is the starting point.
 * 
 * @param v vertex coordinate
 * @param bulge The bulge of the arc (see DXF documentation)
 * @param prepend true: Prepend instead of append at end
 *
 * @return Pointer to the entity that was created or NULL if this
 *         was the first vertex added.
 */
GV_Entity* GV_Polyline::createVertex(const GV_Vector& v, double bulge, bool prepend) {

    GV_Entity* entity=NULL;

//    RB_DEBUG->print("GV_Polyline::createVertex: %f/%f to %f/%f bulge: %f",
//                    data.endpoint.x, data.endpoint.y, v.x, v.y, bulge);

    // create line for the polyline:
    if (fabs(bulge)<GV_TOLERANCE) {
        if (prepend==false) {
            entity = new GV_Line(this, GV_LineData(data.endpoint, v));
        }
        else {
            entity = new GV_Line(this, GV_LineData(v, data.startpoint));
        }
        entity->setSelected(isSelected());
        entity->setPen(GV_Pen(GV2::FlagInvalid));
        entity->setLayer(NULL);
        //GV_EntityContainer::addEntity(entity);
        //data.endpoint = v;
    }

    // create arc for the polyline:
    else {
        bool reversed = (bulge<0.0);
        double alpha = atan(bulge)*4.0;

        if (fabs(alpha) > 2*M_PI-1.0e-4) {
            return NULL;
        }

        double radius;
        GV_Vector center;
        GV_Vector middle;
        double dist;
        double angle;

        if (prepend==false) {
            middle = (data.endpoint+v)/2.0;
            dist = data.endpoint.distanceTo(v)/2.0;
            angle = data.endpoint.angleTo(v);
        }
        else {
            middle = (data.startpoint+v)/2.0;
            dist = data.startpoint.distanceTo(v)/2.0;
            angle = v.angleTo(data.startpoint);
        }

        // alpha can't be 0.0 at this point
        radius = fabs(dist / sin(alpha/2.0));

        double wu = fabs(GV_Math::pow(radius, 2.0) - GV_Math::pow(dist, 2.0));
        double h = sqrt(wu);

        if (bulge>0.0) {
            angle+=M_PI/2.0;
        } else {
            angle-=M_PI/2.0;
        }

        if (fabs(alpha)>M_PI) {
            h*=-1.0;
        }

        center.setPolar(h, angle);
        center+=middle;

        double a1;
        double a2;

        if (prepend==false) {
            a1 = center.angleTo(data.endpoint);
            a2 = center.angleTo(v);
        }
        else {
            a1 = center.angleTo(v);
            a2 = center.angleTo(data.startpoint);
        }

        GV_ArcData d(center, radius,
                     a1, a2,
                     reversed);

        entity = new GV_Arc(this, d);
        entity->setSelected(isSelected());
        entity->setPen(GV_Pen(GV2::FlagInvalid));
        entity->setLayer(NULL);
    }

    return entity;
}


/**
 * Ends polyline and adds the last entity if the polyline is closed
 */
void GV_Polyline::endPolyline() {
    RB_DEBUG->print("GV_Polyline::endPolyline");

    if (isClosed()) {
        RB_DEBUG->print("GV_Polyline::endPolyline: adding closing entity");
        
        // remove old closing entity:
        if (closingEntity!=NULL) {
            removeEntity(closingEntity);
        }

        // add closing entity to the polyline:
        closingEntity = createVertex(data.startpoint, nextBulge);
        if (closingEntity!=NULL) {
            GV_EntityContainer::addEntity(closingEntity);
            //data.endpoint = data.startpoint;
        }
    }
}
    
    

/**
 * @return The bulge of the closing entity.
 */
double GV_Polyline::getClosingBulge() {
    if (isClosed()) {
        GV_Entity* e = lastEntity();
        if (e!=NULL && e->rtti()==GV2::EntityArc) {
            return dynamic_cast<GV_Arc*>(e)->getBulge();
        }
    }

    return 0.0;
}


/**
 * Sets the polylines start and endpoint to match the first and last vertex.
 */
void GV_Polyline::updateEndpoints() {
    GV_Entity* e1 = firstEntity();
    if (e1!=NULL && e1->isAtomic()) {
        GV_Vector v = dynamic_cast<GV_AtomicEntity*>(e1)->getStartpoint();
        setStartpoint(v);
    }
    
    GV_Entity* e2 = lastEntity();
    if (isClosed()) {
        e2 = prevEntity();
    }
    if (e2!=NULL && e2->isAtomic()) {
        GV_Vector v = dynamic_cast<GV_AtomicEntity*>(e2)->getEndpoint();
        setEndpoint(v);
    }
}



/**
 * Reimplementation of the addEntity method for a normal container.
 * This reimplementation deletes the given entity!
 *
 * To add entities use addVertex() or addSegment() instead.
 */
void GV_Polyline::addEntity(GV_Entity* entity) {
    RB_DEBUG->print(RB_Debug::D_WARNING, "GV_Polyline::addEntity:"
                    " should never be called");

    if (entity==NULL) {
        return;
    }
    delete entity;
}


/**
 * Adds a segment to the polyline.
 */
/*void GV_Polyline::addSegment(GV_Entity* entity) {
    GV_EntityContainer::addEntity(entity);
    // TODO: reorder and check polyline
}*/



GV_VectorList GV_Polyline::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret;

    int i=0;
    ret.set(0, data.startpoint);
    i++;
    
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone), i++) {
        if (e->isAtomic()) {
            ret.set(i, dynamic_cast<GV_AtomicEntity*>(e)->getEndpoint());
        }
    }
    
    //ret.set(count(), data.endpoint);

    return ret;
}

GV_Vector GV_Polyline::getNearestRef(const GV_Vector& coord,
                                   double* dist,
                                   GV2::RefMode refMode) {

    return GV_Entity::getNearestRef(coord, dist, refMode);
}

GV_Vector GV_Polyline::getNearestSelectedRef(const GV_Vector& coord,
        double* dist,
        GV2::RefMode refMode) {

    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}



/*
void GV_Polyline::reorder() {
    // current point:
    GV_Vector cp;

    bool done = false;
    do {
        
    } while(!done);
}
*/



void GV_Polyline::move(const GV_Vector& offset) {
    GV_EntityContainer::move(offset);
    data.startpoint.move(offset);
    data.endpoint.move(offset);
}



void GV_Polyline::rotate(const GV_Vector& center, double angle) {
    GV_EntityContainer::rotate(center, angle);
    data.startpoint.rotate(center, angle);
    data.endpoint.rotate(center, angle);
}



void GV_Polyline::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_EntityContainer::scale(center, factor);
    data.startpoint.scale(center, factor);
    data.endpoint.scale(center, factor);
}



void GV_Polyline::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_EntityContainer::mirror(axisPoint1, axisPoint2);
    data.startpoint.mirror(axisPoint1, axisPoint2);
    data.endpoint.mirror(axisPoint1, axisPoint2);
}



void GV_Polyline::isoProject(GV2::IsoProjectionType type, double segmentLength) {
    GV_EntityContainer::isoProject(type, segmentLength);
    data.startpoint.isoProject(type);
    data.endpoint.isoProject(type);
    calculateBorders();
}


void GV_Polyline::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_EntityContainer::moveRef(ref, offset);
    if (ref.distanceTo(data.startpoint)<1.0e-4) {
       data.startpoint.move(offset);
    }
    if (ref.distanceTo(data.endpoint)<1.0e-4) {
       data.endpoint.move(offset);
    }
    //update();
}



void GV_Polyline::stretch(const GV_Vector& firstCorner,
                                 const GV_Vector& secondCorner,
                                 const GV_Vector& offset) {

    if (data.startpoint.isInWindow(firstCorner, secondCorner)) {
        data.startpoint.move(offset);
    } 
    if (data.endpoint.isInWindow(firstCorner, secondCorner)) {
        data.endpoint.move(offset);
    } 
    
    GV_EntityContainer::stretch(firstCorner, secondCorner, offset);
}


/**
 * Slightly optimized drawing for polylines.
 */
void GV_Polyline::draw(GV_Painter* painter, GV_GraphicView* view, 
    double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }
    RB_DEBUG->print("GV_Polyline::draw");
    
    // draw first entity and set correct pen:
    GV_Entity* e = firstEntity(GV2::ResolveNone);
    
    // v2.1.0.0:
    view->drawEntityPlain(e);
    
    // draw subsequent entities with same pen:
    for (GV_Entity* e=nextEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone)) {

        view->drawEntityPlain(e);
    }
    RB_DEBUG->print("GV_Polyline::draw: OK");
}



