/****************************************************************************
** $Id: rs_modification.cpp 9431 2008-03-19 10:16:58Z andrew $
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

#include "rs_modification.h"

#include "rs_clipboard.h"
#include "rs_creation.h"
#include "rs_entity.h"
#include "rs_graphic.h"
#include "rs_hatch.h"
#include "rs_information.h"
#include "rs_insert.h"
#include "rs_polyline.h"
#include "rs_text.h"
#include "rs_settings.h"
#include "rs_solid.h"
#include "rs_valuelist.h"

#ifdef RS_ARCH
#include "rs_archutils.h"
#endif

#ifdef RS_ALGO
#include "triangulation/hpolygon.h"
#include "triangulation/mgexcept.h"
#endif

#ifdef RS_ALGO
#include "kbool/include/booleng.h"
#endif



/**
 * Default constructor.
 *
 * @param container The container to which we will add
 *        entities. Usually that's an RS_Graphic entity but
 *        it can also be a polyline, text, ...
 * @param graphicView Pointer to graphic view or NULL if you don't want the
 *        any views to be updated.
 * @param handleUndo true: Handle undo functionalitiy.
 * @param keepSelection true: newly created entities are selected. 
 *        Only partly implemented.
 */
RS_Modification::RS_Modification(RS_EntityContainer& container,
                                 RS_GraphicView* graphicView,
                                 bool handleUndo,
                                 bool keepSelection) {
    this->container = &container;
    this->graphicView = graphicView;
    this->handleUndo = handleUndo;
    this->keepSelection = keepSelection;
    graphic = container.getGraphic();
    document = container.getDocument();

    if (document!=NULL) {
        if (!document->isUndoEnabled()) {
            this->handleUndo = false;
        }
    }
}



/**
 * Deletes all selected entities.
 */
void RS_Modification::remove() {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::remove: no valid container");
        return;
    }

    if (document!=NULL) {
        document->startUndoCycle();
    }

    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {

        if (e!=NULL && e->isSelected()) {
            e->setSelected(false, false);

            e->changeUndoState();
            if (document!=NULL) {
                document->addUndoable(e);
            }
        }
    }
    
    RS_DEBUG->print("RS_Modification::remove: setSelected(false, false): end of loop");

    if (document!=NULL) {
        document->endUndoCycle();
    }

    graphicView->redraw();
}



/**
 * Move entities to background or bring them to the front.
 */
void RS_Modification::moveToBack(bool toBack) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::moveToBack: no valid container");
        return;
    }

    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {

        if (e!=NULL && e->isSelected()) {
            e->setSelected(false);
            container->moveEntityToBack(e, toBack);
            e = container->firstEntity();
        }
    }

    graphicView->redraw();
}



/**
 * Changes the attributes of all selected 
 */
bool RS_Modification::changeAttributes(RS_AttributesData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::changeAttributes: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL) {
        document->startUndoCycle();
    }

    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isSelected()) {
            RS_Entity* ec = e->clone();
            ec->setSelected(false);

            RS_Pen pen = ec->getPen(false);

            if (data.changeLayer==true) {
                ec->setLayer(data.layer);
            }

            if (data.changeColor==true) {
                pen.setColor(data.pen.getColor());
            }
            if (data.changeLineType==true) {
                pen.setLineType(data.pen.getLineType());
            }
            if (data.changeWidth==true) {
                pen.setWidth(data.pen.getWidth());
            }

            ec->setPen(pen);

            ec->update();
            addList.append(ec);
        }
    }

    deselectOriginals(true);
    addNewEntities(addList);

    if (document!=NULL) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}


/**
 * Copies all selected entities from the given container to the clipboard.
 * Layers and blocks that are needed are also copied if the container is
 * or is part of an RS_Graphic.
 *
 * @param container The entity container.
 * @param ref Reference point. The entities will be moved by -ref.
 * @param cut true: cut instead of copying, false: copy
 */
void RS_Modification::copy(const RS_Vector& ref, const bool cut) {

    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::copy: no valid container");
        return;
    }

    RS_CLIPBOARD->clear();
    if (graphic!=NULL) {
        RS_CLIPBOARD->getGraphic()->setUnit(graphic->getUnit());
    } else {
        RS_CLIPBOARD->getGraphic()->setUnit(RS2::None);
    }

    // start undo cycle for the container if we're cutting
    if (cut && document!=NULL) {
        document->startUndoCycle();
    }

    // copy entities / layers / blocks
    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isSelected()) {
            copyEntity(e, ref, cut);
        }
    }

    if (cut && document!=NULL) {
        document->endUndoCycle();
        graphicView->redraw();
    }
    
    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, 
        "RS_Modification::copy: clipboard contents: ");
    RS_DEBUG->printEntity(
        RS_Debug::D_ERROR, RS_CLIPBOARD->getGraphic());
    */
}



/**
 * Copies the given entity from the given container to the clipboard.
 * Layers and blocks that are needed are also copied if the container is
 * or is part of an RS_Graphic.
 *
 * @param e The entity.
 * @param ref Reference point. The entities will be moved by -ref.
 * @param cut true: cut instead of copying, false: copy
 */
void RS_Modification::copyEntity(RS_Entity* e, const RS_Vector& ref,
                                 const bool cut) {

    if (e!=NULL && e->isSelected()) {
        e->setSelected(false, false);

        // add entity to clipboard:
        RS_Entity* c = e->clone();
        c->move(-ref);
        RS_CLIPBOARD->addEntity(c);

        copyLayers(e);
        copyBlocks(e);

        // change layer to the layer clone:
        RS_Layer* l = e->getLayer();
        if (l!=NULL) {
            c->setLayer(l->getName());
        }

        // make sure all sub entities point to layers of the clipboard
        if (c->isContainer()) {
            RS_EntityContainer* ec = dynamic_cast<RS_EntityContainer*>(c);

            for (RS_Entity* e2 = ec->firstEntity(RS2::ResolveAll); e2!=NULL;
                    e2 = ec->nextEntity(RS2::ResolveAll)) {

                RS_Layer* l2 = e2->getLayer();

                if (l2!=NULL) {
                    e2->setLayer(l2->getName());
                }
            }
        }

        if (cut) {
            e->changeUndoState();
            if (document!=NULL) {
                document->addUndoable(e);
            }
        }
    }

}



/**
 * Copies all layers of the given entity to the clipboard.
 */
void RS_Modification::copyLayers(RS_Entity* e) {

    if (e==NULL /*|| e->getGraphic()==NULL || e->getGraphic()->getLayerList()==NULL*/) {
        return;
    }

    /*
    RS_LayerList* source = e->getGraphic()->getLayerList();
    for (int i=0; i<source->count(); ++i) {
        if (!RS_CLIPBOARD->hasLayer(source->at(i)->getName())) {
            RS_Layer* lc = source->at(i)->clone();
            lc->freeze(false);
            lc->lock(false);
            RS_CLIPBOARD->addLayer(lc);
            RS_DEBUG->print("RS_Modification::copyLayers: "
                "copying layer '%s'", 
                (const char*)source->at(i)->getName().toLatin1());
        }
    }
    */
    
    // add layer(s) of the entity if it's not an insert
    //  (inserts are on layer '0'):
    if (e->rtti()!=RS2::EntityInsert) {
        RS_Layer* l = e->getLayer();
        if (l!=NULL) {
            if (!RS_CLIPBOARD->hasLayer(l->getName())) {
                RS_CLIPBOARD->addLayer(l->clone());
            }
        }
    }

    // special handling of inserts:
    else {
        // insert: add layer(s) of subentities:
        RS_Block* b = dynamic_cast<RS_Insert*>(e)->getBlockForInsert();
        if (b!=NULL) {
            for (RS_Entity* e2=b->firstEntity(); e2!=NULL;
                    e2=b->nextEntity()) {
                copyLayers(e2);
            }
        }
    }
}



/**
 * Copies all blocks of the given entity to the clipboard.
 */
void RS_Modification::copyBlocks(RS_Entity* e) {

    if (e==NULL) {
        return;
    }

    // add block of the entity if it's an insert
    if (e->rtti()==RS2::EntityInsert) {
        RS_Block* b = dynamic_cast<RS_Insert*>(e)->getBlockForInsert();
        if (b!=NULL) {
            // add block of an insert:
            if (!RS_CLIPBOARD->hasBlock(b->getName())) {
                RS_CLIPBOARD->addBlock(
                    dynamic_cast<RS_Block*>(b->clone())
                );
            }

            for (RS_Entity* e2=b->firstEntity(); e2!=NULL;
                    e2=b->nextEntity()) {
                //for (int i=0; i<b->count(); ++i) {
                //RS_Entity* e2 = b->entityAt(i);
                copyBlocks(e2);
            }
        }
    }
}



/**
 * Pastes all entities from the clipboard into the container.
 * Layers and blocks that are needed are also copied if the container is
 * or is part of an RS_Graphic.
 *
 * @param data Paste data.
 * @param source The source from where to paste. NULL means the source
 *      is the clipboard.
 */
void RS_Modification::paste(const RS_PasteData& data, RS_Graphic* source) {

    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::paste: Graphic is NULL");
        return;
    }

    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, 
        "RS_Modification::paste: clipboard contents: ");
    RS_DEBUG->printEntity(
        RS_Debug::D_ERROR, RS_CLIPBOARD->getGraphic());
    */

    double factor = 1.0;

    if (source==NULL) {
        source = RS_CLIPBOARD->getGraphic();

        // graphics from the clipboard need to be scaled. from the part lib not:
        RS2::Unit sourceUnit = source->getUnit();
        RS2::Unit targetUnit = graphic->getUnit();
        factor = RS_Units::convert(1.0, sourceUnit, targetUnit);
    }

    if (document!=NULL) {
        document->startUndoCycle();
    }

    // insert layers:
    if (graphic!=NULL && !data.useCurrentLayer) {
        RS_Layer* layer = graphic->getActiveLayer();
        for(int i=0; i<source->countLayers(); ++i) {
            RS_Layer* l = source->layerAt(i);
            if (l!=NULL) {
                if (graphic->findLayer(l->getName())==NULL) {
                    graphic->addLayer(l->clone());
                }
            }
        }
        graphic->activateLayer(layer);
    }

    // insert blocks:
    if (graphic!=NULL) {
        for(int i=0; i<source->countBlocks(); ++i) {
            RS_Block* b = source->blockAt(i);
            if (b!=NULL) {
                if (graphic->findBlock(b->getName())==NULL) {
                    RS_Block* bc = dynamic_cast<RS_Block*>(
                        b->clone()
                    );
                    bc->reparent(container);

                    // scale block but don't scale inserts in block
                    //  (they already scale with their block)
                    for(int i2=0; i2<bc->count(); ++i2) {
                        RS_Entity* e = bc->entityAt(i2);
                        if (e!=NULL && e->rtti()!=RS2::EntityInsert) {
                            e->scale(bc->getBasePoint(),
                                     RS_Vector(factor, factor));
                        } else {
                            RS_Vector ip = dynamic_cast<RS_Insert*>(e)->getInsertionPoint();
                            ip.scale(bc->getBasePoint(),
                                     RS_Vector(factor, factor));
                            dynamic_cast<RS_Insert*>(e)->setInsertionPoint(ip);
                            e->update();
                        }
                    }

                    graphic->addBlock(bc);
                }
            }
        }
    }

    // add entities to this host (graphic or a new block)
    RS_EntityContainer* host = container;
    RS_String blockName;

    // create new block:
    if (graphic!=NULL && data.asInsert==true) {
        RS_BlockList* blkList = graphic->getBlockList();
        if (blkList!=NULL) {
            blockName = blkList->newName(data.blockName);

            RS_Block* blk =
                new RS_Block(graphic,
                             RS_BlockData(blockName,
                                          RS_Vector(0.0,0.0), false));
            graphic->addBlock(blk);

            host = blk;
        }
    }

    // insert entities:
    for (RS_Entity* e=dynamic_cast<RS_EntityContainer*>(source)->firstEntity();
            e!=NULL;
            e=dynamic_cast<RS_EntityContainer*>(source)->nextEntity()) {

        RS_String layerName = "0";
        RS_Layer* layer = e->getLayer();
        if (layer!=NULL) {
            layerName = layer->getName();
        }

        RS_Entity* e2 = e->clone();
        e2->setUpdateEnabled(false);
        e2->setSelected(false, false);
        e2->reparent(host);
        if (data.asInsert==false) {
            e2->scale(RS_Vector(0,0), RS_Vector(data.factor, data.factor));
            e2->rotate(RS_Vector(0,0), data.angle);
            if (data.flipHorizontal) {
                e2->mirror(RS_Vector(0,0), RS_Vector(0,1));
            }
            if (data.flipVertical) {
                e2->mirror(RS_Vector(0,0), RS_Vector(1,0));
            }
            e2->move(data.insertionPoint);
        }

        // don't adjust insert factor - block was already adjusted to unit
        if (e2->rtti()==RS2::EntityInsert) {
            RS_Vector ip = dynamic_cast<RS_Insert*>(e2)->getInsertionPoint();
            ip.scale(data.insertionPoint, RS_Vector(factor, factor));
            dynamic_cast<RS_Insert*>(e2)->setInsertionPoint(ip);
            e2->update();
        } else {
            e2->scale(data.insertionPoint, RS_Vector(factor, factor));
        }
        host->addEntity(e2);
        if (data.useCurrentLayer) {
            e2->setLayerToActive();
        }
        else {
            e2->setLayer(layerName);
        }

        // make sure all sub entities point to layers of the container
        if (e2->isContainer()) {
            RS_EntityContainer* ec = dynamic_cast<RS_EntityContainer*>(e2);

            for (RS_Entity* e3 = ec->firstEntity(RS2::ResolveAll); e3!=NULL;
                    e3 = ec->nextEntity(RS2::ResolveAll)) {

                if (data.useCurrentLayer) {
                    e3->setLayerToActive();
                }
                else {
                    RS_Layer* l2 = e3->getLayer();
                    if (l2!=NULL) {
                        e3->setLayer(l2->getName());
                    }
                }
            }
        }
        
        e2->setUpdateEnabled(true);
        e2->update();
        e2->calculateBorders();

        if (document!=NULL && data.asInsert==false) {
            document->addUndoable(e2);
        }
    }

    if (data.asInsert==true) {
        RS_Insert* ins =
            new RS_Insert(container,
                          RS_InsertData(
                              blockName,
                              data.insertionPoint,
                              RS_Vector(data.factor, data.factor),
                              data.angle,
                              1,1,RS_Vector(0.0,0.0)));
        container->addEntity(ins);
        ins->setLayerToActive();
        ins->setPenToActive();

        if (document!=NULL) {
            document->addUndoable(ins);
        }
    }

    if (document!=NULL) {
        document->endUndoCycle();
    }
}
    
    
    
