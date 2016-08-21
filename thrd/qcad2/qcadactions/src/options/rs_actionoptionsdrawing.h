/****************************************************************************
** $Id: rs_actionoptionsdrawing.h 4010 2006-12-27 19:08:54Z andrew $
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

#ifndef RS_ACTIONOPTIONSDRAWING_H
#define RS_ACTIONOPTIONSDRAWING_H

#include "rs_actioninterface.h"


/**
 * This action class can handle user events to change general 
 * application options.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionOptionsDrawing : public RS_ActionInterface {
    Q_OBJECT
public:
    RS_ActionOptionsDrawing(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    ~RS_ActionOptionsDrawing() {}

    static RS_String getName() {
        return "Options Drawing";
    }

    virtual RS_String name() {
        return RS_ActionOptionsDrawing::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void init(int status=0);

    virtual void trigger();
};

#endif
