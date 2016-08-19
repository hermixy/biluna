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

#include "qg_cadtoolbarpolylines.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondrawpolyline.h"
#include "rs_actionpolylineadd.h"
#include "rs_actionpolylineappend.h"
#include "rs_actionpolylinedel.h"
#include "rs_actionpolylinedelbetween.h"
#include "rs_actionpolylinetrim.h"
#include "rs_actionpolylineequidistant.h"
#include "rs_actionpolylinefromsegments.h"

/**
 * Constructor
 */
QG_CadToolBarPolylines::QG_CadToolBarPolylines(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarPolylines::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bPolyline->setDefaultAction(
        RS_ActionDrawPolyline::createGuiAction());
    bPolylineAdd->setDefaultAction(
        RS_ActionPolylineAdd::createGuiAction());
    bPolylineDel->setDefaultAction(
        RS_ActionPolylineDel::createGuiAction());
    bPolylineDelBetween->setDefaultAction(
        RS_ActionPolylineDelBetween::createGuiAction());
    bPolylineTrim->setDefaultAction(
        RS_ActionPolylineTrim::createGuiAction());
    bPolylineAppend->setDefaultAction(
        RS_ActionPolylineAppend::createGuiAction());
    bPolylineEquidistant->setDefaultAction(
        RS_ActionPolylineEquidistant::createGuiAction());
    bPolylineFromSegments->setDefaultAction(
        RS_ActionPolylineFromSegments::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bPolyline, SIGNAL(clicked()),
        actionHandler, SLOT(slotDrawPolyline()));
    connect(bPolylineAdd, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineAdd()));
    connect(bPolylineDel, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineDel()));
    connect(bPolylineDelBetween, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineDelBetween()));
    connect(bPolylineTrim, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineTrim()));
    connect(bPolylineAppend, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineAppend()));
    connect(bPolylineEquidistant, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineEquidistant()));
    connect(bPolylineFromSegments, SIGNAL(clicked()),
        actionHandler, SLOT(slotPolylineFromSegments()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarPolylines::~QG_CadToolBarPolylines() {}


