/****************************************************************************
** $Id: gv_entity.cpp 7935 2008-01-18 15:39:49Z andrew $
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


#include "gv_entity.h"

#include <iostream>

#include "gv_arc.h"
#include "gv_circle.h"
#include "rb_debug.h"
#include "gv_document.h"
#include "gv_ellipse.h"
#include "gv_graphic.h"
#include "gv_graphicview.h"
#include "gv_insert.h"
#include "gv_layer.h"
#include "gv_line.h"
#include "gv_painter.h"
#include "gv_point.h"
#include "gv_polyline.h"
#include "gv_text.h"
#include <QStringList>

#ifdef GV_ARCH
#include "gv_archutils.h"
#endif


const char* GV_Entity::propertyType = QT_TRANSLATE_NOOP("QObject", "Type");
const char* GV_Entity::propertyLayer = QT_TRANSLATE_NOOP("QObject", "Layer");
#ifdef GV_ARCH
const char* GV_Entity::propertyFloor = QT_TRANSLATE_NOOP("QObject", "Floor");
#endif

/**
 * Default constructor.
 * @param parentContainer The parent entity of this entity.
 *               E.g. a line might have a graphic entity or
 *               a polyline entity as parent.
 */
GV_Entity::GV_Entity(GV_EntityContainer* parentContainer) {

    this->parentContainer = parentContainer;
    init();
}


/**
 * Destructor.
 */
GV_Entity::~GV_Entity() {}



/**
 * Initialisation. Called from all constructors.
 */
void GV_Entity::init() {
    resetBorders();

    setFlag(GV2::FlagVisible);
    //layer = NULL;
    //pen = GV_Pen();
    updateEnabled = true;
    setLayerToActive();
    setPenToActive();
    initId();
}

/**
 * Gives this entity a new unique id.
 */
void GV_Entity::initId() {
    static unsigned long int idCounter=0;
    id = idCounter++;
}
    


/**
 * Copies all basic entity properties from an existing entity.
 */
void GV_Entity::copyPropertiesFrom(GV_Entity* e) {
    parentContainer = e->parentContainer;
    minV = e->minV;
    maxV = e->maxV;
    layer = e->layer;
    pen = e->pen;
    updateEnabled = e->updateEnabled;
    GV_PropertyOwner::copyPropertiesFrom(e);
    setFlags(e->getFlags());
#ifdef GV_COMPAT
    varList = e->varList;
#endif
}



/**
 * Resets the borders of this element. 
 */
void GV_Entity::resetBorders() {
    // TODO: Check that. windoze XP crashes with MAXDOUBLE
    double maxd = GV_MAXDOUBLE;
    double mind = GV_MINDOUBLE;

    minV.set(maxd, maxd);
    maxV.set(mind, mind);
}



/**
 * Selects or deselects this entity.
 *
 * @param select True to select, false to deselect.
 * @param updateDependent Re-implementations might choose to 
 *   select dependent entities together with this. In some cases
 *   this is not desirable. updateDependent is False in these cases.
 * 
 * @return True on success, False if the layer is locked and select is True.
 */
bool GV_Entity::setSelected(bool select, bool /*updateDependent*/) {
    // layer is locked:
    if (select && isLocked()) {
        return false;
    }

    if (select) {
        setFlag(GV2::FlagSelected);
    } else {
        delFlag(GV2::FlagSelected);
    }

    setSelectionVisible(true);

    return true;
}



/**
 * Decides whether the selection of this entity is made visible to the user.
 * This is used for entities with dependent entities (e.g. walls with 
 * apertures) to make sure that selecting the entity also selects the 
 * dependent entities and yet only the properties of the selected entity
 * can be modified in the property editor.
 *
 * @param select True to make visible, false to make invisible.
 */
void GV_Entity::setSelectionVisible(bool on) {
    if (on) {
        setFlag(GV2::FlagSelectionVisible);
    } else {
        delFlag(GV2::FlagSelectionVisible);
    }
}



/**
 * Toggles select on this entity.
 */
bool GV_Entity::toggleSelected() {
    return setSelected(!isSelected());
}



/**
 * @return True if the entity is selected. Note that an entity might
 * not be selected but one of its parents is selected. In that case
 * this function returns false.
 */
bool GV_Entity::isSelected() const {
    return getFlag(GV2::FlagSelected);
}
    
    

