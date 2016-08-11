/****************************************************************************
** $Id: gv_entity.h 9301 2008-03-03 14:14:57Z andrew $
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


#ifndef GV_ENTITY_H
#define GV_ENTITY_H

#include "gv.h"
//#include "gv_application.h"
//#include "gv_hash.h"
#include "gv_layer.h"
#include "gv_math.h"
#include "gv_pen.h"
#include "gv_propertyowner.h"
#include <QStringList>
#include "gv_undoable.h"
#include <QVariant>
#include "gv_vector.h"
#include "rb_debug.h"

class GV_Block;
class GV_Document;
class GV_EntityContainer;
class GV_Graphic;
class GV_GraphicView;
class GV_Insert;
class GV_Line;
class GV_Painter;


#define GV_ENTITY_TYPE   QObject::tr(GV_Entity::propertyType)
#define GV_ENTITY_LAYER  QObject::tr(GV_Entity::propertyLayer)

#ifdef GV_ARCH
#define GV_ENTITY_FLOOR  QObject::tr(GV_Entity::propertyFloor)
#endif
/*
#define GV_ENTITY_PEN_COLOR  QObject::tr(GV_Entity::propertyPenColor)
#define GV_ENTITY_PEN_WIDTH  QObject::tr(GV_Entity::propertyPenWidth)
#define GV_ENTITY_PEN_LINE_TYPE  QObject::tr(GV_Entity::propertyPenLineType)
*/



/**
 * Base class for an entity (line, arc, circle, ...)
 *
 * @author Andrew Mustun
 */
class GV_Entity : public GV_Undoable, public GV_PropertyOwner {

public:
    GV_Entity(GV_EntityContainer* parentContainer);
    virtual ~GV_Entity();

    void init();
    virtual void initId();

    /**
     * @todo: add cloneFrom(GV_Entity* e); to make better use of
     * inheritance.
     */
    virtual GV_Entity* clone() = 0;
    virtual void copyPropertiesFrom(GV_Entity* e);

    virtual void reparent(GV_EntityContainer* parentContainer) {
        this->parentContainer = parentContainer;
    }

    /**
     * Reimplement to get notified if this entity is added to a container.
     */
    virtual void addedEvent(GV_EntityContainer* /*container*/) {}

    void resetBorders();

