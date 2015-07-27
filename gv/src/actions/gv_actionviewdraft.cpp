/*****************************************************************
 * $Id: gv_actionviewdraft.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:21:28 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rsactionviewdraf.h 3703
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionviewdraft.h"

// #include <QPixmap>

#include "rb_settings.h"

RB_GuiAction* GV_ActionViewDraft::action = NULL;

/**
 * Constructor.
 */
GV_ActionViewDraft::GV_ActionViewDraft(GV_Drawing& dwg, GV_GraphicsView& gv)
                                        : GV_ActionBase(dwg, gv) {

}


RB_GuiAction* GV_ActionViewDraft::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionViewDraft::getName(),
                	"viewdraft",
                	tr("&Draft"));
        action->setStatusTip(tr("Enables/disables the draft mode"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setCommand("draft,df");
        action->setKeycode("df");
        action->setFactory(GV_ActionViewDraft::factory);
        
    }

    return action;
}


void GV_ActionViewDraft::init(int status) {
    RB_DEBUG->print("GV_ActionViewDraft::init");

    GV_ActionBase::init(status);
    trigger();
}



RB_Action* GV_ActionViewDraft::factory() {
	GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
	GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

	if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionViewDraft(*drawing, *graphicsView);
		drawing->setCurrentAction(a);
		return a;
	}

    return NULL;
}


void GV_ActionViewDraft::trigger() {
	RB_DEBUG->print("GV_ActionViewDraft::trigger");
    
	RB_SETTINGS->beginGroup("/Appearance");
	bool draftMode = !(bool)RB_SETTINGS->readNumEntry("/DraftMode", 0);
	RB_SETTINGS->endGroup();

	bool doIt = false;
    
    if (draftMode) {
		if (GV_DIALOGFACTORY->requestOneTimeDialog(
            tr("You have enabled the draft mode.\n"
            "In draft mode complex entities and line widths\n"
            "are not shown to increase the display performance."),
            "DraftMode")) {

			doIt = true;
		} else {
            draftMode = false;
			doIt = false;
        }
	} else {
		doIt = true;
    }

	if (doIt) {
		RB_SETTINGS->beginGroup("/Appearance");
		RB_SETTINGS->writeEntry("/DraftMode", (int)(draftMode));
		RB_SETTINGS->endGroup();
    
//        mainWindow->redrawAll();
		if (mGraphicsView->renderHints() & QPainter::Antialiasing) {
			mGraphicsView->setRenderHints(QPainter::TextAntialiasing);
		} else {
			mGraphicsView->setRenderHints(QPainter::TextAntialiasing
										 | QPainter::Antialiasing);
		}
    }
        
	if (action != NULL) {
        action->setChecked(draftMode);
    }

    finish();
}

