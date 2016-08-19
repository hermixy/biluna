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

#include "qg_cadtoolbarlines.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondrawlineangle.h"
#include "rs_actiondrawlinebisector.h"
#include "rs_actiondrawlinefree.h"
#include "rs_actiondrawline.h"
#include "rs_actiondrawlinehorizontal.h"
#include "rs_actiondrawlinehorvert.h"
#include "rs_actiondrawlineorthogonal.h"
#include "rs_actiondrawlineparallel.h"
#include "rs_actiondrawlineparallelthrough.h"
#include "rs_actiondrawlinepolygon2.h"
#include "rs_actiondrawlinepolygon.h"
#include "rs_actiondrawlinerectangle.h"
#include "rs_actiondrawlinerelangle.h"
#include "rs_actiondrawlinetangent1.h"
#include "rs_actiondrawlinetangent2.h"
#include "rs_actiondrawlinevertical.h"

/**
 * Constructor
 */
QG_CadToolBarLines::QG_CadToolBarLines(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarLines::setupUi(this);

    //assert(actionHandler!=NULL);

    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());

    bNormal->setDefaultAction(
        RS_ActionDrawLine::createGuiAction());
    bAngle->setDefaultAction(
        RS_ActionDrawLineAngle::createGuiAction());
    bHorizontal->setDefaultAction(
        RS_ActionDrawLineHorizontal::createGuiAction());
    bVertical->setDefaultAction(
        RS_ActionDrawLineVertical::createGuiAction());
    bRectangle->setDefaultAction(
        RS_ActionDrawLineRectangle::createGuiAction());
    bBisector->setDefaultAction(
        RS_ActionDrawLineBisector::createGuiAction());
    bParallel->setDefaultAction(
        RS_ActionDrawLineParallel::createGuiAction());
    bParallelThrough->setDefaultAction(
        RS_ActionDrawLineParallelThrough::createGuiAction());
    bTangent1->setDefaultAction(
        RS_ActionDrawLineTangent1::createGuiAction());
    bTangent2->setDefaultAction(
        RS_ActionDrawLineTangent2::createGuiAction());
    bOrthogonal->setDefaultAction(
        RS_ActionDrawLineOrthogonal::createGuiAction());
    bRelAngle->setDefaultAction(
        RS_ActionDrawLineRelAngle::createGuiAction());
    bPolygon->setDefaultAction(
        RS_ActionDrawLinePolygon::createGuiAction());
    bPolygon2->setDefaultAction(
        RS_ActionDrawLinePolygon2::createGuiAction());
    bFree->setDefaultAction(
        RS_ActionDrawLineFree::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bNormal, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLine()));
    connect(bAngle, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineAngle()));
    connect(bHorizontal, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineHorizontal()));
    connect(bVertical, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineVertical()));
    connect(bRectangle, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineRectangle()));
    connect(bBisector, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineBisector()));
    connect(bParallel, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineParallel()));
    connect(bParallelThrough, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineParallelThrough()));
    connect(bTangent1, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineTangent1()));
    connect(bTangent2, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineTangent2()));
    connect(bOrthogonal, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineOrthogonal()));
    connect(bRelAngle, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineRelAngle()));
    connect(bPolygon, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLinePolygon()));
    connect(bPolygon2, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLinePolygon2()));
    connect(bFree, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawLineFree()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarLines::~QG_CadToolBarLines() {}


