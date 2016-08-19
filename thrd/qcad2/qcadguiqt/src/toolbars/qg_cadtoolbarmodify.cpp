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

#include "qg_cadtoolbarmodify.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actionmodifyexplode.h"
#include "rs_actionmodifyattributes.h"
#include "rs_actionmodifybevel.h"
#include "rs_actionmodifymirror.h"
#include "rs_actionmodifydivide.h"
#include "rs_actionmodifydivide2.h"
#include "rs_actionmodifydelete.h"
#include "rs_actionmodifydeletefree.h"
#include "rs_actionmodifydeletequick.h"
#include "rs_actionmodifyentity.h"
#include "rs_actionmodifymove.h"
#include "rs_actionmodifymoverotate.h"
#include "rs_actionmodifyrotate.h"
#include "rs_actionmodifyrotate2.h"
#include "rs_actionmodifyround.h"
#include "rs_actionmodifyscale.h"
#include "rs_actionmodifystretch.h"
#include "rs_actionmodifytext.h"
#include "rs_actionmodifytoback.h"
#include "rs_actionmodifytofront.h"
#include "rs_actionmodifytrim.h"
#include "rs_actionmodifytrimboth.h"
#include "rs_actionmodifytrimamount.h"
#include "rs_actionmodifyexplodetext.h"

/**
 * Constructor
 */
QG_CadToolBarModify::QG_CadToolBarModify(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarModify::setupUi(this);
    
    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());
    
    bMove->setDefaultAction(
        RS_ActionModifyMove::createGuiAction());
    bRotate->setDefaultAction(
        RS_ActionModifyRotate::createGuiAction());
    bEntity->setDefaultAction(
        RS_ActionModifyEntity::createGuiAction());
    bScale->setDefaultAction(
        RS_ActionModifyScale::createGuiAction());
    bDelete->setDefaultAction(
        RS_ActionModifyDelete::createGuiAction());
    bTrim->setDefaultAction(
        RS_ActionModifyTrim::createGuiAction());
    bMirror->setDefaultAction(
        RS_ActionModifyMirror::createGuiAction());
    bTrim2->setDefaultAction(
        RS_ActionModifyTrimBoth::createGuiAction());
    bMoveRotate->setDefaultAction(
        RS_ActionModifyMoveRotate::createGuiAction());
    bTrimAmount->setDefaultAction(
        RS_ActionModifyTrimAmount::createGuiAction());
    bCut->setDefaultAction(
        RS_ActionModifyDivide::createGuiAction());
    bCut2->setDefaultAction(
        RS_ActionModifyDivide2::createGuiAction());
    bRotate2->setDefaultAction(
        RS_ActionModifyRotate2::createGuiAction());
    bStretch->setDefaultAction(
        RS_ActionModifyStretch::createGuiAction());
    bBevel->setDefaultAction(
        RS_ActionModifyBevel::createGuiAction());
    bRound->setDefaultAction(
        RS_ActionModifyRound::createGuiAction());
    bExplode->setDefaultAction(
        RS_ActionModifyExplode::createGuiAction());
    bAttributes->setDefaultAction(
        RS_ActionModifyAttributes::createGuiAction());
    bExplodeText->setDefaultAction(
        RS_ActionModifyExplodeText::createGuiAction());
    bEntityText->setDefaultAction(
        RS_ActionModifyText::createGuiAction());
    bToFront->setDefaultAction(
        RS_ActionModifyToFront::createGuiAction());
    bToBack->setDefaultAction(
        RS_ActionModifyToBack::createGuiAction());

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));
}


/**
 * Destructor
 */
QG_CadToolBarModify::~QG_CadToolBarModify() {}


