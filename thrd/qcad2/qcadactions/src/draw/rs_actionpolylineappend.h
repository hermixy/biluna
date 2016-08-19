/****************************************************************************
** $Id: rs_actionpolylineappend.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONPOLYLINEAPPEND_H
#define RS_ACTIONPOLYLINEAPPEND_H

#include "rs_actionbase.h"
#include "rs_polyline.h"

#include <qaction.h>


/**
 * This action class can handle user events to append nodes to 
 * polylines.
 *
 * @author Andrew Mustun
 */
class RS_ActionPolylineAppend : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetPolylineSegment       /**< Setting the polyline segment (start or end). */
    };

public:
    RS_ActionPolylineAppend(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    virtual ~RS_ActionPolylineAppend();

    static RS_String getName() {
        return "Polyline Append";
    }

    virtual RS_String name() {
        return RS_ActionPolylineAppend::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    void reset();

    virtual void init(int status=0);
    virtual void trigger();
    
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /**
     * Polyline entity we're working on.
     */
    RS_Polyline* polyline;
    /**
     * Polyline segment where the node will be added.
     */
    RS_AtomicEntity* segment;
};

#endif