/**
 * @return True if the entitiy's selection is made visible to the
 * user, false otherwise.
 */
bool GV_Entity::isSelectionVisible() const {
    return getFlag(GV2::FlagSelectionVisible);
}



/**
 * @return true if a parent entity of this entity is selected.
 */
bool GV_Entity::isParentContainerSelected() {
    GV_Entity* p = this;

    do {
        p = p->getParentContainer();
        if (p!=NULL && p->isSelected()==true) {
            return true;
        }
    } while(p!=NULL);

    return false;
}



/**
 * Gets the nearest reference point of this entity if it is selected. 
 * Containers re-implement this method to return the nearest reference
 * point of a selected sub entity.
 *
 * @param coord Coordinate (typically a mouse coordinate)
 * @param dist Pointer to a value which will contain the measured
 * distance between 'coord' and the closest point. The passed
 * pointer can also be NULL in which case the distance will be
 * lost.
 *
 * @return The closest point with the given distance to the endpoint.
 */
GV_Vector GV_Entity::getNearestSelectedRef(const GV_Vector& coord,
                                 double* dist,
                                 GV2::RefMode refMode) { 
    RB_DEBUG->print("GV_Entity::getNearestSelectedRef");

    if (isSelected()) {
        return getNearestRef(coord, dist, refMode);
    }
    else {
        return GV_Vector(false); 
    }
}



/**
 * Sets or resets the processed flag of this entity.
 *
 * @param on True to set, false to reset.
 */
void GV_Entity::setProcessed(bool on) {
    if (on) {
        setFlag(GV2::FlagProcessed);
    } else {
        delFlag(GV2::FlagProcessed);
    }
}



/**
 * @return True if the processed flag is set. 
 */
bool GV_Entity::isProcessed() const {
    return getFlag(GV2::FlagProcessed);
}



/**
 * @return True if the temp flag is set (for generated entities). 
 */
bool GV_Entity::isTemp() const {
    return getFlag(GV2::FlagTemp);
}


/**
 * Called when the undo state changed.
 *
 * @param undone true: entity has become invisible.
 *               false: entity has become visible.
 */
void GV_Entity::undoStateChanged(bool /*undone*/) {
    setSelected(false, false);
    update();
}


/**
 * @return true if this entity or any parent entities are undone.
 */
bool GV_Entity::isUndone() const {
    if (parentContainer==NULL) {
        return GV_Undoable::isUndone();
    }
    else {
        return GV_Undoable::isUndone() || parentContainer->isUndone();
    }
}


/**
 * @return True if the entity is in the given range.
 */
bool GV_Entity::isInWindow(const GV_Vector& v1, const GV_Vector& v2) {
    double right, left, top, bottom;

    right = std::max(v1.x, v2.x);
    left = std::min(v1.x, v2.x);
    top = std::max(v1.y, v2.y);
    bottom = std::min(v1.y, v2.y);

    return (getMin().x>=left &&
            getMax().x<=right &&
            getMin().y>=bottom &&
            getMax().y<=top);
}


/**
 * @param tolerance Tolerance.
 *
 * @retval true if the given point is on this entity.
 * @retval false otherwise
 */
bool GV_Entity::isPointOnEntity(const GV_Vector& coord,
                                double tolerance) {
    double dist = getDistanceToPoint(coord, NULL, GV2::ResolveNone);
    return (dist<=tolerance);
}


/**
 * Is this entity visible?
 *
 * @return true Only if the entity and the layer it is on are visible.
 * The Layer might also be NULL. In that case the layer visiblity 
* is ignored.
 */
