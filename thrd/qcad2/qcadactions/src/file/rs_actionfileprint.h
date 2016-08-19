/****************************************************************************
** $Id: rs_actionfilesave.h 4010 2006-12-27 19:08:54Z andrew $
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

#ifndef RS_ACTIONFILEPRINT_H
#define RS_ACTIONFILEPRINT_H

#include "rs_actioninterface.h"


/**
 * This action class can handle user events to print the current file.
 *
 * @author Andrew Mustun
 */
class RS_ActionFilePrint : public RS_ActionInterface {
    Q_OBJECT
public:
    RS_ActionFilePrint(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    ~RS_ActionFilePrint() {}

    static RS_String getName() {
        return "File Print";
    }

    virtual RS_String name() {
        return RS_ActionFilePrint::getName();
    }

    static RS_GuiAction* createGuiAction();
};

#endif
