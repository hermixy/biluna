/****************************************************************************
** $Id: rs_actiondimension.h 8361 2008-01-23 15:20:59Z andrew $
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

#ifndef RS_ACTIONDIMENSION_H
#define RS_ACTIONDIMENSION_H

#include "rs_actioninterface.h"
#include "rs_dimension.h"

/**
 * Base class for dimension actions.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionDimension : public RS_ActionInterface {
    Q_OBJECT
public:
    RS_ActionDimension(RS_EntityContainer& container,
                       RS_GraphicView& graphicView);
    ~RS_ActionDimension();

    virtual void reset();

    virtual void init(int status=0);

    virtual void updateMouseCursor();
    virtual void updateToolBar();

    virtual bool optionsFromAction();

    RS_String getText() {
        if (!data.text.isEmpty()) {
            return data.text;
        }
    
        RS_String l = label;

        if (l.isEmpty() &&
            (diameter==true || !tol1.isEmpty() || !tol2.isEmpty())) {
            l = "<>";
        }

        if (diameter==true) {
            l = RS_Char(0x2205) + l;
        }

        if (!tol1.isEmpty() || !tol2.isEmpty()) {
            l += RS_String("\\S%1\\%2;").arg(tol1).arg(tol2);
        }

        return l;
    }
    
    void setText(const RS_String& t) {
        data.text = t;
    }

    RS_String getLabel() {
        return label;
    }
    void setLabel(const RS_String& t) {
        label = t;
    }
    RS_String getTol1() {
        return tol1;
    }
    void setTol1(const RS_String& t) {
        tol1 = t;
    }
    RS_String getTol2() {
        return tol2;
    }
    void setTol2(const RS_String& t) {
        tol2 = t;
    }
    bool getDiameter() {
        return diameter;
    }
    void setDiameter(bool d) {
        diameter = d;
    }

    /*
    static bool isDimensionAction(RS2::ActionType type) {
        return (type==RS2::ActionDimAligned ||
                type==RS2::ActionDimLinear ||
                type==RS2::ActionDimAngular ||
                type==RS2::ActionDimDiametric ||
                type==RS2::ActionDimRadial);
    }
    */

protected:
    /**
     * Generic dimension data.
     */
    RS_DimensionData data;

    RS_String label;
    RS_String tol1;
    RS_String tol2;
    bool diameter;
};

#endif
