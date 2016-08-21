/****************************************************************************
** $Id: rs_actionselectlayer.h 8382 2008-01-23 22:58:26Z andrew $
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

#ifndef RS_ACTIONSELECTCURRENTLAYER_H
#define RS_ACTIONSELECTCURRENTLAYER_H

#include "rs_actionselectbase.h"


/**
 * This action class can handle user events to select contours.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionSelectCurrentLayer : public RS_ActionSelectBase {
    Q_OBJECT
public:
    RS_ActionSelectCurrentLayer(RS_EntityContainer& container,
                          RS_GraphicView& graphicView);
    ~RS_ActionSelectCurrentLayer() {}

    static RS_String getName() {
        return "Select Current Layer";
    }

    virtual RS_String name() {
        return RS_ActionSelectCurrentLayer::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void init(int status);

    virtual void trigger();
};

#endif
