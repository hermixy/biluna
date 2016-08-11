/****************************************************************************
** $Id: gv_entitycontainer.cpp 9471 2008-03-28 11:12:04Z andrew $
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


#include "gv_entitycontainer.h"

//#include <values.h>

#include "rb_debug.h"
#include "gv_dimension.h"
#include "gv_math.h"
#include "gv_layer.h"
#include "gv_line.h"
#include "gv_leader.h"
// #include "gv_modification.h" TODO: Rutger
#include "gv_polyline.h"
#include "gv_text.h"
#include "gv_insert.h"
#include "gv_spline.h"
#include "gv_information.h"
#include "gv_graphicview.h"

bool GV_EntityContainer::autoUpdateBorders = true;

/**
 * Default constructor.
 *
 * @param owner True if we own and also delete the entities.
 */
GV_EntityContainer::GV_EntityContainer(GV_EntityContainer* parentContainer,
                                       bool owner)
        : GV_Entity(parentContainer) {

    RB_DEBUG->print("GV_EntityContainer::GV_EntityContainer");
    
    entities.setAutoDelete(owner);
    subContainer = NULL;
    //autoUpdateBorders = true;

    RB_DEBUG->print("GV_EntityContainer::GV_EntityContainer: OK");
}



/**
 * Destructor.
 */
GV_EntityContainer::~GV_EntityContainer() {
    RB_DEBUG->print("GV_EntityContainer::~GV_EntityContainer");
    clear();
    RB_DEBUG->print("GV_EntityContainer::~GV_EntityContainer: OK");
}



GV_Entity* GV_EntityContainer::clone() {
    GV_EntityContainer* ec = 
        new GV_EntityContainer(parentContainer, entities.autoDelete());
    ec->copyPropertiesFrom(this);
    ec->entities = entities;
    ec->detach();
    ec->initId();
    return ec;
}



/**
 * Detaches shallow copies and creates deep copies of all subentities.
 * This function is called after cloning entity containers.
 */
void GV_EntityContainer::detach() {

    // make deep copies of all entities:
    GV_PtrList<GV_Entity> tmp;
    for (GV_Entity* e=firstEntity();
            e!=NULL;
            e=nextEntity()) {
        if (!e->getFlag(GV2::FlagTemp) && !e->isUndone()) {
            tmp.append(e->clone());
        }
    }

    // clear shared pointers:
    bool autoDel = entities.autoDelete();
    entities.setAutoDelete(false);
    entities.clear();
    entities.setAutoDelete(autoDel);

    // point to new deep copies:
    for (int i=0; i<tmp.size(); ++i) {
        GV_Entity* e = tmp[i];
        e->reparent(this);
        entities.append(e);
    }
}



void GV_EntityContainer::reparent(GV_EntityContainer* parentContainer) {
    GV_Entity::reparent(parentContainer);
}



/**
 * Forces the given pen on all subentities.
 */
void GV_EntityContainer::setOverridePen(const GV_Pen& pen) {
    GV_Entity::setOverridePen(pen);

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->setOverridePen(pen);
    }
}



/**
 * Set the layer of this entity all sub entities to the active layer.
 * Used for pasting to current layer.
 */
void GV_EntityContainer::setLayerToActive() {
    GV_Entity::setLayerToActive();

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->setLayerToActive();
    }
}



/**
 * Called when the undo state changed. Forwards the event to all sub-entities.
 *
 * @param undone true: entity has become invisible.
 *               false: entity has become visible.
 */
void GV_EntityContainer::undoStateChanged(bool undone) {

    GV_Entity::undoStateChanged(undone);

    // ! don't pass on to subentities. undo list handles them
    // All sub-entities:
    /*for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->setUndoState(undone);
}*/
}



void GV_EntityContainer::setVisible(bool v) {
    GV_Entity::setVisible(v);

    // All sub-entities:
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->setVisible(v);
    }
}



/**
 * @return Total length of all entities in this container.
 */
double GV_EntityContainer::getLength() {
    double ret = 0.0;

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        if (e->isVisible()) {
            double l = e->getLength();
            if (l<0.0) {
                ret = -1.0;
                break;
            } else {
                ret += l;
            }
        }
    }

    return ret;
}


/**
 * Resets all processed flags of all entities.
 */
void GV_EntityContainer::setProcessed(bool on) {
    GV_Entity::setProcessed(on);

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->setProcessed(on);
    }
}


/**
 * Selects this entity.
 */
bool GV_EntityContainer::setSelected(bool select, bool /*updateDependent*/) {
    // Try to select this entity:
    if (GV_Entity::setSelected(select)) {

        // Select all sub-entities:
        for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {
            if (e->isVisible() /*&& e->rtti()!=GV2::EntityHatch*/) {
                e->setSelected(select);
            }
        }
        return true;
    } else {
        return false;
    }
}



/**
 * Sets the selection visibility.
 */