bool GV_Entity::isVisible() {
    bool retLayer = false;
    bool retBlock = false;
        
    // entity is not visible:
    if (!getFlag(GV2::FlagVisible)) {
        return false;
    }
    
    // entity is in undo memory:
    if (isUndone()) {
        return false;
    }
    
    // invalid layer:
    if (getLayer()==NULL) {
        retLayer = true;
    }
    else {
        if (layer!=NULL) {
            if (!layer->isFrozen()) {
                retLayer = true;
            } else {
                return false;
            }
        }
        else {
            if (getLayer()==NULL) {
                retLayer = true;
            } else {
                if (!getLayer()->isFrozen()) {
                    retLayer = true;
                } else {
                    return false;
                }
            }
        }
    }
    
    if (getBlockOrInsert()==NULL) {
        retBlock = true;
    }
    else {
        if (getInsert()!=NULL && getInsert()->rtti()==GV2::EntityInsert) {
            GV_Insert* ins = (GV_Insert*)getInsert();
            GV_Block* block = ins->getBlockForInsert();
            if (block!=NULL && block->isFrozen()) {
                return false;
            }
            else {
                retBlock = true;
            }
        }

        if (getBlockOrInsert()->rtti()==GV2::EntityBlock) {
            if (getLayer(false)==NULL || !getLayer(false)->isFrozen()) {
                retBlock = true;
            } else {
                return false;
            }
        }
    
    
        if (getBlockOrInsert()->getLayer()==NULL) {
            retBlock = true;
        }
        else {
            if(getBlockOrInsert()->getLayer()->isFrozen()) {
                return false;
            }
            else {
                retBlock = true;
            }
        }
    }

    return (retBlock && retLayer);
}




/**
 * Sets the highlight status of the entity. Highlighted entities 
 * usually indicate a feedback to a user action.
 */
void GV_Entity::setHighlighted(bool on) {
    if (on) {
        setFlag(GV2::FlagHighlighted);
    } else {
        delFlag(GV2::FlagHighlighted);
    }
}



/**
 * @return true if the entity is highlighted.
 */
bool GV_Entity::isHighlighted() {
    return getFlag(GV2::FlagHighlighted);
}



/**
 * @return true if the layer this entity is on is locked.
 */
bool GV_Entity::isLocked() {
    if (getLayer(true)!=NULL && getLayer()->isLocked()) {
        return true;
    } else {
        return false;
    }
}


/**
 * @return The parent graphic in which this entity is stored
 * or the parent's parent graphic or NULL if none of the parents
 * are stored in a graphic.
 */
GV_Graphic* GV_Entity::getGraphic() {
    if (rtti()==GV2::EntityGraphic) {
        return dynamic_cast<GV_Graphic*>(this);
    } else if (parentContainer==NULL) {
        return NULL;
    } else {
        return parentContainer->getGraphic();
    }
}



/**
 * @return The parent block in which this entity is stored
 * or the parent's parent block or NULL if none of the parents
 * are stored in a block.
 */
GV_Block* GV_Entity::getBlock() {
    if (rtti()==GV2::EntityBlock) {
        return dynamic_cast<GV_Block*>(this);
    } else if (parentContainer==NULL) {
        return NULL;
    } else {
        return parentContainer->getBlock();
    }
}



/**
 * @return The parent insert in which this entity is stored
 * or the parent's parent block or NULL if none of the parents
 * are stored in a block.
 */
GV_Insert* GV_Entity::getInsert() {
    if (rtti()==GV2::EntityInsert) {
        return dynamic_cast<GV_Insert*>(this);
    } else if (parentContainer==NULL) {
        return NULL;
    } else {
        return parentContainer->getInsert();
    }
}



/**
 * @return The parent block or insert in which this entity is stored
 * or the parent's parent block or insert or NULL if none of the parents
 * are stored in a block or insert.
 */
GV_Entity* GV_Entity::getBlockOrInsert() {
    if (rtti()==GV2::EntityBlock || rtti()==GV2::EntityInsert) {
        return this;
    } else if (parentContainer==NULL) {
        return NULL;
    } else {
        return parentContainer->getBlockOrInsert();
    }
}



/**
 * @return The parent document in which this entity is stored
 * or the parent's parent document or NULL if none of the parents
 * are stored in a document. Note that a document is usually 
 * either a Graphic or a Block.
 */
GV_Document* GV_Entity::getDocument() {
    if (isDocument()==true) {
        return dynamic_cast<GV_Document*>(this);
    } else if (parentContainer==NULL) {
        return NULL;
    } else {
        return parentContainer->getDocument();
    }
}



/**
 * Sets a variable value for the parent graphic object.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param val Default value
 */
void GV_Entity::addGraphicVariable(const QString& key, double val, int code) {
    GV_Graphic* graphic = getGraphic();
    if (graphic!=NULL) {
        graphic->addVariable(key, val, code);
    }
}



/**
 * Sets a variable value for the parent graphic object.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param val Default value
 */
void GV_Entity::addGraphicVariable(const QString& key, int val, int code) {
    GV_Graphic* graphic = getGraphic();
    if (graphic!=NULL) {
        graphic->addVariable(key, val, code);
    }
}



