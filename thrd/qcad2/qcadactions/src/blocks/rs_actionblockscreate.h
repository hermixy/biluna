/****************************************************************************
** $Id: rs_actionblockscreate.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONBLOCKSCREATE_H
#define RS_ACTIONBLOCKSCREATE_H

#include "rs_actioninterface.h"
#include "rs_insert.h"

/**
 * This action class can handle user events for creating blocks from 
 * existing entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionBlocksCreate : public RS_ActionInterface {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetReferencePoint,    /**< Setting the reference point. */
        ShowDialog            /**< Showing dialog for name. */
    };

public:
    RS_ActionBlocksCreate(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionBlocksCreate();
    
    static RS_String getName() {
        return "Create Block From Entities";
    }

    virtual RS_String name() {
        return RS_ActionBlocksCreate::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    virtual bool isExclusive() {
        return true;
    }

    virtual void init(int status=0);

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    //virtual void commandEvent(RS_CommandEvent* e);
    //virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

protected:
    RS_Vector referencePoint;
};

#endif
