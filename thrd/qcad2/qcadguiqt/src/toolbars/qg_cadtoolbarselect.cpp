/****************************************************************************
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

#include "qg_cadtoolbarselect.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actionforward.h"
#include "rs_actiondeselectall.h"
#include "rs_actionselectall.h"
#include "rs_actiondeselectintersected.h"
#include "rs_actionselectintersected.h"
#include "rs_actionselectinvert.h"
#include "rs_actionselectlayer.h"
#include "rs_actionselectsingle.h"
#include "rs_actionselectcontour.h"
#include "rs_actiondeselectwindow.h"
#include "rs_actionselectwindow.h"

/**
 * Constructor
 */
QG_CadToolBarSelect::QG_CadToolBarSelect(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarSelect::setupUi(this);

    //assert(actionHandler!=NULL);

    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bUnAll->setDefaultAction(
        RS_ActionDeselectAll::createGuiAction());
    bAll->setDefaultAction(
        RS_ActionSelectAll::createGuiAction());
    bSingle->setDefaultAction(
        RS_ActionSelectSingle::createGuiAction());
    bContour->setDefaultAction(
        RS_ActionSelectContour::createGuiAction());
    bUnWindow->setDefaultAction(
        RS_ActionDeselectWindow::createGuiAction());
    bWindow->setDefaultAction(
        RS_ActionSelectWindow::createGuiAction());
    bUnInters->setDefaultAction(
        RS_ActionDeselectIntersected::createGuiAction());
    bInters->setDefaultAction(
        RS_ActionSelectIntersected::createGuiAction());
    bInvert->setDefaultAction(
        RS_ActionSelectInvert::createGuiAction());
    bLayer->setDefaultAction(
        RS_ActionSelectLayer::createGuiAction());
    
    bForward->setDefaultAction(
        RS_ActionForward::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    connect(bForward, SIGNAL(clicked()),
        this, SLOT(runNextAction()));
}


/**
 * Destructor
 */
QG_CadToolBarSelect::~QG_CadToolBarSelect() {}


void QG_CadToolBarSelect::setNextAction(RS_ActionInterface* (*f)()) {
    this->nextActionFactory = f;
    if (nextActionFactory==NULL) {
        bForward->hide();
    } else {
        bForward->show();
    }
}

void QG_CadToolBarSelect::runNextAction() {
    if (nextActionFactory!=NULL) {
        nextActionFactory();
    }
}

void QG_CadToolBarSelect::back() {
    RS_DEBUG->print("QG_CadToolBarSelect::back");

    // 20080124: cancel everything if user clicks back button
    cadToolBar->showToolBarMain();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    if (graphicView!=NULL) {
        graphicView->killAllActions();
    }
}
