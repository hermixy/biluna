/****************************************************************************
** $Id: rs_actioninterface.h 7382 2007-11-29 16:37:31Z andrew $
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


#ifndef RS_ACTIONBASE_H
#define RS_ACTIONBASE_H

#include "cad_global.h"
#include "rs_actioninterface.h"
#include "rs_actionsetsnapmodeauto.h"
#include "rs_actionsetsnaprestrictionnothing.h"



/**
 * Base class for all actions in module qcadactions.
 * Initializes the snap mode to auto snap and disables
 * snap restrictions.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionBase : public RS_ActionInterface {
    Q_OBJECT

public:
    RS_ActionBase(RS_EntityContainer& container,
                  RS_GraphicView& graphicView)
        : RS_ActionInterface(container, graphicView) {
        
        RS_ActionSetSnapModeAuto::factory();
        RS_ActionSetSnapRestrictionNothing::factory();
    }

    virtual ~RS_ActionBase() {
        //RS_ActionSetSnapModeAuto::factory();
        //RS_ActionSetSnapRestrictionNothing::factory();
    }
};

#endif
