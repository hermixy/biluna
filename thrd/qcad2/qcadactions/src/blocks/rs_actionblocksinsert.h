/****************************************************************************
** $Id: rs_actionblocksinsert.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONBLOCKSINSERT_H
#define RS_ACTIONBLOCKSINSERT_H

#include "rs_actionbase.h"
#include "rs_insert.h"

/**
 * This action class can handle user events for inserting blocks into the
 * current drawing.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_ActionBlocksInsert : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetTargetPoint,    /**< Setting the reference point. */
        SetAngle,          /**< Setting angle in the command line. */
        SetFactor,         /**< Setting factor in the command line. */
        SetColumns,        /**< Setting columns in the command line. */
        SetRows,           /**< Setting rows in the command line. */
        SetColumnSpacing,  /**< Setting column spacing in the command line. */
        SetRowSpacing      /**< Setting row spacing in the command line. */
    };

public:
    RS_ActionBlocksInsert(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionBlocksInsert();
    
    static RS_String getName() {
        return "Insert Block";
    }

    virtual RS_String name() {
        return RS_ActionBlocksInsert::getName();
    }
    
    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();
    
    virtual void init(int status=0);

    void reset();

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual RS_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    
    double getAngle() {
        return data.angle;
    }

    void setAngle(double a) {
        data.angle = a;
    }

    double getFactor() {
        return data.scaleFactor.x;
    }

    void setFactor(double f) {
        data.scaleFactor = RS_Vector(f, f);
    }

    int getColumns() {
        return data.cols;
    }

    void setColumns(int c) {
        data.cols = c;
    }
    
    int getRows() {
        return data.rows;
    }

    void setRows(int r) {
        data.rows = r;
    }

    double getColumnSpacing() {
        return data.spacing.x;
    }

    void setColumnSpacing(double cs) {
        data.spacing.x = cs;
    }
    
    double getRowSpacing() {
        return data.spacing.y;
    }

    void setRowSpacing(double rs) {
        data.spacing.y = rs;
    }

protected:
    RS_Block* block;
    RS_InsertData data;
    
    /** Last status before entering option. */
    Status lastStatus;
};

#endif
