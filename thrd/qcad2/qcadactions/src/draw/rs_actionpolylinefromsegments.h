/****************************************************************************
** $Id: rs_actiondrawlineparallel.h 1092 2004-03-16 18:01:08Z andrew $
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

#ifndef RS_ACTIONPOLYLINEFROMSEGMENTS_H
#define RS_ACTIONPOLYLINEFROMSEGMENTS_H

#include "rs_actionbase.h"


/**
 * This action class can handle user events to create
 * polylines from existing segments.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionPolylineFromSegments : public RS_ActionBase {
    Q_OBJECT
private:
    enum Status {
        SetEntity    /**< Choose initial segment. */
    };
    
public:
    RS_ActionPolylineFromSegments(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionPolylineFromSegments() {}

    static RS_String getName() {
        return "Polyline From Segments";
    }

    virtual RS_String name() {
        return RS_ActionPolylineFromSegments::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    virtual void trigger();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    /** Original entity. */
    RS_Entity* entity;
};

#endif