/**
 * Duplicates the given layer and all entities on it.
 */
void RS_Modification::duplicateLayer(RS_Layer* originalLayer) {

    if (originalLayer==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::duplicateLayer: layer is NULL");
        return;
    }
    
    if (graphic==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::duplicateLayer: Graphic is NULL");
        return;
    }

    RS_LayerList* layerList = graphic->getLayerList();
    if (layerList==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::duplicateLayer: Layer list is NULL");
        return;
    }
    
    RS_String name;
#ifdef RS_ARCH
    if (originalLayer->getName().startsWith(RS_FLOOR_PREFIX)) {
        name = layerList->generateNewLayerName(RS_FLOOR_PREFIX);
    }
    else {
#endif
        name = layerList->generateNewLayerName("");
#ifdef RS_ARCH
    }
#endif

    RS_Layer* layer = new RS_Layer(name);
    layer->copyPropertiesFrom(originalLayer);
#ifdef RS_ARCH
    layer->freeze(true);
#endif

#ifdef RS_ARCH
    if (originalLayer->getName().startsWith(RS_FLOOR_PREFIX)) {
        layer->setProperty("elevation", RS_ArchUtils::findHighestPoint(*graphic));
        //layer->setProperty("elevation", 
            //layer->getProperty("elevation").toDouble()+3.0);
    }
#endif
    
    layerList->add(layer);
    layerList->activate(name);
    
    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
        
    // Create new entites
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        
        if (e->getLayer()==originalLayer) {
            RS_Entity* ec = e->clone();
            ec->setLayer(layer);
            ec->setSelected(false);
            addList.append(ec);
        }
    }

    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
}



/**
 * Changes the given property of the selection and handles undo / redo.
 */
bool RS_Modification::changeProperty(const RS_String& name, 
    const RS_Variant& value) {

    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::changeProperty: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        if (!e->isUndone() && e!=NULL && e->isSelected()) {
            RS_Entity* ec = e->clone();

            if (e->isSelectionVisible()) {
                ec->setProperty(name, value);
            }
            ec->setSelected(true);

            addList.append(ec);
        }
    }

    deselectOriginals(true, true);
    
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}


/**
 * Splits a polyline into two, leaving out a gap.
 *
 * @param polyline The original polyline
 * @param e1 1st entity on which the first cutting point is.
 * @param v1 1st cutting point.
 * @param e2 2nd entity on which the first cutting point is.
 * @param v2 2nd cutting point.
 * @param polyline1 Pointer to a polyline pointer which will hold the 
 *        1st resulting new polyline. Pass NULL if you don't
 *        need those pointers.
 * @param polyline2 Pointer to a polyline pointer which will hold the 
 *        2nd resulting new polyline. Pass NULL if you don't
 *        need those pointers.
 *
 * @todo Support arcs in polylines, check for wrong parameters
 *
 * @return true
 */
bool RS_Modification::splitPolyline(RS_Polyline& polyline,
                                    RS_Entity& e1, RS_Vector v1,
                                    RS_Entity& e2, RS_Vector v2,
                                    RS_Polyline** polyline1,
                                    RS_Polyline** polyline2) const {

    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::splitPolyline: no valid container");
        return false;
    }

    RS_Entity* firstEntity = polyline.firstEntity();
    RS_Vector firstPoint(false);
    if (firstEntity->rtti()==RS2::EntityLine) {
        firstPoint = dynamic_cast<RS_Line*>(firstEntity)->getStartpoint();
    }
    RS_Polyline* pl1 =
        new RS_Polyline(container,
                        RS_PolylineData(firstPoint, RS_Vector(0.0,0.0), 0));
    RS_Polyline* pl2 = new RS_Polyline(container);
    RS_Polyline* pl = pl1;    // Current polyline
    RS_Line* line = NULL;
    RS_Arc* arc = NULL;

    if (polyline1!=NULL) {
        *polyline1 = pl1;
    }
    if (polyline2!=NULL) {
        *polyline2 = pl2;
    }

    for (RS_Entity* e = polyline.firstEntity();
            e != NULL;
            e = polyline.nextEntity()) {

        if (e->rtti()==RS2::EntityLine) {
            line = dynamic_cast<RS_Line*>(e);
            arc = NULL;
        } else if (e->rtti()==RS2::EntityArc) {
            arc = dynamic_cast<RS_Arc*>(e);
            line = NULL;
        } else {
            line = NULL;
            arc = NULL;
        }

        if (line!=NULL /*|| arc!=NULL*/) {

            if (e==&e1 && e==&e2) {
                // Trim within a single entity:
                RS_Vector sp = line->getStartpoint();
                double dist1 = (v1-sp).magnitude();
                double dist2 = (v2-sp).magnitude();
                pl->addVertex(dist1<dist2 ? v1 : v2, 0.0);
                pl = pl2;
                pl->setStartpoint(dist1<dist2 ? v2 : v1);
                pl->addVertex(line->getEndpoint(), 0.0);
            } else if (e==&e1 || e==&e2) {
                // Trim entities:
                RS_Vector v = (e==&e1 ? v1 : v2);
                if (pl==pl1) {
                    // Trim endpoint of entity to first vector
                    pl->addVertex(v, 0.0);
                    pl = NULL;
                } else {
                    // Trim startpoint of entity to second vector
                    pl = pl2;
                    pl->setStartpoint(v);
                    pl->addVertex(line->getEndpoint(), 0.0);
                }
            } else {
                // Add entities to polylines
                if (line!=NULL && pl!=NULL) {
                    pl->addVertex(line->getEndpoint(), 0.0);
                }
            }
        }
    }

    container->addEntity(pl1);
    container->addEntity(pl2);
    polyline.changeUndoState();

    return true;
}



/**
 * Moves all selected entities with the given data for the move
 * modification.
 */
bool RS_Modification::move(RS_MoveData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::move: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
        
    RS_DEBUG->print("RS_Modification::move: copies: %d", data.number);

    // Create new entites
    for (int num=1;
            num<=data.number || (data.number==0 && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                e->setVisible(false);

                ec->setVisible(true);
                ec->move(data.offset*num);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                if (ec->rtti()==RS2::EntityInsert) {
                    dynamic_cast<RS_Insert*>(ec)->update();
                }
                // since 2.0.4.0: keep selection
                ec->setSelected(true);
                addList.append(ec);
            }
        }
    }

    deselectOriginals(data.number==0);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}




/**
 * Rotates all selected entities with the given data for the rotation.
 */
bool RS_Modification::rotate(RS_RotateData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::rotate: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (int num=1;
            num<=data.number || (data.number==0 && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            //for (int i=0; i<container->count(); ++i) {
            //RS_Entity* e = container->entityAt(i);

            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                ec->setSelected(false, false);

                ec->rotate(data.center, data.angle*num);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                if (ec->rtti()==RS2::EntityInsert) {
                    dynamic_cast<RS_Insert*>(ec)->update();
                }
                addList.append(ec);
            }
        }
    }

    deselectOriginals(data.number==0);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}



/**
 * Moves all selected entities with the given data for the scale
 * modification.
 */
bool RS_Modification::scale(RS_ScaleData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::scale: no valid container");
        return false;
    }
        
    RS_DEBUG->print("RS_Modification::scale: center: %f/%f", 
        data.referencePoint.x, data.referencePoint.y);

    bool keepProportions = true;
    if (fabs(data.factor.x-data.factor.y)>1.0e-6) {
        keepProportions = false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (int num=1;
            num<=data.number || (data.number==0 && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            if (e!=NULL && e->isSelected()) {
                if (keepProportions || e->isOfType(RS2::EntityLine)) {
                    RS_Entity* ec;
                    ec = e->clone();

                    /*
                    // scaling arcs without keeping proportions:
                    if (e->rtti()==RS2::EntityArc) {
                        ec = dynamic_cast<RS_Arc*>(e)->createEllipse();
                    }
                    else if (e->rtti()==RS2::EntityCircle) {
                        ec = dynamic_cast<RS_Circle*>(e)->createEllipse();
                    }
                    else {
                        ec = e->clone();
                    }
                    */

                    ec->setSelected(false, false);

                    ec->scale(data.referencePoint, 
                        RS_Vector(RS_Math::pow(data.factor.x, num), 
                            RS_Math::pow(data.factor.y, num)));
                    if (data.useCurrentLayer) {
                        ec->setLayerToActive();
                    }
                    if (data.useCurrentAttributes) {
                        ec->setPenToActive();
                    }
                    if (ec->rtti()==RS2::EntityInsert) {
                        dynamic_cast<RS_Insert*>(ec)->update();
                    }
                    addList.append(ec);
                }
                else {
                    e->setSelected(false, false);
                }
            }
        }
    }

    deselectOriginals(data.number==0);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}



/**
 * Mirror all selected entities with the given data for the mirror
 * modification.
 */
bool RS_Modification::mirror(RS_MirrorData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::mirror: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (int num=1;
            num<=(int)data.copy || (data.copy==false && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            //for (int i=0; i<container->count(); ++i) {
            //RS_Entity* e = container->entityAt(i);

            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                ec->setSelected(false, false);

                ec->mirror(data.axisPoint1, data.axisPoint2);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                if (ec->rtti()==RS2::EntityInsert) {
                    dynamic_cast<RS_Insert*>(ec)->update();
                }
                addList.append(ec);
            }
        }
    }

    deselectOriginals(data.copy==false);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}



/**
 * Rotates entities around two centers with the given parameters.
 */
bool RS_Modification::rotate2(RS_Rotate2Data& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::rotate2: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (int num=1;
            num<=data.number || (data.number==0 && num<=1);
            num++) {

        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            //for (int i=0; i<container->count(); ++i) {
            //RS_Entity* e = container->entityAt(i);

            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                ec->setSelected(false, false);

                ec->rotate(data.center1, data.angle1*num);
                RS_Vector center2 = data.center2;
                center2.rotate(data.center1, data.angle1*num);

                ec->rotate(center2, data.angle2*num);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                if (ec->rtti()==RS2::EntityInsert) {
                    dynamic_cast<RS_Insert*>(ec)->update();
                }
                addList.append(ec);
            }
        }
    }

    deselectOriginals(data.number==0);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}



/**
 * Moves and rotates entities with the given parameters.
 */
bool RS_Modification::moveRotate(RS_MoveRotateData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::moveRotate: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (int num=1;
            num<=data.number || (data.number==0 && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            //for (int i=0; i<container->count(); ++i) {
            //RS_Entity* e = container->entityAt(i);

            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                ec->setSelected(false, false);

                ec->move(data.offset*num);
                ec->rotate(data.referencePoint + data.offset*num,
                           data.angle*num);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                if (ec->rtti()==RS2::EntityInsert) {
                    dynamic_cast<RS_Insert*>(ec)->update();
                }
                addList.append(ec);
            }
        }
    }

    deselectOriginals(data.number==0);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}
    
    

/**
 * Creates an ISO projection from the selected entities with the given parameters.
 */
bool RS_Modification::isoProject(RS_IsoProjectData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::isoProject: no valid container");
        return false;
    }
        
    RS_DEBUG->print("RS_Modification::isoProject");

    RS_EntityContainer addContainer;

    bool bakKeepSelection = keepSelection;
    bool bakHandleUndo = handleUndo;
    keepSelection = true;
    handleUndo = false;
    
    RS_DEBUG->print("RS_Modification::isoProject: explode");

    // explode, keep selection, make copy
    explode(true);
    
    RS_DEBUG->print("RS_Modification::isoProject: explode 2");

    // explode, keep selection, make no copy
    explode(false);
    
    RS_DEBUG->print("RS_Modification::isoProject: explode: OK");

    handleUndo = bakHandleUndo;
    keepSelection = bakKeepSelection;
    
    if (document!=NULL && handleUndo) {
        RS_DEBUG->print("RS_Modification::isoProject: 000a");
        document->startUndoCycle();
        RS_DEBUG->print("RS_Modification::isoProject: 000b");
    }
    
    RS_DEBUG->print("RS_Modification::isoProject: 000");

    data.copy = false;

    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, "after explode");
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, container);
    RS_DEBUG->print(RS_Debug::D_ERROR, "---");
    */

    // Create new entites
    for (int num=1;
            num<=(int)data.copy || (data.copy==false && num<=1);
            num++) {
        for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
            if (e!=NULL && e->isSelected()) {
                RS_Entity* ec = e->clone();
                ec->setSelected(false, false);

                //ec->move(-data.referencePoint);
                //ec->isoProject(data.type);
                //ec->move(data.targetPoint);
                if (data.useCurrentLayer) {
                    ec->setLayerToActive();
                }
                if (data.useCurrentAttributes) {
                    ec->setPenToActive();
                }
                addContainer.addEntity(ec);
                //addList.append(ec);
            }
        }
    }

    RS_DEBUG->print("RS_Modification::isoProject: 001");
    
    addContainer.move(-data.referencePoint);
    addContainer.isoProject(data.type, data.segmentLength);
    addContainer.move(data.targetPoint);
    
    RS_DEBUG->print("RS_Modification::isoProject: 002");
    
    RS_PtrList<RS_Entity> addList;
    for (RS_Entity* e=addContainer.firstEntity();
        e!=NULL;
        e=addContainer.nextEntity()) {
        RS_Entity* cl = e->clone();
        addList.append(cl);
    }
    
    RS_DEBUG->print("RS_Modification::isoProject: 003");
    
    bakHandleUndo = handleUndo;
    handleUndo = false;
    deselectOriginals(data.copy==false);
    handleUndo = bakHandleUndo;
    
    RS_DEBUG->print("RS_Modification::isoProject: 004");

    addNewEntities(addList);
    
    RS_DEBUG->print("RS_Modification::isoProject: 005");

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }
    
    RS_DEBUG->print("RS_Modification::isoProject: 006");

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    RS_DEBUG->print("RS_Modification::isoProject: 007");

    return true;
}