void GV_EntityContainer::setSelectionVisible(bool on) {
    GV_Entity::setSelectionVisible(on);

    // All sub-entity's:
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        if (e->isVisible() /*&& e->rtti()!=GV2::EntityHatch*/) {
            e->setSelectionVisible(on);
        }
    }
}



/**
 * Toggles select on this entity.
 */
bool GV_EntityContainer::toggleSelected() {
    // Toggle this entity's select:
    if (GV_Entity::toggleSelected()) {

        // Toggle all sub-entity's select:
        /*for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {
            e->toggleSelected();
    }*/
        return true;
    } else {
        return false;
    }
}



/**
 * Selects all entities within the given area.
 *
 * @param select True to select, False to deselect the entities.
 */
void GV_EntityContainer::selectWindow(const GV_Vector& v1, const GV_Vector& v2,
                                      bool select) {

    bool included;
    bool cross = (v2.x<v1.x);

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {

        included = false;

        if (e->isVisible()) {
            if (e->isInWindow(v1, v2)) {
                //e->setSelected(select);
                included = true;
            } else if (cross==true) {
                GV_Line** l = new GV_Line*[4];
                l[0] = new GV_Line(NULL, GV_LineData(v1, GV_Vector(v2.x, v1.y)));
                l[1] = new GV_Line(NULL, GV_LineData(GV_Vector(v2.x, v1.y), v2));
                l[2] = new GV_Line(NULL, GV_LineData(v2, GV_Vector(v1.x, v2.y)));
                l[3] = new GV_Line(NULL, GV_LineData(GV_Vector(v1.x, v2.y), v1));
                GV_VectorList sol;

                if (e->isContainer()) {
                    GV_EntityContainer* ec = dynamic_cast<GV_EntityContainer*>(e);
                    for (GV_Entity* se=ec->firstEntity(GV2::ResolveAll);
                            se!=NULL && included==false;
                            se=ec->nextEntity(GV2::ResolveAll)) {

                        for (int i=0; i<4; ++i) {
                            sol = GV_Information::getIntersection(
                                      se, l[i], true);
                            if (sol.hasValid()) {
                                included = true;
                                break;
                            }
                        }
                    }
                } else {
                    for (int i=0; i<4; ++i) {
                        sol = GV_Information::getIntersection(e, l[i], true);
                        if (sol.hasValid()) {
                            included = true;
                            break;
                        }
                    }
                }

                for (int i=0; i<4; ++i) {
                    delete l[i];
                }
                delete[] l;
            }
        }

        if (included) {
            e->setSelected(select);
        }
    }
}



/**
 * Adds a entity to this container and updates the borders of this 
 * entity-container if autoUpdateBorders is true.
 */
void GV_EntityContainer::addEntity(GV_Entity* entity) {
    if (entity==NULL) {
        return;
    }

    if (isSelected()) {
        entity->setSelected(true);
    }

    if (entity->alwaysPrepend()) {
        entities.prepend(entity);
    }
    else {
        entities.append(entity);
    }
    
    entity->addedEvent(this);
    
    if (autoUpdateBorders) {
        adjustBorders(entity);
    }
}


/**
 * Inserts a entity to this container at the given position and updates 
 * the borders of this entity-container if autoUpdateBorders is true.
 */
void GV_EntityContainer::insertEntity(int index, GV_Entity* entity) {
    if (entity==NULL) {
        return;
    }

    entities.insert(index, entity);

    if (autoUpdateBorders) {
        adjustBorders(entity);
    }
}



/**
 * Replaces the entity at the given index with the given entity
 * and updates the borders of this entity-container if autoUpdateBorders is true.
 */
void GV_EntityContainer::replaceEntity(int index, GV_Entity* entity) {
    if (entity==NULL) {
        RB_DEBUG->print("GV_EntityContainer::replaceEntity: entity is NULL");
        return;
    }

    entities.replace(index, entity);

    if (autoUpdateBorders) {
        adjustBorders(entity);
    }
}



/**
 * Removes an entity from this container and updates the borders of 
 * this entity-container if autoUpdateBorders is true.
 */
bool GV_EntityContainer::removeEntity(GV_Entity* entity) {
    int idx = entities.indexOf(entity);
    if (idx==-1) {
        return false;
    }
    else {
        /*
        if (getDocument()!=NULL) {
            //getDocument()->removeUndoablePointers(entity);
        }
        */
        if (isOfType(GV2::EntityDocument)) {
            dynamic_cast<GV_Document*>(this)->removeUndoablePointers(entity);
        }
        entities.removeAt(idx);
    }
    if (autoUpdateBorders) {
        calculateBorders();
    }
    return true;
}



/**
 * Moves the given entity in the background (at the beginning of the entity list).
 */    
void GV_EntityContainer::moveEntityToBack(GV_Entity* entity, bool toBack) {
    int idx = entities.indexOf(entity);
    if (idx!=-1) {
        if (toBack) {
            entities.prepend(entities.takeAt(idx));
        }
        else {
            entities.append(entities.takeAt(idx));
        }
    }
}



