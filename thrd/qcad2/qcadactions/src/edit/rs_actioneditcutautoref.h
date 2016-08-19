/****************************************************************************
** $Id: rs_actioneditcopy.h 7530 2007-12-07 14:13:41Z andrew $
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

#ifndef RS_ACTIONEDITCUTAUTOREF_H
#define RS_ACTIONEDITCUTAUTOREF_H

#include "rs_actioneditcopy.h"


/**
 * This action class can handle user events for cutting entities 
 * to the clipboard.
 *
 * @author Andrew Mustun
 */
class RS_ActionEditCutAutoRef : public RS_ActionEditCopy {
    Q_OBJECT
    
public:
    RS_ActionEditCutAutoRef(RS_EntityContainer& container,
                     RS_GraphicView& graphicView);
    ~RS_ActionEditCutAutoRef();

    static RS_String getName() {
        return "Edit Cut Auto Ref";
    }

    virtual RS_String name() {
        return RS_ActionEditCutAutoRef::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
};

#endif
