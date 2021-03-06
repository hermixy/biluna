/****************************************************************************
** $Id: rs_actiondrawlinehorvert.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWLINEHORVERT_H
#define RS_ACTIONDRAWLINEHORVERT_H

#include "rs_actionbase.h"
#include "rs_line.h"

/**
 * This action class can handle user events to draw 
 * horizontal/vertical lines from a given startpoint
 * to an endpoint closest to a second given point.
 * OBSOLETE
 *
 * @author Ulf Lehnert
 */
class CAD_EXPORT RS_ActionDrawLineHorVert : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,      /**< Setting the 1st point.  */
        SetEndpoint       /**< Setting the 2nd point. */
    };

public:
    RS_ActionDrawLineHorVert(RS_EntityContainer& container,
                             RS_GraphicView& graphicView);
    ~RS_ActionDrawLineHorVert();

    static RS_String getName() {
        return "Draw Line Hor Vert";
    }

    virtual RS_String name() {
        return RS_ActionDrawLineHorVert::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /**
     * Line data.
     */
    RS_LineData data;
    /**
     * 2 points
     */
    RS_Vector p1;
    RS_Vector p2;
};

#endif
