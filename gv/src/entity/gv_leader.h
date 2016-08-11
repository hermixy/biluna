/****************************************************************************
** $Id: gv_leader.h 6089 2007-08-21 21:03:27Z andrew $
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


#ifndef GV_LEADER_H
#define GV_LEADER_H

#include "gv_entity.h"
#include "gv_entitycontainer.h"



/**
 * Holds the data that defines a leader.
 */
class GV_LeaderData {
public:
    GV_LeaderData() {}
    GV_LeaderData(bool arrowHeadFlag) {
        arrowHead = arrowHeadFlag;
    }

    /** true: leader has an arrow head. false: no arrow. */
    bool arrowHead;
};



/**
 * Class for a leader entity (kind of a polyline arrow).
 *
 * @author Andrew Mustun
 */
class GV_Leader : public GV_EntityContainer {
public:
    GV_Leader(GV_EntityContainer* parentContainer=NULL);
    GV_Leader(GV_EntityContainer* parentContainer,
                const GV_LeaderData& d);
    virtual ~GV_Leader();

    virtual GV_Entity* clone() {
        GV_Leader* p = new GV_Leader(parentContainer, data);
        p->copyPropertiesFrom(this);
        p->entities = entities;
        p->entities.setAutoDelete(entities.autoDelete());
        p->initId();
        p->detach();
        return p;
    }

    /**    @return GV2::EntityDimLeader */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityDimLeader;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityDimLeader || GV_EntityContainer::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Leader");
    }

    virtual void update();

    /** @return Copy of data that defines the leader. */
    GV_LeaderData getData() const {
        return data;
    }

    double getArrowSize();

    /** @return true: if this leader has an arrow at the beginning. */
    bool hasArrowHead() {
        return data.arrowHead;
    }

    virtual GV_Entity* addVertex(const GV_Vector& v);
    virtual void addEntity(GV_Entity* entity);

    virtual double getLength() {
        return -1.0;
    }
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    
    void overrideArrowSize(double f) {
        arrowSize = f;
    }
    
    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);

protected:
    GV_LeaderData data;
    bool empty;
    double arrowSize;
};

#endif
