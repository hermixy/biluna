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

#include "qg_cadtoolbarinfo.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actioninfoangle.h"
#include "rs_actioninfoarea.h"
#include "rs_actioninfodist.h"
#include "rs_actioninfodist2.h"
#include "rs_actioninfototallength.h"

/**
 * Constructor
 */
QG_CadToolBarInfo::QG_CadToolBarInfo(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarInfo::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bDist->setDefaultAction(
        RS_ActionInfoDist::createGuiAction());
    bDist2->setDefaultAction(
        RS_ActionInfoDist2::createGuiAction());
    bAngle->setDefaultAction(
        RS_ActionInfoAngle::createGuiAction());
    bTotalLength->setDefaultAction(
        RS_ActionInfoTotalLength::createGuiAction());
    bArea->setDefaultAction(
        RS_ActionInfoArea::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bDist, SIGNAL(clicked()),
        actionHandler, SLOT(slotInfoDist()));
    connect(bDist2, SIGNAL(clicked()),
        actionHandler, SLOT(slotInfoDist2()));
    connect(bAngle, SIGNAL(clicked()),
        actionHandler, SLOT(slotInfoAngle()));
    connect(bTotalLength, SIGNAL(clicked()),
        actionHandler, SLOT(slotInfoTotalLength()));
    connect(bArea, SIGNAL(clicked()),
        actionHandler, SLOT(slotInfoArea()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarInfo::~QG_CadToolBarInfo() {}


