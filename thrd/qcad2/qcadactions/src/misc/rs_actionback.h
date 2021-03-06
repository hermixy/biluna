/****************************************************************************
** $Id: rs_actionblocksadd.h 2487 2005-07-20 00:30:38Z andrew $
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

#ifndef RS_ACTIONBACK_H
#define RS_ACTIONBACK_H

#include "rs_actioninterface.h"

/**
 * GUI action for back button.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionBack : public RS_ActionInterface {
  Q_OBJECT
public:
    RS_ActionBack(RS_EntityContainer& container, RS_GraphicView& graphicView);
    ~RS_ActionBack() {}

    static RS_String getName() {
        return "Back";
    }

    virtual RS_String name() {
        return RS_ActionBack::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
};

#endif
