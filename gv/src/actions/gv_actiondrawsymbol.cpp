/************************************************************
 * $Id: gv_actiondrawsymbol.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 16, 2008 1:22:32 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionblocksinsert.cpp 10309
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiondrawsymbol.h"

#include "gv_dialogfactory.h"
#include "gv_gvlegend.h"
#include "gv_legendwidget.h"
//#include "gv_pipeline.h"
//#include "gv_pipelinedialog.h"


/**
 * Action for drawing a symbol
 */
GV_ActionDrawSymbol::GV_ActionDrawSymbol(GV_Drawing& dwg, GV_GraphicsView& gv)
		:GV_ActionBase(dwg, gv) {
	RB_DEBUG->print("GV_ActionDrawSymbol::GV_ActionDrawSymbol()");
    mLegendSymbol = NULL;
	mInsertionPoint = RVector();
}

/**
 * Destructor
 */
GV_ActionDrawSymbol::~GV_ActionDrawSymbol() {
    // nothing
	RB_DEBUG->print("GV_ActionDrawSymbol::~GV_ActionDrawSymbol() OK");
}

RB_GuiAction* GV_ActionDrawSymbol::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionDrawSymbol::getName(),
                                  QIcon(":/images/icons/shape_square_add.png"),
                                  tr("Draw &Symbol"));
        action->setStatusTip(tr("Draws selected symbol"));
        action->setCommand("ds");
        action->setKeycode("ds");
        action->setFactory(GV_ActionDrawSymbol::factory);
    }
    return action;
}

void GV_ActionDrawSymbol::init(int status) {
	RB_DEBUG->print("GV_ActionDrawSymbol::init()");
	
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

RB_Action* GV_ActionDrawSymbol::factory() {
	RB_DEBUG->print("GV_ActionDrawSymbol::factory()");
	
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
    
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionDrawSymbol(*drawing, *graphicsView);
		a->setSelectedComponent(drawing->getSelectedComponent()); // for drop
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

/*
void GV_ActionDrawSymbol::reset() {
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


void GV_ActionDrawSymbol::trigger() {
	RB_DEBUG->print("GV_ActionDrawSymbol::trigger()");

    // get current selected component first otherwise selComp = symbol in connectTo()
    GV_Component* selComp = getSelectedComponent();
    // add symbol (will set mSymbol which is returned by getSelectedComponent())
    GV_Symbol* symbol = addNewSymbol(mLegendSymbol);
    // set symbol at position of click or drop event
    dynamic_cast<GV_Component*>(symbol)->setPos(QPointF(mInsertionPoint.x, mInsertionPoint.y));

    /* TODO: rewrite with getPort()
	if (selComp) {
		// pre-rotate symbol according selected component
		GV_Node* selNode = selComp->origineNode();
        dynamic_cast<GV_Component*>(symbol)->rotate(selNode->getRotation());
		// and connect symbol if selected component
		GV_CoordinateEvent ce(mInsertionPoint);
		connectSymbolTo(&ce, symbol, selComp);
    }
*/
	getDrawing()->clearSelection();
    symbol->setSelected(true);

    finish();
	getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
	RB_DEBUG->print("GV_ActionDrawSymbol::trigger() OK");
}


void GV_ActionDrawSymbol::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
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
 * A symbol is dropped on the drawing
 * @param e QGraphicsSceneDragDropEvent
 */
void GV_ActionDrawSymbol::dropEvent(QGraphicsSceneDragDropEvent* e) {
	RB_DEBUG->print("GV_ActionDrawSymbol::dropEvent()");

	// use void* pointer
	QVariant var = e->mimeData()->imageData();
	void* ptr = var.value<void*>();
    mLegendSymbol = static_cast<GV_Symbol*>(ptr);

	// create coordinate event and trigger action
    GV_CoordinateEvent ce(snapPoint(e));
    
    if (ce.isValid()) {
        coordinateEvent(&ce);
    } else {
        finish();
    }
}


void GV_ActionDrawSymbol::coordinateEvent(GV_CoordinateEvent* e) {
	if (e == NULL) {
        return;
    }

    mInsertionPoint = e->getCoordinate();
    trigger();
}



void GV_ActionDrawSymbol::commandEvent(GV_CommandEvent* e) {
    RB_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        GV_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    // Refer to drawInsert
    // ...
}



RB_StringList GV_ActionDrawSymbol::getAvailableCommands() {
    RB_StringList cmd = GV_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetTargetPoint:
        cmd += command("angle");
        cmd += command("factor");
        cmd += command("columns");
        cmd += command("rows");
        cmd += command("columnspacing");
        cmd += command("rowspacing");
        break;
    default:
        break;
    }

    return cmd;
}

/*

void GV_ActionDrawSymbol::updateMouseButtonHints() {
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

void GV_ActionDrawSymbol::updateMouseCursor() {
    // graphicView->setMouseCursor(RS2::CadCursor);
	getGraphicsView()->setMouseCursor(Qt::CrossCursor);
}

/*

void GV_ActionDrawSymbol::updateToolBar() {
    if (!isFinished()) {
        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}
*/
