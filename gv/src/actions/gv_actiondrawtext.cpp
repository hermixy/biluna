/************************************************************
 * $Id: gv_actiondrawtext.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: May 17, 2010 1:22:32 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiondrawtext.h"

#include "gv_dialogfactory.h"
#include "gv_gvlegend.h"
#include "gv_legendwidget.h"


/**
 * Action for drawing a text
 */
GV_ActionDrawText::GV_ActionDrawText(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {
    RB_DEBUG->print("GV_ActionDrawText::GV_ActionDrawText()");
//    mLegendSymbol = NULL;
    mInsertionPoint = RVector();
}

/**
 * Destructor
 */
GV_ActionDrawText::~GV_ActionDrawText() {
    // nothing
    RB_DEBUG->print("GV_ActionDrawText::~GV_ActionDrawText() OK");
}

RB_GuiAction* GV_ActionDrawText::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionDrawText::getName(),
                                  QIcon(":/images/icons/font_add.png"),
                                  tr("Draw &Text"));
        action->setStatusTip(tr("Draw text"));
        action->setCommand("dt");
        action->setKeycode("dt");
        action->setFactory(GV_ActionDrawText::factory);
    }
    return action;
}

void GV_ActionDrawText::init(int status) {
    RB_DEBUG->print("GV_ActionDrawText::init()");

    GV_ActionBase::init(status);

//    reset();

//    if (graphic!=NULL) {
//        block = graphic->getActiveBlock();
//        if (block!=NULL) {
//            data.name = block->getName();
//        } else {
//            finish();
//        }
//    }
}

RB_Action* GV_ActionDrawText::factory() {
    RB_DEBUG->print("GV_ActionDrawText::factory()");

    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionDrawText(*drawing, *graphicsView);
        a->setSelectedComponent(drawing->getSelectedComponent()); // for drop
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

/*
void GV_ActionDrawText::reset() {
    data = GV_InsertData("",
                         RVector(0.0,0.0),
                         RVector(1.0,1.0),
                         0.0,
                         1, 1,
                         RVector(1.0,1.0),
                         NULL,
                         RS2::Update);
}
*/


void GV_ActionDrawText::trigger() {
    RB_DEBUG->print("GV_ActionDrawText::trigger()");

    // get current selected component first otherwise selComp = text in connectTo()
    GV_Component* selComp = getSelectedComponent();
    // add text (will set mSymbol which is returned by getSelectedComponent())
    GV_Text* text = addNewText(mLegendText);
    // set text at position of click or drop event
    text->setPos(QPointF(mInsertionPoint.x, mInsertionPoint.y));

    if (selComp) {
        // pre-rotate texy according selected component
        GV_Node* selNode = selComp->origineNode();
        dynamic_cast<GV_Component*>(text)->rotate(selNode->getRotation());
        // and connect text if selected component
        GV_CoordinateEvent ce(mInsertionPoint);
//        connectSymbolTo(&ce, text, selComp);
    }

    getDrawing()->clearSelection();
    text->setSelected(true);

    finish();
    getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
    RB_DEBUG->print("GV_ActionDrawText::trigger() OK");
}


void GV_ActionDrawText::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        GV_CoordinateEvent ce(snapPoint(e));

        if (ce.isValid()) {
            coordinateEvent(&ce);
        }

    } else if (e->button()==Qt::RightButton) {
        init(getStatus()-1);
    }

//    GV_ActionBase::mouseReleaseEvent(e); was not in working code
}

/**
 * A text is dropped on the drawing
 * @param e QGraphicsSceneDragDropEvent
 */
//void GV_ActionDrawText::dropEvent(QGraphicsSceneDragDropEvent* e) {
//	RB_DEBUG->print("GV_ActionDrawText::dropEvent()");
//
//	// use void* pointer
//	QVariant var = e->mimeData()->imageData();
//	void* ptr = var.value<void*>();
//    mLegendSymbol = static_cast<GV_Symbol*>(ptr);
//
//	// create coordinate event and trigger action
//    GV_CoordinateEvent ce(snapPoint(e));
//
//    if (ce.isValid()) {
//        coordinateEvent(&ce);
//    } else {
//        finish();
//    }
//}


void GV_ActionDrawText::coordinateEvent(GV_CoordinateEvent* e) {
    if (e == NULL) {
        return;
    }

    mInsertionPoint = e->getCoordinate();
    trigger();
}



void GV_ActionDrawText::commandEvent(GV_CommandEvent* e) {
    RB_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        GV_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    // Refer to drawInsert
    // ...
}



RB_StringList GV_ActionDrawText::getAvailableCommands() {
    RB_StringList cmd = GV_ActionBase::getAvailableCommands();

    switch (getStatus()) {
//    case SetTargetPoint:
//        cmd += command("angle");
//        cmd += command("factor");
//        cmd += command("columns");
//        cmd += command("rows");
//        cmd += command("columnspacing");
//        cmd += command("rowspacing");
//        break;
    default:
        break;
    }

    return cmd;
}

/*

void GV_ActionDrawText::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetTargetPoint:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetAngle:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter angle:"),
                                            "");
        break;
    case SetFactor:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter factor:"),
                                            "");
        break;
    case SetColumns:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter columns:"),
                                            "");
        break;
    case SetRows:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter rows:"),
                                            "");
        break;
    case SetColumnSpacing:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter column spacing:"),
                                            "");
        break;
    case SetRowSpacing:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Enter row spacing:"),
                                            "");
        break;
    default:
        GV_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}

*/

void GV_ActionDrawText::updateMouseCursor() {
    // graphicView->setMouseCursor(RS2::CadCursor);
    getGraphicsView()->setMouseCursor(Qt::CrossCursor);
}

/*

void GV_ActionDrawText::updateToolBar() {
    if (!isFinished()) {
        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}
*/
