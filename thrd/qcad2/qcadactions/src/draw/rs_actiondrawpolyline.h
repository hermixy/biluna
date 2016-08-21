/****************************************************************************
** $Id: rs_actiondrawpolyline.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWPOLYLINE_H
#define RS_ACTIONDRAWPOLYLINE_H

#include "rs_actionbase.h"
#include "rs_polyline.h"
#include "rs_list.h"

#include <qaction.h>


/**
 * This action class can handle user events to draw 
 * polylines.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDrawPolyline : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,    /**< Setting the startpoint.  */
        SetNextPoint,     /**< Setting the next vertex behind the endpoint. */
        SetPrevPoint      /**< Setting the next vertex before the startpoint. */
    };

public:
    RS_ActionDrawPolyline(RS_EntityContainer& container,
                          RS_GraphicView& graphicView);
    virtual ~RS_ActionDrawPolyline();

    static RS_String getName() {
        return "Draw Polyline";
    }

    virtual RS_String name() {
        return RS_ActionDrawPolyline::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    void reset();

    virtual void init(int status=0);
    void initForAppend(RS_Polyline* pl, RS_AtomicEntity* sg);
    void backToAppend();
    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    void close();
    void undo();

    RS_ArcData getNextArcData(const RS_Vector& nextPos, bool* ok=NULL);

    void setRadius(double r);
    double getRadius() {
        return nextRadius;
    }
    void setArc(bool on);
    bool isArc() {
        return nextArc;
    }

protected:
    /**
     * Polyline entity we're working on.
     */
    RS_Polyline* polyline;

    /**
     * last point.
     */
    RS_Vector point;

    /**
     * Start point of the series of lines. Used for close function.
     */
    RS_Vector start;

    /**
     * Point history (for undo)
     */
    RS_List<RS_Vector> history;

    /**
     * Bulge history (for undo)
     */
    RS_List<double> bHistory;

    /**
     * Next segment is an arc.
     */
    bool nextArc;

    /**
     * Radius of next segment.
     */
    double nextRadius;
    /**
     * We're appending to either start or end of polyline.
     */
    bool append;
};

#endif
