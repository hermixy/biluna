/****************************************************************************
** $Id: rs_actionmodifydeletefree.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONMODIFYDELETEFREE_H
#define RS_ACTIONMODIFYDELETEFREE_H

#include "rs_actionbase.h"


/**
 * This action class can handle user events to delete entities.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyDeleteFree : public RS_ActionBase {
    Q_OBJECT
public:
    RS_ActionModifyDeleteFree(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionModifyDeleteFree() {}

    static RS_String getName() {
        return "Modify Delete Free";
    }

    virtual RS_String name() {
        return RS_ActionModifyDeleteFree::getName();
    }

    static RS_GuiAction* createGuiAction();

    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();

private:
    RS_Polyline* polyline;
    RS_Entity* e1;
    RS_Vector v1;
    RS_Entity* e2;
    RS_Vector v2;
};

#endif