    /**
     * Must be overwritten to return the rtti of this entity
     * (e.g. GV2::EntityArc).
     */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityUnknown;
    }
    
    /**
     * Must be overwritten to return True if the entity is of the given
     * type or is derrived from it.
     */
    virtual bool isOfType(GV2::EntityType /*t*/) const {
        return false;
    }
    
    /**
     * Must be overwritten to return the name of this entity type (e.g. "Arc").
     */
    virtual QString typeName() const {
        return QObject::tr("Unknown");
    }

    /**
     * Identify all entities as undoable entities.
     * @return GV2::UndoableEntity
     */
    virtual GV2::UndoableType undoRtti() const {
        return GV2::UndoableEntity;
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
    GV_EntityContainer* getParentContainer() const {
        return parentContainer;
    }

    /**
     * Reparents this entity.
     */
    void setParentContainer(GV_EntityContainer* p) {
        parentContainer = p;
    }

    GV_Graphic* getGraphic();
    GV_Block* getBlock();
    GV_Insert* getInsert();
    GV_Entity* getBlockOrInsert();
    GV_Document* getDocument();

    void setLayer(const QString& name);
    void setLayer(GV_Layer* l);
    virtual void setLayerToActive();
    virtual GV_Layer* getLayer(bool resolve = true) const;

    /**
     * Sets the explicit pen for this entity or a pen with special
     * attributes such as BY_LAYER, ..
     */
    void setPen(const GV_Pen& pen) {
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
    virtual void setOverridePen(const GV_Pen& pen) {
        setPen(pen);
    }
    void setPenToActive();
    GV_Pen getPen(bool resolve = true) const;

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
    virtual bool isInWindow(const GV_Vector& v1, const GV_Vector& v2);
    virtual bool hasEndpointsWithinWindow(const GV_Vector& /*v1*/, 
                                          const GV_Vector& /*v2*/) {
        return false;
    }
    virtual bool isVisible();
    virtual void setVisible(bool v) {
        if (v) {
            setFlag(GV2::FlagVisible);
        } else {
            delFlag(GV2::FlagVisible);
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
    GV_Vector getMin() const {
        return minV;
    }

    /**
     * This method doesn't do any calculations. 
     * @return minimum coordinate of the entity.
     * @see calculateBorders()
     */
    GV_Vector getMax() const {
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
    GV_Vector getSize() const {
        return maxV-minV;
    }

    void addGraphicVariable(const QString& key, double val, int code);
    void addGraphicVariable(const QString& key, int val, int code);
    void addGraphicVariable(const QString& key, const QString& val, int code);
    
    double getGraphicVariableDouble(const QString& key, double def);
    int getGraphicVariableInt(const QString& key, int def);
    QString getGraphicVariableString(const QString& key,
                                       const QString& def);

    GV2::Unit getGraphicUnit();

    /**
     * Must be overwritten to get all reference points of the entity. 
     */
    virtual GV_VectorList getRefPoints(GV2::RefMode /*refMode*/=GV2::RefAll) {
        GV_VectorList ret;
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
    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
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
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& /*coord*/,
            bool onEntity = true, double* dist = NULL,
            GV_Entity** entity = NULL) = 0;

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
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
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
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
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
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
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
    virtual GV_Vector getNearestDist(double /*distance*/,
                                     bool /*startp*/) {
        return GV_Vector(false);
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
    virtual GV_Vector getNearestRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll) { 
        GV_VectorList s = getRefPoints(refMode);
        return s.getClosest(coord, dist); 
    }
    
    virtual GV_Vector getNearestSelectedRef(const GV_Vector& coord,
                                     double* dist = NULL,
                                     GV2::RefMode refMode=GV2::RefAll);

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
     * @sa GV2::ResolveLevel
     *
     * @return The measured distance between \p coord and the entity.
     */
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity = NULL,
                                      GV2::ResolveLevel level = GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false) = 0;

    virtual bool isPointOnEntity(const GV_Vector& coord,
                                 double tolerance=GV_TOLERANCE);
    
    /**
     * Implementations must return GV2::RightHand if coord is on the
     * right hand side of this entity, GV2::LeftHand if it is on the
     * left hand side and GV2::NoSide otherwise (on entity, entity has
     * no side, ..).
     */
    virtual GV2::Side getSideOfPoint(const GV_Vector& coord) = 0;

    /**
     * Implementations must move the entity by the given vector.
     */
    virtual void move(const GV_Vector& offset) = 0;

    /**
     * Implementations must rotate the entity by the given angle around
     * the given center.
     */
    virtual void rotate(const GV_Vector& center, double angle) = 0;

    /**
     * Implementations must scale the entity by the given factors.
     */
    virtual void scale(const GV_Vector& center, const GV_Vector& factor) = 0;

    /**
     * Acts like scale(GV_Vector) but with equal factors.
     * Equal to scale(center, GV_Vector(factor, factor)).
     */
    virtual void scale(const GV_Vector& center, double factor) {
        scale(center, GV_Vector(factor, factor, factor));
    }

    /**
     * Implementations must mirror the entity by the given axis.
     */
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) = 0;

    virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);
    
    /**
     * Implementations must create an isometric projection with the given perspective of the entity.
     */
    virtual void isoProject(GV2::IsoProjectionType /*type*/, double /*segmentLength*/) {}

    /**
     * Implementations must drag the reference point(s) of all
     * (sub-)entities that are very close to ref by offset.
     */
    virtual void moveRef(const GV_Vector& /*ref*/, 
        const GV_Vector& /*offset*/) {
        return;
    }
    
    /**
     * Implementations must drag the reference point(s) of selected 
     * (sub-)entities that are very close to ref by offset.
     */
    virtual void moveSelectedRef(const GV_Vector& /*ref*/, 
        const GV_Vector& /*offset*/) {
        return;
    }

    /**
     * Implementations must draw the entity on the given device.
     */
    virtual void draw(GV_Painter* painter, GV_GraphicView* view, 
        double patternOffset = 0.0) = 0;
    /**
     * Implementations might want to polish the entity after 
     * all other entities have been drawn.
     */
    /*
    virtual void draw2(GV_Painter* / *painter* /, GV_GraphicView* / *view* /, 
        double / *patternOffset* / = 0.0) {}
    */

    double getStyleFactor(GV_GraphicView* view);
    
#ifdef GV_COMPAT
    QString* getUserDefVar(const QString& key);
    QStringList getAllKeys();
    void setUserDefVar(const QString& key, const QString& val);
    void delUserDefVar(const QString& key);
#endif

    virtual void setProperty(const QString& name, const QVariant& value);
    virtual QVariant getProperty(const QString& name,
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    //virtual QStringList getPropertyChoices(const QString& name);
    virtual GV_PropertyAttributes getPropertyAttributes(
        const QString& propertyName);

    /** Recalculates the borders of this entity. */
    virtual void calculateBorders(bool visibleOnly=false) = 0;
    
    double getGraphicVariable(const QString& key, double defMM, int code);

protected:
    //! Entity's parent entity or NULL if this entity has no parent.
    GV_EntityContainer* parentContainer;
    //! minimum coordinates
    GV_Vector minV;
    //! maximum coordinates
    GV_Vector maxV;

    //! Pointer to layer
    GV_Layer* layer;

    //! Entity id
    unsigned long int id;

    //! pen (attributes) for this entity
    GV_Pen pen;

    //! auto updating enabled?
    bool updateEnabled;

private:
#ifdef GV_COMPAT
    //! OBSOLETE. Use properties instead.
    GV_Hash<QString, QString> varList;
#endif

public:
    static const char* propertyType;
    static const char* propertyLayer;
#ifdef GV_ARCH
    static const char* propertyFloor;
#endif
};

#endif
