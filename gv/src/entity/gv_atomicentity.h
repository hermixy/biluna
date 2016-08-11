/****************************************************************************
** $Id: gv_atomicentity.h 8339 2008-01-20 16:11:04Z andrew $
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


#ifndef GV_ATOMICENTITY_H
#define GV_ATOMICENTITY_H

#include "gv_entity.h"


/**
 * Class representing an atomic entity.
 * Typical atomic entities are points, lines, arc, circles but NOT
 * polylines, splines, hatches, texts and other containers.
 *
 * @author Andrew Mustun
 */
class GV_AtomicEntity : public virtual GV_Entity {

public:
    /**
     * Construtor.
     */
    GV_AtomicEntity(GV_EntityContainer* parent) 
        : GV_Entity(parent) {
    }
    /**
     * Destrutor.
     */
    virtual ~GV_AtomicEntity() {}

    /**
     * @return false because entities made from subclasses are 
     *  atomic entities.
     */
    virtual bool isContainer() const {
        return false;
    }

    /**
     * @return true because entities made from subclasses are 
     *  atomic entities.
     */
    virtual bool isAtomic() const {
        return true;
    }

    /**
     * @return Always 1 for atomic entities.
     */
    virtual int count() {
        return 1;
    }
    
    /**
     * @return Always 1 for atomic entities.
     */
    virtual int countDeep() {
        return 1;
    }

    /**
     * Implementation must return the endpoint of the entity or
     * an invalid vector if the entity has no endpoint.
     */
    virtual GV_Vector getEndpoint() const {
        return GV_Vector(false);
    }

    /**
     * Implementation must return the startpoint of the entity or
     * an invalid vector if the entity has no startpoint.
     */
    virtual GV_Vector getStartpoint() const {
        return GV_Vector(false);
    }
    
    /**
     * Implementation must return the angle in which direction the 
     * entity starts.
     */
    virtual double getDirection1() const {
        return 0.0;
    }
    
    /**
     * Implementation must return the angle in which direction the 
     * entity starts the opposite way.
     */
    virtual double getDirection2() const {
        return 0.0;
    }
    
    /**
     * Implementation must return the bulge of the entity.
     */
    virtual double getBulge() const {
        return 1.0;
    }

    /**
     * (De-)selects startpoint.
     */
    virtual void setStartpointSelected(bool select) {
        if (select) {
            setFlag(GV2::FlagSelected1);
        } else {
            delFlag(GV2::FlagSelected1);
        }
    }

    /**
     * (De-)selects endpoint.
     */
    virtual void setEndpointSelected(bool select) {
        if (select) {
            setFlag(GV2::FlagSelected2);
        } else {
            delFlag(GV2::FlagSelected2);
        }
    }

    /**
     * @return True if the entities startpoint is selected. 
     */
    bool isStartpointSelected() const {
        return getFlag(GV2::FlagSelected1);
    }

    /**
     * @return True if the entities endpoint is selected. 
     */
    bool isEndpointSelected() const {
        return getFlag(GV2::FlagSelected2);
    }

    /**
     * Implementation must move the startpoint of the entity to
     * the given position.
     */
    virtual void moveStartpoint(const GV_Vector& /*pos*/) {}

    /**
     * Implementation must move the endpoint of the entity to
     * the given position.
     */
    virtual void moveEndpoint(const GV_Vector& /*pos*/) {}
    
    /**
     * Implementation must trim the startpoint of the entity to
     * the given position.
     */
    virtual void trimStartpoint(const GV_Vector& pos) {
        moveStartpoint(pos);
    }

    /**
     * Implementation must trim the endpoint of the entity to
     * the given position.
     */
    virtual void trimEndpoint(const GV_Vector& pos) {
        moveEndpoint(pos);
    }
    
    /**
     * Implementation must return which ending of the entity will
     * be trimmed if 'coord' is the coordinate chosen to indicate the
     * trim entity and 'trimPoint' is the point to which the entity will
     * be trimmed.
     */
    virtual GV2::Ending getTrimPoint(const GV_Vector& /*coord*/,
                                     const GV_Vector& /*trimPoint*/) {
        return GV2::EndingNone;
    }

    virtual void reverse() { }

    virtual void moveSelectedRef(const GV_Vector& ref, const GV_Vector& offset) {
        if (isSelected()) {
            moveRef(ref, offset);
        }
    }
}
;


#endif