/**
 * Sets a variable value for the parent graphic object.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param val Default value
 */
void GV_Entity::addGraphicVariable(const QString& key,
                                   const QString& val, int code) {
    GV_Graphic* graphic = getGraphic();
    if (graphic!=NULL) {
        graphic->addVariable(key, val, code);
    }
}



/**
 * A safe member function to return the given variable.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param def Default value
 *
 * @return value of variable or default value if the given variable
 *    doesn't exist.
 */
double GV_Entity::getGraphicVariableDouble(const QString& key, double def) {
    GV_Graphic* graphic = getGraphic();
    double ret=def;
    if (graphic!=NULL) {
        ret = graphic->getVariableDouble(key, def);
    }
    return ret;
}



/**
 * A safe member function to return the given variable.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param def Default value
 *
 * @return value of variable or default value if the given variable
 *    doesn't exist.
 */
int GV_Entity::getGraphicVariableInt(const QString& key, int def) {
    GV_Graphic* graphic = getGraphic();
    int ret=def;
    if (graphic!=NULL) {
        ret = graphic->getVariableInt(key, def);
    }
    return ret;
}



/**
 * A safe member function to return the given variable.
 *
 * @param key Variable name (e.g. "$DIMASZ")
 * @param def Default value
 *
 * @return value of variable or default value if the given variable
 *    doesn't exist.
 */
QString GV_Entity::getGraphicVariableString(const QString& key,
        const QString&  def) {
    GV_Graphic* graphic = getGraphic();
    QString ret=def;
    if (graphic!=NULL) {
        ret = graphic->getVariableString(key, def);
    }
    return ret;
}



/**
 * @return The unit the parent graphic works on or None if there's no
 * parent graphic.
 */
GV2::Unit GV_Entity::getGraphicUnit() {
    GV_Graphic* graphic = getGraphic();
    GV2::Unit ret = GV2::None;
    if (graphic!=NULL) {
        ret = graphic->getUnit();
    }
    return ret;
}



/**
 * Returns a pointer to the layer this entity is on or NULL.
 *
 * @para resolve true: if the layer is ByBlock, the layer of the 
 *               block this entity is in is returned. 
 *               false: the layer of the entity is returned.
 *
 * @return pointer to the layer this entity is on. If the layer
 * is set to NULL the layer of the next parent that is not on
 * layer NULL is returned. If all parents are on layer NULL, NULL
 * is returned.
 */
GV_Layer* GV_Entity::getLayer(bool resolve) const {
    if (resolve) {
        // we have no layer but a parent that might have one.
        // return parent's layer instead:
        if (layer==NULL /*|| layer->getName()=="ByBlock"*/) {
            if (parentContainer!=NULL) {
                return parentContainer->getLayer(true);
            } else {
                return NULL;
            }
        }
    }

    // return our layer. might still be NULL:
    return layer;
}



/**
 * Sets the layer of this entity to the layer with the given name 
 */
void GV_Entity::setLayer(const QString& name) {
    RB_DEBUG->print("GV_Entity::setLayer: " + name);

    GV_Graphic* graphic = getGraphic();
    if (graphic!=NULL) {
        layer = graphic->findLayer(name);
    } else {
        RB_DEBUG->print("GV_Entity::setLayer: graphic is NULL");
        layer = NULL;
    }
}



/**
 * Sets the layer of this entity to the layer given.
 */
void GV_Entity::setLayer(GV_Layer* l) {
    layer = l;
}



/**
 * Sets the layer of this entity to the current layer of
 * the graphic this entity is in. If this entity (and none
 * of its parents) are in a graphic the layer is set to NULL.
 */
void GV_Entity::setLayerToActive() {
    GV_Graphic* graphic = getGraphic();

    if (graphic!=NULL) {
        layer = graphic->getActiveLayer();
    } else {
        layer = NULL;
    }
}



/**
 * Gets the pen needed to draw this entity.
 * The attributes can also come from the layer this entity is on
 * if the flags are set accordingly.
 * 
 * @param resolve true: Resolve the pen to a drawable pen (e.g. the pen 
 *         from the layer or parent..)
 *         false: Don't resolve and return a pen or ByLayer, ByBlock, ...
 *
 * @return Pen for this entity. 
 */
