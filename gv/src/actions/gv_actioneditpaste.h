/*****************************************************************
 * $Id: gv_actioneditpaste.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:43:02 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actioneditpaste.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITPASTE_H
#define GV_ACTIONEDITPASTE_H

#include "gv_actionbase.h"
/*
#include "rs_modification.h"
#include "rs_clipboard.h"
*/
//#include "qg_dialogfactory.h"



#define GV_ACTION_EDIT_PASTE_ROTATION QObject::tr(GV_ActionEditPaste::propertyRotation)
#define GV_ACTION_EDIT_PASTE_FACTOR QObject::tr(GV_ActionEditPaste::propertyFactor)
#define GV_ACTION_EDIT_PASTE_FLIP_HORIZONTAL QObject::tr(GV_ActionEditPaste::propertyFlipHorizontal)
#define GV_ACTION_EDIT_PASTE_FLIP_VERTICAL QObject::tr(GV_ActionEditPaste::propertyFlipVertical)
#define GV_ACTION_EDIT_PASTE_CURRENT_LAYER QObject::tr(GV_ActionEditPaste::propertyCurrentLayer)



/**
 * This action class can handle user events for pasting components from
 * the clipboard into the current document.
 */
class GV_ActionEditPaste : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    /**
     * Action States.
     */
    enum Status {
        SetTargetPoint    /**< Setting the reference point. */
    };

public:
    GV_ActionEditPaste(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionEditPaste();

    static RB_String getName() {
        return "Edit Paste";
    }

    virtual RB_String name() {
        return GV_ActionEditPaste::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
/*
    virtual void init(int status=0);
    virtual void resume();
    
    virtual void finish() {
        GV_ActionBase::finish();
#ifdef RS_PROPERTYEDITOR
        RS_DIALOGFACTORY->updatePropertyEditor((RS_PropertyOwner*)NULL, false);
        RS_DIALOGFACTORY->updatePropertyEditorGui();
#endif
    }

    virtual void trigger();

    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    
    RS_StringList getPropertyNames(bool includeGeometry);

    virtual void setRotation(double a) {
        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_ROTATION, a);
    }
    
    virtual double getRotation() {
        return getProperty(RS_ACTION_EDIT_PASTE_ROTATION).toDouble();
    }
    
    double getFactor() {
        return getProperty(RS_ACTION_EDIT_PASTE_FACTOR).toDouble();
    }

    void setFactor(double f) {
        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FACTOR, f);
    }

    bool getFlipHorizontal() {
        return getProperty(RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL).toBool();
    }

    void setFlipHorizontal(bool fh) {
        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL, fh);
    }
    
    bool getFlipVertical() {
        return getProperty(RS_ACTION_EDIT_PASTE_FLIP_VERTICAL).toBool();
    }

    void setFlipVertical(bool fv) {
        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FLIP_VERTICAL, fv);
    }

    bool getUseCurrentLayer() {
        return getProperty(RS_ACTION_EDIT_PASTE_CURRENT_LAYER).toBool();
    }

    void setUseCurrentLayer(bool cl) {
        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_CURRENT_LAYER, cl);
    }
    

protected:
    RS_PasteData data;
*/
private:
    static const char* propertyRotation;
    static const char* propertyFactor;
    static const char* propertyFlipHorizontal;
    static const char* propertyFlipVertical;
    static const char* propertyCurrentLayer;
};

#endif
