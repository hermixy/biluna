/****************************************************************************
** $Id: rs_entitycontainer.h 9301 2008-03-03 14:14:57Z andrew $
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


#ifndef RS_ENTITYCONTAINER_H
#define RS_ENTITYCONTAINER_H

#include "rs_arc.h"
#include "rs_circle.h"
#include "rs_ellipse.h"
#include "rs_entity.h"
#include "rs_line.h"
#include "rs_point.h"
#include "rs_ptrlist.h"
#include "rs_propertyownercontainerinterface.h"

/**
 * Class representing a tree of entities.
 * Typical entity containers are graphics, polylines, groups, texts, ...)
 *
 * @author Andrew Mustun
 */
class RS_EntityContainer : public virtual RS_Entity, public RS_PropertyOwnerContainerInterface {

public:

    RS_EntityContainer(RS_EntityContainer* parentContainer=NULL, bool owner=true);
    virtual ~RS_EntityContainer();

    virtual RS_Entity* clone();
    virtual void detach();

    /** @return RS2::EntityContainer */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityContainer;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityContainer || RS_Entity::isOfType(t));
    }
    
    virtual RS_String typeName() const {
        return QObject::tr("Container");
    }
    
    virtual void reparent(RS_EntityContainer* parentContainer);

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
    
    virtual void setOverridePen(const RS_Pen& pen);
    virtual void setLayerToActive();
    
    virtual double getLength();

    virtual void undoStateChanged(bool undone);
    virtual void setVisible(bool v);

    virtual void setProcessed(bool on);
    
    virtual bool setSelected(bool select=true, bool updateDependent=true);
    virtual void setSelectionVisible(bool on);
    virtual bool toggleSelected();

    virtual void selectWindow(const RS_Vector& v1, const RS_Vector& v2, 
        bool select=true);

    virtual void addEntity(RS_Entity* entity);
    virtual void insertEntity(int index, RS_Entity* entity);
    virtual void replaceEntity(int index, RS_Entity* entity);
    virtual bool removeEntity(RS_Entity* entity);
    virtual void moveEntityToBack(RS_Entity* entity, bool toBack = true);
    virtual void moveEntityToFront(RS_Entity* entity);
    virtual RS_Entity* firstEntity(RS2::ResolveLevel level=RS2::ResolveNone);
    virtual RS_Entity* lastEntity(RS2::ResolveLevel level=RS2::ResolveNone);
    virtual RS_Entity* nextEntity(RS2::ResolveLevel level=RS2::ResolveNone);
    virtual RS_Entity* prevEntity(RS2::ResolveLevel level=RS2::ResolveNone);
    virtual RS_Entity* entityAt(int index);
    virtual RS_Entity* currentEntity();
    virtual int entityAt();
    virtual int findEntity(RS_Entity* entity);
    virtual void clear();
    
    RS_MutableListIterator<RS_Entity*> createIterator();
    
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
    virtual void adjustBorders(RS_Entity* entity);
    virtual void calculateBorders(bool visibleOnly=false);
    //virtual void forcedCalculateBorders();
    virtual int updateDimensions(bool autoText=true, bool onlySelected=false);
    virtual void updateInserts();
    virtual void updateSplines();
    virtual void update();
    virtual void renameInserts(const RS_String& oldName, 
        const RS_String& newName);


    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);

    virtual RS_Entity* getNearestEntity(const RS_Vector& point,
                                double* dist = NULL,
                                RS2::ResolveLevel level=RS2::ResolveAll,
                                bool visually=false);

    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity = true,
            double* dist = NULL,
            RS_Entity** entity=NULL);

    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual RS_Vector getNearestIntersection(const RS_Vector& coord,
            double* dist = NULL);
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);
            
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector&) {
        return RS2::NoSide;
    }

    virtual bool optimizeContours();
    virtual RS2::ContourOrientation getContourOrientation(bool* ok=NULL);
    
    virtual bool hasEndpointsWithinWindow(const RS_Vector& v1, const RS_Vector& v2);

    virtual void reverse();
    virtual void reverseOrder();
    
    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    
    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);
    virtual void moveSelectedRef(const RS_Vector& ref, const RS_Vector& offset);
    virtual void isoProject(RS2::IsoProjectionType type, double segmentLength);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);
    
    // implementations from RS_PropertyOwnerContainerInterface:
    virtual RS_PropertyOwner* firstPropertyOwner() {
        return (RS_PropertyOwner*)firstEntity();
    }
    virtual RS_PropertyOwner* nextPropertyOwner() {
        return (RS_PropertyOwner*)nextEntity();
    }
    virtual void changeProperty(const RS_String& propertyName, 
        const RS_Variant& propertyValue);
    
    /*
    virtual RS_PropertyAttributes getPropertyAttributesOfChildren(const RS_String& propertyName) {
        return RS_PropertyOwnerContainerInterface::getPropertyAttributesOfChildren(propertyName);
    }
    */

protected:
    /** entities in the container */
    RS_PtrList<RS_Entity> entities;

    /** sub container used only temporarly for iteration. */
    RS_EntityContainer* subContainer;

    /**
     * Automatically update the borders of the container when entities
     * are added or removed. 
     */
    static bool autoUpdateBorders;
};


#endif
