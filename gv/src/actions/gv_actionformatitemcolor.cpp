/*****************************************************************
 * $Id: gv_actionformatitemcolor.cpp 1198 2010-06-13 12:06:07Z rutger $
 * Created: June 12, 2010 11:27:41 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionformatitemcolor.h"

#include "gv_actioneditundo.h"


/**
 * Action for setting a line pen
 */
GV_ActionFormatItemColor::GV_ActionFormatItemColor(GV_Drawing& dwg, GV_GraphicsView& gv)
        		: GV_ActionBase(dwg, gv) {

    RB_DEBUG->print("GV_ActionFormatItemColor::GV_ActionFormatItemColor()");
    // reset();

    //history.setAutoDelete(true);

    // mNewLine = NULL;
}

/**
 * Destructor
 */
GV_ActionFormatItemColor::~GV_ActionFormatItemColor() {
    RB_DEBUG->print("GV_ActionFormatItemColor::~GV_ActionFormatItemColor() OK");
}

RB_GuiAction* GV_ActionFormatItemColor::createGuiAction() {
    static RB_GuiAction* ga = NULL;
    
    if (ga==NULL) {
        ga = new RB_GuiAction(GV_ActionFormatItemColor::getName(),
                             QIcon(":/images/icons/paintbrush.png"),
                             tr("&Item Color"));
        ga->setStatusTip(tr("Set item color"));
        ga->setCommand("itemcol,col");
        ga->setKeycode("ic");
        ga->setFactory(GV_ActionFormatItemColor::factory);
    }

    return ga;
}



void GV_ActionFormatItemColor::init(int status) {
    RB_DEBUG->print("GV_ActionFormatItemColor::init");
    GV_ActionBase::init(status);
    trigger(); // not triggered by mouse- or keyboard events
}

/**
 * Create action and set as current drawing action,
 * is a static function
 */
RB_Action* GV_ActionFormatItemColor::factory() {
    RB_DEBUG->print("GV_ActionFormatItemColor::factory()");
	
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionFormatItemColor(*drawing, *graphicsView);
        a->setSelectedComponent(drawing->getSelectedComponent()); // for edit
		drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}


/**
 * Trigger action
 */
void GV_ActionFormatItemColor::trigger() {
    RB_DEBUG->print("GV_ActionFormatItemColor::trigger()");
	
    GV_GraphicsScene* gs = dynamic_cast<GV_GraphicsScene*>(getGraphicsView()->scene());
    gs->killAllActions();

    GV_Component* comp = this->getSelectedComponent();
    QColor color = comp->getForegroundColor();
    comp->setForegroundColor(QColorDialog::getColor(color, GV_DIALOGFACTORY->getMainWindow()));

	finish();
	getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
}

