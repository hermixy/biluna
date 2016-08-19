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

#include "qg_cadtoolbarellipses.h"

#include <assert.h>

#include "qg_cadtoolbar.h"

#include "rs_actionback.h"
#include "rs_actiondrawellipseaxis.h"
#include "rs_actiondrawellipsearcaxis.h"

/**
 * Constructor
 */
QG_CadToolBarEllipses::QG_CadToolBarEllipses(QG_CadToolBar* parent)
        : QG_CadToolBarBase(parent) {
    
    Ui::QG_CadToolBarEllipses::setupUi(this);

    //assert(actionHandler!=NULL);
    
    bBack->setDefaultAction(
        RS_ActionBack::createGuiAction());

    bEllipseAxes->setDefaultAction(
        RS_ActionDrawEllipseAxis::createGuiAction());
    bEllipseArcAxes->setDefaultAction(
        RS_ActionDrawEllipseArcAxis::createGuiAction());
    
    connect(bBack, SIGNAL(clicked()),
            this, SLOT(back()));

    /*
    connect(bEllipseAxes, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawEllipseAxis()));
    connect(bEllipseArcAxes, SIGNAL(clicked()),
            actionHandler, SLOT(slotDrawEllipseArcAxis()));
    */
}


/**
 * Destructor
 */
QG_CadToolBarEllipses::~QG_CadToolBarEllipses() {}


