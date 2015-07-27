/*****************************************************************
 * $Id: gv_actioneditcopy.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 4, 2008 6:14:49 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcopy.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditcopy.h"
/*
#include "rs_modification.h"
#include "rs_clipboard.h"

#include <QPixmap>
*/
/**
 * Constructor.
 * @param undo true for undo and false for redo.
 */
GV_ActionEditCopy::GV_ActionEditCopy(GV_Drawing& dwg, GV_GraphicsView& gv,
                                     bool copy, bool autoRef)
                    : GV_ActionBase(dwg, gv) {
    this->copy = copy;
    this->autoRef = autoRef;
    referencePoint = RVector();
}


GV_ActionEditCopy::~GV_ActionEditCopy() {}



RB_GuiAction* GV_ActionEditCopy::createGuiAction() {
    
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditCopy::getName(),
                	"editcopyref",
                	tr("&Copy with reference"));
        action->setStatusTip(tr("Copies entities to the clipboard with a reference point"));
        action->setShortcut("Shift+Ctrl+C");
        action->setCommand("copyref,rc");
        action->setKeycode("rc");
        action->setFactory(GV_ActionEditCopy::factory);
    }

    return action;
}


RB_Action* GV_ActionEditCopy::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* action = 
//            new RS_ActionEditCopy(*container, *graphicView, true, false);
//        graphicView->setCurrentAction(action);
//        return action;
//    }

    return NULL;
}


/*
void RS_ActionEditCopy::init(int status) {
    RS_ActionBase::init(status);
    if (autoRef) {
        trigger();
    }
}



void RS_ActionEditCopy::trigger() {

    clearPreview();
    graphicView->restore();

    if (autoRef) {
        RS_Vector min(RS_MAXDOUBLE, RS_MAXDOUBLE);
        RS_Vector max(RS_MINDOUBLE, RS_MINDOUBLE);
        for (RS_Entity* e=container->firstEntity(); e!=NULL; e=container->nextEntity()) {
            if (!e->isUndone() && e->isSelected()) {
                min = RS_Vector::minimum(min, e->getMin());
                max = RS_Vector::maximum(max, e->getMax());
            }
        }
        referencePoint = (min + max) / 2.0;
        RS_DEBUG->print("RS_ActionEditCopy::trigger: "
            "reference point: %f/%f", referencePoint.x, referencePoint.y);
    }

    RS_Modification m(*container, graphicView);
    m.copy(referencePoint, !copy);

    graphicView->redraw();
    finish();
    //graphicView->killSelectActions();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}


void RS_ActionEditCopy::mouseMoveEvent(RS_MouseEvent* e) {
    clearPreview();
    snapPoint(e);
    drawPreview();
}



void RS_ActionEditCopy::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deleteSnapper();
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionEditCopy::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    referencePoint = e->getCoordinate();
    trigger();
}



void RS_ActionEditCopy::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionEditCopy::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionEditCopy::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}
*/
