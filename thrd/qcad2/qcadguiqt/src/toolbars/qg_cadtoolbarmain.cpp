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

#include "qg_cadtoolbarmain.h"

#include "qg_cadtoolbar.h"

#include "rs_actiondrawpoint.h"
#include "rs_actiondrawspline.h"
#include "rs_actiondrawtext.h"
#include "rs_actiondrawhatch.h"
#include "rs_actiondrawimage.h"
#include "rs_actionblockscreate.h"
#include "rs_actionmodifyisoproject.h"

/**
 * Constructor
 */
QG_CadToolBarMain::QG_CadToolBarMain(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    Ui::QG_CadToolBarMain::setupUi(this);

    //assert(actionHandler!=NULL);
    //assert(cadToolBar!=NULL);
    
    bMenuPoint->setDefaultAction(
        RS_ActionDrawPoint::createGuiAction());
    bMenuSpline->setDefaultAction(
        RS_ActionDrawSpline::createGuiAction());
    bMenuText->setDefaultAction(
        RS_ActionDrawText::createGuiAction());
    bMenuHatch->setDefaultAction(
        RS_ActionDrawHatch::createGuiAction());
    bMenuImage->setDefaultAction(
        RS_ActionDrawImage::createGuiAction());
    bMenuIsoProject->setDefaultAction(
        RS_ActionModifyIsoProject::createGuiAction());


    //connect(bMenuPoint, SIGNAL(clicked()),
    //        cadToolBar, SLOT(showToolBarPoints()));
    //connect(bMenuPoint, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawPoint()));
    connect(bMenuLine, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarLines()));
    connect(bMenuArc, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarArcs()));
    connect(bMenuCircle, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarCircles()));
    connect(bMenuEllipse, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarEllipses()));
    //connect(bMenuSpline, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawSpline()));
    connect(bMenuPolyline, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarPolylines()));
    
    //connect(bMenuText, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawText()));
    connect(bMenuDim, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarDim()));
    //connect(bMenuHatch, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawHatch()));
    //connect(bMenuImage, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawImage()));
    
    connect(bMenuModify, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarModify()));
    connect(bMenuInfo, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarInfo()));

    //connect(bMenuBlock, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotBlocksCreate()));
    bMenuBlock->setDefaultAction(
        RS_ActionBlocksCreate::createGuiAction());
    connect(bMenuSelect, SIGNAL(clicked()),
            cadToolBar, SLOT(showToolBarSelect()));

#ifndef RS_PROF
    bMenuPolyline->hide();
#endif
    
}


/**
 * Destructor
 */
QG_CadToolBarMain::~QG_CadToolBarMain() {}

