/****************************************************************************
** $Id: rs_actionpolylinedelbetween.h 10343 2008-04-06 17:43:02Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
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

#ifndef RS_ACTIONPOLYLINEDELBETWEEN_H
#define RS_ACTIONPOLYLINEDELBETWEEN_H

#include "rs_actionbase.h"
#include "rs_polyline.h"

#include <qaction.h>


/**
 * This action class can handle user events to delete all nodes of 
 * a polyline that are between two given nodes.
 *
 * @author Andrew Mustun
 */
class RS_ActionPolylineDelBetween : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetPolyline,             /**< Defining the polyline entity. */
        SetNode1,                /**< Setting the position of the first limiting node. */
        SetNode2                 /**< Setting the position of the second limiting node. */
    };

public:
    RS_ActionPolylineDelBetween(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    virtual ~RS_ActionPolylineDelBetween();

    static RS_String getName() {
        return "Polyline Del Between";
    }

    virtual RS_String name() {
        return RS_ActionPolylineDelBetween::getName();
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

protected:
    /**
     * Polyline entity we're working on.
     */
    RS_Polyline* polyline;
    /**
     * Polyline segment that was clicked.
     */
    RS_AtomicEntity* segment;
    /**
     * First limiting node.
     */
    RS_Vector node1;
    /**
     * Second limiting node.
     */
    RS_Vector node2;
};

#endif
