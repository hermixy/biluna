/****************************************************************************
** $Id: rs_actioninterface.cpp 9523 2008-04-02 21:24:23Z andrew $
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


#include "rs_actioninterface.h"

#include "rs_painter.h"
#include "rs_preview.h"
#include "rs_text.h"
#include "rs_widget.h"


//RS_ActionInterface::idCounter = 0;

    
/**
 * Constructor.
 *
 * Sets the entity container on which the action class inherited 
 * from this interface operates.
 *
 * @param container Entity container this action operates on.
 * @param graphicView Graphic view instance this action operates on.
 *                    Please note that an action belongs to this
 *                    view.
 */
RS_ActionInterface::RS_ActionInterface(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView) :
RS_Snapper(container, graphicView) {

    RS_DEBUG->print("RS_ActionInterface::RS_ActionInterface");

    status = 0;
    finished = false;

    // graphic provides a pointer to the graphic if the
    // entity container is a graphic (i.e. can also hold
    // layers).
    graphic = container.getGraphic();

    // document pointer will be used for undo / redo
    document = container.getDocument();
    
    preview = graphicView.getPreview();
    //visible = false;

    mainWindow = NULL;

    //id = idCounter++;

    RS_DEBUG->print("RS_ActionInterface::RS_ActionInterface: OK");
}

/**
 * Destructor.
 */
RS_ActionInterface::~RS_ActionInterface() {
    // would be pure virtual now:
    // hideOptions();
}

/**
 * Must be implemented to return the ID of this action.
 *
 * @todo no default implementation
 */
/*
RS2::ActionType RS_ActionInterface::rtti()  {
    return type;
}
*/

/**
 * @return name of this action
 */
/*
RS_String RS_ActionInterface::getName() {
    return name;
}
*/
    
    
/**
 * Called only once in the beginning of an action to initialize
 * stuff.
 */
void RS_ActionInterface::init() {
    /*
    TODO
    RS_String cmd = RS_GuiAction::getCommandForAction(rtti());
    if (!cmd.isEmpty()) {
        RS_DIALOGFACTORY->commandMessage(cmd);
    }
    */
}
    

/**
 * Called to initialize an action to a new status.
 *
 * @param status The status on which to initialize this action.
 * default is 0 to begin the action.
 */
void RS_ActionInterface::init(int status) {
    RS_DEBUG->print("RS_ActionInterface::init");
    initSnapper();
    
    setStatus(status);
    if (status>=0) {
        updateMouseButtonHints();
        updateMouseCursor();
        updateToolBar();
    }
    else {
        //graphicView->redraw();
        updateToolBar();
    }
    
    clearPreview();
    
    RS_DEBUG->print("RS_ActionInterface::init: OK");
}



/**
 * Called when the mouse moves and this is the current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation keeps track of the mouse position.
 */
void RS_ActionInterface::mouseMoveEvent(RS_MouseEvent*) {}

/**
 * Called when the left mouse button is pressed and this is the 
 * current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::mousePressEvent(RS_MouseEvent*) {}

/**
 * Called when the left mouse button is released and this is 
 * the current action. 
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::mouseReleaseEvent(RS_MouseEvent*) {}

/**
 * Called when a key is pressed and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::keyPressEvent(RS_KeyEvent* e) {
    e->ignore();
}

/**
 * Called when a key is released and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::keyReleaseEvent(RS_KeyEvent* e) {
    e->ignore();
}
    
    
/**
 * Called when the user starts dragging something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RS_ActionInterface::dragEnterEvent(QDragEnterEvent* e) {
    e->ignore();
}
    

/**
 * Called when the user is dragging something around in the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RS_ActionInterface::dragMoveEvent(QDragMoveEvent* e) {
    e->ignore();
}

/**
 * Called when the user is dropping something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RS_ActionInterface::dropEvent(QDropEvent* e) {
    e->ignore();
}

/**
 * Coordinate event. Triggered usually from a command line.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::coordinateEvent(RS_CoordinateEvent*) {}

/**
 * Called when a command from the command line is launched.
 * and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RS_ActionInterface::commandEvent(RS_CommandEvent*) {}
    

/**
 * @return Only those available commands that start with 'start'.
 * This is used to complete commands with tab.
 */