/**
 * Moves the given entity in the foreground (at the end of the entity list).
 */    
void GV_EntityContainer::moveEntityToFront(GV_Entity* entity) {
    moveEntityToBack(entity, false);
}


/**
 * Erases all entities in this container and resets the borders..
 */
void GV_EntityContainer::clear() {
    entities.clear();
    resetBorders();
}


/**
 * Counts all entities (branches of the tree). 
 */
int GV_EntityContainer::count() {
    return entities.count();
}


/**
 * Counts all entities (leaves of the tree). 
 */
int GV_EntityContainer::countDeep() {
    int c=0;

    for (GV_Entity* t=firstEntity(GV2::ResolveNone);
            t!=NULL;
            t=nextEntity(GV2::ResolveNone)) {
        c+=t->countDeep();
    }

    return c;
}



/**
 * Counts the selected entities in this container.
 */
int GV_EntityContainer::countSelected() {
    int c=0;

    for (GV_Entity* t=firstEntity(GV2::ResolveNone);
            t!=NULL;
            t=nextEntity(GV2::ResolveNone)) {

        if (t->isSelected()) {
            c++;
        }
    }

    return c;
}



/**
 * Adjusts the borders of this graphic (max/min values)
 */
void GV_EntityContainer::adjustBorders(GV_Entity* entity) {
    if (entity!=NULL && !entity->isUndone()) {
        // make sure a container is not empty (otherwise the border
        //   would get extended to 0/0):
        if (!entity->isContainer() || entity->count()>0) {
            minV = GV_Vector::minimum(entity->getMin(),minV);
            maxV = GV_Vector::maximum(entity->getMax(),maxV);
        }
    }
}


/**
 * Recalculates the borders of this entity container.
 */
void GV_EntityContainer::calculateBorders(bool visibleOnly) {
    // optimization 20071205:
    if (!isUpdateEnabled()) {
        return;
    }

    resetBorders();
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {

        if (!e->isUndone()) {
            if (!visibleOnly || e->isVisible()) {
                e->calculateBorders();
                adjustBorders(e);
            }
        }
    }

    // needed for correcting corrupt data (PLANS.dxf)
    if (minV.x>maxV.x || minV.x>GV_MAXDOUBLE || maxV.x>GV_MAXDOUBLE
            || minV.x<GV_MINDOUBLE || maxV.x<GV_MINDOUBLE) {

        minV.x = 0.0;
        maxV.x = 0.0;
    }
    if (minV.y>maxV.y || minV.y>GV_MAXDOUBLE || maxV.y>GV_MAXDOUBLE
            || minV.y<GV_MINDOUBLE || maxV.y<GV_MINDOUBLE) {

        minV.y = 0.0;
        maxV.y = 0.0;
    }
        
//    RB_DEBUG->print("GV_EntityContainer::calculateBorders: "
//        "%f/%f - %f/%f", minV.x, minV.y, maxV.x, maxV.y);
}



/**
 * Updates all Dimension entities in this container and 
 * reposition their labels if autoText is true.
 * If onlySelected is true, only selected dimensions will
 * be modified, or all if nothing is selected.
 *
 * @return Number of entities that were updated.
 */
int GV_EntityContainer::updateDimensions(bool autoText, bool onlySelected) {

    RB_DEBUG->print("GV_EntityContainer::updateDimensions()");

    if (onlySelected && countSelected()==0) {
        onlySelected = false;
    }

    int num = 0;
    GV_PtrList<GV_Entity>::iterator e;
    for (e = entities.begin(); e != entities.end(); ++e) {
        if (!(*e)->isUndone()) {
            if (!onlySelected || (*e)->isSelected()) {
                if ((*e)->isOfType(GV2::EntityDimension)) {
                    (*e)->setSelected(false);
                    // update and reposition label:
                    dynamic_cast<GV_Dimension*>(*e)->update(autoText);
                    num++;
                } else if ((*e)->rtti()==GV2::EntityDimLeader) {
                    (*e)->setSelected(false);
                    dynamic_cast<GV_Leader*>(*e)->update();
                    num++;
                } else if ((*e)->isContainer()) {
                    num += dynamic_cast<GV_EntityContainer*>(*e)->
                        updateDimensions(autoText, onlySelected);
                }
            }
        }
    }

    RB_DEBUG->print("GV_EntityContainer::updateDimensions() OK");

    return num;
}



/**
 * Updates all Insert entities in this container.
 */
