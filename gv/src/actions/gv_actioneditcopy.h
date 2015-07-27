/*****************************************************************
 * $Id: gv_actioneditcopy.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 4, 2008 6:28:41 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcopy.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITCOPY_H
#define GV_ACTIONEDITCOPY_H
/*
#include "rs_actionbase.h"
*/
#include "gv_actionbase.h"

/**
 * This action class can handle user events for copying or cutting entities 
 * to the clipboard.
 */
class GV_ActionEditCopy : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    /**
     * Action States.
     */
    enum Status {
        SetReferencePoint    /**< Setting the reference point. */
    };
    
public:
    GV_ActionEditCopy(GV_Drawing& dwg, GV_GraphicsView& gv,
                      bool copy,
                      bool autoRef);
    ~GV_ActionEditCopy();

    static RB_String getName() {
        return "Edit Copy";
    }

    virtual RB_String name() {
        return GV_ActionEditCopy::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
/*
    virtual void init(int status=0);

    virtual void trigger();
    
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    
    virtual void coordinateEvent(RS_CoordinateEvent* e);
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
*/
protected:
    /** Copy (true) or cut (false) */
    bool copy;
    /** Auto reference point on copy / cut (true) or 
        set reference point manually (false) */
    bool autoRef;

    RVector referencePoint;
};

#endif