RS_StringList RS_ActionInterface::getAvailableCommands(const RS_String& start) {
    RS_StringList ret;
    RS_StringList all = getAvailableCommands();

    for (int i=0; i<all.count(); ++i) {
        if (start.isEmpty() || all.at(i).startsWith(start)) {
            ret.append(all.at(i));
        }
    }

    return ret;
}


/**
 * Can be implemented to return the currently available commands
 * that can be triggered from the command line. The default implemetation
 * returns an empty list.
 */
RS_StringList RS_ActionInterface::getAvailableCommands() {
    //RS_StringList cmd = RS_COMMANDS->getCommands();
    //return cmd;
    return RS_StringList();
}

/**
 * Sets the current status (progress) of this action.
 * The default implementation sets the class variable 'status' to the
 * given value and finishes the action if 'status' is negative.
 *
 * @param status Status number. It's up to the action implementor
 *               what the action uses the status for. However, a
 *               negative status number finishes the action. Usually
 *               the status of an action increases for every step
 *               of progress and decreases when the user goes one
 *               step back (i.e. presses the right mouse button).
 */
void RS_ActionInterface::setStatus(int status) {
    this->status = status;
    if (status<0) {
        finish();
        status=0;
    }
    else {
        updateMouseButtonHints();
        updateToolBar();
        updateMouseCursor();
        if (status!=0) {
            graphicView->emulateMouseMoveEvent();
        }
    }
}

/**
 * @return Current status of this action.
 */
int RS_ActionInterface::getStatus() {
    return status;
}

/**
 * Triggers this action. This should be called after all
 * data needed for this action was collected / set.
 * The default implementation does nothing.
 */
void RS_ActionInterface::trigger() {
    //clearPreview();
    //deletePreview();
}

/**
 * Should be overwritten to update the mouse button hints
 * wherever they might needed.
 */
void RS_ActionInterface::updateMouseButtonHints() {}

/**
 * Should be overwritten to set the mouse cursor for this action.
 */
void RS_ActionInterface::updateMouseCursor() {}

/**
 * Should be overwritten to set the toolbar for this action.
 */
void RS_ActionInterface::updateToolBar() {}

/**
 * @return true, if the action is finished and can be deleted.
 */
bool RS_ActionInterface::isFinished() {
    return finished;
}


/**
 * Forces a termination of the action without any cleanup.
 */
void RS_ActionInterface::setFinished() {
    status = -1;
}


/**
 * Finishes this action.
 */
void RS_ActionInterface::finish() {
    RS_DEBUG->print("RS_ActionInterface::finish");
    status = -1;
    finished = true;
    graphicView->setMouseCursor(RS2::ArrowCursor);
    updateToolBar();
    //updateMouseButtonHints();
    RS_DIALOGFACTORY->updateMouseWidget("", "");
    hideOptions();
    RS_Snapper::finish();
    
    clearPreview();
    //deletePreview();
    RS_DEBUG->print("RS_ActionInterface::finish: OK");
}

/**
 * Called by the event handler to give this action a chance to
 * communicate with its predecessor.
 */
void RS_ActionInterface::setPredecessor(RS_ActionInterface* pre) {
    predecessor = pre;
}

/**
 * Suspends this action while another action takes place.
 */
void RS_ActionInterface::suspend() {
    graphicView->setMouseCursor(RS2::ArrowCursor);
    RS_Snapper::suspend();
}



/**
 * Resumes an action after it was suspended.
 */
void RS_ActionInterface::resume() {
    updateMouseCursor();
    updateMouseButtonHints();
    updateToolBar();
    RS_Snapper::resume();
}



/**
 * Hides the tool options. Default implementation does nothing.
 */
void RS_ActionInterface::hideOptions() {
    RS_DEBUG->print("RS_ActionInterface::hideOptions");
    RS_Snapper::hideSnapOptions();
    RS_DIALOGFACTORY->requestOptions(this, false);
    RS_DEBUG->print("RS_ActionInterface::hideOptions: OK");
}



