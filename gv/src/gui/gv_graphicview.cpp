/****************************************************************************
** $Id: gv_graphicview.cpp 9538 2008-04-03 10:32:33Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#include "gv_graphicview.h"

#include <stdio.h>

#include "gv_application.h"
//#include "gv_actioninterface.h"
#include "gv_block.h"
//#include "gv_eventhandler.h"
#include "gv_entity.h"
#include "gv_graphic.h"
#include "gv_grid.h"
#include "gv_insert.h"
//#include "gv_keyevent.h"
#include "gv_layer.h"
#include "gv_line.h"
//#include "gv_mouseevent.h"
#include "gv_painter.h"
//#include "gv_painterqt.h"
//#include "gv_preview.h"
#include "gv_text.h"
#include "gv_settings.h"
#include "gv_solid.h"




/**
 * Constructor.
 */
GV_GraphicView::GV_GraphicView()
        : background(), foreground() {
    drawingMode = GV2::ModeFull;
    printing = false;
    deleteMode = false;
    factor = GV_Vector(1.0,1.0);
    offsetX = 0;
    offsetY = 0;
    previousFactor = GV_Vector(1.0,1.0);
    previousOffsetX = 0;
    previousOffsetY = 0;
    container = NULL;
//    eventHandler = new GV_EventHandler(this);
    gridColor = GV_Color(Qt::gray);
    metaGridColor = GV_Color(64,64,64);
    grid = new GV_Grid(this);
    updateEnabled = 0;
    zoomFrozen = false;
    draftMode = false;
    painter = NULL;
    
//    preview = new GV_Preview(NULL);

    borderLeft = 0;
    borderTop = 0;
    borderRight = 0;
    borderBottom = 0;

    relativeZero = GV_Vector(false);
    relativeZeroLocked=false;

    mx = my = 0;

    defaultSnapMode = GV2::ActionSnapAuto;
    defaultSnapRes = GV2::ActionRestrictNothing;

    GV_SETTINGS->beginGroup("/Appearance");
    setBackground(QColor(GV_SETTINGS->readEntry("/BackgroundColor", "#000000")));
    setGridColor(QColor(GV_SETTINGS->readEntry("/GridColor", "#7F7F7F")));
    setMetaGridColor(QColor(GV_SETTINGS->readEntry("/MetaGridColor", "#3F3F3F")));
    setSelectedColor(QColor(GV_SETTINGS->readEntry("/SelectedColor", "#A54747")));
    setHighlightedColor(QColor(GV_SETTINGS->readEntry("/HighlightedColor",
                               "#739373")));
    GV_SETTINGS->endGroup();

    printPreview = false;

    simulationSpeed = 100;
    simulationSmooth = false;
    simulationRapid = false;
    simulationRunning = false;
}



/**
 * Destructor.
 */
GV_GraphicView::~GV_GraphicView() {
    RB_DEBUG->print("GV_GraphicView::~GV_GraphicView");
//    delete eventHandler;
    if (painter!=NULL) {
        delete painter;
    }
    delete grid;
//    delete preview;
    RB_DEBUG->print("GV_GraphicView::~GV_GraphicView: OK");
}



/**
 * Must be called by any derrived class in the destructor.
 */
void GV_GraphicView::cleanUp() {
}



/**
 * Sets the pointer to the graphic which contains the entities
 * which are visualized by this widget.
 */
void GV_GraphicView::setContainer(GV_EntityContainer* container) {
    this->container = container;
//    preview->setParentContainer(container);
}



/**
 * Sets the zoom factor in X for this visualization of the graphic.
 */
bool GV_GraphicView::setFactorX(double f) {
//    RB_DEBUG->print("GV_GraphicView::setFactorX: %f", f);
    if (!zoomFrozen) {
        f = fabs(f);
        if (checkFactor(f)) {
            factor.x = f;
            return true;
        }
    }

    return false;
}



/**
 * Sets the zoom factor in Y for this visualization of the graphic.
 */
bool GV_GraphicView::setFactorY(double f) {
    if (!zoomFrozen) {
        f = fabs(f);
        if (checkFactor(f)) {
            factor.y = f;
            return true;
        }
    }
    
    return false;
}



/**
 * @return true if the grid is switched on.
 */
bool GV_GraphicView::isGridOn() {
    if (container!=NULL) {
        GV_Graphic* g = container->getGraphic();
        if (g!=NULL) {
            return g->isGridOn();
        }
    }
    return true;
}




/**
 * Centers the drawing in x-direction.
 */
void GV_GraphicView::centerOffsetX() {
    if (container!=NULL && !zoomFrozen) {
        offsetX = (int)(((getWidth()-borderLeft-borderRight)
                         - (container->getSize().x*factor.x))/2.0
                        - (container->getMin().x*factor.x)) + borderLeft;
    }
}



/**
 * Centers the drawing in y-direction.
 */
void GV_GraphicView::centerOffsetY() {
    if (container!=NULL && !zoomFrozen) {
        offsetY = (int)((getHeight()-borderTop-borderBottom
                         - (container->getSize().y*factor.y))/2.0
                        - (container->getMin().y*factor.y)) + borderBottom;
    }
}



/**
 * Centers the given coordinate in the view in x-direction.
 */
void GV_GraphicView::centerX(double v) {
    if (!zoomFrozen) {
        offsetX = (int)((v*factor.x)
                        - (double)(getWidth()-borderLeft-borderRight)/2.0);
    }
}



/**
 * Centers the given coordinate in the view in y-direction.
 */
void GV_GraphicView::centerY(double v) {
    if (!zoomFrozen) {
        offsetY = (int)((v*factor.y)
                        - (double)(getHeight()-borderTop-borderBottom)/2.0);
    }
}



/**
 * Updates the view controls.
 */
/*
void GV_GraphicView::updateView() {
    static int running = 0;
    running++;
    if (running<100) {
        adjustZoomControls();
        adjustOffsetControls();
    }
    running--;
    if(running==0) {
        redraw();
    }
}
*/


/**
 * @return Current action or NULL.
 */
GV_ActionInterface* GV_GraphicView::getDefaultAction() {
//    if (eventHandler!=NULL) {
//        return eventHandler->getDefaultAction();
//    } else {
        return NULL;
//    }
}



/**
 * Sets the default action of the event handler.
 */
void GV_GraphicView::setDefaultAction(GV_ActionInterface* action) {
//    if (eventHandler!=NULL) {
//        eventHandler->setDefaultAction(action);
//    }
}



/**
 * @return Current action or NULL.
 */
GV_ActionInterface* GV_GraphicView::getCurrentAction() {
//    if (eventHandler!=NULL) {
//        return eventHandler->getCurrentAction();
//    } else {
        return NULL;
//    }
}



/**
 * Sets the current action of the event handler.
 */
void GV_GraphicView::setCurrentAction(GV_ActionInterface* action) {
    RB_DEBUG->print("GV_GraphicView::setCurrentAction");
    if (eventHandler!=NULL) {
//        eventHandler->setCurrentAction(action);
    }
    RB_DEBUG->print("GV_GraphicView::setCurrentAction: OK");
}


/**
 * Kills all running selection actions. Called when a selection action
 * is launched to reduce confusion.
 */
/*
void GV_GraphicView::killSelectActions() {
    if (eventHandler!=NULL) {
        eventHandler->killSelectActions();
    }
}
*/



/**
 * Kills all running actions. 
 */
void GV_GraphicView::killAllActions() {
    if (eventHandler!=NULL) {
//        eventHandler->killAllActions();
    }
}