void GV_EntityContainer::updateInserts() {

    RB_DEBUG->print("GV_EntityContainer::updateInserts()");

    //for (GV_Entity* e=firstEntity(GV2::ResolveNone);
    //        e!=NULL;
    //        e=nextEntity(GV2::ResolveNone)) {
    
    GV_PtrList<GV_Entity>::iterator e;
    for (e = entities.begin(); e != entities.end(); ++e) {
        //// Only update our own inserts and not inserts of inserts
        if ((*e)->rtti()==GV2::EntityInsert) {
            dynamic_cast<GV_Insert*>(*e)->update();
        } else if ((*e)->isContainer() && 
            (*e)->rtti()!=GV2::EntityHatch) {
            dynamic_cast<GV_EntityContainer*>(*e)->updateInserts();
        }
    }

    RB_DEBUG->print("GV_EntityContainer::updateInserts() OK");
}



/**
 * Renames all inserts with name 'oldName' to 'newName'. This is
 *   called after a block was rename to update the inserts.
 */
void GV_EntityContainer::renameInserts(const QString& oldName,
                                       const QString& newName) {
    RB_DEBUG->print("GV_EntityContainer::renameInserts()");

    GV_PtrList<GV_Entity>::iterator e;
    for (e = entities.begin(); e != entities.end(); ++e) {
        if ((*e)->rtti()==GV2::EntityInsert) {
            GV_Insert* i = dynamic_cast<GV_Insert*>(*e);
            if (i->getName()==oldName) {
                i->setName(newName);
            }
        } else if ((*e)->isContainer()) {
            dynamic_cast<GV_EntityContainer*>(*e)->renameInserts(oldName, newName);
        }
    }

    RB_DEBUG->print("GV_EntityContainer::renameInserts() OK");

}



/**
 * Updates all Spline entities in this container.
 */
void GV_EntityContainer::updateSplines() {

    RB_DEBUG->print("GV_EntityContainer::updateSplines()");

    GV_PtrList<GV_Entity>::iterator e;
    for (e = entities.begin(); e != entities.end(); ++e) {
        if ((*e)->rtti()==GV2::EntitySpline) {
            dynamic_cast<GV_Spline*>(*e)->update();
        } else if ((*e)->isContainer() && (*e)->rtti()!=GV2::EntityHatch) {
            dynamic_cast<GV_EntityContainer*>(*e)->updateSplines();
        }
    }

    RB_DEBUG->print("GV_EntityContainer::updateSplines() OK");
}


/**
 * Updates the sub entities of this container. 
 */
void GV_EntityContainer::update() {
    RB_DEBUG->print("GV_EntityContainer::update");
    GV_PtrList<GV_Entity>::iterator e;
    for (e = entities.begin(); e != entities.end(); ++e) {
        (*e)->update();
    }
}



/**
 * Returns the first entity or NULL if this graphic is empty.
 * @param level 
 */
GV_Entity* GV_EntityContainer::firstEntity(GV2::ResolveLevel level) {
    switch (level) {
    case GV2::ResolveNone:
        return entities.first();
        break;

    case GV2::ResolveAll: {
            subContainer=NULL;
            GV_Entity* e = entities.first();
            if (e!=NULL && e->isContainer()) {
                subContainer = dynamic_cast<GV_EntityContainer*>(e);
                e = subContainer->firstEntity(level);
                // emtpy container:
                if (e==NULL) {
                    subContainer = NULL;
                    e = nextEntity(level);
                }
            }
            return e;
        }
        break;
    }

    return NULL;
}



/**
 * Returns the last entity or \p NULL if this graphic is empty.
 *
 * @param level \li \p 0 Groups are not resolved
 *              \li \p 1 (default) only Groups are resolved
 *              \li \p 2 all Entity Containers are resolved
 */
GV_Entity* GV_EntityContainer::lastEntity(GV2::ResolveLevel level) {
    switch (level) {
    case GV2::ResolveNone:
        return entities.last();
        break;

    case GV2::ResolveAll: {
            GV_Entity* e = entities.last();
            subContainer = NULL;
            if (e!=NULL && e->isContainer()) {
                subContainer = dynamic_cast<GV_EntityContainer*>(e);
                e = dynamic_cast<GV_EntityContainer*>(e)->lastEntity(level);
            }
            return e;
        }
        break;
    }

    return NULL;
}



/**
 * Returns the next entity or container or \p NULL if the last entity 
 * returned by \p next() was the last entity in the container.
 */
GV_Entity* GV_EntityContainer::nextEntity(GV2::ResolveLevel level) {
    switch (level) {
    case GV2::ResolveNone:
        return entities.next();
        break;

    case GV2::ResolveAll: {
            GV_Entity* e=NULL;
            if (subContainer!=NULL) {
                e = subContainer->nextEntity(level);
                if (e!=NULL) {
                    return e;
                } else {
                    e = entities.next();
                }
            } else {
                e = entities.next();
            }
            if (e!=NULL && e->isContainer()) {
                subContainer = dynamic_cast<GV_EntityContainer*>(e);
                e = dynamic_cast<GV_EntityContainer*>(e)->firstEntity(level);
                // emtpy container:
                if (e==NULL) {
                    subContainer = NULL;
                    e = nextEntity(level);
                }
            }
            return e;
        }
        break;
    }
    return NULL;
}