/**
 * Shows the tool options. Default implementation does nothing.
 */
void RS_ActionInterface::showOptions() {
    RS_Snapper::showSnapOptions();
    RS_DIALOGFACTORY->requestOptions(this, true, optionsFromAction());
}
    
    

/**
 * @return True: the options widget is updated with the value
 * of this action on initialisation. False: the options widget
 * is updated from the config file (default).
 */
bool RS_ActionInterface::optionsFromAction() {
    return false;
}



/**
 * Implementations must create an option widget that allows the user
 * to change the options of this action.
 */
/*
RS_Widget* RS_ActionInterface::createOptionWidget() {
    return NULL;
}
*/



/**
 * Snap to a coordinate in the drawing using the current snap mode.
 *
 * @param e A mouse event.
 * @return The coordinates of the point or an invalid vector.
 */
RS_Vector RS_ActionInterface::snapPoint(RS_MouseEvent* e) {
    snapSpot = RS_Vector(false);

    if (e==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_Snapper::snapPoint: event is NULL");
        return snapSpot;
    }

    return snapPoint(e->x(), e->y());
}



/**
 * Snap to a point and add snap marker to preview.
 */
RS_Vector RS_ActionInterface::snapPoint(int x, int y) {
    RS_Vector ret = RS_Snapper::snapPoint(x, y);
    //preview->setSnapMarker((RS_Snapper&)(*this), 
    //    graphicView->getFactor().x);
    double factor = graphicView->getFactor().x;
    
    if (!finished && snapSpot.valid) {
        RS_EntityContainer* snapMarker = new RS_EntityContainer(preview);
        snapMarker->setLayer(NULL);
        if (snapCoord.valid) {
            // snap point visualization:
            RS_Circle* c = new RS_Circle(snapMarker, RS_CircleData(snapCoord, 4/factor));
            c->setPen(snapperPen);
            c->setLayer(NULL);
            snapMarker->addEntity(c);
                    
            RS_SETTINGS->beginGroup("/Appearance");
            RS_String lang = RS_SETTINGS->readEntry("/Language", "");
            RS_SETTINGS->endGroup();

            // show small text to indicate what point we snap to:
            if (snapMode==RS2::ActionSnapAuto && showAutoSnapAid==true) {
                RS_String snapLabel;
                bool inEnglish = false;
                // for these translations we don't have the glyphs in font
                //   'standard' to show these labels localized:
                if (lang=="zh" || lang=="el" || lang=="ru" || lang=="ja" || lang=="bg") {
                    inEnglish = true;
                }
                switch (snapStatus) {
                case RS2::ActionSnapEndpoint:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("End");
                    }
                    else {
                        snapLabel = "End";
                    }
                    break;
                case RS2::ActionSnapGrid:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("Grid");
                    }
                    else {
                        snapLabel = "Grid";
                    }
                    break;
                case RS2::ActionSnapIntersection:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("Intersection");
                    }
                    else {
                        snapLabel = "Intersection";
                    }
                    break;
                case RS2::ActionSnapOnEntity:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("On Entity");
                    }
                    else {
                        snapLabel = "On Entity";
                    }
                    break;
                case RS2::ActionSnapMiddle:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("Middle");
                    }
                    else {
                        snapLabel = "Middle";
                    }
                    break;
                case RS2::ActionSnapReference:
                    if (!inEnglish) {
                        snapLabel = QObject::tr("Reference");
                    }
                    else {
                        snapLabel = "Reference";
                    }
                    break;
                default:
                    break;
                }

                if (!snapLabel.isEmpty()) {
                    RS_String font = "standard";
                    RS_Text* t = new RS_Text(snapMarker, 
                        RS_TextData(snapCoord + 
                            RS_Vector(graphicView->toGraphDX(8), graphicView->toGraphDY(-8)),
                            graphicView->toGraphDY(10),
                            100.0,
                            RS2::VAlignTop,
                            RS2::HAlignLeft,
                            RS2::LeftToRight,
                            RS2::Exact,
                            1.0,
                            snapLabel,
                            font,
                            0.0));
                    t->setPen(snapperPen);
                    t->setLayer(NULL);
                    t->update();
                    RS_Text* ty = t;

                    t = dynamic_cast<RS_Text*>(t->clone());
                    t->move(RS_Vector(graphicView->toGraphDX(1), graphicView->toGraphDY(-1)));
                    t->setPen(RS_Pen(RS_Color(1,1,1), RS2::Width00, RS2::SolidLine));
                    t->update();
                    snapMarker->addEntity(t);
                    
                    snapMarker->addEntity(ty);
                }
            }
            
            // crosshairs:
            if (showCrosshairs==true) {
                RS_Pen chPen = snapperPen;
                chPen.setLineType(RS2::DashLine);
                
                double left = graphicView->toGraphX(0);
                double right = graphicView->toGraphX(graphicView->getWidth());
                double top = graphicView->toGraphY(0);
                double bottom = graphicView->toGraphY(graphicView->getHeight());
                    
                RS_Line* l;
                l = new RS_Line(snapMarker, 
                        RS_LineData(snapCoord,
                            RS_Vector(right, snapCoord.y)));
                l->setPen(chPen);
                l->setLayer(NULL);
                snapMarker->addEntity(l);
                
                l = new RS_Line(snapMarker, 
                        RS_LineData(snapCoord,
                            RS_Vector(left, snapCoord.y)));
                l->setPen(chPen);
                l->setLayer(NULL);
                snapMarker->addEntity(l);
                
                l = new RS_Line(snapMarker, 
                        RS_LineData(snapCoord,
                            RS_Vector(snapCoord.x, top)));
                l->setPen(chPen);
                l->setLayer(NULL);
                snapMarker->addEntity(l);
                
                l = new RS_Line(snapMarker, 
                        RS_LineData(snapCoord,
                            RS_Vector(snapCoord.x, bottom)));
                l->setPen(chPen);
                l->setLayer(NULL);
                snapMarker->addEntity(l);
            }
        }
        if (snapCoord.valid && snapCoord!=snapSpot) {
            RS_Line* l;
            l = new RS_Line(snapMarker, 
                    RS_LineData(snapSpot+RS_Vector(-5,0)/factor,
                                snapSpot+RS_Vector(-1,4)/factor));
            l->setPen(snapperPen);
            l->setLayer(NULL);
            snapMarker->addEntity(l);
            l = new RS_Line(snapMarker, 
                    RS_LineData(snapSpot+RS_Vector(0,5)/factor,
                                snapSpot+RS_Vector(4,1)/factor));
            l->setPen(snapperPen);
            l->setLayer(NULL);
            snapMarker->addEntity(l);
            l = new RS_Line(snapMarker, 
                    RS_LineData(snapSpot+RS_Vector(5,0)/factor,
                                snapSpot+RS_Vector(1,-4)/factor));
            l->setPen(snapperPen);
            l->setLayer(NULL);
            snapMarker->addEntity(l);
            l = new RS_Line(snapMarker, 
                    RS_LineData(snapSpot+RS_Vector(0,-5)/factor,
                                snapSpot+RS_Vector(-4,-1)/factor));
            l->setPen(snapperPen);
            l->setLayer(NULL);
            snapMarker->addEntity(l);
        }

        preview->setSnapMarker(snapMarker);
    }
    return ret;
}



/**
 * Clears the preview by removing all entities in it.
 */
void RS_ActionInterface::clearPreview() {
    preview->clear();
}



/**
 * Draws the preview on the screen.
 */
void RS_ActionInterface::drawPreview() {
    if (!preview->isEmpty()) {
        RS_DEBUG->print("RS_ActionInterface::drawPreview");
        preview->moveSnapMarkerToFront();
        graphicView->redrawPreview();
        graphicView->destroyPainter();
        RS_DEBUG->print("RS_ActionInterface::drawPreview: OK");
    }
    else {
        deletePreview();
    }
}



/**
 * Deletes the preview from the screen.
 */
void RS_ActionInterface::deletePreview() {
    graphicView->restore();
}

