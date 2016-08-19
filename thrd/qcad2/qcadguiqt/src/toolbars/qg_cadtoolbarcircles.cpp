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

#include "qg_cadtoolbarcircles.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondrawcircle.h"
#include "rs_actiondrawcircle2p.h"
#include "rs_actiondrawcircle3p.h"
#include "rs_actiondrawcirclecr.h"
#include "rs_actiondrawcircleparallel.h"

/**
 * Constructor
 */
QG_CadToolBarCircles::QG_CadToolBarCircles(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarCircles::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());

    bCircle->setDefaultAction(
        RS_ActionDrawCircle::createGuiAction());
    bCircleCR->setDefaultAction(
        RS_ActionDrawCircleCR::createGuiAction());
    bCircle2P->setDefaultAction(
        RS_ActionDrawCircle2P::createGuiAction());
    bCircle3P->setDefaultAction(
        RS_ActionDrawCircle3P::createGuiAction());
    bCircleParallel->setDefaultAction(
        RS_ActionDrawCircleParallel::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bCircle, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawCircle()));
    connect(bCircleCR, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawCircleCR()));
    connect(bCircle2P, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawCircle2P()));
    connect(bCircle3P, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawCircle3P()));
    connect(bCircleParallel, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawCircleParallel()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarCircles::~QG_CadToolBarCircles() {}


