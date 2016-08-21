/****************************************************************************
** $Id: rs_polyline.h 6826 2007-10-30 08:52:30Z andrew $
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


#ifndef RS_POLYLINE_H
#define RS_POLYLINE_H

#include "rs_entity.h"
#include "rs_entitycontainer.h"



/**
 * Holds the data that defines a polyline.
 */
class CAD_EXPORT RS_PolylineData {
public:
    RS_PolylineData() {
        startpoint = endpoint = RS_Vector(false);
        closed = false;
    }
    RS_PolylineData(const RS_Vector& startpoint,
                    const RS_Vector& endpoint,
                    bool closed) {

        this->startpoint = startpoint;
        this->endpoint = endpoint;
        this->closed = closed;
    }

    friend class RS_Polyline;

private:
    RS_Vector startpoint;
    RS_Vector endpoint;
    bool closed;
};



/**
 * Class for a poly line entity (lots of connected lines and arcs).
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Polyline : public RS_EntityContainer {
public:
    RS_Polyline(RS_EntityContainer* parentContainer=NULL);
    RS_Polyline(RS_EntityContainer* parentContainer,
                const RS_PolylineData& d);
    virtual ~RS_Polyline();

    virtual RS_Entity* clone() {
        RS_Polyline* p = new RS_Polyline(parentContainer, data);
        p->copyPropertiesFrom(this);
        p->entities = entities;
        p->entities.setAutoDelete(entities.autoDelete());
        p->initId();
        p->detach();
        p->closingEntity = closingEntity;
        p->nextBulge = nextBulge;
        return p;
    }

    /** @return RS2::EntityPolyline */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityPolyline;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityPolyline || RS_EntityContainer::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Polyline");
    }

    /** @return Copy of data that defines the polyline. */
    RS_PolylineData getData() const {
        return data;
    }

    /** sets a new start point of the polyline */
    void setStartpoint(const RS_Vector& v) {
        data.startpoint = v;
        if (!data.endpoint.valid) {
            data.endpoint = v;
        }
    }

    /** @return Start point of the entity */
    RS_Vector getStartpoint() {
        return data.startpoint;
    }
    
    /** sets a new end point of the polyline */
    void setEndpoint(const RS_Vector& v) {
        data.endpoint = v;
    }

    /** @return End point of the entity */
    RS_Vector getEndpoint() {
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
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);

    virtual RS_Entity* addVertex(const RS_Vector& v, 
        double bulge=0.0, bool prepend=false);
    virtual void setNextBulge(double bulge) {
        nextBulge = bulge;
    }
    virtual void addEntity(RS_Entity* entity);
    virtual void removeLastVertex();
    virtual void endPolyline();
    
    virtual bool optimizeContours();

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    virtual void isoProject(RS2::IsoProjectionType type, double segmentLength);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    
    virtual void draw(RS_Painter* painter, RS_GraphicView* view, 
        double patternOffset=0.0);

protected:
    virtual RS_Entity* createVertex(const RS_Vector& v, 
        double bulge=0.0, bool prepend=false);

protected:
    RS_PolylineData data;
    RS_Entity* closingEntity;
    double nextBulge;
};

#endif
