/****************************************************************************
** $Id: rs_actiondimangular.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDIMANGULAR_H
#define RS_ACTIONDIMANGULAR_H

#include "rs_actiondimension.h"
#include "rs_dimangular.h"


/**
 * This action class can handle user events to draw angular dimensions.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDimAngular : public RS_ActionDimension {
    Q_OBJECT
private:
    enum Status {
        SetLine1,      /**< Choose 1st line. */
        SetLine2,      /**< Choose 2nd line. */
        SetPos,        /**< Choose position. */
        SetText        /**< Setting text label in console. */
    };

public:
    RS_ActionDimAngular(RS_EntityContainer& container,
                              RS_GraphicView& graphicView);
    ~RS_ActionDimAngular() {}
    
    static RS_String getName() {
        return "Draw Angular Dimension";
    }

    virtual RS_String name() {
        return RS_ActionDimAngular::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void reset();
    
    virtual void trigger();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateToolBar();
    virtual void updateMouseButtonHints();

private:
    /** 1st chosen line */
    RS_Line* line1;
    /** 2nd chosen line */
    RS_Line* line2;
    /** Base arc */
    RS_Arc* arc;
    /** Center of arc */
    RS_Vector center;
    /** Data of new dimension */
    RS_DimAngularData edata;
    /** Last status before entering text. */
    Status lastStatus;
};

#endif