/**
 * Deselects all selected entities and removes them if remove is true;
 *
 * @param remove true: Remove entites.
 * @param onlyVisibleSelections true: only affect entities with a visible
 *      selection (used by property editor).
 */
void RS_Modification::deselectOriginals(bool remove, bool /*onlyVisibleSelections*/) {

    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, "");
    RS_DEBUG->print(RS_Debug::D_ERROR, "RS_Modification::deselectOriginals 001:");
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, container);
    */

    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {

        if (e->isSelected()) {
            e->setVisible(true);
            e->setSelected(false, false);
            if (remove) {
                e->changeUndoState();
                if (document!=NULL && handleUndo) {
                    document->addUndoable(e);
                }
            }
        }
    }
    
    // no undo handling: delete originals for good:
    if (!handleUndo && remove) {
        bool done;
        do {
            done = true;
            for (RS_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {
                if (e->isUndone()) {
                    container->removeEntity(e);
                    done = false;
                    break;
                }
            }
        } while(!done);
    }
    
    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, "");
    RS_DEBUG->print(RS_Debug::D_ERROR, "RS_Modification::deselectOriginals 002:");
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, container);
    */
}



/**
 * Adds the given entities to the container and draws the entities if 
 * there's a graphic view available.
 *
 * @param addList Entities to add.
 */
void RS_Modification::addNewEntities(RS_PtrList<RS_Entity>& addList) {
    for (int i=0; i<addList.size(); ++i) {
        RS_Entity* e = addList[i];
        if (e!=NULL) {
            if (keepSelection) {
                e->setSelected(true);
            }
            container->addEntity(e);
            if (document!=NULL && handleUndo) {
                document->addUndoable(e);
            }
        }
    }
    
    // update all new entities:
    for (int i=0; i<addList.size(); ++i) {
        RS_Entity* e = addList[i];
        if (e!=NULL) {
            // this update is questionable, but needed for updating entity contraints:
            e->update();
        }
    }
    
}



/**
 * Trims or extends the given trimEntity to the intersection point of the
 * trimEntity and the limitEntity.
 *
 * @param trimCoord Coordinate which defines which endpoint of the 
 *   trim entity to trim.
 * @param trimEntity Entity which will be trimmed.
 * @param limitCoord Coordinate which defines the intersection to which the
 *    trim entity will be trimmed.
 * @param limitEntity Entity to which the trim entity will be trimmed.
 * @param both true: Trim both entities. false: trim trimEntity only.
 */
bool RS_Modification::trim(const RS_Vector& trimCoord,
                           RS_AtomicEntity* trimEntity,
                           const RS_Vector& limitCoord,
                           RS_Entity* limitEntity,
                           bool both) {
    RS_DEBUG->print("RS_Modification::trim");

    if (trimEntity==NULL || limitEntity==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::trim: At least one entity is NULL");
        return false;
    }

    if (both && !limitEntity->isAtomic()) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::trim: limitEntity is not atomic");
        return false;
    }

    RS_VectorList sol;
    if (limitEntity->isAtomic()) {
        // intersection(s) of the two entities:
        sol = RS_Information::getIntersection(trimEntity, limitEntity, false);
    } else if (limitEntity->isContainer()) {
        RS_EntityContainer* ec = dynamic_cast<RS_EntityContainer*>(limitEntity);

        int i=0;

        for (RS_Entity* e=ec->firstEntity(RS2::ResolveAll); e!=NULL;
                e=ec->nextEntity(RS2::ResolveAll)) {
            //for (int i=0; i<container->count(); ++i) {
            //    RS_Entity* e = container->entityAt(i);

            if (e!=NULL) {

                RS_VectorList s2 = RS_Information::getIntersection(trimEntity,
                                        e, false);

                if (s2.hasValid()) {
                    for (int k=0; k<s2.count(); ++k) {
                        if (i<128 && s2.get(k).valid) {
                            // 20070924: if the container is a block, we can trim to a member line 
                            //   without the intersection being on that line
                            // 20071011: if it's a polyline or spline, the intersection needs to 
                            //   be on the segment.
                            if (e->rtti()==RS2::EntityInsert || e->isPointOnEntity(s2.get(k), 1.0e-4)) {
                                sol.set(i++, s2.get(k));
                            }
                        }
                    }
                    //break;
                }
            }
        }
    }

    if (sol.hasValid()==false) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::trim: No solution");
        return false;
    }

    RS_AtomicEntity* trimmed1 = NULL;
    RS_AtomicEntity* trimmed2 = NULL;

    // remove trim entity from view:
    if (trimEntity->isOfType(RS2::EntityCircle)) {
        // convert a circle into a trimmable arc
        RS_Circle* c = dynamic_cast<RS_Circle*>(trimEntity);
        double am = c->getCenter().angleTo(trimCoord);
        RS_ArcData d(c->getCenter(),
                     c->getRadius(),
                     RS_Math::correctAngle(am-M_PI/2),
                     RS_Math::correctAngle(am+M_PI/2), false);
        trimmed1 = new RS_Arc(trimEntity->getParentContainer(), d);
        trimmed1->setPen(trimEntity->getPen(false));
        trimmed1->setLayer(trimEntity->getLayer(false));
    } else {
        trimmed1 = dynamic_cast<RS_AtomicEntity*>(trimEntity->clone());
        trimmed1->setHighlighted(false);
    }
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(trimEntity);
    }

    // remove limit entity from view:
    if (both) {
        trimmed2 = dynamic_cast<RS_AtomicEntity*>(limitEntity->clone());
        trimmed2->setHighlighted(false);
        if (graphicView!=NULL) {
            //graphicView->deleteEntity(limitEntity);
        }
    }

    // trim trim entity
    int ind = 0;
    RS_Vector is = sol.getClosest(limitCoord, NULL, &ind);
    RS_DEBUG->print("RS_Modification::trim: limitCoord: %f/%f", limitCoord.x, limitCoord.y);
    RS_DEBUG->print("RS_Modification::trim: sol.get(0): %f/%f", sol.get(0).x, sol.get(0).y);
    RS_DEBUG->print("RS_Modification::trim: sol.get(1): %f/%f", sol.get(1).x, sol.get(1).y);
    RS_DEBUG->print("RS_Modification::trim: ind: %d", ind);
    RS_Vector is2 = sol.get(ind==0 ? 1 : 0);
    RS_DEBUG->print("RS_Modification::trim: is2: %f/%f", is2.x, is2.y);

    RS2::Ending ending = trimmed1->getTrimPoint(trimCoord, is);

    switch (ending) {
    case RS2::EndingStart:
        trimmed1->trimStartpoint(is);
        if (trimEntity->isOfType(RS2::EntityCircle)) {
            trimmed1->trimEndpoint(is2);
        }
        break;
    case RS2::EndingEnd:
        trimmed1->trimEndpoint(is);
        if (trimEntity->isOfType(RS2::EntityCircle)) {
            trimmed1->trimStartpoint(is2);
        }
        break;
    default:
        break;
    }

    // trim limit entity:
    if (both) {
        RS_Vector is = sol.getClosest(limitCoord);

        RS2::Ending ending = trimmed2->getTrimPoint(limitCoord, is);

        switch (ending) {
        case RS2::EndingStart:
            trimmed2->trimStartpoint(is);
            break;
        case RS2::EndingEnd:
            trimmed2->trimEndpoint(is);
            break;
        default:
            break;
        }
    }

    // add new trimmed trim entity:
    container->addEntity(trimmed1);

    // add new trimmed limit entity:
    if (both) {
        container->addEntity(trimmed2);
    }

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
        document->addUndoable(trimmed1);
        trimEntity->setUndoState(true);
        document->addUndoable(trimEntity);
        if (both) {
            document->addUndoable(trimmed2);
            limitEntity->setUndoState(true);
            document->addUndoable(limitEntity);
        }
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    RS_DEBUG->print("RS_Modification::trim");
    return true;
}



/**
 * Trims or extends the given trimEntity by the given amount.
 *
 * @param trimCoord Coordinate which defines which endpoint of the 
 *   trim entity to trim.
 * @param trimEntity Entity which will be trimmed.
 * @param dist Amount to trim by.
 */
bool RS_Modification::trimAmount(const RS_Vector& trimCoord,
                                 RS_AtomicEntity* trimEntity,
                                 double dist) {

    if (trimEntity==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::trimAmount: Entity is NULL");
        return false;
    }

    RS_AtomicEntity* trimmed = NULL;

    // remove trim entity:
    trimmed = dynamic_cast<RS_AtomicEntity*>(trimEntity->clone());
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(trimEntity);
    }

    // trim trim entity
    RS_Vector is = trimmed->getNearestDist(-dist, trimCoord);
    if (is.valid) {
        if (trimCoord.distanceTo(trimmed->getStartpoint()) <
            trimCoord.distanceTo(trimmed->getEndpoint())) {
        trimmed->trimStartpoint(is);
        } else {
            trimmed->trimEndpoint(is);
        }
    }
    else {
        delete trimmed;
        return false;
    }

    // add new trimmed trim entity:
    container->addEntity(trimmed);

    if (graphicView!=NULL) {
        //graphicView->drawEntity(trimmed);
    }

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
        document->addUndoable(trimmed);
        trimEntity->setUndoState(true);
        document->addUndoable(trimEntity);
        document->endUndoCycle();
    }
    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}



/**
 * Cuts the given entity at the given point.
 */
bool RS_Modification::divide(const RS_Vector& cutCoord,
                          RS_AtomicEntity* cutEntity) {

    if (cutEntity==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide: Entity is NULL");
        return false;
    }

    if (!cutCoord.valid) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide: Point invalid.");
        return false;
    }

    // divide point is at endpoint of entity:
    if (cutCoord.distanceTo(cutEntity->getStartpoint())<1.0e-6 ||
            cutCoord.distanceTo(cutEntity->getEndpoint())<1.0e-6) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide: Cutting point on endpoint");
        return false;
    }

    // delete divide entity on the screen:
    if (graphicView!=NULL) {
        graphicView->deleteEntity(cutEntity);
    }

    RS_AtomicEntity* rest1 = NULL;
    RS_AtomicEntity* rest2 = NULL;

    // create new two halves:
    if (cutEntity->isOfType(RS2::EntityCircle)) {
        RS_Circle* c = dynamic_cast<RS_Circle*>(cutEntity);
        rest1 = new RS_Arc(cutEntity->getParentContainer(),
                          RS_ArcData(c->getCenter(),
                                     c->getRadius(),
                                     0.0,0.0, false));
        rest1->setPen(cutEntity->getPen(false));
        rest1->setLayer(cutEntity->getLayer(false));
        rest2 = NULL;

        rest1->trimEndpoint(cutCoord);
        rest1->trimStartpoint(cutCoord);
    } else {
        rest1 = dynamic_cast<RS_AtomicEntity*>(cutEntity->clone());
        rest2 = dynamic_cast<RS_AtomicEntity*>(cutEntity->clone());

        rest1->trimEndpoint(cutCoord);
        rest2->trimStartpoint(cutCoord);
    }

    // add new rest entities:
    container->addEntity(rest1);
    if (rest2!=NULL) {
        container->addEntity(rest2);
    }

    if (graphicView!=NULL) {
        graphicView->drawEntity(rest1);
        if (rest2!=NULL) {
            graphicView->drawEntity(rest2);
        }
    }

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
        document->addUndoable(rest1);
        if (rest2!=NULL) {
            document->addUndoable(rest2);
        }
        cutEntity->setUndoState(true);
        document->addUndoable(cutEntity);
        document->endUndoCycle();
    }

    return true;
}
    


/**
 * Removes the segment of the given cutEntity that is limited by the
 * two intersections on the cutEntity that are closest to cursor.
 *
 * @param cutEntity: The entity to divide and remove a segment from.
 * @param cursor: The mouse cursor position when selecting the entity.
 * @param remove: true: remove segment, false: divide only
 */
