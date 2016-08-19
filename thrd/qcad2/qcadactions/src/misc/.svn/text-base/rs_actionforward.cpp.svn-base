/****************************************************************************
** $Id: rs_actionblocksadd.cpp 2487 2005-07-20 00:30:38Z andrew $
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

#include "rs_actionforward.h"



RS_ActionForward::RS_ActionForward(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {}



RS_GuiAction* RS_ActionForward::createGuiAction() {
    static RS_GuiAction* action = NULL;
    
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionForward::getName(), 
            "forward", 
            tr("Continue Action"));
        action->setStatusTip(tr("Continue action (selection is complete)"));
    }
    return action;
}


