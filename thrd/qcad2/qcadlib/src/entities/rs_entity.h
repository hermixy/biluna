/****************************************************************************
** $Id: rs_entity.h 9301 2008-03-03 14:14:57Z andrew $
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


#ifndef RS_ENTITY_H
#define RS_ENTITY_H

#include "cad_global.h"
#include "rs.h"
#include "rs_application.h"
#include "rs_hash.h"
#include "rs_layer.h"
#include "rs_math.h"
#include "rs_pen.h"
#include "rs_propertyowner.h"
#include "rs_string.h"
#include "rs_stringlist.h"
#include "rs_undoable.h"
#include "rs_variant.h"
#include "rs_vector.h"

class RS_Block;
class RS_Document;
class RS_EntityContainer;
class RS_Graphic;
class RS_GraphicView;
class RS_Insert;
class RS_Line;
class RS_Painter;


#define RS_ENTITY_TYPE   QObject::tr(RS_Entity::propertyType)
#define RS_ENTITY_LAYER  QObject::tr(RS_Entity::propertyLayer)

#ifdef RS_ARCH
#define RS_ENTITY_FLOOR  QObject::tr(RS_Entity::propertyFloor)
#endif
/*
#define RS_ENTITY_PEN_COLOR  QObject::tr(RS_Entity::propertyPenColor)
#define RS_ENTITY_PEN_WIDTH  QObject::tr(RS_Entity::propertyPenWidth)
#define RS_ENTITY_PEN_LINE_TYPE  QObject::tr(RS_Entity::propertyPenLineType)
*/



