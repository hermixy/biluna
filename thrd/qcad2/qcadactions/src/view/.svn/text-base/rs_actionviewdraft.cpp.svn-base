/****************************************************************************
** $Id: rs_actionzoomauto.cpp 3703 2006-11-28 17:50:03Z andrew $
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

#include "rs_actionviewdraft.h"

#include <QPixmap>

#include "rs_settings.h"

RS_GuiAction* RS_ActionViewDraft::action = NULL;

/**
 * Constructor.
 */
RS_ActionViewDraft::RS_ActionViewDraft(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView,
                                     RS_MainWindowInterface& mainWindow)
        :RS_ActionInterface( container, graphicView) {

    this->mainWindow = &mainWindow;
}


RS_GuiAction* RS_ActionViewDraft::createGuiAction() {
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionViewDraft::getName(),
            
                "viewdraft",
            tr("&Draft"));
        action->setStatusTip(tr("Enables/disables the draft mode"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setCommand("draft,df");
        action->setKeycode("df");
        action->setFactory(RS_ActionViewDraft::factory);
        
    }

    return action;
}


void RS_ActionViewDraft::init(int status) {
    RS_DEBUG->print("RS_ActionViewDraft::init");

    RS_ActionInterface::init(status);
    trigger();
}



RS_ActionInterface* RS_ActionViewDraft::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    RS_MainWindowInterface* mainWindow = RS_DIALOGFACTORY->getMainWindow();
    
    if (container!=NULL && graphicView!=NULL && mainWindow!=NULL) {
        RS_ActionInterface* a = new RS_ActionViewDraft(*container, *graphicView, *mainWindow);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionViewDraft::trigger() {
    RS_DEBUG->print("RS_ActionViewDraft::trigger");
    
    RS_SETTINGS->beginGroup("/Appearance");
    bool draftMode = !(bool)RS_SETTINGS->readNumEntry("/DraftMode", 0);
    RS_SETTINGS->endGroup();

    bool doit = false;
    
    if (draftMode) {
        if (RS_DIALOGFACTORY->requestOneTimeDialog(
            tr("You have enabled the draft mode.\n"
            "In draft mode complex entities and line widths\n"
            "are not shown to increase the display performance."),
            "DraftMode")) {

            doit = true;
        }
        else {
            draftMode = false;
            doit = false;
        }
    }
    else {
        doit = true;
    }

    if (doit) {
        RS_SETTINGS->beginGroup("/Appearance");
        RS_SETTINGS->writeEntry("/DraftMode", (int)(draftMode));
        RS_SETTINGS->endGroup();
    
        mainWindow->redrawAll();
    }
        
    if (action!=NULL) {
        action->setChecked(draftMode);
    }

    finish();
}