bool RS_Modification::divide2(RS_AtomicEntity* cutEntity, const RS_Vector& cursor, bool remove) {
    RS_Vector iClosestRight(false);
    double distRight = RS_MAXDOUBLE;
    RS_Vector iClosestLeft(false);
    double distLeft = RS_MAXDOUBLE;

    RS_Line* orthoLine = NULL;
    
    if (!cutEntity->isOfType(RS2::EntityLine) &&
        !cutEntity->isOfType(RS2::EntityArc) &&
        !cutEntity->isOfType(RS2::EntityCircle)) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide2: Entity type not supported.");
        return false;
    }
    
    // auxiliary line othogonal to entity and through cursor:
    RS_Vector p = cutEntity->getNearestPointOnEntity(cursor, true, NULL, NULL);
    if (cutEntity->isOfType(RS2::EntityLine)) {
        double orthoAngle = cutEntity->getDirection1()+M_PI/2.0;
        RS_Vector r;
        r.setPolar(1.0, orthoAngle);
        orthoLine = new RS_Line(NULL, RS_LineData(cursor, cursor+r));
    }
    else if (cutEntity->isOfType(RS2::EntityArc)) {
        RS_Arc* arc = dynamic_cast<RS_Arc*>(cutEntity);
        orthoLine = new RS_Line(NULL, RS_LineData(arc->getCenter(), cursor));
    }
    else if (cutEntity->isOfType(RS2::EntityCircle)) {
        RS_Circle* circle = dynamic_cast<RS_Circle*>(cutEntity);
        orthoLine = new RS_Line(NULL, RS_LineData(circle->getCenter(), cursor));
    }

    if (orthoLine==NULL) {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "RS_Modification::divide2: unsupported entity.");
        return false;
    }

    // collect all intersections:
    RS_ValueList<RS_Vector> intersections;
    if (!cutEntity->isOfType(RS2::EntityCircle)) {
        intersections.append(cutEntity->getStartpoint());
        intersections.append(cutEntity->getEndpoint());
    }
    for (RS_Entity* e=container->firstEntity(RS2::ResolveAll);
            e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
        if (e!=NULL && e->isVisible()) {

            // find closest intersections (cutting points):
            RS_VectorList sol = RS_Information::getIntersection(cutEntity, e, true);
            for (int i=0; i<sol.count(); i++) {
                RS_Vector inters = sol.get(i);
                intersections.append(inters);
            }
        }
    }

    RS_ValueList<RS_Vector>::iterator it;
    for (it = intersections.begin(); it!=intersections.end(); ++it) {
        if ((*it).valid) {
            // find closest intersections (cutting points):
            RS_Vector inters = (*it);
            RS2::Side s = orthoLine->getSideOfPoint(inters);
            double dist = inters.distanceTo(cursor);

            if (s==RS2::RightHand) {
                if (dist<distRight) {
                    iClosestRight = inters;
                    distRight = dist;
                }
            }
            else if (s==RS2::LeftHand) {
                if (dist<distLeft) {
                    iClosestLeft = inters;
                    distLeft = dist;
                }
            }
        }
    }

    if (orthoLine!=NULL) {
        delete orthoLine;
    }

    return divide2(iClosestRight, iClosestLeft, cutEntity, cursor, remove);
}
    
    
/**
 * Cuts the given entity at the two given coordinates and optionally
 * removes the segment in between.
 *
 * @param divideCoord1: 1st dividing point on cutEntity.
 * @param divideCoord2: 2nd dividing point on cutEntity.
 * @param cutEntity: The entity to divide and remove a segment from.
 * @param cursor: The mouse cursor position when selecting the entity.
 *   This is needed for circles to determine which segment of the circle
 *   has to be removed (the one closer to the mouse cursor).
 * @param remove: true: remove segment, false: divide only
 */
bool RS_Modification::divide2(const RS_Vector& divideCoord1, const RS_Vector& divideCoord2, 
        RS_AtomicEntity* cutEntity, const RS_Vector& cursor, bool remove) {

    RS_DEBUG->print("RS_Modification::divide2");

    if (cutEntity==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide2: Entity is NULL");
        return false;
    }

    if (!cutEntity->isOfType(RS2::EntityCircle) && 
        (!divideCoord1.valid || !divideCoord2.valid)) {

        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide2: At least one dividing point invalid.");
        return false;
    }

    if (!cutEntity->isOfType(RS2::EntityLine) &&
        !cutEntity->isOfType(RS2::EntityArc) &&
        !cutEntity->isOfType(RS2::EntityCircle)) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::divide2: Entity type not supported.");
        return false;
    }

    // delete divide entity on the screen:
    if (graphicView!=NULL) {
        graphicView->deleteEntity(cutEntity);
    }
        
    RS_AtomicEntity* rest1 = NULL;
    RS_AtomicEntity* rest2 = NULL;
    RS_AtomicEntity* segment = NULL;

    // lines:
    if (cutEntity->isOfType(RS2::EntityLine)) {
        RS_Line* line = dynamic_cast<RS_Line*>(cutEntity);
        rest1 = dynamic_cast<RS_AtomicEntity*>(line->clone());
        rest2 = dynamic_cast<RS_AtomicEntity*>(line->clone());

        if (line->getStartpoint().distanceTo(divideCoord1) < 
            line->getStartpoint().distanceTo(divideCoord2)) {
            rest1->trimEndpoint(divideCoord1);
            rest2->trimStartpoint(divideCoord2);
        }
        else {
            rest1->trimEndpoint(divideCoord2);
            rest2->trimStartpoint(divideCoord1);
        }

        if (!remove) {
            segment = dynamic_cast<RS_AtomicEntity*>(line->clone());
            segment->trimStartpoint(divideCoord1);
            segment->trimEndpoint(divideCoord2);
        }

        if (rest1->getLength()<1.0e-6) {
            delete rest1;
            rest1 = NULL;
        }
        
        if (rest2->getLength()<1.0e-6) {
            delete rest2;
            rest2 = NULL;
        }
    }

    // arcs:
    else if (cutEntity->isOfType(RS2::EntityArc)) {
        RS_Arc* arc = dynamic_cast<RS_Arc*>(cutEntity);
        rest1 = dynamic_cast<RS_AtomicEntity*>(arc->clone());
        rest2 = dynamic_cast<RS_AtomicEntity*>(arc->clone());
        if (!remove) {
            segment = dynamic_cast<RS_AtomicEntity*>(arc->clone());
        }
        
        rest1->trimEndpoint(divideCoord1);
        rest2->trimStartpoint(divideCoord2);

        if (!remove) {
            segment->trimStartpoint(divideCoord1);
            segment->trimEndpoint(divideCoord2);
        }
        
        double angleLength1 = dynamic_cast<RS_Arc*>(rest1)->getAngleLength(true);
        double angleLength2 = dynamic_cast<RS_Arc*>(rest2)->getAngleLength(true);
        
        if (angleLength1+angleLength2 > arc->getAngleLength()) {
            rest1->trimEndpoint(divideCoord2);
            rest2->trimStartpoint(divideCoord1);

            if (!remove) {
                segment->trimStartpoint(divideCoord2);
                segment->trimEndpoint(divideCoord1);
            }
        
            angleLength1 = (dynamic_cast<RS_Arc*>(rest1))->getAngleLength(true);
            angleLength2 = (dynamic_cast<RS_Arc*>(rest2))->getAngleLength(true);
        }
        
        if (angleLength1<1.0e-6) {
            delete rest1;
            rest1 = NULL;
        }
        
        if (angleLength2<1.0e-6) {
            delete rest2;
            rest2 = NULL;
        }
    }

    // circles:
    else if (cutEntity->isOfType(RS2::EntityCircle)) {
        if (!divideCoord1.valid || !divideCoord2.valid) {
            rest1 = NULL;
            rest2 = NULL;
        }
        else {
            RS_Circle* circle = dynamic_cast<RS_Circle*>(cutEntity);

            double angle1 = circle->getCenter().angleTo(divideCoord1);
            double angle2 = circle->getCenter().angleTo(divideCoord2);

            rest1 = dynamic_cast<RS_AtomicEntity*>(
                new RS_Arc(container, 
                    RS_ArcData(
                        circle->getCenter(),
                        circle->getRadius(),
                        angle1, angle2,
                        false
                    )
                )
            );
            rest1->setPen(circle->getPen(false));
            rest1->setLayer(circle->getLayer(false));
            rest2 = NULL;
            
            if (!remove) {
                segment = dynamic_cast<RS_AtomicEntity*>(
                    new RS_Arc(container, 
                        RS_ArcData(
                            circle->getCenter(),
                            circle->getRadius(),
                            angle2, angle1,
                            false
                        )
                    )
                );
                segment->setPen(circle->getPen(false));
                segment->setLayer(circle->getLayer(false));
            }

            double cursorAngle = circle->getCenter().angleTo(cursor);

            RS_DEBUG->print("RS_Modification::divide2: center: %f/%f", 
                circle->getCenter().x, circle->getCenter().y);
            RS_DEBUG->print("RS_Modification::divide2: cursor: %f/%f", 
                cursor.x, cursor.y);
            
            RS_DEBUG->print("RS_Modification::divide2: cursorAngle: %f", cursorAngle);
            RS_DEBUG->print("RS_Modification::divide2: angle1: %f", angle1);
            RS_DEBUG->print("RS_Modification::divide2: angle2: %f", angle2);

            if (RS_Math::isAngleBetween(cursorAngle, angle1, angle2, false)) {
                RS_DEBUG->print("RS_Modification::divide2: reversing..");
                (dynamic_cast<RS_Arc*>(rest1))->setAngle1(angle2);
                (dynamic_cast<RS_Arc*>(rest1))->setAngle2(angle1);
                if (!remove) {
                    (dynamic_cast<RS_Arc*>(segment))->setAngle1(angle1);
                    (dynamic_cast<RS_Arc*>(segment))->setAngle2(angle2);
                }
                rest1->calculateBorders();
            }
            
            double angleLength1 = (dynamic_cast<RS_Arc*>(rest1))->getAngleLength(true);
            
            if (angleLength1<1.0e-6) {
                delete rest1;
                rest1 = NULL;
            }
        }
    }
        
    // add new rest entities:
    if (rest1!=NULL) {
        container->addEntity(rest1);
    }
    if (rest2!=NULL) {
        container->addEntity(rest2);
    }
    if (segment!=NULL) {
        container->addEntity(segment);
    }

    /*
    if (graphicView!=NULL) {
        if (rest1!=NULL) {
            graphicView->drawEntity(rest1);
        }
        if (rest2!=NULL) {
            graphicView->drawEntity(rest2);
        }
    }
    */

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
        if (rest1!=NULL) {
            document->addUndoable(rest1);
        }
        if (rest2!=NULL) {
            document->addUndoable(rest2);
        }
        if (segment!=NULL) {
            document->addUndoable(segment);
        }
        cutEntity->setUndoState(true);
        document->addUndoable(cutEntity);
        document->endUndoCycle();
    }
    else {
        container->removeEntity(cutEntity);
    }
    
    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    
    RS_DEBUG->print("RS_Modification::divide2: OK");
    return true;
    
}



/**
 * Stretching.
 */
bool RS_Modification::stretch(const RS_Vector& firstCorner,
                              const RS_Vector& secondCorner,
                              const RS_Vector& offset) {

    if (!offset.valid) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::stretch: Offset invalid");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        e->setSelected(false, false);

        if (e!=NULL &&
                e->isVisible() &&
                !e->isLocked() &&
                (e->isInWindow(firstCorner, secondCorner) ||
                 e->hasEndpointsWithinWindow(firstCorner, secondCorner))) {

            RS_Entity* ec = e->clone();
            ec->stretch(firstCorner, secondCorner, offset);
            addList.append(ec);
            e->setSelected(true);
        }
    }

    deselectOriginals(true);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}



/**
 * Bevels a corner.
 *
 * @param coord1 Mouse coordinate to specify direction from intersection.
 * @param entity1 First entity of the corner.
 * @param coord2 Mouse coordinate to specify direction from intersection.
 * @param entity2 Second entity of the corner.
 * @param data Lengths and trim flag.
 */
