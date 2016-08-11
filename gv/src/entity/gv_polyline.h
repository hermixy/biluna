/****************************************************************************
** $Id: gv_polyline.h 6826 2007-10-30 08:52:30Z andrew $
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


#ifndef GV_POLYLINE_H
#define GV_POLYLINE_H

#include "gv_entity.h"
#include "gv_entitycontainer.h"



/**
 * Holds the data that defines a polyline.
 */
class GV_PolylineData {
public:
    GV_PolylineData() {
        startpoint = endpoint = GV_Vector(false);
        closed = false;
    }
    GV_PolylineData(const GV_Vector& startpoint,
                    const GV_Vector& endpoint,
                    bool closed) {

        this->startpoint = startpoint;
        this->endpoint = endpoint;
        this->closed = closed;
    }

    friend class GV_Polyline;

private:
    GV_Vector startpoint;
    GV_Vector endpoint;
    bool closed;
};



/**
 * Class for a poly line entity (lots of connected lines and arcs).
 *
 * @author Andrew Mustun
 */
class GV_Polyline : public GV_EntityContainer {
public:
    GV_Polyline(GV_EntityContainer* parentContainer=NULL);
    GV_Polyline(GV_EntityContainer* parentContainer,
                const GV_PolylineData& d);
    virtual ~GV_Polyline();

    virtual GV_Entity* clone() {
        GV_Polyline* p = new GV_Polyline(parentContainer, data);
        p->copyPropertiesFrom(this);
        p->entities = entities;
        p->entities.setAutoDelete(entities.autoDelete());
        p->initId();
        p->detach();
        p->closingEntity = closingEntity;
        p->nextBulge = nextBulge;
        return p;
    }

    /** @return GV2::EntityPolyline */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityPolyline;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityPolyline || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Polyline");
    }

    /** @return Copy of data that defines the polyline. */
    GV_PolylineData getData() const {
        return data;
    }

    /** sets a new start point of the polyline */
    void setStartpoint(const GV_Vector& v) {
        data.startpoint = v;
        if (!data.endpoint.valid) {
            data.endpoint = v;
        }
    }

    /** @return Start point of the entity */
    GV_Vector getStartpoint() {
        return data.startpoint;
    }
    
    /** sets a new end point of the polyline */
    void setEndpoint(const GV_Vector& v) {
        data.endpoint = v;
    }

    /** @return End point of the entity */
    GV_Vector getEndpoint() {
        return data.endpoint;
    }

    double getClosingBulge();

    void updateEndpoints();

    /** @return true if the polyline is closed. false otherwise */
    bool isClosed() const {
        return data.closed;
    }

    void setClosed(bool cl) {
        data.closed = cl;
    }

    bool validate();
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);

    virtual GV_Entity* addVertex(const GV_Vector& v, 
        double bulge=0.0, bool prepend=false);
    virtual void setNextBulge(double bulge) {
        nextBulge = bulge;
    }
    virtual void addEntity(GV_Entity* entity);
    virtual void removeLastVertex();
    virtual void endPolyline();
    
    virtual bool optimizeContours();

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void isoProject(GV2::IsoProjectionType type, double segmentLength);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    
    virtual void draw(GV_Painter* painter, GV_GraphicView* view, 
        double patternOffset=0.0);

protected:
    virtual GV_Entity* createVertex(const GV_Vector& v, 
        double bulge=0.0, bool prepend=false);

protected:
    GV_PolylineData data;
    GV_Entity* closingEntity;
    double nextBulge;
};

#endif