/**
 * Base class for an entity (line, arc, circle, ...)
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Entity : public RS_Undoable, public RS_PropertyOwner {

public:
    RS_Entity(RS_EntityContainer* parentContainer);
    virtual ~RS_Entity();

    void init();
    virtual void initId();

    /**
     * @todo: add cloneFrom(RS_Entity* e); to make better use of
     * inheritance.
     */
    virtual RS_Entity* clone() = 0;
    virtual void copyPropertiesFrom(RS_Entity* e);

    virtual void reparent(RS_EntityContainer* parentContainer) {
        this->parentContainer = parentContainer;
    }

    /**
     * Reimplement to get notified if this entity is added to a container.
     */
    virtual void addedEvent(RS_EntityContainer* /*container*/) {}

    void resetBorders();

    /**
     * Must be overwritten to return the rtti of this entity
     * (e.g. RS2::EntityArc).
     */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityUnknown;
    }
    
    /**
     * Must be overwritten to return True if the entity is of the given
     * type or is derrived from it.
     */
    virtual bool isOfType(RS2::EntityType /*t*/) const {
        return false;
    }
    
    /**
     * Must be overwritten to return the name of this entity type (e.g. "Arc").
     */
    virtual RS_String typeName() const {
        return QObject::tr("Unknown");
    }

    /**
     * Identify all entities as undoable entities.
     * @return RS2::UndoableEntity
     */
    virtual RS2::UndoableType undoRtti() const {
        return RS2::UndoableEntity;
    }

    /**
     * @return Unique Id of this entity.
     */
    unsigned long int getId() const {
        return id;
    }

    /**
     * Overwrite this to return true if an entity type should always stay 
     * in the background of a drawing (e.g. slabs).
     */
    virtual bool alwaysPrepend() const {
        return false;
    }

    /**
     * This method must be overwritten in subclasses and return the 
     * number of <b>atomic</b> entities in this entity.
     */
    virtual int count() = 0;
    
    /**
     * This method must be overwritten in subclasses and return the 
     * number of <b>atomic</b> entities in this entity including sub containers.
     */
    virtual int countDeep() = 0;


    /**
     * Implementations must return the total length of the entity
     * or a negative number if the entity has no length (e.g. a text or hatch).
     */
    virtual double getLength() {
        return -1.0;
    }

    /**
     * @return Parent of this entity or NULL if this is a root entity.
     */
    RS_EntityContainer* getParentContainer() const {
        return parentContainer;
    }

    /**
     * Reparents this entity.
     */
    void setParentContainer(RS_EntityContainer* p) {
        parentContainer = p;
    }

    RS_Graphic* getGraphic();
    RS_Block* getBlock();
    RS_Insert* getInsert();
    RS_Entity* getBlockOrInsert();
    RS_Document* getDocument();

    void setLayer(const RS_String& name);
    void setLayer(RS_Layer* l);
    virtual void setLayerToActive();
    virtual RS_Layer* getLayer(bool resolve = true) const;

    /**
     * Sets the explicit pen for this entity or a pen with special
     * attributes such as BY_LAYER, ..
     */
    void setPen(const RS_Pen& pen) {
        this->pen = pen;
    }
    /**
     * Derrived entity classes need to reimplement this function
     * to set the given pen as only pen for all visualizaton of 
     * the entity. For example a wall entity might normally draw 
     * itself in a constant gray pen unless an override pen was
     * set. 
     * This is used to highlight entities on mouse over.
     */
    virtual void setOverridePen(const RS_Pen& pen) {
        setPen(pen);
    }
    void setPenToActive();
    RS_Pen getPen(bool resolve = true) const;

    /**
     * Must be overwritten to return true if an entity type 
     * is a container for other entities (e.g. polyline, group, ...).
     */
    virtual bool isContainer() const = 0;

    /**
     * Must be overwritten to return true if an entity type 
     * is an atomic entity.
     */
    virtual bool isAtomic() const = 0;
    
    /**
     * Must be overwritten to return true if an entity type 
     * is a potential edge entity of a contour. By default
     * this returns false.
     */
    virtual bool isEdge() const {
        return false;
    }

    /**
     * @return true for all document entities (e.g. Graphics or Blocks).
     * false otherwise.
     */
    virtual bool isDocument() const {
        return false;
    }

    virtual bool setSelected(bool select, bool updateDependent=true);
    virtual void setSelectionVisible(bool on);
    virtual bool toggleSelected();
    virtual bool isSelected() const;
    virtual bool isSelectionVisible() const;
    virtual bool isSelectedForPropertyEditing() {
        return (isSelected() && isSelectionVisible());
    }
    virtual bool isParentContainerSelected();
    virtual bool isProcessed() const;
    virtual void setProcessed(bool on);
    virtual bool isInWindow(const RS_Vector& v1, const RS_Vector& v2);
    virtual bool hasEndpointsWithinWindow(const RS_Vector& /*v1*/, 
                                          const RS_Vector& /*v2*/) {
        return false;
    }
    virtual bool isVisible();
    virtual void setVisible(bool v) {
        if (v) {
            setFlag(RS2::FlagVisible);
        } else {
            delFlag(RS2::FlagVisible);
        }
    }
    virtual void setHighlighted(bool on);
    virtual bool isHighlighted();
    
    virtual bool isLocked();

    virtual void undoStateChanged(bool undone);
    virtual bool isUndone() const;
    
    virtual bool isTemp() const;

    /**
     * Can be implemented by child classes to update the entities
     * temporary subentities. update() is called if the entity's
     * paramters or undo state changed.
     */
    virtual void update() {}
    
    virtual void setUpdateEnabled(bool on) {
        updateEnabled = on;
    }
    
    virtual bool isUpdateEnabled() {
        return updateEnabled;
    }

    /**
     * This method doesn't do any calculations. 
     * @return minimum coordinate of the entity.
     * @see calculateBorders()
     */
    RS_Vector getMin() const {
        return minV;
    }

    /**
     * This method doesn't do any calculations. 
     * @return minimum coordinate of the entity.
     * @see calculateBorders()
     */
    RS_Vector getMax() const {
        return maxV;
    }

    /**
     * This method returns the difference of max and min returned 
     * by the above functions. 
     * @return size of the entity.
     * @see calculateBorders()
     * @see getMin()
     * @see getMax()
     */
    RS_Vector getSize() const {
        return maxV-minV;
    }

    void addGraphicVariable(const RS_String& key, double val, int code);
    void addGraphicVariable(const RS_String& key, int val, int code);
    void addGraphicVariable(const RS_String& key, const RS_String& val, int code);
    
    double getGraphicVariableDouble(const RS_String& key, double def);
    int getGraphicVariableInt(const RS_String& key, int def);
    RS_String getGraphicVariableString(const RS_String& key,
                                       const RS_String& def);

    RS2::Unit getGraphicUnit();

    /**
     * Must be overwritten to get all reference points of the entity. 
     */
    virtual RS_VectorList getRefPoints(RS2::RefMode /*refMode*/=RS2::RefAll) {
        RS_VectorList ret;
        return ret;
    }
    
    /**
     * Must be overwritten to get the closest endpoint to the 
     * given coordinate for this entity. 
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest endpoint. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest endpoint.
     */
    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL) = 0;

    /**
     * Must be overwritten to get the closest coordinate to the 
    * given coordinate which is on this entity.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between \p coord and the point. The passed pointer can 
     * also be \p NULL in which case the distance will be lost.
     *
     * @return The closest coordinate.
     */
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& /*coord*/,
            bool onEntity = true, double* dist = NULL,
            RS_Entity** entity = NULL) = 0;

    /**
     * Must be overwritten to get the (nearest) center point to the 
     * given coordinate for this entity. 
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest center point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest center point.
     */
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL) = 0;

    /**
     * Must be overwritten to get the (nearest) middle point to the 
     * given coordinate for this entity. 
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest middle point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest middle point.
     */
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL) = 0;

    /**
     * Must be overwritten to get the nearest point with a given
     * distance to the endpoint to the given coordinate for this entity. 
     *
     * @param distance Distance to endpoint.
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL) = 0;
                                     
    /**
     * Must be overwritten to get the point with a given
     * distance to the start- or endpoint to the given coordinate for this entity. 
     *
     * @param distance Distance to endpoint.
     * @param startp true = measured from Startpoint, false = measured from Endpoint
     *
     * @return The point with the given distance to the start- or endpoint.
     */
    virtual RS_Vector getNearestDist(double /*distance*/,
                                     bool /*startp*/) {
        return RS_Vector(false);
    }
                                     
    /**
     * Must be overwritten to get the nearest reference point for this entity. 
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param dist Pointer to a value which will contain the measured
     * distance between 'coord' and the closest point. The passed
     * pointer can also be NULL in which case the distance will be
     * lost.
     *
     * @return The closest point with the given distance to the endpoint.
     */
    virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll) { 
        RS_VectorList s = getRefPoints(refMode);
        return s.getClosest(coord, dist); 
    }
    
    virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                     double* dist = NULL,
                                     RS2::RefMode refMode=RS2::RefAll);

    /**
     * Must be overwritten to get the shortest distance between this 
     * entity and a coordinate.
     *
     * @param coord Coordinate (typically a mouse coordinate)
     * @param entity Pointer which will contain the (sub-)entity which is 
     *               closest to the given point or NULL if the caller is not 
     *               interested in this information.
     * @param level The resolve level. 
     * @param visually: True measure the visual distance, not the geometrical one.
     *               Used for entities with a complex visual representation
     *               such as the RWBE entities walls, doors, ...
     *
     * @sa RS2::ResolveLevel
     *
     * @return The measured distance between \p coord and the entity.
     */
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity = NULL,
                                      RS2::ResolveLevel level = RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false) = 0;

    virtual bool isPointOnEntity(const RS_Vector& coord,
                                 double tolerance=RS_TOLERANCE);
    
    /**
     * Implementations must return RS2::RightHand if coord is on the
     * right hand side of this entity, RS2::LeftHand if it is on the
     * left hand side and RS2::NoSide otherwise (on entity, entity has
     * no side, ..).
     */
    virtual RS2::Side getSideOfPoint(const RS_Vector& coord) = 0;

    /**
     * Implementations must move the entity by the given vector.
     */
    virtual void move(const RS_Vector& offset) = 0;

    /**
     * Implementations must rotate the entity by the given angle around
     * the given center.
     */
    virtual void rotate(const RS_Vector& center, double angle) = 0;

    /**
     * Implementations must scale the entity by the given factors.
     */
    virtual void scale(const RS_Vector& center, const RS_Vector& factor) = 0;

    /**
     * Acts like scale(RS_Vector) but with equal factors.
     * Equal to scale(center, RS_Vector(factor, factor)).
     */
    virtual void scale(const RS_Vector& center, double factor) {
        scale(center, RS_Vector(factor, factor, factor));
    }

    /**
     * Implementations must mirror the entity by the given axis.
     */
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) = 0;

    virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);
    
    /**
     * Implementations must create an isometric projection with the given perspective of the entity.
     */
    virtual void isoProject(RS2::IsoProjectionType /*type*/, double /*segmentLength*/) {}

    /**
     * Implementations must drag the reference point(s) of all
     * (sub-)entities that are very close to ref by offset.
     */
    virtual void moveRef(const RS_Vector& /*ref*/, 
        const RS_Vector& /*offset*/) {
        return;
    }
    
    /**
     * Implementations must drag the reference point(s) of selected 
     * (sub-)entities that are very close to ref by offset.
     */
    virtual void moveSelectedRef(const RS_Vector& /*ref*/, 
        const RS_Vector& /*offset*/) {
        return;
    }

    /**
     * Implementations must draw the entity on the given device.
     */
    virtual void draw(RS_Painter* painter, RS_GraphicView* view, 
        double patternOffset = 0.0) = 0;
    /**
     * Implementations might want to polish the entity after 
     * all other entities have been drawn.
     */
    /*
    virtual void draw2(RS_Painter* / *painter* /, RS_GraphicView* / *view* /, 
        double / *patternOffset* / = 0.0) {}
    */

    double getStyleFactor(RS_GraphicView* view);
    
