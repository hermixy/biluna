/****************************************************************************
** $Id: rs_actionfilesave.cpp 4269 2007-01-20 18:51:56Z andrew $
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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

#include "rs_actionfilesvg.h"

#include "rs_graphic.h"
//Added by qt3to4:
#include <QPixmap>



RS_ActionFileSvg::RS_ActionFileSvg(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {}


RS_GuiAction* RS_ActionFileSvg::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionFileSvg::getName(),
            
                "filesvg",
            tr("&SVG Export"));
        action->setStatusTip(tr("Exports the current drawing to SVG"));
    }                                  
    return action;
}


