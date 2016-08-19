/****************************************************************************
** $Id: rs_actiondrawcirclecr.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWCIRCLECR_H
#define RS_ACTIONDRAWCIRCLECR_H

#include "rs_actionbase.h"
#include "rs_circle.h"

/**
 * This action class can handle user events to draw 
 * simple arcs with the center, radius, start- and endangle given.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawCircleCR : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetCenter,         /**< Setting the center point. */
        SetRadius          /**< Setting radius in command line. */
    };

public:
    RS_ActionDrawCircleCR(RS_EntityContainer& container,
                          RS_GraphicView& graphicView);
    ~RS_ActionDrawCircleCR();

    static RS_String getName() {
        return "Draw Circle C R";
    }

    virtual RS_String name() {
        return RS_ActionDrawCircleCR::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    void reset();

    virtual void init(int status=0);

    virtual void trigger();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    
    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    double getRadius() {
        return data.radius;
    }

    void setRadius(double r) {
        data.radius = r;
    }

protected:
    /**
     * Circle data defined so far.
     */
    RS_CircleData data;
    
};

#endif
