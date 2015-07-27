/*****************************************************************
 * $Id: gv_actionformatlineattributes.h 1202 2010-06-14 20:59:27Z rutger $
 * Created: June 14, 2010 10:00:51 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFORMATLINEATTRIBUTES_H
#define GV_ACTIONFORMATLINEATTRIBUTES_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to set the line properties
 */
class GV_ActionFormatLineAttributes : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionFormatLineAttributes(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionFormatLineAttributes();

    static RB_String getName() {
        return "Set Line Attributes";
    }

    virtual RB_String name() {
        return GV_ActionFormatLineAttributes::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    virtual void init(int status = 0);
    virtual void trigger();
    
protected:
    
};

#endif
