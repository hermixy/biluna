/****************************************************************************
** $Id: rs_actionsnapintersectionmanual.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONSNAPINTERSECTIONMANUAL_H
#define RS_ACTIONSNAPINTERSECTIONMANUAL_H

#include "rs_actioninterface.h"


/**
 * This action class can handle user events to trim entities.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionSnapIntersectionManual : public RS_ActionInterface {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        ChooseEntity1,      /**< Choosing the 1st entity. */
        ChooseEntity2       /**< Choosing the 2nd entity. */
    };

public:
    RS_ActionSnapIntersectionManual(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionSnapIntersectionManual() {}

    static RS_String getName() {
        return "Snap Intersection Manual";
    }

    virtual RS_String name() {
        return RS_ActionSnapIntersectionManual::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    RS_Entity* entity1;
    RS_Entity* entity2;
    RS_Vector coord;
};

#endif
