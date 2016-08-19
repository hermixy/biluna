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

#include "qg_cadtoolbarsnap.h"

#include <assert.h>

#include "rs_actionback.h"
#include "rs_actionsetsnapmode.h"
#include "rs_actionsetsnaprestriction.h"
#include "rs_actionsnapintersectionmanual.h"
#include "rs_actionsetrelativezero.h"
#include "rs_actionlockrelativezero.h"
#include "rs_actionsetsnapmodefree.h"
#include "rs_actionsetsnapmodegrid.h"
#include "rs_actionsetsnapmodeendpoint.h"
#include "rs_actionsetsnapmodeonentity.h"
#include "rs_actionsetsnapmodecenter.h"
#include "rs_actionsetsnapmodemiddle.h"
#include "rs_actionsetsnapmodedist.h"
#include "rs_actionsetsnapmodeintersection.h"
#include "rs_actionsnapintersectionmanual.h"
#include "rs_actionsetsnapmodereference.h"
#include "rs_actionsetsnapmodeauto.h"
#include "rs_actionsetsnapmodecoordinate.h"
#include "rs_actionsetsnapmodecoordinatepolar.h"
#include "rs_actionsetsnaprestrictionnothing.h"
#include "rs_actionsetsnaprestrictionorthogonal.h"
#include "rs_actionsetsnaprestrictionhorizontal.h"
#include "rs_actionsetsnaprestrictionvertical.h"

#include "qg_cadtoolbar.h"


/**
 * Constructor
 */
QG_CadToolBarSnap::QG_CadToolBarSnap(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {

    RS_DEBUG->print("QG_CadToolBarSnap::QG_CadToolBarSnap");
    
    Ui::QG_CadToolBarSnap::setupUi(this);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    bFree->setDefaultAction(
        RS_ActionSetSnapModeFree::createGuiAction());
    bGrid->setDefaultAction(
        RS_ActionSetSnapModeGrid::createGuiAction());
    bEndpoint->setDefaultAction(
        RS_ActionSetSnapModeEndpoint::createGuiAction());
    bOnEntity->setDefaultAction(
        RS_ActionSetSnapModeOnEntity::createGuiAction());
    bCenter->setDefaultAction(
        RS_ActionSetSnapModeCenter::createGuiAction());
    bMiddle->setDefaultAction(
        RS_ActionSetSnapModeMiddle::createGuiAction());
    bDist->setDefaultAction(
        RS_ActionSetSnapModeDist::createGuiAction());
    bIntersection->setDefaultAction(
        RS_ActionSetSnapModeIntersection::createGuiAction());
    bIntersectionManual->setDefaultAction(
        RS_ActionSnapIntersectionManual::createGuiAction());
    bRef->setDefaultAction(
        RS_ActionSetSnapModeReference::createGuiAction());
    bAuto->setDefaultAction(
        RS_ActionSetSnapModeAuto::createGuiAction());
    
    bCoordinate->setDefaultAction(
        RS_ActionSetSnapModeCoordinate::createGuiAction());
    bCoordinatePolar->setDefaultAction(
        RS_ActionSetSnapModeCoordinatePolar::createGuiAction());

    bResNothing->setDefaultAction(
        RS_ActionSetSnapRestrictionNothing::createGuiAction());
    bResOrthogonal->setDefaultAction(
        RS_ActionSetSnapRestrictionOrthogonal::createGuiAction());
    bResHorizontal->setDefaultAction(
        RS_ActionSetSnapRestrictionHorizontal::createGuiAction());
    bResVertical->setDefaultAction(
        RS_ActionSetSnapRestrictionVertical::createGuiAction());
    
    bRelZero->setDefaultAction(
        RS_ActionSetRelativeZero::createGuiAction());
    bLockRelZero->setDefaultAction(
        RS_ActionLockRelativeZero::createGuiAction());
    
    RS_DEBUG->print("QG_CadToolBarSnap::QG_CadToolBarSnap: OK");
}


/**
 * Destructor
 */
QG_CadToolBarSnap::~QG_CadToolBarSnap() {}


