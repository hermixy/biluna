/*****************************************************************
 * $Id: gv_actiondeleteitem.cpp 1198 2010-06-13 12:06:07Z rutger $
 * Created: Feb 11, 2009 9:13:46 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actiondeleteitem.h"

#include "db_dialogfactory.h"
#include "gv_actioneditundo.h"
#include "gv_graphicsscene.h"



GV_ActionDeleteItem::GV_ActionDeleteItem(GV_Drawing& dwg, GV_GraphicsView& gv)
					: GV_ActionBase(dwg, gv) {
	RB_DEBUG->print("GV_ActionDeleteItem::GV_ActionDeleteItem()");
	
}

GV_ActionDeleteItem::~GV_ActionDeleteItem() {
    RB_DEBUG->print("GV_ActionDeleteItem::~GV_ActionDeleteItem() OK");
}

RB_GuiAction* GV_ActionDeleteItem::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action == NULL) {
        action = new RB_GuiAction(GV_ActionDeleteItem::getName(),
                             QIcon(":/images/icons/shape_square_delete.png"),
                             tr("&Delete Item"));
        action->setStatusTip(tr("Delete Item"));
        action->setCommand("del,di,d");
        action->setKeycode("di");
        action->setFactory(GV_ActionDeleteItem::factory);
    }

    return action;
}

void GV_ActionDeleteItem::init(int status) {
    GV_ActionBase::init(status); // does nothing
    trigger();
}

RB_Action* GV_ActionDeleteItem::factory() {
    RB_DEBUG->print("GV_ActionDeleteItem::factory()");

    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionDeleteItem(*drawing, *graphicsView);
        a->setSelectedComponent(drawing->getSelectedComponent()); // for delete
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void GV_ActionDeleteItem::trigger() {
    RB_DEBUG->print("GV_ActionDeleteItem::trigger()");

    int answer = DB_DIALOGFACTORY->requestYesNoCancelDialog(tr("Delete Item"),
                           tr("Do you want to delete\r the selected item?"));
    if (answer == QMessageBox::Yes) {
        GV_GraphicsScene* gs = dynamic_cast<GV_GraphicsScene*>(getGraphicsView()->scene());
        gs->killAllActions();
    } else if (answer == QMessageBox::No) {
        GV_GraphicsScene* gs = dynamic_cast<GV_GraphicsScene*>(getGraphicsView()->scene());
        gs->killAllActions();
        finish();
        return;
    } else if (answer == QMessageBox::Cancel) {
        finish();
        return;
    }

    GV_Component* comp = this->getSelectedComponent();
    if (comp) {
        this->removeComponent(comp);
    }

    finish();
    getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
    RB_DEBUG->print("GV_ActionDeleteItem::trigger() OK");
}
