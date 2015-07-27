/*****************************************************************
 * $Id: gv_actiondrawline.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 5, 2008 4:27:41 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actiondrawline.cpp 10309
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiondrawline.h"

#include "gv_actioneditundo.h"
#include "gv_gvlegend.h"
//#include "gv_pipeline.h"


/**
 * Action for drawing a line
 */
GV_ActionDrawLine::GV_ActionDrawLine(GV_Drawing& dwg, GV_GraphicsView& gv)
        		: GV_ActionBase(dwg, gv) {

	RB_DEBUG->print("GV_ActionDrawLine::GV_ActionDrawLine()");
    reset();

    //history.setAutoDelete(true);

    mNewLine = NULL;
}

/**
 * Destructor
 */
GV_ActionDrawLine::~GV_ActionDrawLine() {
	RB_DEBUG->print("GV_ActionDrawLine::~GV_ActionDrawLine() OK");
}

RB_GuiAction* GV_ActionDrawLine::createGuiAction() {
    static RB_GuiAction* action = NULL;
    
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionDrawLine::getName(),
                             "linesnormal",
                             tr("&2 Points"));
        action->setStatusTip(tr("Draw lines"));
        action->setCommand("line,ln,li,l");
        action->setKeycode("li");
        action->setFactory(GV_ActionDrawLine::factory);
    }

    return action;
}



void GV_ActionDrawLine::init(int status) {
    RB_DEBUG->print("GV_ActionDrawLine::init");
    GV_ActionBase::init(status);
    reset();
}

/**
 * Create action and set as current drawing action,
 * is a static function
 */
RB_Action* GV_ActionDrawLine::factory() {
	RB_DEBUG->print("GV_ActionDrawLine::factory()");
	
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionDrawLine(*drawing, *graphicsView);
		a->setSelectedComponent(drawing->getSelectedComponent()); // for drop
		drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void GV_ActionDrawLine::reset() {
	RB_DEBUG->print("GV_ActionDrawLine::reset");
//    data = RS_LineData(RVector(false), RVector(false));
	// TODO


        start = RVector();
	history.clear();
	RB_DEBUG->print("GV_ActionDrawLine::reset: OK");
}


/**
 * Trigger action after selection of start and end point selection of line
 */
void GV_ActionDrawLine::trigger() {
	RB_DEBUG->print("GV_ActionDrawLine::trigger()");
	
    // GV_ActionBase::trigger(); does nothing
    
	// TODO: what could be the function of this trigger now with GV_Scene? finish!
	// -> for example undo update

    
	finish();
	getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
}

/**
 * Mouse move event, only for placing Port1 (second point)
 */
void GV_ActionDrawLine::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    // RB_DEBUG->print("GV_ActionDrawLine::mouseMoveEvent begin");

    if (mNewLine != 0) {
        RVector mouse = snapPoint(e);
        GV_CoordinateEvent ce(mouse);
        mNewLine->port1()->updatePosition(&ce);
    }  

	GV_ActionBase::mouseMoveEvent(e);
}

/**
 * Mouse presse event will select an item if applicable and set the
 * relevant pipe line or equipment
 * @param e mouse scene event
 */
void GV_ActionDrawLine::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    // RB_DEBUG->print("GV_ActionDrawLine::mousePressEvent()");
    GV_ActionBase::mousePressEvent(e);
}

/**
 * Mouse release event will set the starting point or end point of the line
 * @param e mouse scene event
 */
void GV_ActionDrawLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
	RB_DEBUG->print("GV_ActionDrawLine::mouseReleaseEvent");
	
    if (e->button() == Qt::LeftButton) {
        GV_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (e->button() == Qt::RightButton) {
		// line was already created
        removeComponent(mNewLine);
        mNewLine = NULL;
        init(getStatus()-1);
    }
    
    GV_ActionBase::mouseReleaseEvent(e);
}

/**
 * A line is dropped on the drawing
 * @param e QGraphicsSceneDragDropEvent
 */
void GV_ActionDrawLine::dropEvent(QGraphicsSceneDragDropEvent* e) {
	RB_DEBUG->print("GV_ActionDrawLine::dropEvent()");

	// NOTE: static_cast is danger because no compiler check
	QVariant var = e->mimeData()->imageData();
	GV_Line* legendLine = static_cast<GV_Line*>(var.value<void*>());

	if (!legendLine) {
		RB_DEBUG->print(RB_Debug::D_ERROR,
						"GV_ActionDrawLine::trigger() no legend line ERROR");
		return;
	}

	// get current selected component first otherwise selComp = symbol in
	// connectTo()
	GV_Component* selComp = getSelectedComponent();
	// add line (will set mSymbol which is returned by getSelectedComponent())
    mNewLine = addNewLine(legendLine);
	// set line at position of click or drop event
	GV_CoordinateEvent ce(snapPoint(e));
	QPointF p0(ce.pos());
	QPointF p1(ce.pos().x() + legendLine->line().length(), ce.pos().y());
    mNewLine->setLine(QLineF(p0, p1));

/* TODO: rewrite with getPort()

	// connect symbol to a component if selected
	if (selComp) {
        connectComponent(&ce, mNewLine);
		// check which port is not connected and connected
        if (mNewLine->port0()->connectNode() && !mNewLine->port1()->connectNode()) {
			// check node for direction
            GV_Node* tmpNode = mNewLine->port0()->connectNode();
			GV_Component* comp = tmpNode->parentComponent();
			GV_Node* node = comp->port0()->connectNode();
			node->synchronizeObjectData();
			RVector dir = node->direction();
			dir.normalize();
			dir *= legendLine->line().length();

			// port0 is rotation point and remains at position
            QPointF tp0 = mNewLine->point0();
			QLineF tmpLine(tp0, QPointF(tp0.x() + dir.x, tp0.y() + dir.y));
            mNewLine->setLine(tmpLine);
        } else if (mNewLine->port1()->connectNode()
                    && mNewLine->port0()->connectNode() / * origine node * /) {
			// check node for direction
            GV_Node* node = mNewLine->port1()->connectNode();
			node->synchronizeObjectData();
			RVector dir = node->direction();
			dir.normalize();
			dir *= legendLine->line().length();

			// port0 is rotation point and remains at position
            QPointF tp1 = mNewLine->point1();
			QPointF tp0 = QPointF(tp1.x() - dir.x, tp1.y() - dir.y);
			QLineF tmpLine(tp0, tp1);
            mNewLine->setReverseOrder(false); // or tp0 and tp1 to be reversed
			// TODO: improve programming with reverseOrder, the reverseOrder
			// is only required for drawing a line by hand not for the drop
			// action. Also related to idea of removing the parent and connect
			// identification of ports in a GV_Node
            mNewLine->setLine(tmpLine);
		}
	}
*/

//	mNewLine->updatePosition(); not required for GV_Line
    mNewLine->setReverseOrder(false);
	getDrawing()->clearSelection();
    mNewLine->setSelected(true);

	finish();
	getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
	RB_DEBUG->print("GV_ActionDrawLine::dropEvent() OK");
}


