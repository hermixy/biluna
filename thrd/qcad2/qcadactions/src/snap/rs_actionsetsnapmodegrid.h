/****************************************************************************
** $Id: rs_actionsetsnapmode.h 7068 2007-11-13 09:37:53Z andrew $
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

#ifndef RS_ACTIONSETSNAPMODEGRID_H
#define RS_ACTIONSETSNAPMODEGRID_H

#include "rs_actionsetsnapmode.h"


/**
 * Sets the current snap mode to auto snap.
 *
 * @author Andrew Mustun
 */
class RS_ActionSetSnapModeGrid : public RS_ActionSetSnapMode {
    Q_OBJECT

public:
    RS_ActionSetSnapModeGrid(RS_EntityContainer& container,
                         RS_GraphicView& graphicView);
    ~RS_ActionSetSnapModeGrid() {}

    static RS_String getName() {
        return "Set Snap Mode Grid";
    }

    virtual RS_String name() {
        return RS_ActionSetSnapModeGrid::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

protected:
    static RS_GuiAction* action;
};

#endif
