/*****************************************************************
 * $Id: gv_actionformatlineattributes.cpp 1479 2011-11-17 15:51:46Z rutger $
 * Created: June 12, 2010 11:27:41 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionformatlineattributes.h"

#include "gv_actioneditundo.h"
#include "gv_lineattributesdialog.h"


/**
 * Action for setting line attributes
 */
GV_ActionFormatLineAttributes::GV_ActionFormatLineAttributes(GV_Drawing& dwg, GV_GraphicsView& gv)
        		: GV_ActionBase(dwg, gv) {

    RB_DEBUG->print("GV_ActionFormatLineAttributes::GV_ActionFormatLineAttributes()");
    // reset();

    //history.setAutoDelete(true);

    // mNewLine = NULL;
}

/**
 * Destructor
 */
GV_ActionFormatLineAttributes::~GV_ActionFormatLineAttributes() {
    RB_DEBUG->print("GV_ActionFormatLineAttributes::~GV_ActionFormatLineAttributes() OK");
}

RB_GuiAction* GV_ActionFormatLineAttributes::createGuiAction() {
    static RB_GuiAction* ga = NULL;
    
    if (ga==NULL) {
        ga = new RB_GuiAction(GV_ActionFormatLineAttributes::getName(),
                             QIcon(":/images/modifyattributes.png"),
                             tr("&Line Attributes"));
        ga->setStatusTip(tr("Set line attributes"));
        ga->setCommand("lineattr,lineat");
        ga->setKeycode("la");
        ga->setFactory(GV_ActionFormatLineAttributes::factory);
    }

    return ga;
}



void GV_ActionFormatLineAttributes::init(int status) {
    RB_DEBUG->print("GV_ActionFormatLineAttributes::init");
    GV_ActionBase::init(status);
    trigger(); // not triggered by mouse- or keyboard events
}

/**
 * Create action and set as current drawing action,
 * is a static function
 */
RB_Action* GV_ActionFormatLineAttributes::factory() {
    RB_DEBUG->print("GV_ActionFormatLineAttributes::factory()");
	
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionFormatLineAttributes(*drawing, *graphicsView);
        a->setSelectedComponent(drawing->getSelectedComponent()); // for edit
		drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}


/**
 * Trigger action
 */
void GV_ActionFormatLineAttributes::trigger() {
    RB_DEBUG->print("GV_ActionFormatLineAttributes::trigger()");
	
    GV_GraphicsScene* gs = dynamic_cast<GV_GraphicsScene*>(getGraphicsView()->scene());
    gs->killAllActions();

    GV_Line* line = dynamic_cast<GV_Line*>(getSelectedComponent());

    if (line) {
        RB_Dialog* dlg = GV_DIALOGFACTORY->getDialog(GV_DialogFactory::DialogLineAttributes);
        GV_LineAttributesDialog* dlgLa = dynamic_cast<GV_LineAttributesDialog*>(dlg);
        dlgLa->setPen(line->pen());

        if (dlgLa->exec() == QDialog::Accepted) {
            line->setPen(dlgLa->getPen());
        }
        delete dlg;
    }

	finish();
	getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
}