bool RS_Modification::bevel(const RS_Vector& coord1, RS_AtomicEntity* entity1,
                            const RS_Vector& coord2, RS_AtomicEntity* entity2,
                            RS_BevelData& data) {

    RS_DEBUG->print("RS_Modification::bevel");
    
    if (entity1==NULL || entity2==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::bevel: At least one entity is NULL");
        return false;
    }

    RS_EntityContainer* baseContainer = container;
    bool isPolyline = false;
    bool isClosedPolyline = false;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // find out whether we're bevelling within a polyline:
    if (entity1->getParentContainer()!=NULL && 
        entity1->getParentContainer()->rtti()==RS2::EntityPolyline) {
        
        RS_DEBUG->print("RS_Modification::bevel: trimming polyline segments");
        
        if (entity1->getParentContainer()!=entity2->getParentContainer()) {
            RS_DEBUG->print(RS_Debug::D_WARNING,
                            "RS_Modification::bevel: entities not in "
                            "the same polyline");
            if (document!=NULL && handleUndo) {
                document->endUndoCycle();
            }
            return false;
        }
        
        if (dynamic_cast<RS_Polyline*>(entity1->getParentContainer())->validate()==false) {
            RS_DEBUG->print(RS_Debug::D_ERROR,
                            "RS_Modification::bevel: polyline is not valid");
            return false;
        }
        
        // clone polyline for undo
        if (document!=NULL && handleUndo) {
            RS_EntityContainer* cl =
                dynamic_cast<RS_EntityContainer*>(entity1->getParentContainer()->clone());
            container->addEntity(cl);
            document->addUndoable(cl);

            document->addUndoable(entity1->getParentContainer());
            entity1->getParentContainer()->setUndoState(true);

            baseContainer = cl;
        }
        else {
            baseContainer = dynamic_cast<RS_EntityContainer*>(entity1->getParentContainer());
        }
        
        RS_Polyline* baseContainerPl = dynamic_cast<RS_Polyline*>(baseContainer);

        entity1 = 
            dynamic_cast<RS_AtomicEntity*>(
                baseContainerPl->entityAt(
                    entity1->getParentContainer()->findEntity(entity1)
                )
            );
        entity2 = 
            dynamic_cast<RS_AtomicEntity*>(
                baseContainerPl->entityAt(
                    entity2->getParentContainer()->findEntity(entity2)
                )
            );

        isPolyline = true;
        isClosedPolyline = baseContainerPl->isClosed();
    }
        
    RS_DEBUG->print("RS_Modification::bevel: getting intersection");

    RS_VectorList sol =
        RS_Information::getIntersection(entity1, entity2, false);

    if (sol.count()==0) {
        return false;
    }

    RS_AtomicEntity* trimmed1 = NULL;
    RS_AtomicEntity* trimmed2 = NULL;
    RS2::Ending ending1 = RS2::EndingStart;
    RS2::Ending ending2 = RS2::EndingStart;

    //if (data.trim || isPolyline) {
        if (isPolyline) {
            trimmed1 = entity1;
            trimmed2 = entity2;
        } else {
            trimmed1 = dynamic_cast<RS_AtomicEntity*>(entity1->clone());
            trimmed2 = dynamic_cast<RS_AtomicEntity*>(entity2->clone());
        }

        // trim entities to intersection
        RS_DEBUG->print("RS_Modification::bevel: trim entities to intersection 01");
        bool start1 = false;
        RS_Vector is = sol.getClosest(coord2);
        ending1 = trimmed1->getTrimPoint(coord1, is);
        switch (ending1) {
        case RS2::EndingStart:
            trimmed1->trimStartpoint(is);
            start1 = true;
            break;
        case RS2::EndingEnd:
            trimmed1->trimEndpoint(is);
            start1 = false;
            break;
        default:
            break;
        }

        RS_DEBUG->print("RS_Modification::bevel: trim entities to intersection 02");
        bool start2 = false;
        is = sol.getClosest(coord1);
        ending2 = trimmed2->getTrimPoint(coord2, is);
        switch (ending2) {
        case RS2::EndingStart:
            trimmed2->trimStartpoint(is);
            start2 = true;
            break;
        case RS2::EndingEnd:
            trimmed2->trimEndpoint(is);
            start2 = false;
            break;
        default:
            break;
        }
    //}


    // find definitive bevel points
    RS_DEBUG->print("RS_Modification::bevel: find definitive bevel points");
    RS_Vector bp1 = trimmed1->getNearestDist(data.length1, start1);
    RS_Vector bp2 = trimmed2->getNearestDist(data.length2, start2);

    // final trim:
    RS_DEBUG->print("RS_Modification::bevel: final trim");
    if (data.trim==true) {
        switch (ending1) {
        case RS2::EndingStart:
            trimmed1->trimStartpoint(bp1);
            break;
        case RS2::EndingEnd:
            trimmed1->trimEndpoint(bp1);
            break;
        default:
            break;
        }

        switch (ending2) {
        case RS2::EndingStart:
            trimmed2->trimStartpoint(bp2);
            break;
        case RS2::EndingEnd:
            trimmed2->trimEndpoint(bp2);
            break;
        default:
            break;
        }

        // add new trimmed entities:
        if (isPolyline==false) {
            container->addEntity(trimmed1);
            container->addEntity(trimmed2);
        }
    }


    // add bevel line:
    RS_DEBUG->print("RS_Modification::bevel: add bevel line");
    RS_Line* bevel = new RS_Line(baseContainer, RS_LineData(bp1, bp2));

    if (isPolyline==false) {
        baseContainer->addEntity(bevel);
    } else {
        int idx1 = baseContainer->findEntity(trimmed1);
        int idx2 = baseContainer->findEntity(trimmed2);

        bevel->setSelected(baseContainer->isSelected());
        bevel->setLayer(baseContainer->getLayer(false));
        bevel->setPen(baseContainer->getPen(false));
        
        bool insertAfter1 = false;
        if (ending1==RS2::EndingEnd) {
            insertAfter1 = true;
        }
        else {
            insertAfter1 = false;
        }

        // insert bevel at the right position:
        if (insertAfter1) {
            if (trimmed1->getEndpoint().distanceTo(bevel->getStartpoint())>1.0e-4) {
                bevel->reverse();
            }
            baseContainer->insertEntity(idx1+1, bevel);
        } else {
            if (trimmed2->getEndpoint().distanceTo(bevel->getStartpoint())>1.0e-4) {
                bevel->reverse();
            }
            baseContainer->insertEntity(idx2+1, bevel);
        }
    }

    if (isPolyline) {
        dynamic_cast<RS_Polyline*>(baseContainer)->updateEndpoints();
    }

    RS_DEBUG->print("RS_Modification::bevel: handling undo");

    if (document!=NULL && handleUndo) {
        if (isPolyline==false && data.trim==true) {
            document->addUndoable(trimmed1);
            entity1->setUndoState(true);
            document->addUndoable(entity1);

            document->addUndoable(trimmed2);
            entity2->setUndoState(true);
            document->addUndoable(entity2);
        }

        if (isPolyline==false) {
            document->addUndoable(bevel);
        }

        document->endUndoCycle();
    }

    if (data.trim==false) {
        RS_DEBUG->print("RS_Modification::bevel: delete trimmed elements");
        delete trimmed1;
        delete trimmed2;
        RS_DEBUG->print("RS_Modification::bevel: delete trimmed elements: ok");
    }
    
    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;

}



/**
 * Rounds a corner.
 *
 * @param coord Mouse coordinate to specify the rounding.
 * @param entity1 First entity of the corner.
 * @param entity2 Second entity of the corner.
 * @param data Radius and trim flag.
 */
bool RS_Modification::round(const RS_Vector& coord,
                            const RS_Vector& coord1,
                            RS_AtomicEntity* entity1,
                            const RS_Vector& coord2,
                            RS_AtomicEntity* entity2,
                            RS_RoundData& data) {

    RS_DEBUG->print("RS_Modification::round");
                    
    if (entity1==NULL || entity2==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Modification::round: At least one entity is NULL");
        return false;
    }

    RS_EntityContainer* baseContainer = container;
    bool isPolyline = false;
    bool isClosedPolyline = false;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // find out whether we're rounding within a polyline:
    if (entity1->getParentContainer()!=NULL && 
        entity1->getParentContainer()->rtti()==RS2::EntityPolyline) {
    
        RS_DEBUG->print("RS_Modification::round: rounding inside polyline");

        if (entity1->getParentContainer()!=entity2->getParentContainer()) {
            RS_DEBUG->print(RS_Debug::D_WARNING,
                            "RS_Modification::round: entities not in "
                            "the same polyline");
            if (document!=NULL && handleUndo) {
                document->endUndoCycle();
            }
            return false;
        }

        if (dynamic_cast<RS_Polyline*>(entity1->getParentContainer())->validate()==false) {
            RS_DEBUG->print(RS_Debug::D_ERROR,
                            "RS_Modification::round: polyline is not valid");
            return false;
        }

        // clone polyline for undo
        if (document!=NULL && handleUndo) {
            RS_EntityContainer* cl =
                dynamic_cast<RS_EntityContainer*>(entity1->getParentContainer()->clone());
            container->addEntity(cl);
            document->addUndoable(cl);

            document->addUndoable(entity1->getParentContainer());
            entity1->getParentContainer()->setUndoState(true);

            baseContainer = cl;
        }
        else {
            baseContainer = (RS_EntityContainer*)entity1->getParentContainer();
        }

        RS_Polyline* baseContainerPl = (RS_Polyline*)baseContainer;

        entity1 = 
            dynamic_cast<RS_AtomicEntity*>(
                baseContainerPl->entityAt(
                    entity1->getParentContainer()->findEntity(entity1)
                )
            );
        entity2 = 
            dynamic_cast<RS_AtomicEntity*>(
                baseContainerPl->entityAt(
                    entity2->getParentContainer()->findEntity(entity2)
                )
            );
        
        isPolyline = true;
        isClosedPolyline = baseContainerPl->isClosed(); /*||
            baseContainerPl->getStartpoint().distanceTo(baseContainerPl->getEndpoint())<1.0e-6;*/
        
        RS_DEBUG->print("RS_Modification::round: polyline closed: %d", 
            (int)isClosedPolyline);
    }

    // create 2 tmp parallels
    RS_DEBUG->print("RS_Modification::round: creating tmp parallels");
    RS_Creation creation(NULL, NULL);
    RS_Entity* par1 = creation.createParallel(coord, data.radius, 1, entity1);
    RS_Entity* par2 = creation.createParallel(coord, data.radius, 1, entity2);

    RS_VectorList sol2 =
        RS_Information::getIntersection(entity1, entity2, false);

    RS_VectorList sol =
        RS_Information::getIntersection(par1, par2, false);

    if (sol.count()==0) {
        if (document!=NULL && handleUndo) {
            document->endUndoCycle();
        }
        RS_DEBUG->print("RS_Modification::round: parallels don't intersect. "
            "Aborting..");
        return false;
    }

    // there might be two intersections: choose the closest:
    RS_Vector is = sol.getClosest(coord);
    RS_Vector p1 = entity1->getNearestPointOnEntity(is, false);
    RS_Vector p2 = entity2->getNearestPointOnEntity(is, false);
    double ang1 = is.angleTo(p1);
    double ang2 = is.angleTo(p2);
    bool reversed = (RS_Math::getAngleDifference(ang1, ang2)>M_PI);

    RS_Arc* arc = new RS_Arc(baseContainer,
                             RS_ArcData(is,
                                        data.radius,
                                        ang1, ang2,
                                        reversed));


    RS_AtomicEntity* trimmed1 = NULL;
    RS_AtomicEntity* trimmed2 = NULL;
    RS2::Ending ending1 = RS2::EndingStart;
    RS2::Ending ending2 = RS2::EndingStart;

    if (data.trim || isPolyline) {
        if (isPolyline) {
            trimmed1 = entity1;
            trimmed2 = entity2;
        } else {
            trimmed1 = dynamic_cast<RS_AtomicEntity*>(entity1->clone());
            trimmed2 = dynamic_cast<RS_AtomicEntity*>(entity2->clone());
        }

        // trim entities to intersection
        RS_Vector is2 = sol2.getClosest(coord2);
        ending1 = trimmed1->getTrimPoint(coord1, is2);
        switch (ending1) {
        case RS2::EndingStart:
            trimmed1->trimStartpoint(p1);
            break;
        case RS2::EndingEnd:
            trimmed1->trimEndpoint(p1);
            break;
        default:
            break;
        }

        is2 = sol2.getClosest(coord1);
        ending2 = trimmed2->getTrimPoint(coord2, is2);
        switch (ending2) {
        case RS2::EndingStart:
            trimmed2->trimStartpoint(p2);
            break;
        case RS2::EndingEnd:
            trimmed2->trimEndpoint(p2);
            break;
        default:
            break;
        }

        // add new trimmed entities:
        if (isPolyline==false) {
            container->addEntity(trimmed1);
            container->addEntity(trimmed2);
        }
    }

    // add rounding:
    if (isPolyline==false) {
        baseContainer->addEntity(arc);
    } else {
        // find out which base entity is before the rounding:
        int idx1 = baseContainer->findEntity(trimmed1);
        int idx2 = baseContainer->findEntity(trimmed2);
        
        RS_DEBUG->print("RS_Modification::round: idx1: %d", idx1);
        RS_DEBUG->print("RS_Modification::round: idx2: %d", idx2);

        arc->setSelected(baseContainer->isSelected());
        arc->setLayer(baseContainer->getLayer(false));
        arc->setPen(baseContainer->getPen(false));

        RS_DEBUG->print("RS_Modification::round: idx1<idx2: %d", (int)(idx1<idx2));
        RS_DEBUG->print("RS_Modification::round: idx1!=0: %d", (int)(idx1!=0));
        RS_DEBUG->print("RS_Modification::round: idx2==0: %d", (int)(idx2==0));
        RS_DEBUG->print("RS_Modification::round: idx1==(int)baseContainer->count()-1: %d", 
            (int)(idx1==(int)baseContainer->count()-1));

        bool insertAfter1 = false;
        if (ending1==RS2::EndingEnd) {
            insertAfter1 = true;
        }
        else {
            insertAfter1 = false;
        }
        
        RS_DEBUG->print("RS_Modification::round: insertAfter1: %d", (int)(insertAfter1));

        // insert rounding at the right position:
        if (insertAfter1) {
            if (trimmed1->getEndpoint().distanceTo(arc->getStartpoint())>1.0e-4) {
                arc->reverse();
            }
            baseContainer->insertEntity(idx1+1, arc);
        } else {
            if (trimmed2->getEndpoint().distanceTo(arc->getStartpoint())>1.0e-4) {
                arc->reverse();
            }
            baseContainer->insertEntity(idx2+1, arc);
        }
    }

    if (isPolyline) {
        RS_DEBUG->print("RS_Modification::round: updating endpoints of polyline");
        ((RS_Polyline*)baseContainer)->updateEndpoints();
    }

    if (document!=NULL && handleUndo) {
        if (isPolyline==false && data.trim==true) {
            document->addUndoable(trimmed1);
            entity1->setUndoState(true);
            document->addUndoable(entity1);

            document->addUndoable(trimmed2);
            entity2->setUndoState(true);
            document->addUndoable(entity2);
        }

        if (isPolyline==false) {
            document->addUndoable(arc);
        }

        document->endUndoCycle();
    }

    delete par1;
    delete par2;
    
    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    
    RS_DEBUG->print("RS_Modification::round: OK");

    return true;
}



/**
 * Removes the selected entity containers and adds the entities in them as
 * new single entities.
 */
bool RS_Modification::explode(bool copy) {

    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "RS_Modification::explode: no valid container"
            " for addinge entities");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
    
    RS_SETTINGS->beginGroup("/Modify");
    int ellipseExplodeSegments = RS_SETTINGS->readNumEntry("/EllipseExplodeSegments", 32);
    RS_SETTINGS->endGroup();

    for (RS_Entity* e=container->firstEntity();
        e!=NULL;
        e=container->nextEntity()) {

        if (e!=NULL && e->isSelected()) {
            // explode ellipse:
            if (e->rtti()==RS2::EntityEllipse) {
                RS_Ellipse* ellipse = dynamic_cast<RS_Ellipse*>(e);
                RS_Polyline* pl = ellipse->approximateWithArcs(ellipseExplodeSegments);
                if (pl!=NULL) {
                    pl->reparent(container);
                    addList.append(pl);
                }
            }

            // explode container:
            else if (e->isContainer()) {

                // add entities from container:
                RS_EntityContainer* ec = dynamic_cast<RS_EntityContainer*>(e);

                // iterate and explode container:
                RS2::ResolveLevel rl;
                bool resolvePen;
                bool resolveLayer;

                switch (ec->rtti()) {
                case RS2::EntityText:
                case RS2::EntityPolyline:
                    rl = RS2::ResolveAll;
                    resolveLayer = true;
                    resolvePen = false;
                    break;
                
                case RS2::EntityHatch:
                    rl = RS2::ResolveAll;
                    resolveLayer = true;
                    resolvePen = false;
                    break;

                case RS2::EntityInsert:
                    rl = RS2::ResolveNone;
                    resolveLayer = false;
                    resolvePen = false;
                    break;

                case RS2::EntityDimAligned:
                case RS2::EntityDimLinear:
                case RS2::EntityDimRadial:
                case RS2::EntityDimDiametric:
                case RS2::EntityDimAngular:
                case RS2::EntityDimLeader:
                    rl = RS2::ResolveNone;
                    resolveLayer = true;
                    resolvePen = false;
                    break;

                default:
                    rl = RS2::ResolveAll;
                    resolveLayer = true;
                    resolvePen = false;
                    break;
                }

                for (RS_Entity* e2 = ec->firstEntity(rl); e2!=NULL;
                        e2 = ec->nextEntity(rl)) {
                
                    if (e2!=NULL) {
                        bool add = true;

                        // 20071110: ignore hatch outlines which have flag RS2::FlagTemp NOT set
                        if (ec->rtti()==RS2::EntityHatch) {
                            add = false;
                            if (e2->getParentContainer()!=NULL &&
                                e2->getParentContainer()->getFlag(RS2::FlagTemp)) {
                                add = true;
                            }
                        }

                        if (add) {
                            RS_Entity* clone = e2->clone();
                            clone->setSelected(false);
                            clone->reparent(container);

                            if (resolveLayer) {
                                clone->setLayer(ec->getLayer());
                            } else {
                                clone->setLayer(e2->getLayer());
                            }

                            clone->setPen(ec->getPen(resolvePen));

                            addList.append(clone);

                            clone->update();
                        }
                    }
                }
            } else {
                /*
                if (!keepSelection) {
                    e->setSelected(false);
                }
                */
                RS_Entity* clone = e->clone();
                addList.append(clone);
            }
        }
    }

    deselectOriginals(!copy);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}



