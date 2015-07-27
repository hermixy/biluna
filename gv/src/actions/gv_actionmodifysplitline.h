/*****************************************************************
 * $Id: gv_actionmodifysplitline.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Feb 18, 2009 1:07:16 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONMODIFYSPLITLINE_H
#define GV_ACTIONMODIFYSPLITLINE_H

#include "gv_actionbase.h"
#include "gv_line.h"


/**
 * This action class can handle user events to cut a line in two segments
 */
class GV_ActionModifySplitLine : public GV_ActionBase {
	
    Q_OBJECT
    
public:
	GV_ActionModifySplitLine(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionModifySplitLine() {}

    static RB_String getName() {
        return "Split line";
    }

    virtual RB_String name() {
        return GV_ActionModifySplitLine::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    virtual void finish();
    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    
private:
    static RB_GuiAction* action;

};

#endif /*GV_ACTIONMODIFYSPLITLINE_H*/