/**
 * Returns the prev entity or container or \p NULL if the last entity 
 * returned by \p prev() was the first entity in the container.
 */
GV_Entity* GV_EntityContainer::prevEntity(GV2::ResolveLevel level) {
    switch (level) {

    case GV2::ResolveNone:
        return entities.prev();
        break;
    
    case GV2::ResolveAll: {
            GV_Entity* e=NULL;
            if (subContainer!=NULL) {
                e = subContainer->prevEntity(level);
                if (e!=NULL) {
                    return e;
                } else {
                    e = entities.prev();
                }
            } else {
                e = entities.prev();
            }
            if (e!=NULL && e->isContainer()) {
                subContainer = dynamic_cast<GV_EntityContainer*>(e);
                e = dynamic_cast<GV_EntityContainer*>(e)->lastEntity(level);
                // emtpy container:
                if (e==NULL) {
                    subContainer = NULL;
                    e = prevEntity(level);
                }
            }
            return e;
        }
    }
    return NULL;
}



/**
 * @return Entity at the given index or NULL if the index is out of range.
 */
GV_Entity* GV_EntityContainer::entityAt(int index) {
    entities.setCurrentIndex(index);
    return entities[index];
}



/**
 * @return Current index.
 */
int GV_EntityContainer::entityAt() {
    return entities.currentIndex();
}


/**
 * Finds the given entity and makes it the current entity if found.
 */
int GV_EntityContainer::findEntity(GV_Entity* entity) {
    if (entity!=NULL) {
        entities.setCurrent(entity);
        return entities.currentIndex();
    }
    else {
        return -1;
    }
}



/**
 * @return The current entity.
 */
GV_Entity* GV_EntityContainer::currentEntity() {
    return entities.current();
}


/**
 * Returns the copy to a new iterator for traversing the entities.
 */
GV_MutableListIterator<GV_Entity*> GV_EntityContainer::createIterator() {
    return GV_MutableListIterator<GV_Entity*>(entities);
}



/**
 * @return The point which is closest to 'coord' 
 * (one of the vertexes)
 */
GV_Vector GV_EntityContainer::getNearestEndpoint(const GV_Vector& coord,
        double* dist) {

    double minDist = GV_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    GV_Vector closestPoint(false);  // closest found endpoint
    GV_Vector point;                // endpoint found

    for (GV_Entity* en = firstEntity();
            en != NULL;
            en = nextEntity()) {

        if (en->isVisible()) {
            point = en->getNearestEndpoint(coord, &curDist);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist!=NULL) {
                    *dist = curDist;
                }
            }
        }
    }

    return closestPoint;
}



GV_Vector GV_EntityContainer::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity, double* dist, GV_Entity** entity) {

    GV_Vector point(false);

    GV_Entity* e = getNearestEntity(coord, dist, GV2::ResolveNone);

    if (e!=NULL && e->isVisible()) {
        point = e->getNearestPointOnEntity(coord, onEntity, dist, entity);
    }

    return point;
}



GV_Vector GV_EntityContainer::getNearestCenter(const GV_Vector& coord,
        double* dist) {

    GV_Vector point(false);
    GV_Entity* closestEntity;

    //closestEntity = getNearestEntity(coord, NULL, GV2::ResolveAll);
    closestEntity = getNearestEntity(coord, NULL, GV2::ResolveNone);

    if (closestEntity!=NULL) {
        point = closestEntity->getNearestCenter(coord, dist);
    }

    return point;
}



GV_Vector GV_EntityContainer::getNearestMiddle(const GV_Vector& coord,
        double* dist) {

    GV_Vector point(false);
    GV_Entity* closestEntity;

    closestEntity = getNearestEntity(coord, NULL, GV2::ResolveNone);

    if (closestEntity!=NULL) {
        point = closestEntity->getNearestMiddle(coord, dist);
    }

    return point;


    /*
       double minDist = GV_MAXDOUBLE;  // minimum measured distance
       double curDist;                 // currently measured distance
       GV_Vector closestPoint;         // closest found endpoint
       GV_Vector point;                // endpoint found

       for (GV_Entity* en = firstEntity();
               en != NULL;
               en = nextEntity()) {

           if (en->isVisible()) {
               point = en->getNearestMiddle(coord, &curDist);
               if (curDist<minDist) {
                   closestPoint = point;
                   minDist = curDist;
                   if (dist!=NULL) {
                       *dist = curDist;
                   }
               }
           }
       }

       return closestPoint;
    */
}



GV_Vector GV_EntityContainer::getNearestDist(double distance,
        const GV_Vector& coord,
        double* dist) {

    GV_Vector point(false);
    GV_Entity* closestEntity;

    closestEntity = getNearestEntity(coord, NULL, GV2::ResolveNone);

    if (closestEntity!=NULL) {
        point = closestEntity->getNearestDist(distance, coord, dist);
    }

    return point;
}