bool RS_Modification::explodeTextIntoLetters() {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Modification::explodeTextIntoLetters: no valid container"
            " for addinge entities");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isSelected()) {
            if (e->rtti()==RS2::EntityText) {
                // add letters of text:
                RS_Text* text = dynamic_cast<RS_Text*>(e);
                explodeTextIntoLetters(text, addList);
            } else {
                e->setSelected(false);
            }
        }
    }

    deselectOriginals(true);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}



bool RS_Modification::explodeTextIntoLetters(RS_Text* text, RS_PtrList<RS_Entity>& addList) {

    if (text==NULL) {
        return false;
    }

    // iterate though lines:
    for (RS_Entity* e2 = text->firstEntity(); e2!=NULL;
            e2 = text->nextEntity()) {

        if (e2==NULL) {
            break;
        }


        // text lines:
        if (e2->rtti()==RS2::EntityContainer) {

            RS_EntityContainer* line = dynamic_cast<RS_EntityContainer*>(e2);

            // iterate though letters:
            for (RS_Entity* e3 = line->firstEntity(); e3!=NULL;
                    e3 = line->nextEntity()) {

                if (e3==NULL) {
                    break;
                }

                // super / sub texts:
                if (e3->rtti()==RS2::EntityText) {
                    explodeTextIntoLetters(dynamic_cast<RS_Text*>(e3), addList);
                }

                // normal letters:
                else if (e3->rtti()==RS2::EntityInsert) {

                    RS_Insert* letter = dynamic_cast<RS_Insert*>(e3);

                    RS_Text* tl = new RS_Text(
                                      container,
                                      RS_TextData(letter->getInsertionPoint(),
                                                  text->getHeight(),
                                                  100.0,
                                                  RS2::VAlignBottom, RS2::HAlignLeft,
                                                  RS2::LeftToRight, RS2::Exact,
                                                  1.0,
                                                  letter->getName(),
                                                  text->getStyle(),
                                                  letter->getAngle(),
                                                  RS2::Update));

                    tl->setLayer(text->getLayer(false));
                    tl->setPen(text->getPen(false));

                    addList.append(tl);
                    tl->update();
                }
            }
        }
    }

    return true;
}



bool RS_Modification::explodeTextIntoLines(RS_Text* text, RS_PtrList<RS_Entity>& addList) {

    if (text==NULL) {
        return false;
    }
    
    RS_StringList lines = text->getText().split('\n');
    for (int i=0; i<lines.size(); ++i) {
        // insertion point:
        RS_Vector insPoint = text->getLineInsertionPoints().get(i);
        
        RS_Text* tl = new RS_Text(
                          container,
                          RS_TextData(insPoint,
                                      text->getHeight(),
                                      100.0,
                                      RS2::VAlignTop, 
                                      text->getHAlign(),
                                      RS2::LeftToRight, 
                                      RS2::Exact,
                                      1.0,
                                      lines.at(i),
                                      text->getStyle(),
                                      text->getAngle(),
                                      RS2::Update));

        tl->setLayer(text->getLayer(false));
        tl->setPen(text->getPen(false));

        addList.append(tl);
        tl->update();
    }

    return true;
}



/**
 * Moves all reference points of selected entities with the given data.
 */
bool RS_Modification::moveRef(RS_MoveRefData& data) {
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::moveRef: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isSelected() && e->isVisible()) {
            RS_Entity* ec = e->clone();
            // disable original entity when surrounding walls are updated:
            e->setVisible(false);

            ec->moveRef(data.ref, data.offset);
            ec->setSelected(true);

            //RS_DEBUG->printEntity(RS_Debug::D_ERROR, ec);
            
            addList.append(ec);
        }
    }

    RS_DEBUG->print("RS_Modification::moveRef: "
        "%d entities in addList", addList.count());

    deselectOriginals(true);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    return true;
}


/**
 * Triangulates all selected hatches and slabs in this container.
 */
#ifdef RS_ALGO
bool RS_Modification::triangulate() {
    RS_DEBUG->print("RS_Modification::triangulate");
    
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::triangulate: no valid container");
        return false;
    }

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }

    // Create new entites
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isSelected() && e->isOfType(RS2::EntityHatch)) {

            // triangulate using HGRD
            RS_Hatch* hatch = dynamic_cast<RS_Hatch*>(e);
            
            HPolygon poly;
            
            int nLoops = hatch->countLoops();
            int iLoop = 0;

            // nLoops is the number of loops (closed contours):
            RS_DEBUG->print("RS_Modification::triangulate: "
                "poly.mtabSize.resize(%d)", nLoops);
            poly.mtabSize.resize(nLoops);

            // iterate through loops:
            for (RS_Entity* l=hatch->firstEntity(); l!=NULL; l=hatch->nextEntity()) {
                if (l->isContainer()) {
                    RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);
                        
                    RS_DEBUG->print("RS_Modification::triangulate: "
                        "poly.mtabSize[%d] = %d", 
                        iLoop+1, loop->count()+1);
                    // set number of entities in this loop:
                    poly.mtabSize[iLoop++] = loop->count()+1;
                    if (iLoop>nLoops) {
                        break;
                    }
                        
                    // iterate through entities of current loop:
                    bool first = true;
                    for (RS_Entity* d=loop->firstEntity(); d!=NULL; d=loop->nextEntity()) {
                        if (!d->isAtomic()) {
                            continue;
                        }

                        RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(d);

                        // for the first entity of the loop, insert start coordinates
                        if (first) {
                            RS_DEBUG->print("RS_Modification::triangulate: "
                                "poly.mtabPnt.insert(%f,%f)", 
                                ae->getStartpoint().x, ae->getStartpoint().y);
                            poly.mtabPnt.insert(poly.mtabPnt.end(), 
                                Vect2D(ae->getStartpoint().x, ae->getStartpoint().y));
                            first = false;
                        }

                        // for all entities in the loop, insert end coordinates
                        RS_DEBUG->print("RS_Modification::triangulate: "
                            "poly.mtabPnt.insert(%f,%f)", 
                            ae->getEndpoint().x, ae->getEndpoint().y);
                        poly.mtabPnt.insert(poly.mtabPnt.end(), 
                            Vect2D(ae->getEndpoint().x, ae->getEndpoint().y) );
                    }
                }
            }

            RS_DEBUG->print("RS_Modification::triangulate: triangulation");
            // run triangulation algorithm:
            poly.Triangulate();
            RS_DEBUG->print("RS_Modification::triangulate: triangulation: OK");

            // create triangles as new hatches
            for (uint i=0; i<poly.mtabCell.size(); ++i) {
                RS_DEBUG->print("RS_Modification::triangulate: "
                    "%f/%f   %f/%f   %f/%f", 
                    poly.mtabPnt[poly.mtabCell[i].Index(0)].X(),
                    poly.mtabPnt[poly.mtabCell[i].Index(0)].Y(),
                    poly.mtabPnt[poly.mtabCell[i].Index(1)].X(),
                    poly.mtabPnt[poly.mtabCell[i].Index(1)].Y(),
                    poly.mtabPnt[poly.mtabCell[i].Index(2)].X(),
                    poly.mtabPnt[poly.mtabCell[i].Index(2)].Y()
                    );

                RS_Vector p[3];
                for (int k=0; k<3; k++) {
                    p[k] = RS_Vector(
                        poly.mtabPnt[poly.mtabCell[i].Index(k)].X(), 
                        poly.mtabPnt[poly.mtabCell[i].Index(k)].Y());
                }

                RS_Hatch* triangle = new RS_Hatch(container, hatch->getData());
                triangle->setLayer(hatch->getLayer());
                triangle->setPen(hatch->getPen());
                RS_EntityContainer* loop = new RS_EntityContainer(triangle);
                for (int k=0; k<3; k++) {
                    loop->addEntity(new RS_Line(loop, RS_LineData(p[k], p[(k+1)%3])));
                }
                triangle->addEntity(loop);
                triangle->update();
                addList.append(triangle);
            }
        }
    }

    RS_DEBUG->print("RS_Modification::triangulate: "
        "%d entities in addList", addList.count());

    deselectOriginals(true);
    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }

    return true;
}
#endif



/**
 * Combines all hatch entities in the container with the given 
 * boolean operation.
 *
 * Clockwise polygons are contours, ccw polygons are holes. 
 *
 * Limitations: each hatch has only one loop.
 * For subtractions: all ccw hatches are subtracted from the 
 * cw hatches in the container.
 *
 * @param op: operation to run.
 */
#ifdef RS_ALGO
bool RS_Modification::doBooleanOperation(BooleanOperation op/*, bool orientationEntryMode*/) {
    RS_DEBUG->print("RS_Modification::doBooleanOperation");
    
    if (container==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::doBooleanOperation: no valid container");
        return false;
    }

    /*
    if (container->count()<2) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "RS_Modification::doBooleanOperation: not enough hatches");
        return false;
    }
    */

    RS_PtrList<RS_Entity> addList;

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
    }
    
    Bool_Engine* booleng = new Bool_Engine();
    booleng->SetLog(false);
    booleng->SetOrientationEntryMode(true);
    booleng->SetLinkHoles(true);
    booleng->SetMarge(0.00001);
    booleng->SetGrid(10000);
    booleng->SetDGrid(1000);
    booleng->SetCorrectionFactor(500.0);
    booleng->SetCorrectionAber(1.0);
    booleng->SetSmoothAber(10.0);
    booleng->SetMaxlinemerge(1000.0);
    booleng->SetRoundfactor(1.5);

    GroupType gt = GROUP_A;
            
    RS_Hatch* hatch = NULL;
    RS_Solid* solid = NULL;
    int count = 0;
    
    // iterate though hatches and add them to the kbool engine:
    for (RS_Entity* e=container->firstEntity();
            e!=NULL;
            e=container->nextEntity()) {
        if (!e->isSelected()) {
            continue;
        }

        if (e->isOfType(RS2::EntityHatch)) {
            hatch = dynamic_cast<RS_Hatch*>(e);
            count++;
            
            // iterate through loops:
            for (RS_Entity* l=hatch->firstEntity(); l!=NULL; l=hatch->nextEntity()) {
                if (!l->isContainer()) {
                    continue;
                }

                RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);

                // ignore hatch pattern itself:
                if (loop->getFlag(RS2::FlagTemp)) {
                    continue;
                }

                if (loop->getContourOrientation()==RS2::ClockWise) {
                    gt = GROUP_A;
                }
                else {
                    gt = GROUP_B;
                }

                if (!booleng->StartPolygonAdd(gt)) {
                    continue;
                }

                // iterate through entities of current loop:
                bool first = true;
                for (RS_Entity* d=loop->firstEntity(); d!=NULL; d=loop->nextEntity()) {
                    if (!d->isAtomic()) {
                        continue;
                    }

                    RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(d);

                    // for all entities in the loop, insert end coordinates
                    booleng->AddPoint(ae->getEndpoint().x, ae->getEndpoint().y);
                }

                booleng->EndPolygonAdd(); 
            }
        }
        else if (e->isOfType(RS2::EntitySolid)) {
            solid = dynamic_cast<RS_Solid*>(e);
            count++;
            
            if (solid->getOrientation()==RS2::ClockWise) {
                gt = GROUP_A;
            }
            else {
                gt = GROUP_B;
            }

            if (!booleng->StartPolygonAdd(gt)) {
                continue;
            }
            for (int i=0; i<3; i++) {
                booleng->AddPoint(solid->getCorner(i).x, solid->getCorner(i).y);
            }
            booleng->EndPolygonAdd(); 
        }
    }
    
    if (count>1) {
        
        RS_DEBUG->print("RS_Modification::doBooleanOperation: "
            "running boolean engine");

        // run kbool algorithm:
        switch (op) {
        case BoolAnd:
            booleng->Do_Operation(BOOL_AND);
            break;

        case BoolOr:
            booleng->Do_Operation(BOOL_OR);
            break;

        case BoolXor:
            booleng->Do_Operation(BOOL_EXOR);
            break;

        case BoolSub:
            booleng->Do_Operation(BOOL_A_SUB_B);
            break;

        default:
            break;
        }
        
        RS_DEBUG->print("RS_Modification::doBooleanOperation: "
            "running boolean engine: OK");
    }
    
    // create result as new hatches:
	if (hatch!=NULL || solid!=NULL) {
        RS_HatchData data;
        RS_Layer* layer = NULL;
        RS_Pen pen;

        if (hatch!=NULL) {
            data = hatch->getData();
            layer = hatch->getLayer();
            pen = hatch->getPen();
        }
        else if (solid!=NULL) {
            data = RS_HatchData(true, 1.0, 0.0, "SOLID");
            layer = solid->getLayer();
            pen = solid->getPen();
        }

        /*
        RS_DEBUG->print("RS_Modification::doBooleanOperation: "
            "booleng->StartPolygonGet()");
        */
        while (booleng->StartPolygonGet()) {
            RS_Hatch* poly = new RS_Hatch(container, data);
            poly->setLayer(layer);
            poly->setPen(pen);
            RS_EntityContainer* loop = new RS_EntityContainer(poly);
            //RS_DEBUG->printEntity(RS_Debug::D_ERROR, loop);

            // for each point in the polygon
            bool first = true;
            RS_Vector firstPoint;
            RS_Vector prevPoint;
            RS_Vector thisPoint;
        
            /*
            RS_DEBUG->print("RS_Modification::doBooleanOperation: "
                "booleng->PolygonHasMorePoints()");
            */
            while (booleng->PolygonHasMorePoints()) {
                thisPoint.x = booleng->GetPolygonXPoint();
                thisPoint.y = booleng->GetPolygonYPoint();

                if (!first) {
                    loop->addEntity(new RS_Line(loop, RS_LineData(prevPoint, thisPoint)));
                    /*
                    RS_DEBUG->print("RS_Modification::doBooleanOperation: "
                        "result: %f/%f - %f/%f", 
                        prevPoint.x, prevPoint.y, 
                        thisPoint.x, thisPoint.y);
                    */
                }
                else {
                    firstPoint = thisPoint;
                }

                prevPoint = thisPoint;
                first = false;
            }
            
            loop->addEntity(new RS_Line(loop, RS_LineData(prevPoint, firstPoint)));
            /*
            RS_DEBUG->print("RS_Modification::doBooleanOperation: "
                "result: %f/%f - %f/%f", 
                prevPoint.x, prevPoint.y, 
                firstPoint.x, firstPoint.y);
            */

            booleng->EndPolygonGet();
            
            poly->addEntity(loop);
            poly->update();
            addList.append(poly);
        }
    }
    
    delete booleng;
    
    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, "");
    RS_DEBUG->print(RS_Debug::D_ERROR, "RS_Modification::doBooleanOperation 001:");
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, addList.at(0));
    */

    RS_DEBUG->print("RS_Modification::doBooleanOperation: "
        "%d entities in addList", addList.count());

    deselectOriginals(true);
    
    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, "");
    RS_DEBUG->print(RS_Debug::D_ERROR, "RS_Modification::doBooleanOperation 002:");
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, addList.at(0));
    */

    addNewEntities(addList);

    if (document!=NULL && handleUndo) {
        document->endUndoCycle();
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
    
    RS_DEBUG->print("RS_Modification::doBooleanOperation: OK");

    return true;
}
#endif



