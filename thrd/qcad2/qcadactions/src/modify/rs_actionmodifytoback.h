/****************************************************************************
** $Id: rs_actionmodifydelete.h 2487 2005-07-20 00:30:38Z andrew $
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

#ifndef RS_ACTIONMODIFYTOBACK_H
#define RS_ACTIONMODIFYTOBACK_H

#include "rs_actionbase.h"


/**
 * This action class can handle user events to move entities to
 * the background / foreground.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionModifyToBack : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        Acknowledge    /**< Acknowledge or cancel. */
    };

public:
    RS_ActionModifyToBack(RS_EntityContainer& container,
                          RS_GraphicView& graphicView,
                          bool toBack = true);
    ~RS_ActionModifyToBack() {}

    static RS_String getName() {
        return "Modify To Back";
    }

    virtual RS_String name() {
        return RS_ActionModifyToBack::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void init(int status=0);
    virtual void trigger();
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

private:
    bool toBack;
};

#endif
