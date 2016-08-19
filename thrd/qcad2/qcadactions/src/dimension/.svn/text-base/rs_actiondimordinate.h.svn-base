/****************************************************************************
** $Id: rs_actiondimaligned.h 7068 2007-11-13 09:37:53Z andrew $
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

#ifndef RS_ACTIONDIMORDINATE_H
#define RS_ACTIONDIMORDINATE_H

#include "rs_actiondimension.h"
#include "rs_dimordinate.h"

/**
 * This action class can handle user events to draw 
 * ordinate dimensions.
 *
 * @author Andrew Mustun
 */
class RS_ActionDimOrdinate : public RS_ActionDimension {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetDefPoint1,    /**< Setting the 1st ext point.  */
        SetDefPoint2,    /**< Setting the 2nd ext point. */
        SetText          /**< Setting the text label in command line */
    };

public:
    RS_ActionDimOrdinate(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionDimOrdinate();
    
    static RS_String getName() {
        return "Draw Ordinate Dimensions";
    }

    virtual RS_String name() {
        return RS_ActionDimOrdinate::getName();
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
    
    virtual void updateMouseButtonHints();

protected:
    /**
     * Ordinate dimension data.
     */
    RS_DimOrdinateData edata;

    /** Last status before entering text. */
    Status lastStatus;
}
;

#endif
