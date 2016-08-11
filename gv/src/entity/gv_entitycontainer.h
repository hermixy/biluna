/****************************************************************************
** $Id: gv_entitycontainer.h 9301 2008-03-03 14:14:57Z andrew $
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


#ifndef GV_ENTITYCONTAINER_H
#define GV_ENTITYCONTAINER_H

#include "gv_arc.h"
#include "gv_circle.h"
#include "gv_ellipse.h"
#include "gv_entity.h"
#include "gv_line.h"
#include "gv_point.h"
#include "gv_ptrlist.h"
#include "gv_propertyownercontainerinterface.h"

/**
 * Class representing a tree of entities.
 * Typical entity containers are graphics, polylines, groups, texts, ...)
 *
 * @author Andrew Mustun
 */
class GV_EntityContainer : public virtual GV_Entity, public GV_PropertyOwnerContainerInterface {

public:

    GV_EntityContainer(GV_EntityContainer* parentContainer=NULL, bool owner=true);
    virtual ~GV_EntityContainer();

    virtual GV_Entity* clone();
    virtual void detach();

    /** @return GV2::EntityContainer */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityContainer;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityContainer || GV_Entity::isOfType(t));
    }
    
    virtual QString typeName() const {
        return QObject::tr("Container");
    }
    
    virtual void reparent(GV_EntityContainer* parentContainer);

    /**
     * @return true: because entities made from this class
     *         and subclasses are containers for other entities.
     */
    virtual bool isContainer() const {
        return true;
    }
    
    /**
     * @return false: because entities made from this class
     *         and subclasses are containers for other entities.
     */
    virtual bool isAtomic() const {
        return false;
    }
    
    virtual void setOverridePen(const GV_Pen& pen);
    virtual void setLayerToActive();
    
    virtual double getLength();

    virtual void undoStateChanged(bool undone);
    virtual void setVisible(bool v);

    virtual void setProcessed(bool on);
    
    virtual bool setSelected(bool select=true, bool updateDependent=true);
    virtual void setSelectionVisible(bool on);
    virtual bool toggleSelected();

    virtual void selectWindow(const GV_Vector& v1, const GV_Vector& v2, 
        bool select=true);

    virtual void addEntity(GV_Entity* entity);
    virtual void insertEntity(int index, GV_Entity* entity);
    virtual void replaceEntity(int index, GV_Entity* entity);
    virtual bool removeEntity(GV_Entity* entity);
    virtual void moveEntityToBack(GV_Entity* entity, bool toBack = true);
    virtual void moveEntityToFront(GV_Entity* entity);
    virtual GV_Entity* firstEntity(GV2::ResolveLevel level=GV2::ResolveNone);
    virtual GV_Entity* lastEntity(GV2::ResolveLevel level=GV2::ResolveNone);
    virtual GV_Entity* nextEntity(GV2::ResolveLevel level=GV2::ResolveNone);
    virtual GV_Entity* prevEntity(GV2::ResolveLevel level=GV2::ResolveNone);
    virtual GV_Entity* entityAt(int index);
    virtual GV_Entity* currentEntity();
    virtual int entityAt();
    virtual int findEntity(GV_Entity* entity);
    virtual void clear();
    
    GV_MutableListIterator<GV_Entity*> createIterator();
    
    virtual bool isEmpty() {
        return count()==0;
    }
    virtual int count();
    virtual int countDeep();
    virtual int countSelected();

    /**
     * Enables / disables automatic update of borders on entity removals
     * and additions. By default this is turned on.
     */
    virtual void setAutoUpdateBorders(bool enable) {
        autoUpdateBorders = enable;
    }
    virtual void adjustBorders(GV_Entity* entity);
    virtual void calculateBorders(bool visibleOnly=false);
    //virtual void forcedCalculateBorders();
    virtual int updateDimensions(bool autoText=true, bool onlySelected=false);
    virtual void updateInserts();
    virtual void updateSplines();
    virtual void update();
    virtual void renameInserts(const QString& oldName, 
        const QString& newName);


    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);

    virtual GV_Entity* getNearestEntity(const GV_Vector& point,
                                double* dist = NULL,
                                GV2::ResolveLevel level=GV2::ResolveAll,
                                bool visually=false);

    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity = true,
            double* dist = NULL,
            GV_Entity** entity=NULL);

    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual GV_Vector getNearestIntersection(const GV_Vector& coord,
            double* dist = NULL);
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);
            
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector&) {
        return GV2::NoSide;
    }

    virtual bool optimizeContours();
    virtual GV2::ContourOrientation getContourOrientation(bool* ok=NULL);
    
    virtual bool hasEndpointsWithinWindow(const GV_Vector& v1, const GV_Vector& v2);

    virtual void reverse();
    virtual void reverseOrder();
    
    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    
    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void moveSelectedRef(const GV_Vector& ref, const GV_Vector& offset);
    virtual void isoProject(GV2::IsoProjectionType type, double segmentLength);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);
    
    // implementations from GV_PropertyOwnerContainerInterface:
    virtual GV_PropertyOwner* firstPropertyOwner() {
        return (GV_PropertyOwner*)firstEntity();
    }
    virtual GV_PropertyOwner* nextPropertyOwner() {
        return (GV_PropertyOwner*)nextEntity();
    }
    virtual void changeProperty(const QString& propertyName, 
        const QVariant& propertyValue);
    
    /*
    virtual GV_PropertyAttributes getPropertyAttributesOfChildren(const QString& propertyName) {
        return GV_PropertyOwnerContainerInterface::getPropertyAttributesOfChildren(propertyName);
    }
    */

protected:
    /** entities in the container */
    GV_PtrList<GV_Entity> entities;

    /** sub container used only temporarly for iteration. */
    GV_EntityContainer* subContainer;

    /**
     * Automatically update the borders of the container when entities
     * are added or removed. 
     */
    static bool autoUpdateBorders;
};


#endif
