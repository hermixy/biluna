/****************************************************************************
** $Id: rs_actiondrawline.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWLINE_H
#define RS_ACTIONDRAWLINE_H

#include "rs_actionbase.h"
#include "rs_line.h"
#include "rs_valuelist.h"

#include <qaction.h>


/**
 * This action class can handle user events to draw 
 * simple lines with the start- and endpoint given.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDrawLine : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,   /**< Setting the startpoint.  */
        SetEndpoint      /**< Setting the endpoint. */
    };

public:
    RS_ActionDrawLine(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    virtual ~RS_ActionDrawLine();

    static RS_String getName() {
        return "Draw Line";
    }

    virtual RS_String name() {
        return RS_ActionDrawLine::getName();
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

    void close();
    void undo();

protected:
    /**
    * Line data defined so far.
    */
    RS_LineData data;
    /**
     * Start point of the series of lines. Used for close function.
     */
    RS_Vector start;

    /**
     * Point history (for undo)
     */
    RS_ValueList<RS_Vector> history;
    
};

#endif
