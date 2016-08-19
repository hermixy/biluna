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

#include "qg_cadtoolbar.h"

#include "rs.h"
#include "rs_debug.h"

#include "qg_cadtoolbararcs.h"
#include "qg_cadtoolbarcircles.h"
#include "qg_cadtoolbardim.h"
#include "qg_cadtoolbarellipses.h"
#include "qg_cadtoolbarinfo.h"
#include "qg_cadtoolbarlines.h"
#include "qg_cadtoolbarmain.h"
#include "qg_cadtoolbarmodify.h"
#include "qg_cadtoolbarpoints.h"
#include "qg_cadtoolbarselect.h"
#include "qg_cadtoolbarsnap.h"
#include "qg_cadtoolbarsplines.h"

/**
 * Constructor
 */
QG_CadToolBar::QG_CadToolBar(QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    
    //actionHandler = NULL;
    currentTb = NULL;

    tbMain = NULL;

    tbPoints = NULL;
    tbLines = NULL;
    tbArcs = NULL;
    tbCircles = NULL;
    tbEllipses = NULL;
    tbSplines = NULL;
#ifdef RS_PROF
    tbPolylines = NULL;
#endif

    tbDim = NULL;

    tbModify = NULL;
    tbInfo = NULL;
    tbSelect = NULL;

    tbSnap = NULL;
}


/**
 * Destructor
 */
QG_CadToolBar::~QG_CadToolBar() {}


/**
 * @return Pointer to action handler or NULL.
 */
//RS_ActionHandler* QG_CadToolBar::getActionHandler() {
//    return actionHandler;
//}

/**
 * Called from the sub toolbar
 */
void QG_CadToolBar::back() {
    emit(signalBack());
}

/**
 * Called from the application.
 */
void QG_CadToolBar::forceNext() {
    if (currentTb!=NULL && currentTb==tbSelect) {
        tbSelect->runNextAction();
    }
}

void QG_CadToolBar::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::RightButton) {
        back();
        e->accept();
    }
}

void QG_CadToolBar::contextMenuEvent(QContextMenuEvent *e) {
    e->accept();
}

/**
 * Creates all tool bars and shows the main toolbar.
 *
 * @param ah Pointer to action handler which will deal with the actions in this tool bar.
 */
void QG_CadToolBar::createSubToolBars(/*RS_ActionHandler* ah*/) {
    //actionHandler = ah;
    tbMain = new QG_CadToolBarMain(this);
    currentTb = tbMain;

    tbPoints = new QG_CadToolBarPoints(this);
    tbPoints->hide();

    tbLines = new QG_CadToolBarLines(this);
    tbLines->hide();

    tbArcs = new QG_CadToolBarArcs(this);
    tbArcs->hide();

    tbCircles = new QG_CadToolBarCircles(this);
    tbCircles->hide();

    tbEllipses = new QG_CadToolBarEllipses(this);
    tbEllipses->hide();

    tbSplines = new QG_CadToolBarSplines(this);
    tbSplines->hide();

#ifdef RS_PROF
    tbPolylines = new QG_CadToolBarPolylines(this);
    tbPolylines->hide();
#endif

    tbDim = new QG_CadToolBarDim(this);
    tbDim->hide();

    tbInfo = new QG_CadToolBarInfo(this);
    tbInfo->hide();

    tbModify = new QG_CadToolBarModify(this);
    tbModify->hide();

    tbSnap = new QG_CadToolBarSnap(this);
    tbSnap->hide();

    tbSelect = new QG_CadToolBarSelect(this);
    tbSelect->hide();
}


void QG_CadToolBar::showToolBar(int id) {
    RS_DEBUG->print("QG_CadToolBar::showToolBar: %d", id);
    QWidget* newTb = NULL;

    switch (id) {
    default:
    case RS2::ToolBarMain:
        newTb = tbMain;
        break;
    case RS2::ToolBarPoints:
        newTb = tbPoints;
        break;
    case RS2::ToolBarLines:
        newTb = tbLines;
        break;
    case RS2::ToolBarArcs:
        newTb = tbArcs;
        break;
    case RS2::ToolBarCircles:
        newTb = tbCircles;
        break;
    case RS2::ToolBarEllipses:
        newTb = tbEllipses;
        break;
    case RS2::ToolBarSplines:
        newTb = tbSplines;
        break;
#ifdef RS_PROF
    case RS2::ToolBarPolylines:
        newTb = tbPolylines;
        break;
#endif

    case RS2::ToolBarDim:
        newTb = tbDim;
        break;

    case RS2::ToolBarInfo:
        newTb = tbInfo;
        break;
    case RS2::ToolBarModify:
        newTb = tbModify;
        break;
    case RS2::ToolBarSnap:
        newTb = tbSnap;
        break;
    case RS2::ToolBarSelect:
        newTb = tbSelect;
        break;
    }

    if (currentTb==newTb) {
        RS_DEBUG->print("set same toolbar");
        return;
    }
    if (currentTb!=NULL) {
        RS_DEBUG->print("hiding previous toolbar");
        currentTb->hide();
    }
    currentTb = newTb;
    if (currentTb!=NULL) {
        RS_DEBUG->print("showing toolbar");
        currentTb->show();
    }
}

void QG_CadToolBar::showToolBarMain() {
    showToolBar(RS2::ToolBarMain);
}

void QG_CadToolBar::showToolBarPoints() {
    showToolBar(RS2::ToolBarPoints);
}

void QG_CadToolBar::showToolBarLines() {
    showToolBar(RS2::ToolBarLines);
}

void QG_CadToolBar::showToolBarArcs() {
    showToolBar(RS2::ToolBarArcs);
}

void QG_CadToolBar::showToolBarCircles() {
    showToolBar(RS2::ToolBarCircles);
}

void QG_CadToolBar::showToolBarEllipses() {
    showToolBar(RS2::ToolBarEllipses);
}

void QG_CadToolBar::showToolBarSplines() {
    showToolBar(RS2::ToolBarSplines);
}

#ifdef RS_PROF
void QG_CadToolBar::showToolBarPolylines() {
    showToolBar(RS2::ToolBarPolylines);
}
#endif

void QG_CadToolBar::showToolBarInfo() {
    showToolBar(RS2::ToolBarInfo);
}

void QG_CadToolBar::showToolBarModify() {
    showToolBar(RS2::ToolBarModify);
}

void QG_CadToolBar::showToolBarSnap() {
    showToolBar(RS2::ToolBarSnap);
}

void QG_CadToolBar::showToolBarDim() {
    showToolBar(RS2::ToolBarDim);
}

void QG_CadToolBar::showToolBarSelect() {
    showToolBarSelect(NULL);
}

void QG_CadToolBar::showToolBarSelect(RS_ActionInterface* (*f)()) {

    tbSelect->setNextAction(f);
    showToolBar(RS2::ToolBarSelect);
}
