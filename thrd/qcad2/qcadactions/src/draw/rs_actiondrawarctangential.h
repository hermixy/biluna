/****************************************************************************
** $Id: rs_actiondrawarctangential.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWARCTANGENTIAL_H
#define RS_ACTIONDRAWARCTANGENTIAL_H

#include "rs_actionbase.h"
#include "rs_arc.h"

/**
 * This action class can handle user events to draw 
 * arcs with three points given.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDrawArcTangential : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetBaseEntity,   /**< Setting base entity. */
        SetEndAngle      /**< Setting end angle. */
    };

public:
    RS_ActionDrawArcTangential(RS_EntityContainer& container,
                       RS_GraphicView& graphicView);
    ~RS_ActionDrawArcTangential();
    
    static RS_String getName() {
        return "Draw Tangential Arc";
    }

    virtual RS_String name() {
        return RS_ActionDrawArcTangential::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    void reset();

    virtual void init(int status=0);
    
    virtual void trigger();
    void preparePreview();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    void setRadius(double r) {
        data.radius = r;
    }

    double getRadius() {
        return data.radius;
    }

protected:
    /**
     * Base entity.
     */
    RS_AtomicEntity* baseEntity;
    /**
     * Start point of base entity clicked?
     */
    bool isStartPoint;
    /**
     * Point that determines end angle.
     */
    RS_Vector point;
    /**
     * Arc data calculated.
     */
    RS_ArcData data;
};

#endif