/**
 * @return The intersection which is closest to 'coord' 
 */
GV_Vector GV_EntityContainer::getNearestIntersection(const GV_Vector& coord,
        double* dist) {

    double minDist = GV_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    GV_Vector closestPoint(false); // closest found endpoint
    GV_Vector point;                // endpoint found
    GV_VectorList sol;
    GV_Entity* closestEntity;

    closestEntity = getNearestEntity(coord, NULL, GV2::ResolveAll);

    if (closestEntity!=NULL) {
        for (GV_Entity* en = firstEntity(GV2::ResolveAll);
                en != NULL;
                en = nextEntity(GV2::ResolveAll)) {

            if (en->isVisible() && en!=closestEntity) {
                sol = GV_Information::getIntersection(closestEntity,
                                                      en,
                                                      true);

                for (int i=0; i<4; i++) {
                    point = sol.get(i);
                    if (point.valid) {
                        curDist = coord.distanceTo(point);

                        if (curDist<minDist) {
                            closestPoint = point;
                            minDist = curDist;
                            if (dist!=NULL) {
                                *dist = curDist;
                            }
                        }
                    }
                }
            }
        }
        //}
    }

    return closestPoint;
}



GV_Vector GV_EntityContainer::getNearestRef(const GV_Vector& coord,
        double* dist, GV2::RefMode refMode) {

    double minDist = GV_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    GV_Vector closestPoint(false);  // closest found endpoint
    GV_Vector point;                // endpoint found

    for (GV_Entity* en = firstEntity();
            en != NULL;
            en = nextEntity()) {

        if (en->isVisible()) {
            point = en->getNearestRef(coord, &curDist, refMode);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist!=NULL) {
                    *dist = curDist;
                }
            }
        }
    }

    return closestPoint;
}


GV_Vector GV_EntityContainer::getNearestSelectedRef(const GV_Vector& coord,
        double* dist, GV2::RefMode refMode) {

    RB_DEBUG->print("GV_EntityContainer::getNearestSelectedRef");

    double minDist = GV_MAXDOUBLE;  // minimum measured distance
    double curDist;                 // currently measured distance
    GV_Vector closestPoint(false);  // closest found endpoint
    GV_Vector point;                // endpoint found

    for (GV_Entity* en=firstEntity();
        en!=NULL;
        en=nextEntity()) {

        if (en->isVisible() && en->isSelected() && !en->isTemp()) {
            point = en->getNearestSelectedRef(coord, &curDist, refMode);
            if (point.valid && curDist<minDist) {
                closestPoint = point;
                minDist = curDist;
                if (dist!=NULL) {
                    *dist = curDist;
                }
            }
        }
    }

    return closestPoint;
}


double GV_EntityContainer::getDistanceToPoint(const GV_Vector& coord,
        GV_Entity** entity,
        GV2::ResolveLevel level,
        double solidDist,
        bool visually) {

    RB_DEBUG->print("GV_EntityContainer::getDistanceToPoint");

    double minDist = GV_MAXDOUBLE;      // minimum measured distance
    double curDist;                     // currently measured distance
    GV_Entity* closestEntity = NULL;    // closest entity found
    GV_Entity* subEntity = NULL;

    for (GV_Entity* e = firstEntity(level);
            e != NULL;
            e = nextEntity(level)) {

        if (e->isVisible()) {
            curDist = e->getDistanceToPoint(coord, &subEntity, level, solidDist, visually);

            if (curDist<minDist) {
                if (level!=GV2::ResolveAll) {
                    closestEntity = e;
                } else {
                    closestEntity = subEntity;
                }
                minDist = curDist;
            }
        }
    }

    if (entity!=NULL) {
        *entity = closestEntity;
    }
    
//    RB_DEBUG->print("GV_EntityContainer::getDistanceToPoint: %f", minDist);

    return minDist;
}



GV_Entity* GV_EntityContainer::getNearestEntity(const GV_Vector& coord,
        double* dist,
        GV2::ResolveLevel level,
        bool visually) {

    RB_DEBUG->print("GV_EntityContainer::getNearestEntity");

    GV_Entity* e = NULL;

    // distance for points inside solids:
    double solidDist = GV_MAXDOUBLE;
    if (dist!=NULL) {
        solidDist = *dist;
    }

    double d = getDistanceToPoint(coord, &e, level, solidDist, visually);
    
//    RB_DEBUG->print("GV_EntityContainer::getNearestEntity: dist: %f", d);

    if (e!=NULL && e->isVisible()==false) {
        e = NULL;
    }

    // if d is negative, use the default distance (used for points inside solids)
    if (dist!=NULL) {
        *dist = d;
    }
    RB_DEBUG->print("GV_EntityContainer::getNearestEntity: OK");

    return e;
}



