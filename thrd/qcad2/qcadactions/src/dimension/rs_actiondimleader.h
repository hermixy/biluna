/****************************************************************************
** $Id: rs_actiondimleader.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDIMLEADER_H
#define RS_ACTIONDIMLEADER_H

#include "rs_actionbase.h"
#include "rs_leader.h"
#include "rs_valuelist.h"

/**
 * This action class can handle user events to draw 
 * leaders (arrows).
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDimLeader : public RS_ActionBase {
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
    RS_ActionDimLeader(RS_EntityContainer& container,
                      RS_GraphicView& graphicView);
    ~RS_ActionDimLeader();
    
    static RS_String getName() {
        return "Draw Leaders";
    }

    virtual RS_String name() {
        return RS_ActionDimLeader::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    void reset();

    virtual void init(int status=0);
    virtual void trigger();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void keyPressEvent(RS_KeyEvent*);
    
    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    /**
     * Leader entity.
     */
    RS_Leader* leader;
    /**
     * Points set so far.
     */
    RS_ValueList<RS_Vector> points;

};

#endif