GV_Pen GV_Entity::getPen(bool resolve) const {

    if (!resolve) {
        return pen;
    } else {

        GV_Pen p = pen;
        GV_Layer* l = getLayer(true);

        // use parental attributes (e.g. vertex of a polyline, block
        // entities when they are drawn in block documents):
        if (!p.isValid() || p.getColor().isByBlock()) {
            if (parentContainer!=NULL) {
                p = parentContainer->getPen();
            }
        }
        // use layer attributes:
        else if (l!=NULL) {

            // use layer's color:
            if (p.getColor().isByLayer()) {
                p.setColor(l->getPen().getColor());
            }

            // use layer's width:
            if (p.getWidth()==GV2::WidthByLayer) {
                p.setWidth(l->getPen().getWidth());
            }

            // use layer's linetype:
            if (p.getLineType()==GV2::LineByLayer) {
                p.setLineType(l->getPen().getLineType());
            }
        }

        return p;
    }
}



/**
 * Sets the pen of this entity to the current pen of
 * the graphic this entity is in. If this entity (and none
 * of its parents) are in a graphic the pen is not changed.
 */
void GV_Entity::setPenToActive() {
    GV_Document* doc = getDocument();
    if (doc!=NULL) {
        pen = doc->getActivePen();
    }
}



/**
 * Implementations must stretch the given range of the entity 
 * by the given offset. This default implementation moves the 
 * whole entity if it is completely inside the given range.
 */
void GV_Entity::stretch(const GV_Vector& firstCorner,
                        const GV_Vector& secondCorner,
                        const GV_Vector& offset) {

    if (getMin().isInWindow(firstCorner, secondCorner) &&
            getMax().isInWindow(firstCorner, secondCorner)) {

        move(offset);
    }
}



/**
 * @return Factor for scaling the line styles considering the current 
 * paper scaling and the fact that styles are stored in Millimeter.
 */
double GV_Entity::getStyleFactor(GV_GraphicView* view) {
    double styleFactor = 1.0;

    if (view!=NULL) {
        int penWidth = (int)getPen(true).getWidth();
        
        // draft mode or zero width: no line pattern scaling
        if (penWidth==0 || 
            (view->isPrinting()==false && view->isDraftMode())) {

            styleFactor = 1.0/view->getFactor().x;
        } else {
            // the factor caused by the unit:
            GV2::Unit unit = GV2::None;
            GV_Graphic* g = getGraphic();
            if (g!=NULL) {
                unit = g->getUnit();
                styleFactor = GV_Units::convert(1.0, GV2::Millimeter, unit);
            }

            // the factor caused by the line width:
            if (penWidth>0) {
                styleFactor *= ((double)penWidth/100.0);
            } 
        }

        if (penWidth>0) {
            if (view->isPrinting() || view->isPrintPreview() || view->isDraftMode()==false) {
                GV_Graphic* graphic = getGraphic();
                if (graphic!=NULL && graphic->getPaperScale()>1.0e-9) {
                    styleFactor /= graphic->getPaperScale();
                }
            }
        }
    }

    //RB_DEBUG->print("GV_Entity::getStyleFactor: stylefactor: %f", styleFactor);
    //RB_DEBUG->print("GV_Entity::getStyleFactor: viewfactor: %f", view->getFactor().x);

    if (!view->isPrinting() && styleFactor*view->getFactor().x<0.2) {
        styleFactor = -1.0;
    }

    return styleFactor;
}

/**
 * @return User defined variable connected to this entity.
 *
 * OBSOLETE since QCad 2.1.1
 */
#ifdef GV_COMPAT
QString* GV_Entity::getUserDefVar(const QString& key) {
    if (varList.contains(key)) {
        return &varList[key];
    }
    else {
        return NULL;
    }
}
#endif


/**
 * Add a user defined variable to this entity.
 *
 * OBSOLETE since QCad 2.1.1
 */
#ifdef GV_COMPAT
void GV_Entity::setUserDefVar(const QString& key, const QString& val) {
    varList.insert(key, val);
}
#endif



/**
 * Deletes the given user defined variable.
 *
 * OBSOLETE since QCad 2.1.1
 */
#ifdef GV_COMPAT
void GV_Entity::delUserDefVar(const QString& key) {
    varList.remove(key);
}
#endif



/**
 * @return A list of all keys connected to this entity.
 *
 * OBSOLETE since QCad 2.1.1
 */
#ifdef GV_COMPAT
QStringList GV_Entity::getAllKeys() {
    return varList.keys();
}
#endif



