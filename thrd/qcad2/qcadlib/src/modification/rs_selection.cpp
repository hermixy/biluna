/****************************************************************************
** $Id: rs_selection.cpp 9397 2008-03-12 19:50:35Z andrew $
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

#include "rs_selection.h"

#include "rs_information.h"
#include "rs_polyline.h"
#include "rs_entity.h"
#include "rs_graphic.h"



/**
 * Default constructor.
 *
 * @param container The container to which we will add
 *        entities. Usually that's an RS_Graphic entity but
 *        it can also be a polyline, text, ...
 */
RS_Selection::RS_Selection(RS_EntityContainer& container,
                           RS_GraphicView* graphicView) {
    this->container = &container;
    this->graphicView = graphicView;
    graphic = container.getGraphic();
}



/**
 * Selects or deselects the given entitiy.
 */
void RS_Selection::selectSingle(RS_Entity* e, bool updateView) {
    if (e!=NULL && (e->getLayer()==NULL || e->getLayer()->isLocked()==false)) {

        e->toggleSelected();

        if (updateView && graphicView!=NULL) {
            graphicView->redraw();
        }
    }
}



/**
 * Selects all entities on visible layers.
 */
void RS_Selection::selectAll(bool select, bool updateView) {
    for (RS_Entity* e=container->firstEntity();
             e!=NULL;
             e=container->nextEntity()) {

        // select only visible, deselect all:
        if (e!=NULL && !e->isUndone() && 
            (e->isVisible() || select==false)) {
            e->setSelected(select);
        }
    }

    if (updateView && graphicView!=NULL) {
        graphicView->redraw();
    }
}



/**
 * Selects all entities on visible layers.
 */
void RS_Selection::invertSelection() {
    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {
        if (e!=NULL && e->isVisible()) {
            e->toggleSelected();
        }
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
}



/**
 * Selects all entities that are completely in the given window.
 *
 * @param v1 First corner of the window to select.
 * @param v2 Second corner of the window to select.
 * @param select true: select, false: deselect
 */
void RS_Selection::selectWindow(const RS_Vector& v1, const RS_Vector& v2,
                                bool select) {

    container->selectWindow(v1, v2, select);

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
}



/**
 * Selects all entities that are intersected by the given line.
 *
 * @param v1 Startpoint of line.
 * @param v2 Endpoint of line.
 * @param select true: select, false: deselect
 */
void RS_Selection::selectIntersected(const RS_Vector& v1, const RS_Vector& v2,
                                     bool select) {

    RS_Line line(NULL, RS_LineData(v1, v2));
    bool inters;

    for (RS_Entity* e=container->firstEntity(); e!=NULL;
            e=container->nextEntity()) {

        if (e!=NULL && e->isVisible()) {

            inters = false;

            // select containers / groups:
            if (e->isContainer()) {
                RS_EntityContainer* ec = dynamic_cast<RS_EntityContainer*>(e);

                for (RS_Entity* e2=ec->firstEntity(RS2::ResolveAll); e2!=NULL;
                        e2=ec->nextEntity(RS2::ResolveAll)) {

                    RS_VectorList sol =
                        RS_Information::getIntersection(&line, e2, true);

                    if (sol.hasValid()) {
                        inters = true;
                    }
                }
            } else {

                RS_VectorList sol =
                    RS_Information::getIntersection(&line, e, true);

                if (sol.hasValid()) {
                    inters = true;
                }
            }

            if (inters) {
                /*if (graphicView!=NULL) {
                    graphicView->deleteEntity(e);
                }*/

                e->setSelected(select);

                /*if (graphicView!=NULL) {
                    graphicView->drawEntity(e);
                }*/
            }
        }
    }
    if (graphicView!=NULL) {
        graphicView->redraw();
    }

}



/**
 * Selects all entities that are connected to the given entity.
 *
 * @param e The entity where the algorithm starts. Must be an atomic entity.
 */
void RS_Selection::selectContour(RS_Entity* e) {

    if (e==NULL) {
        return;
    }

    if (!e->isAtomic()) {
        return;
    }

    bool select = !e->isSelected();
    RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
    RS_Vector p1 = ae->getStartpoint();
    RS_Vector p2 = ae->getEndpoint();
    bool found = false;

    // (de)select 1st entity:
    /*
    if (graphicView!=NULL) {
        graphicView->deleteEntity(e);
    }
    */
    e->setSelected(select);
    /*
    if (graphicView!=NULL) {
        graphicView->drawEntity(e);
    }
    */

    do {
        found = false;

        for (RS_Entity* en=container->firstEntity(); en!=NULL;
                en=container->nextEntity()) {

            if (en!=NULL && en->isVisible() && 
                en->isAtomic() && en->isSelected()!=select && 
                (en->getLayer()==NULL || en->getLayer()->isLocked()==false)) {

                ae = dynamic_cast<RS_AtomicEntity*>(en);
                bool doit = false;

                // startpoint connects to 1st point
                if (ae->getStartpoint().distanceTo(p1)<1.0e-4) {
                    doit = true;
                    p1 = ae->getEndpoint();
                }

                // endpoint connects to 1st point
                else if (ae->getEndpoint().distanceTo(p1)<1.0e-4) {
                    doit = true;
                    p1 = ae->getStartpoint();
                }

                // startpoint connects to 2nd point
                else if (ae->getStartpoint().distanceTo(p2)<1.0e-4) {
                    doit = true;
                    p2 = ae->getEndpoint();
                }

                // endpoint connects to 1st point
                else if (ae->getEndpoint().distanceTo(p2)<1.0e-4) {
                    doit = true;
                    p2 = ae->getStartpoint();
                }

                if (doit) {
                    /*if (graphicView!=NULL) {
                        graphicView->deleteEntity(ae);
                    }*/
                    ae->setSelected(select);
                    /*if (graphicView!=NULL) {
                        graphicView->drawEntity(ae);
                    }*/
                    found = true;
                }
            }
        }
    } while(found);

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
}



/**
 * Selects all entities on the given layer.
 */
void RS_Selection::selectLayer(RS_Entity* e) {

    if (e==NULL) {
        return;
    }

    bool select = !e->isSelected();

    RS_Layer* layer = e->getLayer(true);
    if (layer==NULL) {
        return;
    }

    RS_String layerName = layer->getName();
    selectLayer(layerName, select);
}



/**
 * Selects all entities on the given layer.
 */
void RS_Selection::selectLayer(const RS_String& layerName, bool select) {

    for (RS_Entity* en=container->firstEntity(); en!=NULL;
            en=container->nextEntity()) {

        if (en!=NULL && en->isVisible() && 
                en->isSelected()!=select && 
                (en->getLayer()==NULL || en->getLayer()->isLocked()==false)) {

            RS_Layer* l = en->getLayer(true);

            if (l!=NULL && l->getName()==layerName) {
                /*if (graphicView!=NULL) {
                    graphicView->deleteEntity(en);
                }*/
                en->setSelected(select);
                /*if (graphicView!=NULL) {
                    graphicView->drawEntity(en);
                }*/
            }
        }
    }

    if (graphicView!=NULL) {
        graphicView->redraw();
    }
}

    
/**
 * Selects the currently active layer.
 */
void RS_Selection::selectCurrentLayer() {
    selectAll(false, false);
    if (graphic!=NULL) {
        RS_Layer* l = graphic->getActiveLayer();
        if (l!=NULL) {
            selectLayer(l->getName());
        }
    }
}

