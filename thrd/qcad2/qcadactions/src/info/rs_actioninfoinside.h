/****************************************************************************
** $Id: rs_actioninfoinside.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONINFOINSIDE_H
#define RS_ACTIONINFOINSIDE_H

#include "rs_actionbase.h"


/**
 * This action class can handle user events for checking if
 * a given point is inside or outside the selected contour.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionInfoInside : public RS_ActionBase {
    Q_OBJECT
public:
    RS_ActionInfoInside(RS_EntityContainer& container,
                       RS_GraphicView& graphicView);
    ~RS_ActionInfoInside();

    static RS_String getName() {
        return "Info Inside";
    }

    virtual RS_String name() {
        return RS_ActionInfoInside::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    RS_Vector pt;
    RS_EntityContainer* contour;
};

#endif