/**
 * Supports custom properties and layer properties.
 * Ignores the property for the entity type that cannot be changed.
 */    
void GV_Entity::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_ENTITY_LAYER) {
        setLayer(value.toString());
    }
#ifdef GV_ARCH
    else if (name==GV_ENTITY_FLOOR) {
        setLayer(GV_ArchUtils::floorToLayerName(value.toString()));
    }
#endif
    else if (name!=GV_ENTITY_TYPE) {
        GV_PropertyOwner::setProperty(name, value);
    }
}



/**
 * Returns the property with the given name or 'def'.
 *
 * @param name Property name.
 * @param def Default value to return if the given property is undefined.
 */
QVariant GV_Entity::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_ENTITY_TYPE) {
        return typeName();
    }
    else if (name==GV_ENTITY_LAYER) {
        GV_Layer* layer = getLayer();
        if (layer!=NULL) {
#ifdef GV_ARCH
            if (!GV_ArchUtils::isFloor(layer->getName())) {
                return layer->getName();
            }
            else {
                return def;
            }
#else
            return layer->getName();
#endif
        }
        else {
            return def;
        }
    }
#ifdef GV_ARCH
    else if (name==GV_ENTITY_FLOOR) {
        GV_Layer* layer = getLayer();
        if (layer!=NULL) {
            return GV_ArchUtils::layerToFloorName(layer->getName());
        }
        else {
            return def;
        }
    }
#endif
    else {
        return GV_PropertyOwner::getProperty(name, def);
    }
}



/**
 * @return Names of all known properties that are defined for this entity.
 */
QStringList GV_Entity::getPropertyNames(bool /*includeGeometry*/) {
    QStringList ret; // = GV_PropertyOwner::getPropertyNames();
    ret << GV_ENTITY_TYPE;
    if (rtti()!=GV2::EntityGraphic) {
        ret << GV_ENTITY_LAYER;
#ifdef GV_ARCH
        ret << GV_ENTITY_FLOOR;
#endif
    }
    return ret + GV_PropertyOwner::getPropertyNames();
}



GV_PropertyAttributes GV_Entity::getPropertyAttributes(
    const QString& propertyName) {

    GV_PropertyAttributes ret;

#ifdef GV_ARCH
    if (propertyName==GV_ENTITY_FLOOR ||
        propertyName==GV_ENTITY_LAYER) {
        ret.affectsOtherProperties = true;
    }
#endif
    
    // only one choice for the entity type:
    if (propertyName==GV_ENTITY_TYPE) {
        ret.choices << getProperty(propertyName).toString();
    }
    // layer choice:
    else if (propertyName==GV_ENTITY_LAYER) {
        GV_Graphic* graphic = getGraphic();
        if (graphic!=NULL) {
            GV_LayerList* layerList = graphic->getLayerList();
            for (int i=0; i<layerList->count(); i++) {
#ifdef GV_ARCH
                if (!GV_ArchUtils::isFloor(layerList->at(i)->getName())) {
                    ret.choices << layerList->at(i)->getName();
                }
#else
                ret.choices << layerList->at(i)->getName();
#endif
            }
        }
    }
#ifdef GV_ARCH
    // floor choice:
    else if (propertyName==GV_ENTITY_FLOOR) {
        GV_Graphic* graphic = getGraphic();
        if (graphic!=NULL) {
            GV_LayerList* layerList = graphic->getLayerList();
            for (int i=0; i<layerList->count(); i++) {
                if (GV_ArchUtils::isFloor(layerList->at(i)->getName())) {
                    ret.choices << GV_ArchUtils::layerToFloorName(layerList->at(i)->getName());
                }
            }
        }
    }
#endif

    return ret;
}


/**
 * @return the given graphic variable or the default value given in mm 
 * converted to the graphic unit.
 * If the variable is not found it is added with the given default
 * value converted to the local unit.
 */
double GV_Entity::getGraphicVariable(const QString& key, double defMM,
                                        int code) {

    double v = getGraphicVariableDouble(key, GV_MINDOUBLE);
    if (v<=GV_MINDOUBLE) {
        addGraphicVariable(
            key,
            GV_Units::convert(defMM, GV2::Millimeter, getGraphicUnit()),
            code);
        v = getGraphicVariableDouble(key, 1.0);
    }

    return v;
}
