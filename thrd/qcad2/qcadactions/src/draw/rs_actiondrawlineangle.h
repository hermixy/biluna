/****************************************************************************
** $Id: rs_actiondrawlineangle.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONDRAWLINEANGLE_H
#define RS_ACTIONDRAWLINEANGLE_H

#include "rs_actionbase.h"
#include "rs_line.h"



/**
 * This action class can handle user events to draw 
 * simple lines at a given angle.
 *
 * @author Andrew Mustun
 */
class RS_ActionDrawLineAngle : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetPos,       /**< Setting the position.  */
        SetAngle,     /**< Setting angle in the command line. */
        SetLength     /**< Setting length in the command line. */
    };

    RS_ActionDrawLineAngle(RS_EntityContainer& container,
                           RS_GraphicView& graphicView,
                           double angle=0.0,
                           bool fixedAngle=false);
    virtual ~RS_ActionDrawLineAngle();

    static RS_String getName() {
        return "Draw Line Angle";
    }

    virtual RS_String name() {
        return RS_ActionDrawLineAngle::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    void reset();

    virtual void init(int status=0);
    
    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    void preparePreview();
    
    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    
    //virtual RS_Widget* createOptionWidget();

    void setSnapReference(int sp) {
        snapReference = sp;
    }

    int getSnapReference() {
        return snapReference;
    }

    void setAngle(double a) {
        angle = a;
    }

    double getAngle() {
        return angle;
    }

    void setLength(double l) {
        length = l;
    }

    double getLength() {
        return length;
    }

    bool hasFixedAngle() {
        return fixedAngle;
    }

protected:
    /**
     * Line data defined so far.
     */
    RS_LineData data;
    /**
     * Position.
     */
    RS_Vector pos;
    /**
     * Line angle.
     */
    double angle;
    /**
     * Line length.
     */
    double length;
    /**
     * Is the angle fixed?
     */
    bool fixedAngle;
    /**
     * Snap point (start, middle, end).
     */
    int snapReference;
};

#endif