/**
 * Adds a node to the given polyline. The new node is placed between
 * the start and end point of the given segment. 
 *
 * @param node The position of the new node.
 *
 * @return Pointer to the new polyline or NULL.
 */
RS_Polyline* RS_Modification::addPolylineNode(RS_Polyline& polyline,
        const RS_AtomicEntity& segment,
        const RS_Vector& node) {
    RS_DEBUG->print("RS_Modification::addPolylineNode");

    if (container==NULL) {
        RS_DEBUG->print("RS_Modification::addPolylineNode: no valid container",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (segment.getParentContainer()!=&polyline) {
        RS_DEBUG->print("RS_Modification::addPolylineNode: "
                        "segment not part of the polyline",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    RS_Polyline* newPolyline = new RS_Polyline(container);
    newPolyline->setClosed(polyline.isClosed());
    newPolyline->setSelected(polyline.isSelected());
    newPolyline->setLayer(polyline.getLayer());
    newPolyline->setPen(polyline.getPen(false));

    // copy polyline and add new node:
    bool first = true;
    RS_Entity* lastEntity = polyline.lastEntity();
    for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
            e=polyline.nextEntity()) {

        if (e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            double bulge = 0.0;
            if (ae->rtti()==RS2::EntityArc) {
                RS_DEBUG->print("RS_Modification::addPolylineNode: arc segment");
                bulge = dynamic_cast<RS_Arc*>(ae)->getBulge();
            } else {
                RS_DEBUG->print("RS_Modification::addPolylineNode: line segment");
                bulge = 0.0;
            }

            if (first) {
                RS_DEBUG->print("RS_Modification::addPolylineNode: first segment: %f/%f",
                                ae->getStartpoint().x, ae->getStartpoint().y);

                newPolyline->setNextBulge(bulge);
                newPolyline->addVertex(ae->getStartpoint());
                first = false;
            }

            // segment to split:
            if (ae==&segment) {
                RS_DEBUG->print("RS_Modification::addPolylineNode: split segment found");

                RS_DEBUG->print("RS_Modification::addPolylineNode: node: %f/%f",
                                node.x, node.y);

                newPolyline->setNextBulge(0.0);
                newPolyline->addVertex(node);

                RS_DEBUG->print("RS_Modification::addPolylineNode: after node: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);

                if (ae!=lastEntity || polyline.isClosed()==false) {
                    newPolyline->setNextBulge(0.0);
                    newPolyline->addVertex(ae->getEndpoint());
                }
            } else {
                RS_DEBUG->print("RS_Modification::addPolylineNode: normal vertex found: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);

                if (ae!=lastEntity || polyline.isClosed()==false) {
                    newPolyline->setNextBulge(bulge);
                    newPolyline->addVertex(ae->getEndpoint());
                }
            }
        } else {
            RS_DEBUG->print("RS_Modification::addPolylineNode: "
                            "Polyline contains non-atomic entities",
                            RS_Debug::D_WARNING);
        }
    }

    newPolyline->setNextBulge(polyline.getClosingBulge());
    newPolyline->endPolyline();

    // add new polyline:
    container->addEntity(newPolyline);

    if (document!=NULL && handleUndo) {
        document->startUndoCycle();

        polyline.setUndoState(true);
        document->addUndoable(&polyline);
        document->addUndoable(newPolyline);

        document->endUndoCycle();
    }
    
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(&polyline);
        //graphicView->drawEntity(newPolyline);
        graphicView->redraw();
    }

    return newPolyline;
}



/**
 * Deletes a node from a polyline. 
 *
 * @param node The node to delete.
 *
 * @return Pointer to the new polyline or NULL.
 */
RS_Polyline* RS_Modification::deletePolylineNode(RS_Polyline& polyline,
        const RS_Vector& node) {

    RS_DEBUG->print("RS_Modification::deletePolylineNode");

    if (container==NULL) {
        RS_DEBUG->print("RS_Modification::deletePolylineNode: no valid container",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (node.valid==false) {
        RS_DEBUG->print("RS_Modification::deletePolylineNode: "
                        "node not valid",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    // check if the polyline is no longer there after deleting the node:
    if (polyline.count()==1) {
        RS_Entity* e = polyline.firstEntity();
        if (e!=NULL && e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            if (node.distanceTo(ae->getStartpoint())<1.0e-6 ||
                    node.distanceTo(ae->getEndpoint())<1.0e-6) {

                /*
                if (graphicView!=NULL) {
                    graphicView->deleteEntity(&polyline);
                }
                */

                if (document!=NULL && handleUndo) {
                    document->startUndoCycle();
                    polyline.setUndoState(true);
                    document->addUndoable(&polyline);
                    document->endUndoCycle();
                }
            }
        }
        if (graphicView!=NULL) {
            graphicView->redraw();
        }
        return NULL;
    }

    RS_Polyline* newPolyline = new RS_Polyline(container);
    newPolyline->setClosed(polyline.isClosed());
    newPolyline->setSelected(polyline.isSelected());
    newPolyline->setLayer(polyline.getLayer());
    newPolyline->setPen(polyline.getPen(false));

    // copy polyline and drop deleted node:
    bool first = true;
    bool lastDropped = false;
    RS_Entity* lastEntity = polyline.lastEntity();
    for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
            e=polyline.nextEntity()) {

        if (e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            double bulge = 0.0;
            if (ae->rtti()==RS2::EntityArc) {
                RS_DEBUG->print("RS_Modification::deletePolylineNode: arc segment");
                bulge = dynamic_cast<RS_Arc*>(ae)->getBulge();
            } else {
                RS_DEBUG->print("RS_Modification::deletePolylineNode: line segment");
                bulge = 0.0;
            }

            // last entity is closing entity and will be added below with endPolyline()
            if (e==lastEntity && polyline.isClosed()) {
                continue;
            }

            // first vertex (startpoint)
            if (first && node.distanceTo(ae->getStartpoint())>1.0e-6) {
                RS_DEBUG->print("RS_Modification::deletePolylineNode: first node: %f/%f",
                                ae->getStartpoint().x, ae->getStartpoint().y);

                newPolyline->setNextBulge(bulge);
                newPolyline->addVertex(ae->getStartpoint());
                first = false;
            }

            // normal node (not deleted):
            if (first==false && node.distanceTo(ae->getEndpoint())>1.0e-6) {
                RS_DEBUG->print("RS_Modification::deletePolylineNode: normal vertex found: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
                if (lastDropped) {
                    //bulge = 0.0;
                }
                newPolyline->setNextBulge(bulge);
                newPolyline->addVertex(ae->getEndpoint());
                lastDropped = false;
            }

            // drop deleted node:
            else {
                RS_DEBUG->print("RS_Modification::deletePolylineNode: deleting vertex: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
                lastDropped = true;
            }
        } else {
            RS_DEBUG->print("RS_Modification::deletePolylineNode: "
                            "Polyline contains non-atomic entities",
                            RS_Debug::D_WARNING);
        }
    }

    RS_DEBUG->print("RS_Modification::deletePolylineNode: ending polyline");
    newPolyline->setNextBulge(polyline.getClosingBulge());
    newPolyline->endPolyline();

    //if (newPolyline->count()==1) {
    //}

    // add new polyline:
    RS_DEBUG->print("RS_Modification::deletePolylineNode: adding new polyline");
    container->addEntity(newPolyline);

    RS_DEBUG->print("RS_Modification::deletePolylineNode: handling undo");
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();

        polyline.setUndoState(true);
        document->addUndoable(&polyline);
        document->addUndoable(newPolyline);

        document->endUndoCycle();
    }
    
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(&polyline);
        //graphicView->drawEntity(newPolyline);
        graphicView->redraw();
    }

    return newPolyline;
}



/**
 * Deletes all nodes between the two given nodes (exclusive).
 *
 * @param node1 First limiting node. 
 * @param node2 Second limiting node. 
 *
 * @return Pointer to the new polyline or NULL.
 */
RS_Polyline* RS_Modification::deletePolylineNodesBetween(RS_Polyline& polyline,
        RS_AtomicEntity& /*segment*/, const RS_Vector& node1, const RS_Vector& node2) {

    RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween");

    if (container==NULL) {
        RS_DEBUG->print("RS_Modification::addPolylineNodesBetween: no valid container",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (node1.valid==false || node2.valid==false) {
        RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                        "node not valid",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (node1.distanceTo(node2)<1.0e-6) {
        RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                        "nodes are identical",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    // check if there's nothing to delete:
    for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
            e=polyline.nextEntity()) {

        if (e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);

            if ((node1.distanceTo(ae->getStartpoint())<1.0e-6 &&
                    node2.distanceTo(ae->getEndpoint())<1.0e-6) ||
                    (node2.distanceTo(ae->getStartpoint())<1.0e-6 &&
                     node1.distanceTo(ae->getEndpoint())<1.0e-6)) {

                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "nothing to delete",
                                RS_Debug::D_WARNING);
                return NULL;
            }
        }
    }


    // check if the start point is involved:
    bool startpointInvolved = false;
    if (node1.distanceTo(polyline.getStartpoint())<1.0e-6 ||
            node2.distanceTo(polyline.getStartpoint())<1.0e-6) {
        startpointInvolved = true;
    }


    // check which part of the polyline has to be deleted:
    bool deleteStart = false;
    if (polyline.isClosed()) {
        bool found = false;
        double length1 = 0.0;
        double length2 = 0.0;
        RS_Entity* e=polyline.firstEntity();

        if (startpointInvolved) {
            if (e->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
                length1+=ae->getLength();
            }
            e = polyline.nextEntity();
        }
        for (; e!=NULL; e=polyline.nextEntity()) {

            if (e->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);

                if (node1.distanceTo(ae->getStartpoint())<1.0e-6 ||
                        node2.distanceTo(ae->getStartpoint())<1.0e-6) {

                    found = !found;
                }

                if (found) {
                    length2+=ae->getLength();
                } else {
                    length1+=ae->getLength();
                }
            }
        }
        if (length1<length2) {
            deleteStart = true;
        } else {
            deleteStart = false;
        }
    }

    RS_Polyline* newPolyline = new RS_Polyline(container);
    newPolyline->setClosed(polyline.isClosed());
    newPolyline->setSelected(polyline.isSelected());
    newPolyline->setLayer(polyline.getLayer());
    newPolyline->setPen(polyline.getPen(false));

    if (startpointInvolved && deleteStart && polyline.isClosed()) {
        newPolyline->setNextBulge(0.0);
        newPolyline->addVertex(polyline.getStartpoint());
    }

    // copy polyline and drop deleted nodes:
    bool first = true;
    bool removing = deleteStart;
    bool done = false;
    bool nextIsStraight = false;
    RS_Entity* lastEntity = polyline.lastEntity();
    int i=0;
    double bulge = 0.0;
    for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
            e=polyline.nextEntity()) {

        RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: entity: %d", i++);
        RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: removing: %d", (int)removing);

        if (e->isAtomic()) {
            RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
            if (ae->rtti()==RS2::EntityArc) {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: arc segment");
                bulge = dynamic_cast<RS_Arc*>(ae)->getBulge();
            } else {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: line segment");
                bulge = 0.0;
            }

            // last entity is closing entity and will be added below with endPolyline()
            if (e==lastEntity && polyline.isClosed()) {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "dropping last vertex of closed polyline");
                continue;
            }

            // first vertex (startpoint)
            if (first) {
                if (!removing) {
                    RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: first node: %f/%f",
                                    ae->getStartpoint().x, ae->getStartpoint().y);
                    newPolyline->setNextBulge(bulge);
                    newPolyline->addVertex(ae->getStartpoint());
                    first = false;
                }
            }

            // stop removing nodes:
            if (removing==true &&
                    (node1.distanceTo(ae->getEndpoint())<1.0e-6 ||
                     node2.distanceTo(ae->getEndpoint())<1.0e-6)) {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "stop removing at: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
                removing = false;
                done = true;
                if (first==false) {
                    nextIsStraight = true;
                }
            }

            // normal node (not deleted):
            if (removing==false && (done==false || deleteStart==false)) {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "normal vertex found: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
                if (nextIsStraight) {
                    bulge = 0.0;
                    nextIsStraight = false;
                }
                newPolyline->setNextBulge(bulge);
                newPolyline->addVertex(ae->getEndpoint());
            }

            // drop deleted node:
            else {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "deleting vertex: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
            }

            // start to remove nodes from now on:
            if (done==false && removing==false &&
                    (node1.distanceTo(ae->getEndpoint())<1.0e-6 ||
                     node2.distanceTo(ae->getEndpoint())<1.0e-6)) {
                RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                                "start removing at: %f/%f",
                                ae->getEndpoint().x, ae->getEndpoint().y);
                removing = true;
            }

            if (done) {
                done=false;
            }
        } else {
            RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: "
                            "Polyline contains non-atomic entities",
                            RS_Debug::D_WARNING);
        }
    }

    /*if (polyline.isClosed() && deleteStart && startpointInvolved) {
        newPolyline->setNextBulge(0.0);
           newPolyline->addVertex(polyline.getStartpoint());
}*/

    RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: ending polyline");
    newPolyline->setNextBulge(polyline.getClosingBulge());
    newPolyline->endPolyline();

    // add new polyline:
    RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: adding new polyline");
    container->addEntity(newPolyline);

    RS_DEBUG->print("RS_Modification::deletePolylineNodesBetween: handling undo");
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();

        polyline.setUndoState(true);
        document->addUndoable(&polyline);
        document->addUndoable(newPolyline);

        document->endUndoCycle();
    }
    
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(&polyline);
        //graphicView->drawEntity(newPolyline);
        graphicView->redraw();
    }

    return newPolyline;
}



