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

#include "qg_cadtoolbarsplines.h"

#include "rs_actiondrawspline.h"

#include "qg_cadtoolbar.h"

/**
 * Constructor
 */
QG_CadToolBarSplines::QG_CadToolBarSplines(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarSplines::setupUi(this);

    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    //assert(actionHandler!=NULL);

    //connect(bSpline, SIGNAL(clicked()),
    //        actionHandler, SLOT(slotDrawSpline()));
    
    bSpline->setDefaultAction(
        RS_ActionDrawSpline::createGuiAction());
}


/**
 * Destructor
 */
QG_CadToolBarSplines::~QG_CadToolBarSplines() {}


