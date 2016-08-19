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

#include "qg_cadtoolbardim.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondimaligned.h"
#include "rs_actiondimangular.h"
#include "rs_actiondimdiametric.h"
#include "rs_actiondimhorizontal.h"
#include "rs_actiondimleader.h"
#include "rs_actiondimlinear.h"
#include "rs_actiondimordinate.h"
#include "rs_actiondimradial.h"
#include "rs_actiondimvertical.h"

/**
 * Constructor
 */
QG_CadToolBarDim::QG_CadToolBarDim(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarDim::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bAligned->setDefaultAction(
        RS_ActionDimAligned::createGuiAction());
    bLinearHor->setDefaultAction(
        RS_ActionDimHorizontal::createGuiAction());
    bLinearVer->setDefaultAction(
        RS_ActionDimVertical::createGuiAction());
    bLinear->setDefaultAction(
        RS_ActionDimLinear::createGuiAction());
    bOrdinate->setDefaultAction(
        RS_ActionDimOrdinate::createGuiAction());
    bLeader->setDefaultAction(
        RS_ActionDimLeader::createGuiAction());
    bRadial->setDefaultAction(
        RS_ActionDimRadial::createGuiAction());
    bDiametric->setDefaultAction(
        RS_ActionDimDiametric::createGuiAction());
    bAngular->setDefaultAction(
        RS_ActionDimAngular::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bAligned, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimAligned()));
    connect(bLinearHor, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimLinearHor()));
    connect(bLinearVer, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimLinearVer()));
    connect(bLinear, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimLinear()));
    connect(bRadial, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimRadial()));
    connect(bDiametric, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimDiametric()));
    connect(bAngular, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimAngular()));
    connect(bLeader, SIGNAL(clicked()),
            actionHandler, SLOT(slotDimLeader()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarDim::~QG_CadToolBarDim() {}