/**
 * Trims two segments of a polyline all nodes between the two trim segments
 * are removed.
 *
 * @param polyline The polyline entity.
 * @param segment1 First segment to trim.
 * @param segment2 Second segment to trim.
 *
 * @return Pointer to the new polyline or NULL.
 */
RS_Polyline* RS_Modification::polylineTrim(RS_Polyline& polyline,
        RS_AtomicEntity& segment1,
        RS_AtomicEntity& segment2) {

    RS_DEBUG->print("RS_Modification::polylineTrim");

    if (container==NULL) {
        RS_DEBUG->print("RS_Modification::polylineTrim: no valid container",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (segment1.getParentContainer()!=&polyline || segment2.getParentContainer()!=&polyline) {
        RS_DEBUG->print("RS_Modification::polylineTrim: "
                        "segments not in polyline",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    if (&segment1==&segment2) {
        RS_DEBUG->print("RS_Modification::polylineTrim: "
                        "segments are identical",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    RS_VectorList sol;
    sol = RS_Information::getIntersection(&segment1, &segment2, false);

    if (sol.count()==0) {
        RS_DEBUG->print("RS_Modification::polylineTrim: "
                        "segments cannot be trimmed",
                        RS_Debug::D_WARNING);
        return NULL;
    }

    // check which segment comes first in the polyline:
    RS_AtomicEntity* firstSegment;
    if (polyline.findEntity(&segment1) > polyline.findEntity(&segment2)) {
        firstSegment = &segment2;
    } else {
        firstSegment = &segment1;
    }

    // find out if we need to trim towards the open part of the polyline
    bool reverseTrim;
    reverseTrim = !RS_Math::isSameDirection(firstSegment->getDirection1(),
                                            firstSegment->getStartpoint().angleTo(sol.get(0)), M_PI/2.0);

    RS_Polyline* newPolyline = new RS_Polyline(container);
    newPolyline->setClosed(polyline.isClosed());
    newPolyline->setSelected(polyline.isSelected());
    newPolyline->setLayer(polyline.getLayer());
    newPolyline->setPen(polyline.getPen(false));

    // normal trimming: start removing nodes at trim segment. ends stay the same
    if (reverseTrim==false) {
        // copy polyline, trim segments and drop between nodes:
        bool first = true;
        bool removing = false;
        bool nextIsStraight = false;
        RS_Entity* lastEntity = polyline.lastEntity();
        for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
                e=polyline.nextEntity()) {

            if (e->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
                double bulge = 0.0;
                if (ae->rtti()==RS2::EntityArc) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: arc segment");
                    bulge = dynamic_cast<RS_Arc*>(ae)->getBulge();
                } else {
                    RS_DEBUG->print("RS_Modification::polylineTrim: line segment");
                    bulge = 0.0;
                }

                // last entity is closing entity and will be added below with endPolyline()
                if (e==lastEntity && polyline.isClosed()) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: "
                                    "dropping last vertex of closed polyline");
                    continue;
                }

                // first vertex (startpoint)
                if (first) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: first node: %f/%f",
                                    ae->getStartpoint().x, ae->getStartpoint().y);

                    newPolyline->setNextBulge(bulge);
                    newPolyline->addVertex(ae->getStartpoint());
                    first = false;
                }

                // trim and start removing nodes:
                if (removing==false && (ae==&segment1 || ae==&segment2)) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: "
                                    "start removing at trim point %f/%f",
                                    sol.get(0).x, sol.get(0).y);
                    newPolyline->setNextBulge(0.0);
                    newPolyline->addVertex(sol.get(0));
                    removing = true;
                    nextIsStraight = true;
                }

                // stop removing nodes:
                else if (removing==true && (ae==&segment1 || ae==&segment2)) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: stop removing at: %f/%f",
                                    ae->getEndpoint().x, ae->getEndpoint().y);
                    removing = false;
                }

                // normal node (not deleted):
                if (removing==false) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: normal vertex found: %f/%f",
                                    ae->getEndpoint().x, ae->getEndpoint().y);
                    if (nextIsStraight) {
                        newPolyline->setNextBulge(0.0);
                        nextIsStraight = false;
                    } else {
                        newPolyline->setNextBulge(bulge);
                    }
                    newPolyline->addVertex(ae->getEndpoint());
                }
            } else {
                RS_DEBUG->print("RS_Modification::polylineTrim: "
                                "Polyline contains non-atomic entities",
                                RS_Debug::D_WARNING);
            }
        }
    }

    // reverse trimming: remove nodes at the ends and keep those in between
    else {
        // copy polyline, trim segments and drop between nodes:
        //bool first = true;
        bool removing = true;
        bool nextIsStraight = false;
        RS_Entity* lastEntity = polyline.lastEntity();
        for (RS_Entity* e=polyline.firstEntity(); e!=NULL;
                e=polyline.nextEntity()) {

            if (e->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
                double bulge = 0.0;
                if (ae->rtti()==RS2::EntityArc) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: arc segment");
                    bulge = (dynamic_cast<RS_Arc*>(ae))->getBulge();
                } else {
                    RS_DEBUG->print("RS_Modification::polylineTrim: line segment");
                    bulge = 0.0;
                }

                // last entity is closing entity and will be added below with endPolyline()
                if (e==lastEntity && polyline.isClosed()) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: "
                                    "dropping last vertex of closed polyline");
                    continue;
                }

                // first vertex (startpoint)
                /*if (first && !removing) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: first node: %f/%f",
                                    ae->getStartpoint().x, ae->getStartpoint().y);

                    newPolyline->setNextBulge(bulge);
                    newPolyline->addVertex(sol.get(0));
                    first = false;
            }*/

                // trim and stop removing nodes:
                if (removing==true && (ae==&segment1 || ae==&segment2)) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: "
                                    "stop removing at trim point %f/%f",
                                    sol.get(0).x, sol.get(0).y);
                    newPolyline->setNextBulge(0.0);
                    // start of new polyline:
                    newPolyline->addVertex(sol.get(0));
                    removing = false;
                    nextIsStraight = true;
                }

                // start removing nodes again:
                else if (removing==false && (ae==&segment1 || ae==&segment2)) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: start removing at: %f/%f",
                                    ae->getEndpoint().x, ae->getEndpoint().y);
                    newPolyline->setNextBulge(0.0);
                    // start of new polyline:
                    newPolyline->addVertex(sol.get(0));
                    removing = true;
                }

                // normal node (not deleted):
                if (removing==false) {
                    RS_DEBUG->print("RS_Modification::polylineTrim: normal vertex found: %f/%f",
                                    ae->getEndpoint().x, ae->getEndpoint().y);
                    if (nextIsStraight) {
                        newPolyline->setNextBulge(0.0);
                        nextIsStraight = false;
                    } else {
                        newPolyline->setNextBulge(bulge);
                    }
                    newPolyline->addVertex(ae->getEndpoint());
                }
            } else {
                RS_DEBUG->print("RS_Modification::polylineTrim: "
                                "Polyline contains non-atomic entities",
                                RS_Debug::D_WARNING);
            }
        }
    }

    RS_DEBUG->print("RS_Modification::polylineTrim: ending polyline");
    newPolyline->setNextBulge(polyline.getClosingBulge());
    newPolyline->endPolyline();

    // add new polyline:
    RS_DEBUG->print("RS_Modification::polylineTrim: adding new polyline");
    container->addEntity(newPolyline);

    RS_DEBUG->print("RS_Modification::polylineTrim: handling undo");
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();

        polyline.setUndoState(true);
        document->addUndoable(&polyline);
        document->addUndoable(newPolyline);

        document->endUndoCycle();
    }
    
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(&polyline);
        //graphicView->drawEntity(newPolyline);
        graphicView->redraw();
    }

    return newPolyline;
}

    

/**
 * Creates a polyline from existing segments.
 *
 * @param segment Segment that will be part of the new polyline.
 *
 * @return Pointer to the new polyline or NULL.
 */
RS_Polyline* RS_Modification::segmentsToPolyline(RS_AtomicEntity& segment) {
    RS_DEBUG->print("RS_Modification::segmentsToPolyline");

    if (container==NULL) {
        RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
            "no valid container", RS_Debug::D_WARNING);
        return NULL;
    }
    
    container->setProcessed(false);

    // create new polyline with the given segment
    RS_Polyline* newPolyline = new RS_Polyline(container);
    //newPolyline->setClosed(polyline.isClosed());
    newPolyline->setSelected(segment.isSelected());
    newPolyline->setLayer(segment.getLayer());
    newPolyline->setPen(segment.getPen(false));

    RS_Entity* cl1 = segment.clone();
    cl1->reparent(newPolyline);
    newPolyline->insertEntity(0, cl1);
    
    if (document!=NULL && handleUndo) {
        document->startUndoCycle();
                    
        segment.setUndoState(true);
        document->addUndoable(&segment);
    }

    // find segments to append / prepend
    enum Mode {append=0, prepend=1};
    for (int m=(int)append; m<=(int)prepend; m++) {
        if ((Mode)m==append) {
            RS_DEBUG->print("RS_Modification::segmentsToPolyline: appending");
        }
        else {
            RS_DEBUG->print("RS_Modification::segmentsToPolyline: prepending");
        }
    
        RS_Vector currentVertex;
        if ((Mode)m==append) {
            currentVertex = segment.getEndpoint();
        }
        else {
            currentVertex = segment.getStartpoint();
        }
        
        bool done = true;
        do {
            RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                "currentVertex: %f/%f", currentVertex.x, currentVertex.y);

            done = true;
            for (RS_Entity* e=container->firstEntity(); e!=NULL; 
                e=container->nextEntity()) {
                RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                    "checking entity...");
                
                if (!e->isUndone() && e->isVisible() && !e->isProcessed() && 
                    e!=&segment && 
                    (e->rtti()==RS2::EntityLine || e->rtti()==RS2::EntityArc)) {
                
                    RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                        "line or arc found");

                    RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
                    bool addNormal;
                    bool addReversed;
                    addNormal = (currentVertex.distanceTo(ae->getStartpoint())<1.0e-6);
                    addReversed = (currentVertex.distanceTo(ae->getEndpoint())<1.0e-6);

                    if (addNormal || addReversed) {
                        RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                            "connected line or arc found");
                            
                        done = false;
                        ae->setProcessed(true);
    
                        RS_AtomicEntity* cl = dynamic_cast<RS_AtomicEntity*>(ae->clone());
                        if (((Mode)m==append && addReversed) || ((Mode)m==prepend && addNormal)) {
                            RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                                "reversing entity");
                            cl->reverse();
                        }
                        RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                                "reparent");
                        cl->reparent(newPolyline);
                        cl->setLayer(segment.getLayer());
                        cl->setPen(segment.getPen(false));

                        RS_DEBUG->print("RS_Modification::segmentsToPolyline: "
                            "insertEntity to newPolyline");
                            
                        if ((Mode)m==append) {
                            newPolyline->insertEntity(newPolyline->count(), cl);
                            RS_Vector v = cl->getEndpoint();
                        }
                        else {
                            newPolyline->insertEntity(0, cl);
                            RS_Vector v = cl->getStartpoint();
                        }
                        if (addNormal) {
                            currentVertex = ae->getEndpoint();
                        }
                        else {
                            currentVertex = ae->getStartpoint();
                        }
                        
                        if (document!=NULL && handleUndo) {
                            ae->setUndoState(true);
                            document->addUndoable(ae);
                        }
                    }
                }
            }
        } while(!done);
    }

    newPolyline->updateEndpoints();
    
    container->addEntity(newPolyline);
    
    container->setProcessed(false);
    
    RS_DEBUG->print("RS_Modification::segmentsToPolyline: handling undo");
    if (document!=NULL && handleUndo) {
        document->addUndoable(newPolyline);
        document->endUndoCycle();
    }
    
    if (graphicView!=NULL) {
        //graphicView->deleteEntity(&polyline);
        //graphicView->drawEntity(newPolyline);
        graphicView->redraw();
    }

    return newPolyline;
}