/**
 * Rearranges the atomic entities in this container in a way that connected
 * entities are stored in the right order and direction.
 * Non-recoursive. Only affects atomic entities in this container.
 * 
 * @retval true all contours were closed
 * @retval false at least one contour is not closed
 */
bool GV_EntityContainer::optimizeContours() {

    RB_DEBUG->print("GV_EntityContainer::optimizeContours");

    GV_Vector current(false);
    GV_Vector start(false);
    GV_EntityContainer tmp;

//    bool changed = false;
    bool closed = true;

    for (int ci=0; ci<count(); ++ci) {
        GV_Entity* e1=entityAt(ci);

        if (e1!=NULL && e1->isEdge() && !e1->isContainer() &&
                !e1->isProcessed()) {

            GV_AtomicEntity* ce = dynamic_cast<GV_AtomicEntity*>(e1);

            // next contour start:
            ce->setProcessed(true);
            tmp.addEntity(ce->clone());
            current = ce->getEndpoint();
            start = ce->getStartpoint();

            // find all connected entities:
            bool done;
            do {
                done = true;
                for (int ei=0; ei<count(); ++ei) {
                    GV_Entity* e2=entityAt(ei);

                    if (e2!=NULL && e2->isEdge() && !e2->isContainer() &&
                            !e2->isProcessed()) {

                        GV_AtomicEntity* e = dynamic_cast<GV_AtomicEntity*>(e2);

                        if (e->getStartpoint().distanceTo(current) <
                                1.0e-4) {

                            e->setProcessed(true);
                            tmp.addEntity(e->clone());
                            current = e->getEndpoint();

                            done=false;
                        } else if (e->getEndpoint().distanceTo(current) <
                                   1.0e-4) {

                            e->setProcessed(true);
                            GV_AtomicEntity* cl = dynamic_cast<GV_AtomicEntity*>(e->clone());
                            cl->reverse();
                            tmp.addEntity(cl);
                            current = cl->getEndpoint();

//                            changed = true;
                            done=false;
                        }
                    }
                }
//                if (!done) {
//                    changed = true;
//                }
            } while (!done);

            if (current.distanceTo(start)>1.0e-4) {
                closed = false;
            }
        }
    }

    // remove all atomic entities:
    bool done;
    do {
        done = true;
        for (GV_Entity* en=firstEntity(); en!=NULL; en=nextEntity()) {
            if (!en->isContainer()) {
                removeEntity(en);
                done = false;
                break;
            }
        }
    } while (!done);

    // add new sorted entities:
    for (GV_Entity* en=tmp.firstEntity(); en!=NULL; en=tmp.nextEntity()) {
        en->setProcessed(false);
        addEntity(en->clone());
    }

    RB_DEBUG->print("GV_EntityContainer::optimizeContours: OK");
    return closed;
}

    

/**
 * @return The orientation (cw, ccw) of the contour in this container.
 * This container is expected to contain exactly one closed contour
 * (e.g. a loop of a hatch).
 */
GV2::ContourOrientation GV_EntityContainer::getContourOrientation(bool* ok) {
    GV_Vector start(false);
    GV_Vector lastEnd(false);
    double area = 0.0;
                
    if (ok!=NULL) {
        *ok = true;
    }
    
    for (GV_Entity* e=firstEntity(); e!=NULL; e=nextEntity()) {
        if (e->isAtomic()) {
            GV_AtomicEntity* ae = dynamic_cast<GV_AtomicEntity*>(e);
    
            if (!lastEnd.valid || lastEnd.distanceTo(ae->getStartpoint()) < 1.0e-6) {
                if (!lastEnd.valid) {
                    start = ae->getStartpoint();
                }
                lastEnd = ae->getEndpoint();
                
                area += ae->getStartpoint().x * ae->getEndpoint().y - 
                        ae->getStartpoint().y * ae->getEndpoint().x;
            }
            else {
                if (ok!=NULL) {
                    RB_DEBUG->print("GV_EntityContainer::getContourOrientation: failed");
                    *ok = false;
                }
                break;
            }
        }
    }

    if (lastEnd.distanceTo(start) > 1.0e-6) {
        if (ok!=NULL) {
            RB_DEBUG->print("GV_EntityContainer::getContourOrientation: failed");
            *ok = false;
        }
    }

//    RB_DEBUG->print("GV_EntityContainer::getContourOrientation: area: %f", area);

    if (area<0.0) {
        return GV2::ClockWise;
    }
    else {
        return GV2::CounterClockWise;
    }
}



bool GV_EntityContainer::hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2) {
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        if (e->hasEndpointsWithinWindow(v1, v2))  {
            return true;
        }
    }

    return false;
}
    
    

/**
 * Reverses every individual entity in this container. Does NOT
 * reverse the order of those entities!
 */
void GV_EntityContainer::reverse() {
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        if (e->isAtomic()) {
            dynamic_cast<GV_AtomicEntity*>(e)->reverse();
        }
        else if (e->isContainer()) {
            dynamic_cast<GV_EntityContainer*>(e)->reverse();
        }
    }
}



