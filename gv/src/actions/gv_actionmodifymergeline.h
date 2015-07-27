/*****************************************************************
 * $Id: gv_actionmodifymergeline.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Feb 18, 2009 2:22:48 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONMODIFYMERGELINE_H
#define GV_ACTIONMODIFYMERGELINE_H

#include "gv_actionbase.h"
#include "gv_line.h"


/**
 * This action class can handle user events merge two lines based
 * on the user selecting the interconnecting node/port
 */
class GV_ActionModifyMergeLine : public GV_ActionBase {
	
    Q_OBJECT
    
public:
	GV_ActionModifyMergeLine(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionModifyMergeLine() {}

    static RB_String getName() {
        return "Merge lines";
    }

    virtual RB_String name() {
        return GV_ActionModifyMergeLine::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    virtual void finish();
    virtual void init(int status=0);
    virtual void trigger(GV_Port* selPort);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    
private:
    static RB_GuiAction* action;

};

#endif /*GV_ACTIONMODIFYMERGELINE_H*/