/**
 * Go back in menu or current action.
 */
void GV_GraphicView::back() {
//    if (eventHandler!=NULL && eventHandler->hasAction()) {
//        eventHandler->back();
//    } else {
//        if (GV_DIALOGFACTORY!=NULL) {
//            GV_DIALOGFACTORY->requestPreviousMenu();
//        }
//    }
}



/**
 * Go forward with the current action.
 */
void GV_GraphicView::enter() {
    if (simulationRunning) {
        return;
    }
    
//    if (eventHandler!=NULL && eventHandler->hasAction()) {
//        eventHandler->enter();
//    }
}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about mouse events.
// */
//void GV_GraphicView::mousePressEvent(GV_MouseEvent* e) {
//    if (simulationRunning) {
//        return;
//    }

//    if (eventHandler!=NULL) {
//        eventHandler->mousePressEvent(e);
//    }
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about mouse events.
// */
//void GV_GraphicView::mouseReleaseEvent(GV_MouseEvent* e) {
//    if (simulationRunning) {
//        return;
//    }

//    RB_DEBUG->print("GV_GraphicView::mouseReleaseEvent");
//    if (eventHandler!=NULL) {
//        if (GV2::qtToRsButtonState(e->button())!=GV2::RightButton ||
//            eventHandler->hasAction()) {
//            eventHandler->mouseReleaseEvent(e);
//        }
//        else {
//            back();
//            e->accept();
//        }
//    }
//    RB_DEBUG->print("GV_GraphicView::mouseReleaseEvent: OK");
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about mouse events.
// */
//void GV_GraphicView::mouseMoveEvent(GV_MouseEvent* e) {
//    if (simulationRunning) {
//        return;
//    }

//    GV_Graphic* graphic = NULL;

//    if (container!=NULL && container->rtti()==GV2::EntityGraphic) {
//        graphic = dynamic_cast<GV_Graphic*>(container);
//    }

//    if (e!=NULL) {
//        mx = e->x();
//        my = e->y();
//    }

//    if (eventHandler!=NULL) {
//        eventHandler->mouseMoveEvent(e);
//    }

//    if (eventHandler==NULL || (!eventHandler->hasAction() && graphic!=NULL)) {
//        GV_Vector mouse = toGraph(GV_Vector(mx, my));
//        GV_Vector relMouse = mouse - getRelativeZero();

//        if (GV_DIALOGFACTORY!=NULL) {
//            GV_DIALOGFACTORY->updateCoordinateWidget(mouse, relMouse);
//        }
//    }
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about mouse events.
// */
//void GV_GraphicView::mouseLeaveEvent() {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->mouseLeaveEvent();
//    }
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about mouse events.
// */
//void GV_GraphicView::mouseEnterEvent() {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->mouseEnterEvent();
//    }
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about key events.
// */
//void GV_GraphicView::keyPressEvent(GV_KeyEvent* e) {
//    if (simulationRunning) {
//        e->ignore();
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->keyPressEvent(e);
//    }
//    else {
//        e->ignore();
//    }
//}



///**
// * Called by the actual GUI class which implements the GV_GraphicView
// * interface to notify qcadlib about key events.
// */
//void GV_GraphicView::keyReleaseEvent(GV_KeyEvent* e) {
//    if (simulationRunning) {
//        e->ignore();
//        return;
//    }

//    if (eventHandler!=NULL) {
//        eventHandler->keyReleaseEvent(e);
//    }
//    else {
//        e->ignore();
//    }
//}



///**
// * Called by the actual GUI class which implements a command line.
// */
//void GV_GraphicView::commandEvent(GV_CommandEvent* e) {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->commandEvent(e);
//    }
//}
    
    
    
//void GV_GraphicView::dragEnterEvent(QDragEnterEvent* e) {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->dragEnterEvent(e);
//    }
//}
    
    
    
//void GV_GraphicView::dragMoveEvent(QDragMoveEvent* e) {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->dragMoveEvent(e);
//    }
//}



//void GV_GraphicView::dropEvent(QDropEvent* e) {
//    if (simulationRunning) {
//        return;
//    }
    
//    if (eventHandler!=NULL) {
//        eventHandler->dropEvent(e);
//    }
//}



/**
 * Enables coordinate input in the command line.
 */
void GV_GraphicView::enableCoordinateInput() {
//    if (eventHandler!=NULL) {
//        eventHandler->enableCoordinateInput();
//    }
}



/**
 * Disables coordinate input in the command line.
 */
void GV_GraphicView::disableCoordinateInput() {
//    if (eventHandler!=NULL) {
//        eventHandler->disableCoordinateInput();
//    }
}



/**
 * zooms in by factor f
 */
void GV_GraphicView::zoomIn(double f, const GV_Vector& center) {

    if (!checkFactor(f)) {
        RB_DEBUG->warning("GV_GraphicView::zoomIn: invalid factor");
        return;
    }

    if (simulationRunning) {
        return;
    }

    GV_Vector c = center;
    if (c.valid==false) {
        c = toGraph(GV_Vector(getWidth()/2, getHeight()/2));
    }

    zoomWindow(
        toGraph(GV_Vector(0,0))
        .scale(c, GV_Vector(1.0/f,1.0/f)),
        toGraph(GV_Vector(getWidth(),getHeight()))
        .scale(c, GV_Vector(1.0/f,1.0/f)));
}



/**
 * zooms in by factor f in x
 */