void GV_EntityContainer::reverseOrder() {
    GV_PtrList<GV_Entity> tmp;

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        tmp.prepend(e);
    }

    bool autoDel = entities.autoDelete();
    entities.setAutoDelete(false);
    entities.clear();
    entities.setAutoDelete(autoDel);

    for (GV_Entity* e=tmp.first(); e!=NULL; e=tmp.next()) {
        entities.append(e);
    }
}



void GV_EntityContainer::move(const GV_Vector& offset) {
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->move(offset);
    }
    if (autoUpdateBorders) {
        calculateBorders();
    }
}



void GV_EntityContainer::rotate(const GV_Vector& center, double angle) {
    GV_Entity* entityCursor = currentEntity();
    
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->rotate(center, angle);
    }
    if (autoUpdateBorders) {
        calculateBorders();
    }
    findEntity(entityCursor);
}



void GV_EntityContainer::scale(const GV_Vector& center, const GV_Vector& factor) {
    RB_DEBUG->print("GV_EntityContainer::scale");

    if (fabs(factor.x)>GV_TOLERANCE && fabs(factor.y)>GV_TOLERANCE) {
        RB_DEBUG->print("GV_EntityContainer::scale: scaling entities");
        for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {
            e->scale(center, factor);
        }
    }
    if (autoUpdateBorders) {
        RB_DEBUG->print("GV_EntityContainer::scale: calculate borders");
        calculateBorders();
    }
    
    RB_DEBUG->print("GV_EntityContainer::scale: OK");
}



void GV_EntityContainer::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    if (axisPoint1.distanceTo(axisPoint2)>1.0e-6) {
        for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {
            e->mirror(axisPoint1, axisPoint2);
        }
    }
}


void GV_EntityContainer::stretch(const GV_Vector& firstCorner,
                                 const GV_Vector& secondCorner,
                                 const GV_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    } else {
        for (GV_Entity* e=firstEntity(GV2::ResolveNone);
                e!=NULL;
                e=nextEntity(GV2::ResolveNone)) {
            e->stretch(firstCorner, secondCorner, offset);
        }
    }

    // some entitiy containers might need an update (e.g. GV_Leader):
    update();
}



void GV_EntityContainer::moveRef(const GV_Vector& ref,
                                 const GV_Vector& offset) {

    RB_DEBUG->print("GV_EntityContainer::moveRef");

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        if (!e->isUndone() && !e->getFlag(GV2::FlagTemp)) {
            e->moveRef(ref, offset);
        }
    }
    // 20080212:
    //update();
    if (autoUpdateBorders) {
        calculateBorders();
    }
}


void GV_EntityContainer::moveSelectedRef(const GV_Vector& ref,
        const GV_Vector& offset) {

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e=nextEntity(GV2::ResolveNone)) {
        e->moveSelectedRef(ref, offset);
    }
    if (autoUpdateBorders) {
        calculateBorders();
    }
}
    
    
    
void GV_EntityContainer::isoProject(GV2::IsoProjectionType type, double segmentLength) {

    for (int i=0; i<entities.count(); ++i) {
        GV_Entity* e = entities.at(i);

        if (e->rtti()==GV2::EntityArc) {
            GV_Arc* a = dynamic_cast<GV_Arc*>(e);
            GV_Polyline* pl = a->approximateWithLines(segmentLength);
            pl->isoProject(type, segmentLength);
            entities.replace(i, pl);
        }
        else if (e->rtti()==GV2::EntityCircle) {
            GV_Circle* c = dynamic_cast<GV_Circle*>(e);
            GV_Arc a(NULL, GV_ArcData(c->getCenter(), c->getRadius(), 0.0, 2*M_PI, false));
            a.copyPropertiesFrom(c);
            GV_Polyline* pl = a.approximateWithLines(segmentLength);
            pl->isoProject(type, segmentLength);
            entities.replace(i, pl);
        }
        else {
            e->isoProject(type, segmentLength);
        }
    }

    if (autoUpdateBorders) {
        calculateBorders();
    }
}



void GV_EntityContainer::draw(GV_Painter* painter, GV_GraphicView* view,
                              double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }

    RB_DEBUG->print("GV_EntityContainer::draw");

    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone)) {

        view->drawEntity(e);
    }
    
    for (GV_Entity* e=firstEntity(GV2::ResolveNone);
            e!=NULL;
            e = nextEntity(GV2::ResolveNone)) {

        view->drawEntityHandles(e);
    }
    
    RB_DEBUG->print("GV_EntityContainer::draw: OK");
}


void GV_EntityContainer::changeProperty(const QString& propertyName, 
        const QVariant& propertyValue) {
        
//    GV_Modification m(*this, NULL);
//    m.changeProperty(propertyName, propertyValue);
}
