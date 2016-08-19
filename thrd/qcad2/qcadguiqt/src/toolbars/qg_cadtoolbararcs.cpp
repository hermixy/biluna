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

#include "qg_cadtoolbararcs.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondrawarc.h"
#include "rs_actiondrawarc3p.h"
#include "rs_actiondrawarctangential.h"
#include "rs_actiondrawarcparallel.h"

/**
 * Constructor
 */
QG_CadToolBarArcs::QG_CadToolBarArcs(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarArcs::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bArc->setDefaultAction(
        RS_ActionDrawArc::createGuiAction());
    bArc3P->setDefaultAction(
        RS_ActionDrawArc3P::createGuiAction());
    bArcParallel->setDefaultAction(
        RS_ActionDrawArcParallel::createGuiAction());
    bArcTangential->setDefaultAction(
        RS_ActionDrawArcTangential::createGuiAction());
    
    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bArc, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawArc()));
    connect(bArc3P, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawArc3P()));
    connect(bArcParallel, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawArcParallel()));
    connect(bArcTangential, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawArcTangential()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarArcs::~QG_CadToolBarArcs() {}