void GV_GraphicView::zoomInX(double f) {
    if (simulationRunning) {
        return;
    }
    if (setFactorX(factor.x*f)) {
        offsetX=(int)((offsetX-getWidth()/2)*f)+getWidth()/2;
        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
}



/**
 * zooms in by factor f in y
 */
void GV_GraphicView::zoomInY(double f) {
    if (simulationRunning) {
        return;
    }
    if (setFactorY(factor.y*f)) {
        offsetY=(int)((offsetY-getHeight()/2)*f)+getHeight()/2;
        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
}



/**
 * zooms out by factor f
 */
void GV_GraphicView::zoomOut(double f, const GV_Vector& center) {
    if (simulationRunning) {
        return;
    }
    if (!checkFactor(f)) {
        RB_DEBUG->warning("GV_GraphicView::zoomOut: invalid factor");
        return;
    }
    zoomIn(1/f, center);
}



/**
 * zooms out by factor f in x
 */
void GV_GraphicView::zoomOutX(double f) {
    if (simulationRunning) {
        return;
    }
    if (!checkFactor(f)) {
        RB_DEBUG->warning("GV_GraphicView::zoomOutX: invalid factor");
        return;
    }
    if (setFactorX(factor.x/f)) {
        offsetX=(int)(offsetX/f);
        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
}



/**
 * zooms out by factor f y
 */
void GV_GraphicView::zoomOutY(double f) {
    if (simulationRunning) {
        return;
    }
    if (!checkFactor(f)) {
        RB_DEBUG->warning("GV_GraphicView::zoomOutY: invalid factor");
        return;
    }
    if (setFactorY(factor.y/f)) {
        offsetY=(int)(offsetY/f);
        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
}



/**
 * performs autozoom
 *
 * @param axis include axis in zoom
 * @param keepAspectRatio true: keep aspect ratio 1:1
 *                        false: factors in x and y are stretched to the max
 */
void GV_GraphicView::zoomAuto(bool axis, bool keepAspectRatio) {

    if (simulationRunning) {
        return;
    }
    RB_DEBUG->print("GV_GraphicView::zoomAuto");
//    RB_DEBUG->print("GV_GraphicView::zoomAuto: borderTop: %d", borderTop);
    
    saveView();

    if (container!=NULL) {
        // 20080112: only auto zoom to visible entities
        container->calculateBorders(true);
        /*
        GV_Vector minV(GV_MAXDOUBLE,GV_MAXDOUBLE);
        GV_Vector maxV(GV_MINDOUBLE,GV_MINDOUBLE);

        for (GV_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {

            if (e->isVisible()) {
                e->calculateBorders();
                if (!e->isContainer() || e->count()>0) {
                    minV = GV_Vector::minimum(e->getMin(),minV);
                    maxV = GV_Vector::maximum(e->getMax(),maxV);
                }
            }
        }
        */

        GV_Vector s;
        if (axis) {
            s.x = std::max(container->getMax().x, 0.0)
                 - std::min(container->getMin().x, 0.0);
            s.y = std::max(container->getMax().y, 0.0)
                 - std::min(container->getMin().y, 0.0);
        } else {
            s = container->getSize();
        }

        GV_Vector f;

        if (s.x>GV_TOLERANCE) {
            f.x = (getWidth()-borderLeft-borderRight) / s.x;
        } else {
            f.x = 1.0;
        }

        if (s.y>GV_TOLERANCE) {
            f.y = (getHeight()-borderTop-borderBottom) / s.y;
        } else {
            f.y = 2.0;
        }

        if (keepAspectRatio) {
            f.x = f.y = std::min(f.x, f.y);
        }

        if (f.x<GV_TOLERANCE) {
            f.x=f.y=1.0;
        }
        setFactorX(f.x);
        setFactorY(f.y);

        centerOffsetX();
        centerOffsetY();

        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
    RB_DEBUG->print("GV_GraphicView::zoomAuto OK");
}




/**
 * Performs autozoom to selection.
 */
void GV_GraphicView::zoomSelection() {

    if (simulationRunning) {
        return;
    }

    RB_DEBUG->print("GV_GraphicView::zoomSelection");
    
    saveView();

    if (container!=NULL) {
        GV_Vector minV(GV_MAXDOUBLE,GV_MAXDOUBLE);
        GV_Vector maxV(GV_MINDOUBLE,GV_MINDOUBLE);

        for (GV_Entity* e=container->firstEntity();
                e!=NULL;
                e=container->nextEntity()) {

            if (e->isVisible() && e->isSelected()) {
                e->calculateBorders();
                if (!e->isContainer() || e->count()>0) {
                    minV = GV_Vector::minimum(e->getMin(),minV);
                    maxV = GV_Vector::maximum(e->getMax(),maxV);
                }
            }
        }

        GV_Vector s = maxV - minV;
        GV_Vector f;

        if (s.x>GV_TOLERANCE) {
            f.x = (getWidth()-borderLeft-borderRight) / s.x;
        } else {
            f.x = 1.0;
        }

        if (s.y>GV_TOLERANCE) {
            f.y = (getHeight()-borderTop-borderBottom) / s.y;
        } else {
            f.y = 1.0;
        }

        f.x = f.y = std::min(f.x, f.y);

        if (f.x<GV_TOLERANCE) {
            f.x=f.y=1.0;
        }

        setFactorX(f.x);
        setFactorY(f.y);

        adjustOffsetControls();
        adjustZoomControls();
        //centerOffsetX();
        //centerOffsetY();
        offsetX = (int)(((getWidth()-borderLeft-borderRight)
                         - (s.x*factor.x))/2.0
                        - (minV.x*factor.x)) + borderLeft;
        offsetY = (int)((getHeight()-borderTop-borderBottom
                         - (s.y*factor.y))/2.0
                        - (minV.y*factor.y)) + borderBottom;
        updateGrid();
        redraw();
    }

    RB_DEBUG->print("GV_GraphicView::zoomSelection OK");
}



/**
 * Shows previous view. 
 */
void GV_GraphicView::zoomPrevious() {

    RB_DEBUG->print("GV_GraphicView::zoomPrevious");
    if (simulationRunning) {
        return;
    }

    if (container!=NULL) {
        restoreView();
    }
}



/**
 * Saves the current view as previous view to which we can 
 * switch back later with @see restoreView(). 
 */
void GV_GraphicView::saveView() {
    previousOffsetX = offsetX;
    previousOffsetY = offsetY;
    previousFactor = factor;
}



/**
 * Restores the view previously saved with 
 * @see saveView(). 
 */
void GV_GraphicView::restoreView() {
    int pox = previousOffsetX;
    int poy = previousOffsetY;
    GV_Vector pf = previousFactor;

    saveView();
    
    offsetX = pox;
    offsetY = poy;
    factor = pf;
    
    adjustOffsetControls();
    adjustZoomControls();
    updateGrid();
    redraw();
}



/**
 * performs autozoom in y only
 *
 * @param axis include axis in zoom
 */
void GV_GraphicView::zoomAutoY(bool axis) {
    if (simulationRunning) {
        return;
    }
    if (container!=NULL) {
        double visibleHeight = 0.0;
        double minY = GV_MAXDOUBLE;
        double maxY = GV_MINDOUBLE;
        bool noChange = false;

        for (GV_Entity* e=container->firstEntity(GV2::ResolveNone);
                e!=NULL;
                e = container->nextEntity(GV2::ResolveNone)) {

            if (e->rtti()==GV2::EntityLine) {
                GV_Line* l = dynamic_cast<GV_Line*>(e);
                double x1, x2;
                x1 = toGuiX(l->getStartpoint().x);
                x2 = toGuiX(l->getEndpoint().x);
                if ((x1>0.0 && x1<(double)getWidth()) ||
                        (x2>0.0 && x2<(double)getWidth())) {
                    minY = std::min(minY, l->getStartpoint().y);
                    minY = std::min(minY, l->getEndpoint().y);
                    maxY = std::max(maxY, l->getStartpoint().y);
                    maxY = std::max(maxY, l->getEndpoint().y);
                }
            }
        }

        if (axis) {
            visibleHeight = std::max(maxY, 0.0) - std::min(minY, 0.0);
        } else {
            visibleHeight = maxY-minY;
        }

        if (visibleHeight<1.0) {
            noChange = true;
        }

        double fy = 1.0;
        if (visibleHeight>1.0e-6) {
            fy = (getHeight()-borderTop-borderBottom)
                 / visibleHeight;
            if (factor.y<0.000001) {
                noChange = true;
            }
        }

        if (noChange==false) {
            if (setFactorY(fy)) {
                //centerOffsetY();
                offsetY = (int)((getHeight()-borderTop-borderBottom
                                 - (visibleHeight*factor.y))/2.0
                                - (minY*factor.y)) + borderBottom;
                adjustOffsetControls();
                adjustZoomControls();
                updateGrid();
            }
        }
        RB_DEBUG->print("Auto zoom y ok");
    }
}



/**
 * Zooms the area given by v1 and v2.
 *
 * @param keepAspectRatio true: keeps the aspect ratio 1:1
 *                        false: zooms exactly the selected range to the
 *                               current graphic view
 */
void GV_GraphicView::zoomWindow(GV_Vector v1, GV_Vector v2,
                                bool keepAspectRatio) {

    if (simulationRunning) {
        return;
    }

    saveView();
    
    double zoomX=480.0;    // Zoom for X-Axis
    double zoomY=640.0;    // Zoom for Y-Axis   (Set smaller one)
    double dum;            // Dummy for switching values
    int zoomBorder = 0;

    // Switch left/right and top/bottom is necessary:
    if(v1.x>v2.x) {
        dum=v1.x;
        v1.x=v2.x;
        v2.x=dum;
    }
    if(v1.y>v2.y) {
        dum=v1.y;
        v1.y=v2.y;
        v2.y=dum;
    }

    // Get zoom in X and zoom in Y:
    if(v2.x-v1.x>1.0e-6) {
        zoomX = getWidth() / (v2.x-v1.x);
    }
    if(v2.y-v1.y>1.0e-6) {
        zoomY = getHeight() / (v2.y-v1.y);
    }

    // Take smaller zoom:
    if (keepAspectRatio) {
        if(zoomX<zoomY) {
            if(getWidth()!=0) {
                zoomX = zoomY = ((double)(getWidth()-2*zoomBorder)) /
                                (double)getWidth()*zoomX;
            }
        } else {
            if(getHeight()!=0) {
                zoomX = zoomY = ((double)(getHeight()-2*zoomBorder)) /
                                (double)getHeight()*zoomY;
            }
        }
    }

    if(zoomX<0.0) {
        zoomX*=-1;
    }
    if(zoomY<0.0) {
        zoomY*=-1;
    }

    // Borders in pixel after zoom
    int pixLeft  =(int)(v1.x*zoomX);
    int pixTop   =(int)(v2.y*zoomY);
    int pixRight =(int)(v2.x*zoomX);
    int pixBottom=(int)(v1.y*zoomY);

    // Set new offset for zero point:
    if (setFactorX(zoomX) && setFactorY(zoomY)) {
        offsetX = - pixLeft + (getWidth() -pixRight +pixLeft)/2;
        offsetY = - pixTop + (getHeight() -pixBottom +pixTop)/2;
        adjustOffsetControls();
        adjustZoomControls();
        updateGrid();
        redraw();
    }
}



/**
 * Centers the point v1.
 */
void GV_GraphicView::zoomPan(int dx, int dy) {
    if (simulationRunning) {
        return;
    }

    offsetX += dx;
    offsetY -= dy;

    disableUpdate();
    adjustOffsetControls();
    updateGrid();
    enableUpdate();
    redraw();
}



/**
 * Scrolls in the given direction.
 */
void GV_GraphicView::zoomScroll(GV2::Direction direction) {
    if (simulationRunning) {
        return;
    }
    switch (direction) {
    case GV2::Up:
        offsetY-=50;
        break;
    case GV2::Down:
        offsetY+=50;
        break;
    case GV2::Right:
        offsetX+=50;
        break;
    case GV2::Left:
        offsetX-=50;
        break;
    }
    adjustOffsetControls();
    adjustZoomControls();
    updateGrid();
    redraw();
}



/**
 * Zooms to page extends.
 */
void GV_GraphicView::zoomPage() {

    RB_DEBUG->print("GV_GraphicView::zoomPage");
    if (container==NULL) {
        return;
    }
    if (simulationRunning) {
        return;
    }

    GV_Graphic* graphic = container->getGraphic();
    if (graphic==NULL) {
        return;
    }

    GV_Vector s = graphic->getPaperSize();
//    GV_Vector pinsbase = graphic->getPaperInsertionBase();

    double fx, fy;

    if (s.x>GV_TOLERANCE) {
        fx = (getWidth()-borderLeft-borderRight) / s.x;
    } else {
        fx = 1.0;
    }

    if (s.y>GV_TOLERANCE) {
        fy = (getHeight()-borderTop-borderBottom) / s.y;
    } else {
        fy = 1.0;
    }

    fx = fy = std::min(fx, fy);

    if (fx<GV_TOLERANCE) {
        fx=fy=1.0;
    }

    setFactorX(fx);
    setFactorY(fy);

    centerOffsetX();
    centerOffsetY();
    adjustOffsetControls();
    adjustZoomControls();
    updateGrid();
    redraw();
}



/**
 * Draws the entities. If painter is NULL a new painter will 
 * be created and destroyed.
 */
void GV_GraphicView::drawIt() {
    if (!isUpdateEnabled()) {
        return;
    }
    if (simulationRunning) {
        return;
    }
    bool painterCreated = false;

    GV_SETTINGS->beginGroup("/Appearance");
    draftMode = (bool)GV_SETTINGS->readNumEntry("/DraftMode", 0);
    GV_SETTINGS->endGroup();

    RB_DEBUG->print("GV_GraphicView::drawIt");

    if (painter==NULL) {
#ifdef GV_DIRECTPAINTER
        createDirectPainter();
        painter->erase();
#else

        createPainter();
#endif

        painterCreated = true;
    } else {
        painter->erase();
    }

    // drawing paper border:
    if (isPrintPreview()) {
        drawPaper();
    }

    // drawing meta grid:
    if (!isPrintPreview()) {
        drawMetaGrid();
    }

    // drawing entities:
    drawEntity(container, 0.0, true);

    // drawing zero points:
    if (!isPrintPreview()) {
        drawAbsoluteZero();
    }

    // drawing grid:
    if (!isPrintPreview()) {
        drawGrid();
    }

    if (painterCreated==true) {
        destroyPainter();
    }

    GV_Graphic* graphic = getGraphic();
    if (graphic!=NULL) {
//        RB_DEBUG->print("GV_GraphicView::drawIt: mod: %d",
//            (int)graphic->isModifiedSinceRedraw());
    }
    
    RB_DEBUG->print("GV_GraphicView::drawIt: OK");
}


/**
 * Sets the pen of the painter object to the suitable pen for the given
 * entity.
 */
void GV_GraphicView::setPenForEntity(GV_Entity* e) {
    if (drawingMode==GV2::ModePreview) {
        return;
    }

    // set color of entity
    if (painter!=NULL && !painter->isPreviewMode()) {

        // Getting pen from entity (or layer)
        GV_Pen pen = e->getPen(true);

        // use preset fixed screen width:
        //if (pen.getWidth()==GV2::WidthScreen) {
        //}

        // scale pen width:
        //else 
        if (!draftMode && container!=NULL) {
            int w = pen.getWidth();
            if (w<0) {
                w = 0;
            }
            
            double uf = 1.0;  // unit factor
            double wf = 1.0;  // width factor
            GV_Graphic* graphic = container->getGraphic();
            if (graphic!=NULL) {
                uf = GV_Units::convert(1.0, GV2::Millimeter, graphic->getUnit());
                if ((isPrinting() || isPrintPreview()) &&
                        graphic->getPaperScale()>1.0e-6) {

                    wf = 1.0/graphic->getPaperScale();
                }
            }

            pen.setScreenWidth(toGuiDX(w/100.0*uf*wf));
        } else {
            pen.setScreenWidth(0);
        }


        // prevent drawing with 1-width which is slow:
        if (!isPrinting() && GV_Math::mround(pen.getScreenWidth())==1) {
            pen.setScreenWidth(0.0);
        }

#ifdef Q_OS_MACX
        // prevent printing with zero-width under mac os x:
        if (isPrinting() && GV_Math::mround(pen.getScreenWidth())==0) {
            pen.setScreenWidth(1);
        }
#endif

        // prevent background color on background drawing:
        if (pen.getColor().stripFlags()==background.stripFlags()) {
            pen.setColor(foreground);
        }

        // this entity is selected:
        if (e->isSelected() && e->isSelectionVisible()) {
            RB_DEBUG->print("GV_GraphicView::setPenForEntity: "
                "set selected pen");
            pen.setLineType(GV2::SolidLine);
            if (e->rtti()==GV2::EntityHatch) {
                pen.setColor(selectedColor.darker(133));
            }
            else {
                pen.setColor(selectedColor);
            }
        }

        // this entity is highlighted:
        if (e->isHighlighted()) {
            pen.setColor(highlightedColor);
        }

        // deleting not drawing:
        if (getDeleteMode()) {
            pen.setColor(background);
        }

        /*
        // experimental: non-active layers transparent
        if (container->isOfType(GV2::EntityGraphic)) {
            GV_Graphic* g = (GV_Graphic*)container;
            if (e->getLayer()!=g->getActiveLayer()) {
                pen.makeTransparent(128);
            }
        }
        */

        painter->setPen(pen);
        painter->setOpacity(pen.getColor().alphaF());
    }
}

    
    
/**
 * @return True if the given entity is visible in the current view port 
 * of this view.
 */
bool GV_GraphicView::isVisible(GV_Entity* e) {
    // update is disabled:
    if (!isUpdateEnabled()) {
        RB_DEBUG->print("GV_GraphicView::isVisible: updates disabled");
        return false;
    }

    // given entity is NULL:
    if (e==NULL) {
        RB_DEBUG->print("GV_GraphicView::isVisible: NULL entity");
        return false;
    }

    // entity is not visible:
    if (!e->isVisible()) {
        RB_DEBUG->print("GV_GraphicView::isVisible: not visible");
        return false;
    }

    // test if the entity is in the viewport
    if (!e->isContainer() && !printing &&
            (toGuiX(e->getMax().x)<-50 || 
             toGuiX(e->getMin().x)>getWidth()+50 ||
             toGuiY(e->getMin().y)<-50 || 
             toGuiY(e->getMax().y)>getHeight()+50)) {
        RB_DEBUG->print("GV_GraphicView::isVisible: out of viewport");
        return false;
    }
    
#ifdef Q_OS_WIN
    if (!e->isContainer() && !printing &&
        (toGuiX(e->getMax().x)>25000 || 
         toGuiX(e->getMin().x)<-25000 ||
         toGuiY(e->getMin().y)>25000 || 
         toGuiY(e->getMax().y)<-25000)) {
        RB_DEBUG->print("GV_GraphicView::isVisible: "
            "could trigger overflow (windows)");
        return false;
    }
#endif
    
    return true;
}


/**
 * Draws an entity. Might be recusively called e.g. for polylines.
 * If the class wide painter is NULL a new painter will be created 
 * and destroyed afterwards.
 *
 * @param patternOffset Offset of line pattern (used for connected 
 *        lines e.g. in splines).
 * @param db Double buffering on (recommended) / off
 */
void GV_GraphicView::drawEntity(GV_Entity* e,
    double patternOffset, bool db) {

    if (e==NULL) {
        RB_DEBUG->print("GV_GraphicView::drawEntity: entity is NULL");
        return;
    }
        
    if (!isVisible(e)) {
        RB_DEBUG->print("GV_GraphicView::drawEntity: not visible");
        return;
    }

    // was the painter instance created by this call?
    bool painterCreated = false;
    
    // create a temporary painter device
    if (painter==NULL) {
        if (db) {
            createPainter();
        } else {
            createDirectPainter();
        }
        painterCreated = true;
    }
    
    // set pen (color):
    setPenForEntity(e);

    // draft:
    if (draftMode) {
        // large texts as rectangles:
        if (e->rtti()==GV2::EntityText) {
            if (toGuiDX(dynamic_cast<GV_Text*>(e)->getHeight())<4 || e->countDeep()>100) {
                painter->drawRect(toGui(e->getMin()), toGui(e->getMax()));
            } else {
                drawEntityPlain(e, patternOffset);
            }
        } 
        
        // all images as rectangles:
        else if (e->rtti()==GV2::EntityImage) {
            painter->drawRect(toGui(e->getMin()), toGui(e->getMax()));
        } 
        
        // hide hatches:
        else if (e->rtti()==GV2::EntityHatch) {
            // 20071206: QCad >2.1.3.2: draw outline of solid fills:
            drawEntityPlain(e, patternOffset);
        }

        else {
            drawEntityPlain(e, patternOffset);
        }
    } 
    
    // normal:
    else {
        RB_DEBUG->print("GV_GraphicView::drawEntity: draw plain");

        drawEntityPlain(e, patternOffset);
    }

    if (painterCreated==true) {
        destroyPainter();
    }
}

    
    
void GV_GraphicView::drawEntityHandles(GV_Entity* e) {

    if (!isVisible(e)) {
        return;
    }

    // draw reference points:
    if (e->isSelected() && e->isSelectionVisible()) {
        if (!e->isParentContainerSelected()) {
            GV_VectorList s = e->getRefPoints();

            for (int i=0; i<s.count(); ++i) {
                int sz = -1;
                GV_Color col = GV_Color(0,0,255);
                if (e->rtti()==GV2::EntityPolyline) {
                    if (i==0 || i==s.count()-1) {
                        if (i==0) {
                            sz = 6;
                            col = GV_Color(0,64,255);
                        }
                        else {
                            sz = 6;
                            col = GV_Color(0,0,128);
                        }
                    }
                }
                if (getDeleteMode()) {
                    painter->drawHandle(toGui(s.get(i)), background, sz);
                } else {
                    painter->drawHandle(toGui(s.get(i)), col, sz);
                }
            }
        }
    }
}


/**
 * Deletes an entity with the background color. 
 * Might be recusively called e.g. for polylines.
 */
void GV_GraphicView::deleteEntity(GV_Entity* e) {
    setDeleteMode(true);
    drawEntity(e);
    setDeleteMode(false);
}



/**
 * Draws an entity. 
 * The painter must be initialized and all the attributes (pen) must be set.
 */
void GV_GraphicView::drawEntityPlain(GV_Entity* e, double patternOffset) {
    if (e==NULL) {
        RB_DEBUG->print("GV_GraphicView::drawEntityPlain: entity is NULL");
        return;
    }
        
    if (!e->isContainer() && !printing &&
        (toGuiX(e->getMax().x)<-50 || 
         toGuiX(e->getMin().x)>getWidth()+50 ||
         toGuiY(e->getMin().y)<-50 || 
         toGuiY(e->getMax().y)>getHeight()+50)) {
        return;
    }
    
#ifdef Q_OS_WIN
    if (!e->isContainer() && !printing &&
        (toGuiX(e->getMax().x)>25000 || 
         toGuiX(e->getMin().x)<-25000 ||
         toGuiY(e->getMin().y)>25000 || 
         toGuiY(e->getMax().y)<-25000)) {
        return;
    }
#endif
    
    
    e->draw(painter, this, patternOffset);
}



/**
 * Simulates this drawing in slow motion.
 */
void GV_GraphicView::simulateIt() {
    RB_DEBUG->print("GV_GraphicView::simulateIt");

    if (simulationRunning) {
        return;
    }

    simulationRunning = true;
    simulationLast = GV_Vector(0.0,0.0);

    destroyPainter();

    createSimPainter();
    painter->erase();

    // drawing paper border:
    if (isPrintPreview()) {
        drawPaper();
    }

    // drawing meta grid:
    if (!isPrintPreview()) {
        drawMetaGrid();
    }

    // drawing grid:
    if (!isPrintPreview()) {
        drawGrid();
    }

    // drawing entities:
    GV_Pen pen(foreground, GV2::Width00, GV2::SolidLine);
    simulateEntity(container, pen);
    
//    preview->clear();
    redraw();

    // drawing zero points:
    if (!isPrintPreview()) {
        drawAbsoluteZero();
    }
            
    destroyPainter();

    simulationRunning = false;
}



/**
 * Simulates the given entity.
 *
 * @param smooth If true, the entity will be drawn slowly (pixel by pixel).
 */
void GV_GraphicView::simulateEntity(GV_Entity* e, const GV_Pen& pen) {
    RB_DEBUG->print("GV_GraphicView::simulateEntity");
    
    if (e==NULL) {
        RB_DEBUG->print("GV_GraphicView::simulateEntity: entity is NULL");
        return;
    }

    if (e->isContainer()) {
        RB_DEBUG->print("GV_GraphicView::simulateEntity: simulate container");

        GV_EntityContainer* ec = dynamic_cast<GV_EntityContainer*>(e);
        for (GV_Entity* en=ec->firstEntity(GV2::ResolveNone);
                en!=NULL;
                en = ec->nextEntity(GV2::ResolveNone)) {

            if (en->isVisible() && en->isUndone()==false) {

                // draw rapid move:
                if (en->isAtomic() && simulationRapid) {
                    RB_DEBUG->print("GV_GraphicView::simulateEntity: draw rapid move");
                    GV_Vector sp = dynamic_cast<GV_AtomicEntity*>(en)->getStartpoint();
                    if (sp.distanceTo(simulationLast)>1.0e-4) {
                        createSimPainter();
                        GV_Pen rpen(GV_Color(0,0,255), GV2::Width00, GV2::SolidLine);
                        GV_Line rapidLine(NULL, GV_LineData(simulationLast, sp));
                        simulateEntity(&rapidLine, rpen);
                    }
                }

                if (en->isHighlighted()) {
                    GV_Pen hpen(highlightedColor, GV2::Width00, GV2::SolidLine);
                    simulateEntity(en, hpen);
                } else {
                    simulateEntity(en, pen);
                }

                if (en->isAtomic()) {
                    simulationLast = dynamic_cast<GV_AtomicEntity*>(en)->getEndpoint();
                }

                if (!simulationSmooth) {
                    simulationDelay(true);
                }
            }
        }
    } else {
        if (simulationSmooth) {
            RB_DEBUG->print("GV_GraphicView::simulateEntity: smooth simulation");
            switch (e->rtti()) {
            case GV2::EntityLine: {
                    GV_Line* line = dynamic_cast<GV_Line*>(e);
                    drawLineSmooth(toGui(line->getStartpoint()),
                                   toGui(line->getEndpoint()),
                                   pen);
                    //simulationSpeed);
                }
                break;

            case GV2::EntityArc: {
                    GV_Arc* arc = dynamic_cast<GV_Arc*>(e);
                    drawArcSmooth(toGui(arc->getCenter()),
                                  toGuiDX(arc->getRadius()),
                                  arc->getAngle1(), arc->getAngle2(),
                                  arc->isReversed(),
                                  pen);
                }
                break;

            case GV2::EntityCircle: {
                    GV_Circle* circle = dynamic_cast<GV_Circle*>(e);
                    drawArcSmooth(toGui(circle->getCenter()),
                                  toGuiDX(circle->getRadius()),
                                  0.0, 2.0*M_PI,
                                  false,
                                  pen);
                }
                break;

            default:
                break;
            }
        } else {
            RB_DEBUG->print("GV_GraphicView::simulateEntity: draw a single entity");
            createSimPainter();
            painter->setPen(pen);
            drawEntityPlain(e);
        }
    }
}


/**
 * Delay for slow motion simulation.
 *
 * @param step true: stepping mode (entity by entity simulation). adds a delay.
 */
void GV_GraphicView::simulationDelay(bool /*step*/) {
    //RB_DEBUG->print("GV_GraphicView::simulationDelay");

    int delay;
    static double simulationDelay = -1.0;

    if (simulationDelay<0.0) {
        GV_SETTINGS->beginGroup("/CAM");
        simulationDelay = (double)GV_SETTINGS->readNumEntry("/SimulationDelay", 0);
        GV_SETTINGS->endGroup();
    }

    delay = (100-simulationSpeed);
    int maxCalls = simulationSpeed/10;
    
    static int call=0;
    
    if (call>=maxCalls) {
        redraw();
        call = 0;
    }
    else {
        call++;
    }
        
    for (int i=0; i<=delay*simulationDelay; i++) {
        GV_APP->processEvents(QEventLoop::AllEvents, 10);
    }
}



/**
 * Prepares the tool preview during simulation.
 */
void GV_GraphicView::prepareSimulationPreview(const GV_Vector& pos) {
//    preview->clear();
//    GV_Circle* circle = new GV_Circle(preview,
//        GV_CircleData(pos, toGraphDX(5)));
//    circle->setPen(GV_Pen(GV_Color(128,128,128),
//                          GV2::Width00,
//                          GV2::SolidLine));
//    preview->addEntity(circle);
}


/**
 * Draws a line slowly from (x1, y1) to (x2, y2). This is used for simulation only.
 */
void GV_GraphicView::drawLineSmooth(const GV_Vector& p1, const GV_Vector& p2, const GV_Pen& pen) {
    double alpha = p1.angleTo(p2);
    double xStep, yStep;
    bool  xIsOne;

    if (GV_Math::cmpDouble(alpha, 0.0) || GV_Math::cmpDouble(alpha, 2*M_PI)) {
        xStep = 1.0;
        yStep = 0.0;
        xIsOne=true;
    } else if (GV_Math::cmpDouble(alpha, M_PI/2.0)) {
        xStep = 0.0;
        yStep =1.0;
        xIsOne=false;
    } else if (GV_Math::cmpDouble(alpha, M_PI)) {
        xStep =-1.0;
        yStep = 0.0;
        xIsOne=true;
    } else if (GV_Math::cmpDouble(alpha, M_PI/2.0*3.0)) {
        xStep = 0.0;
        yStep =-1.0;
        xIsOne=false;
    } else if (fabs(p2.x-p1.x)>fabs(p2.y-p1.y)) {
        if (p2.x>p1.x) {
            xStep=1.0;
        } else {
            xStep=-1.0;
        }
        yStep = tan(alpha)*xStep;
        xIsOne=true;
    } else {
        if (p2.y>p1.y) {
            yStep=1.0;
        } else {
            yStep=-1.0;
        }
        xStep = yStep/tan(alpha);
        xIsOne=false;
    }

    double lx = p1.x;
    double ly = p1.y;

    do {
        if (lx>=0.0 && lx<=(double)getWidth() && ly>=0.0 && ly<=(double)getHeight()) {
            createSimPainter();
            painter->setPen(pen);
            painter->drawGridPoint(GV_Vector(lx, ly));

            prepareSimulationPreview(toGraph((int)lx, (int)ly));

            simulationDelay();
        }

        lx+=xStep;
        ly+=yStep;

    } while( ( xIsOne && ((lx>=p1.x && lx<=p2.x) || (lx>=p2.x && lx<=p1.x))) ||
             (!xIsOne && ((ly>=p1.y && ly<=p2.y) || (ly>=p2.y && ly<=p1.y))));
}



void GV_GraphicView::drawArcSmooth(const GV_Vector& center,
                                   double radius, double a1, double a2, bool rev,
                                   const GV_Pen& pen) {
    if (radius<=1.4) {
        createSimPainter();
        painter->setPen(pen);
        painter->drawGridPoint(center);
    } else {
        int ix1 = GV_Math::mround(center.x + cos(a1)*radius);
        int iy1 = GV_Math::mround(center.y - sin(a1)*radius);
        int ix2 = GV_Math::mround(center.x + cos(a2)*radius);
        int iy2 = GV_Math::mround(center.y - sin(a2)*radius);
        int k2x=0;            // Next point on circle
        int k2y=0;            //
        int k1x=ix1;          // Prev point on circle
        int k1y=iy1;          //
        double aStep;          // Angle Step (rad)
        double a;              // Actual Angle (rad)
        double a2cp = a2;      // Copy of a2

        if (1.0/(radius*factor.x)<=1.0) {
            aStep=atan(1.0/(radius*factor.x));
        } else {
            aStep = 0.01;
        }

        if (aStep<0.01) {
            aStep = 0.01;
        }

        if (!rev) {

            // Arc Counterclockwise:
            //
            if (a1>a2cp-0.01) {
                a2cp+=2*M_PI;
            }

            for (a=a1+aStep; a<=a2cp; a+=aStep) {
                k2x = GV_Math::mround(center.x+cos(a)*radius);
                k2y = GV_Math::mround(center.y-sin(a)*radius);
                createSimPainter();
                painter->setPen(pen);
                if ((k2x>=0 && k2x<=painter->getWidth() &&
                        k2y>=0 && k2y<=painter->getHeight()) ||
                        (k1x>=0 && k1x<=painter->getWidth() &&
                         k1y>=0 && k1y<=painter->getHeight())) {
                    painter->drawLine(GV_Vector(k1x, k1y), GV_Vector(k2x, k2y));
                    
                    prepareSimulationPreview(toGraph(k2x, k2y));
                    
                    simulationDelay();
                }

                k1x=k2x;
                k1y=k2y;
            }
            createSimPainter();
            painter->setPen(pen);
            painter->drawLine(GV_Vector(k2x, k2y), GV_Vector(ix2, iy2));
        }
        else {

            // Arc Clockwise:
            //
            if (a1<a2cp+0.01) {
                a2cp-=2*M_PI;
            }
            for (a=a1-aStep; a>=a2cp; a-=aStep) {
                k2x = GV_Math::mround(center.x+cos(a)*radius);
                k2y = GV_Math::mround(center.y-sin(a)*radius);
                createSimPainter();
                painter->setPen(pen);
                if ((k2x>=0 && k2x<=painter->getWidth() &&
                        k2y>=0 && k2y<=painter->getHeight()) ||
                        (k1x>=0 && k1x<=painter->getWidth() &&
                         k1y>=0 && k1y<=painter->getHeight())) {
                    painter->drawLine(GV_Vector(k1x, k1y), GV_Vector(k2x, k2y));
                    
                    prepareSimulationPreview(toGraph(k2x, k2y));

                    simulationDelay();
                }
                k1x=k2x;
                k1y=k2y;
            }
            createSimPainter();
            painter->setPen(pen);
            painter->drawLine(GV_Vector(k2x, k2y), GV_Vector(ix2, iy2));
        }
    }
}



/**
 * @return Pointer to the static pattern struct that belongs to the
 * given pattern type or NULL.
 */
GV_LineTypePattern* GV_GraphicView::getPattern(GV2::LineType t) {
    switch (t) {
    case GV2::SolidLine:
        return &patternSolidLine;
        break;

    case GV2::DotLine:
        return &patternDotLine;
        break;
    case GV2::DotLine2:
        return &patternDotLine2;
        break;
    case GV2::DotLineX2:
        return &patternDotLineX2;
        break;

    case GV2::DashLine:
        return &patternDashLine;
        break;
    case GV2::DashLine2:
        return &patternDashLine2;
        break;
    case GV2::DashLineX2:
        return &patternDashLineX2;
        break;

    case GV2::DashDotLine:
        return &patternDashDotLine;
        break;
    case GV2::DashDotLine2:
        return &patternDashDotLine2;
        break;
    case GV2::DashDotLineX2:
        return &patternDashDotLineX2;
        break;

    case GV2::DivideLine:
        return &patternDivideLine;
        break;
    case GV2::DivideLine2:
        return &patternDivideLine2;
        break;
    case GV2::DivideLineX2:
        return &patternDivideLineX2;
        break;

    case GV2::CenterLine:
        return &patternCenterLine;
        break;
    case GV2::CenterLine2:
        return &patternCenterLine2;
        break;
    case GV2::CenterLineX2:
        return &patternCenterLineX2;
        break;

    case GV2::BorderLine:
        return &patternBorderLine;
        break;
    case GV2::BorderLine2:
        return &patternBorderLine2;
        break;
    case GV2::BorderLineX2:
        return &patternBorderLineX2;
        break;

    case GV2::LineByLayer:
        return &patternBlockLine;
        break;
    case GV2::LineByBlock:
        return &patternBlockLine;
        break;
    default:
        break;
    }
    return NULL;
}



/**
 * This virtual method can be overwritten to draw the absolute 
 * zero. It's called from within drawIt(). The default implemetation
 * draws a simple red round zero point.
 *
 * @see drawIt()
 */
void GV_GraphicView::drawAbsoluteZero() {
    RB_DEBUG->print("GV_GraphicView::drawAbsoluteZero");

    bool painterCreated = false;

    // create a temporary painter device
    if (painter==NULL) {
        createDirectPainter();
        painterCreated = true;
    }

    int zr = 20;

    GV_Pen p(GV_Color(255,0,0), GV2::Width00, GV2::SolidLine);
    p.setScreenWidth(0);
    painter->setPen(p);

    painter->drawLine(GV_Vector(toGuiX(0.0)-zr,
                                toGuiY(0.0)),
                      GV_Vector(toGuiX(0.0)+zr,
                                toGuiY(0.0)));

    painter->drawLine(GV_Vector(toGuiX(0.0),
                                toGuiY(0.0)-zr),
                      GV_Vector(toGuiX(0.0),
                                toGuiY(0.0)+zr));

    if (painterCreated) {
        destroyPainter();
    }
    RB_DEBUG->print("GV_GraphicView::drawAbsoluteZero: OK");
}



/**
 * This virtual method can be overwritten to draw the relative 
 * zero point. It's called from within drawIt(). The default implemetation
 * draws a simple red round zero point.
 *
 * @see drawIt()
 */
void GV_GraphicView::drawRelativeZero() {
    RB_DEBUG->print("GV_GraphicView::drawRelativeZero");
    
    bool painterCreated = false;

    if (relativeZero.valid==false) {
        RB_DEBUG->print("GV_GraphicView::drawRelativeZero: pos not valid");
        return;
    }

    // create a temporary painter device
    if (painter==NULL) {
        createDirectPainter();
        painterCreated = true;
    }

    GV_Pen p(GV_Color(255,0,0), GV2::Width00, GV2::SolidLine);
    p.setScreenWidth(0);
    painter->setPen(p);

    int zr=5;

    painter->drawLine(GV_Vector(toGuiX(relativeZero.x)-zr,
                                toGuiY(relativeZero.y)),
                      GV_Vector(toGuiX(relativeZero.x)+zr,
                                toGuiY(relativeZero.y)));

    painter->drawLine(GV_Vector(toGuiX(relativeZero.x),
                                toGuiY(relativeZero.y)-zr),
                      GV_Vector(toGuiX(relativeZero.x),
                                toGuiY(relativeZero.y)+zr));

    painter->drawCircle(toGui(relativeZero), 5);

    if (painterCreated) {
        destroyPainter();
    }
    
    RB_DEBUG->print("GV_GraphicView::drawRelativeZero: OK");
}



/**
 * Draws the paper border (for print previews).
 *
 * @see drawIt()
 */
void GV_GraphicView::drawPaper() {
    bool painterCreated = false;

    if (container==NULL) {
        return;
    }

    GV_Graphic* graphic = container->getGraphic();
    if (graphic==NULL) {
        return;
    }

    if (graphic->getPaperScale()<1.0e-6) {
        return;
    }

    // create a temporary painter device
    if (painter==NULL) {
        createDirectPainter();
        painterCreated = true;
    }

    // draw paper:
    painter->setPen(GV_Pen(Qt::gray));

    GV_Vector pinsbase = graphic->getPaperInsertionBase();
    GV_Vector size = graphic->getPaperSize();
    double scale = graphic->getPaperScale();

    GV_Vector v1 = toGui((GV_Vector(0,0)-pinsbase)/scale);
    GV_Vector v2 = toGui((size-pinsbase)/scale);

    // gray background:
    painter->fillRect(0,0, getWidth(), getHeight(),
                      GV_Color(200,200,200));

    // shadow
    painter->fillRect(
        (int)(v1.x)+6, (int)(v1.y)+6,
        (int)((v2.x-v1.x)), (int)((v2.y-v1.y)),
        GV_Color(64,64,64));

    // border:
    painter->fillRect(
        (int)(v1.x), (int)(v1.y),
        (int)((v2.x-v1.x)), (int)((v2.y-v1.y)),
        GV_Color(64,64,64));

    // paper
    painter->fillRect(
        (int)(v1.x)+1, (int)(v1.y)-1,
        (int)((v2.x-v1.x))-2, (int)((v2.y-v1.y))+2,
        GV_Color(255,255,255));


    if (painterCreated) {
        destroyPainter();
    }
}


/**
 * Draws the grid.
 *
 * @see drawIt()
 */
void GV_GraphicView::drawGrid() {

    if (grid==NULL || isGridOn()==false) {
        return;
    }

    bool painterCreated = false;

    // create a temporary painter device
    if (painter==NULL) {
        createDirectPainter();
        painterCreated = true;
    }

    // draw grid:
    painter->setPen(gridColor);

    GV_Vector* pts = grid->getPoints();
    if (pts!=NULL) {
        for (int i=0; i<grid->count(); ++i) {
            painter->drawGridPoint(toGui(pts[i]));
        }
    }

    // draw grid info:
    QString info = grid->getInfo();

    updateGridStatusWidget(info);

    if (painterCreated) {
        destroyPainter();
    }
}



/**
 * Draws the meta grid.
 *
 * @see drawIt()
 */
void GV_GraphicView::drawMetaGrid() {

    if (grid==NULL || isGridOn()==false) {
        return;
    }

    bool painterCreated = false;

    // create a temporary painter device
    if (painter==NULL) {
        createDirectPainter();
        painterCreated = true;
    }
    
    GV_SETTINGS->beginGroup("/Appearance");
        bool solidGridLines = (bool)GV_SETTINGS->readNumEntry("/SolidGridLines",
#ifdef Q_OS_MACX
    1
#else
    0
#endif
    );
    GV_SETTINGS->endGroup();

    GV_Pen pen;
    if (solidGridLines) {
        pen = GV_Pen(metaGridColor,
                     GV2::Width00,
                     GV2::SolidLine);
    }
    else {
        pen = GV_Pen(metaGridColor,
                     GV2::Width00,
                     GV2::DotLine);
    }
    painter->setPen(pen);

    // draw meta grid:
    double* mx = grid->getMetaX();
    if (mx!=NULL) {
        for (int i=0; i<grid->countMetaX(); ++i) {
            painter->drawLine(GV_Vector(toGuiX(mx[i]), 0),
                              GV_Vector(toGuiX(mx[i]), getHeight()));
        }
    }
    double* my = grid->getMetaY();
    if (my!=NULL) {
        for (int i=0; i<grid->countMetaY(); ++i) {
            painter->drawLine(GV_Vector(0, toGuiY(my[i])),
                              GV_Vector(getWidth(), toGuiY(my[i])));
        }
    }

    if (painterCreated) {
        destroyPainter();
    }
}


/**
 * Updates the grid if there is one. 
 */
void GV_GraphicView::updateGrid() {
    if (grid!=NULL) {
        grid->update();
    }
}



/**
 * Sets the default snap mode used by newly created actions.
 */
void GV_GraphicView::setDefaultSnapMode(GV2::ActionType sm) {
    defaultSnapMode = sm;
//    if (eventHandler!=NULL) {
//        eventHandler->setSnapMode(sm);
//    }
}



/**
 * Sets a snap restriction (e.g. orthogonal).
 */
void GV_GraphicView::setSnapRestriction(GV2::ActionType sr) {
    defaultSnapRes = sr;

//    if (eventHandler!=NULL) {
//        eventHandler->setSnapRestriction(sr);
//    }
}






/**
 * Translates a real coordinate in X to a screen coordinate X.
 * @param visible Pointer to a boolean which will contain true
 * after the call if the coordinate is within the visible range.
 */
double GV_GraphicView::toGuiX(double x, bool* visible) {
    if (visible!=NULL) {
        double res = x*factor.x+offsetX;
        if (res>0.0 && res<getWidth()) {
            *visible = true;
        } else {
            *visible = false;
        }
    }
    return x*factor.x + offsetX;
}



/**
 * Translates a real coordinate in Y to a screen coordinate Y.
 */
double GV_GraphicView::toGuiY(double y) {
    return -y*factor.y + getHeight() - offsetY;
}



/**
 * Translates a real coordinate distance to a screen coordinate distance.
 */
double GV_GraphicView::toGuiDX(double d) {
    return d*factor.x;
}



/**
 * Translates a real coordinate distance to a screen coordinate distance.
 */
double GV_GraphicView::toGuiDY(double d) {
    return d*factor.y;
}



/**
 * Translates a vector in screen coordinates to a vector in real coordinates.
 */
GV_Vector GV_GraphicView::toGraph(const GV_Vector& v) {
    return GV_Vector(toGraphX(GV_Math::mround(v.x)),
                     toGraphY(GV_Math::mround(v.y)), 0.0);
}



/**
 * Translates two screen coordinates to a vector in real coordinates.
 */
GV_Vector GV_GraphicView::toGraph(int x, int y) {
    return GV_Vector(toGraphX(x), toGraphY(y), 0.0);
}


/**
 * Translates a screen coordinate in X to a real coordinate X.
 */
double GV_GraphicView::toGraphX(int x) {
    return (x - offsetX)/factor.x;
}



/**
 * Translates a screen coordinate in Y to a real coordinate Y.
 */
double GV_GraphicView::toGraphY(int y) {
    return -(y - getHeight() + offsetY)/factor.y;
}



/**
 * Translates a screen coordinate distance to a real coordinate distance.
 */
double GV_GraphicView::toGraphDX(int d) {
    return d/factor.x;
}



/**
 * Translates a screen coordinate distance to a real coordinate distance.
 */
double GV_GraphicView::toGraphDY(int d) {
    return d/factor.y;
}



/**
 * Sets the relative zero coordinate (if not locked)
 * without deleting / drawing the point.
 */
void GV_GraphicView::setRelativeZero(const GV_Vector& pos) {
    if (relativeZeroLocked==false) {
        relativeZero = pos;
    }
}



/**
 * Sets the relative zero coordinate, deletes the old position
 * on the screen and draws the new one.
 */
void GV_GraphicView::moveRelativeZero(const GV_Vector& pos) {
    setRelativeZero(pos);
}