#ifdef RS_COMPAT
    RS_String* getUserDefVar(const RS_String& key);
    RS_StringList getAllKeys();
    void setUserDefVar(const RS_String& key, const RS_String& val);
    void delUserDefVar(const RS_String& key);
#endif

    virtual void setProperty(const RS_String& name, const RS_Variant& value);
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    //virtual RS_StringList getPropertyChoices(const RS_String& name);
    virtual RS_PropertyAttributes getPropertyAttributes(
        const RS_String& propertyName);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false) = 0;
    
    double getGraphicVariable(const RS_String& key, double defMM, int code);

protected:
    //! Entity's parent entity or NULL if this entity has no parent.
    RS_EntityContainer* parentContainer;
    //! minimum coordinates
    RS_Vector minV;
    //! maximum coordinates
    RS_Vector maxV;

    //! Pointer to layer
    RS_Layer* layer;

    //! Entity id
    unsigned long int id;

    //! pen (attributes) for this entity
    RS_Pen pen;

    //! auto updating enabled?
    bool updateEnabled;

private:
#ifdef RS_COMPAT
    //! OBSOLETE. Use properties instead.
    RS_Hash<RS_String, RS_String> varList;
#endif

public:
    static const char* propertyType;
    static const char* propertyLayer;
#ifdef RS_ARCH
    static const char* propertyFloor;
#endif
};

#endif
