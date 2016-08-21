/****************************************************************************
** $Id: rs_actionselect.h 10341 2008-04-06 17:41:55Z andrew $
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

#ifndef RS_ACTIONSELECT_H
#define RS_ACTIONSELECT_H

#include "rs_actioninterface.h"


/**
 * This action class handles user events to select entities
 * with any of the available selection functions.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionSelect : public RS_ActionInterface {
    Q_OBJECT
public:
    RS_ActionSelect(RS_EntityContainer& container,
                    RS_GraphicView& graphicView,
                    RS_ActionInterface* (*f)());
    //RS_ActionSelect(RS_EntityContainer& container,
    //                RS_GraphicView& graphicView);
    ~RS_ActionSelect() {}

    static RS_String getName() {
        return "Select";
    }

    virtual RS_String name() {
        return RS_ActionSelect::getName();
    }

    virtual bool isPersistent() {
        return true;
    }

    void init(int status);
    virtual void finish();
    //virtual void keyPressEvent(RS_KeyEvent* e);
    void mouseReleaseEvent(RS_MouseEvent* e);
    void updateToolBar();

private:
    RS_ActionInterface* (*nextActionFactory)();
};

#endif