void GV_ActionDrawLine::coordinateEvent(GV_CoordinateEvent* ce) {
    RB_DEBUG->print("GV_ActionDrawLine::coordinateEvent()");
    
    if (ce == NULL) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_ActionDrawLine::coordinateEvent: events are NULL");
        return;
    }

    RVector mouse;
    
    if (ce) {
	    mouse = ce->getCoordinate();
	    RB_DEBUG->print("GV_ActionDrawLine::coordinateEvent: "
	    		"mouse: %f/%f. valid: %d", 
	    		mouse.x, mouse.y, mouse.valid);
    }
    
    switch (getStatus()) {
    case SetStartpoint: {
        history.clear();
        history.append(mouse);
        setStatus(SetEndpoint);
        updateMouseButtonHints();
        
        mNewLine = addNewLine();
        GV_CoordinateEvent ce(mouse);
        mNewLine->port0()->updatePosition(&ce);
        mNewLine->port1()->updatePosition(&ce);
        mNewLine->port0()->connect();
        getDrawing()->clearSelection();
        mNewLine->setSelected(true);
        break;
    }
    case SetEndpoint:
        history.append(mouse);
        trigger();
        updateMouseButtonHints();
        
        if (mNewLine != 0) {
            GV_CoordinateEvent ce(mouse);
            mNewLine->port1()->updatePosition(&ce);
            mNewLine->port1()->connect();
            mNewLine->updatePosition();
            mNewLine->setReverseOrder(false);
        }
        
        break;
    default:
        break;
    }
}



void GV_ActionDrawLine::commandEvent(GV_CommandEvent* e) {
    RB_DEBUG->print("GV_ActionDrawLine::commandEvent()");
    
    RB_String c = e->getCommand().toLower();

    switch (getStatus()) {
    case SetStartpoint:
        if (checkCommand("help", c)) {
            GV_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
            e->accept();
            return;
        }
        break;

    case SetEndpoint:
        if (checkCommand("close", c)) {
            close();
            updateMouseButtonHints();
            e->accept();
            return;
        }

        if (checkCommand("undo", c)) {
            undo();
            updateMouseButtonHints();
            e->accept();
            return;
        }
        break;

    default:
        break;
    }
}



RB_StringList GV_ActionDrawLine::getAvailableCommands() {
	RB_DEBUG->print("GV_ActionDrawLine::getAvailableCommands()");
	
    RB_StringList cmd = GV_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetStartpoint:
        break;
    case SetEndpoint:
        if (history.count()>=2) {
            cmd += command("undo");
        }
        if (history.count()>=3) {
            cmd += command("close");
        }
        break;
    default:
        break;
    }

    return cmd;
}



void GV_ActionDrawLine::updateMouseButtonHints() {
	RB_DEBUG->print("GV_ActionDrawLine::updateMouseButtonHints()");
	
    switch (getStatus()) {
    case SetStartpoint:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify first point"),
                                            tr("Cancel"));
        break;
    case SetEndpoint: {
            RB_String msg = "";

            if (history.count()>=3) {
                msg += command("close");
                msg += "/";
            }
            if (history.count()>=2) {
                msg += command("undo");
            }

            if (history.count()>=2) {
                GV_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next point or [%1]").arg(msg),
                    tr("Back"));
            } else {
                GV_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next point"),
                    tr("Back"));
            }
        }
        break;
    default:
        GV_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void GV_ActionDrawLine::updateMouseCursor() {
	getGraphicsView()->setMouseCursor(Qt::CrossCursor);
}


void GV_ActionDrawLine::updateToolBar() {
/*    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
*/
}

void GV_ActionDrawLine::close() {
/*    if (history.count()>2 && start.valid) {
        data.endpoint = start;
        trigger();
        setStatus(SetStartpoint);
        graphicView->moveRelativeZero(start);
        graphicView->restore();
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot close sequence of lines: "
               "Not enough entities defined yet."));
    }
*/
}

void GV_ActionDrawLine::undo() {
/*    if (history.count()>1) {
        history.removeLast();
        clearPreview();
        //graphicView->setCurrentAction(
        //    new RS_ActionEditUndo(true, *container, *graphicView));
        RS_ActionEditUndo::factory();
        data.startpoint = history.last();
        graphicView->moveRelativeZero(data.startpoint);
        graphicView->restore();
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot undo: "
               "Not enough entities defined yet."));
    }
*/
}

