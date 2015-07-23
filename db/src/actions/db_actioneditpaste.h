/*****************************************************************
 * $Id: db_actioneditpaste.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITPASTE_H
#define DB_ACTIONEDITPASTE_H

#include "rb_action.h"

/**
 * This action class can handle user events for pasting a selection from
 * the clipboard into the current document.
 */
class DB_EXPORT DB_ActionEditPaste : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditPaste();
    virtual ~DB_ActionEditPaste() {}

    static RB_String getName() {
        return "Edit Paste";
    }

    virtual RB_String name() {
        return DB_ActionEditPaste::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();


//public:
//    /**
//     * Action States.
//     */
//    enum Status {
//        SetTargetPoint    /**< Setting the reference point. */
//    };
//
//public:
//    RS_ActionEditPaste(RS_EntityContainer& container,
//                       RS_GraphicView& graphicView);
//    ~RS_ActionEditPaste();
//
//    static RS_String getName() {
//        return "Edit Paste";
//    }
//
//    virtual RS_String name() {
//        return RS_ActionEditPaste::getName();
//    }
//
//    static RS_GuiAction* createGuiAction();
//    static RS_ActionInterface* factory();
//
//    virtual void init(int status=0);
//    virtual void resume();
//
//    virtual void finish() {
//        RS_ActionBase::finish();
//#ifdef RS_PROPERTYEDITOR
//        RS_DIALOGFACTORY->updatePropertyEditor((RS_PropertyOwner*)NULL, false);
//        RS_DIALOGFACTORY->updatePropertyEditorGui();
//#endif
//    }
//
//    virtual void trigger();
//
//    virtual void mouseMoveEvent(RS_MouseEvent* e);
//    virtual void mouseReleaseEvent(RS_MouseEvent* e);
//
//    virtual void coordinateEvent(RS_CoordinateEvent* e);
//
//    virtual void updateMouseButtonHints();
//    virtual void updateMouseCursor();
//    virtual void updateToolBar();
//
//    RS_StringList getPropertyNames(bool includeGeometry);
//
//    virtual void setRotation(double a) {
//        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_ROTATION, a);
//    }
//
//    virtual double getRotation() {
//        return getProperty(RS_ACTION_EDIT_PASTE_ROTATION).toDouble();
//    }
//
//    double getFactor() {
//        return getProperty(RS_ACTION_EDIT_PASTE_FACTOR).toDouble();
//    }
//
//    void setFactor(double f) {
//        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FACTOR, f);
//    }
//
//    bool getFlipHorizontal() {
//        return getProperty(RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL).toBool();
//    }
//
//    void setFlipHorizontal(bool fh) {
//        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL, fh);
//    }
//
//    bool getFlipVertical() {
//        return getProperty(RS_ACTION_EDIT_PASTE_FLIP_VERTICAL).toBool();
//    }
//
//    void setFlipVertical(bool fv) {
//        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_FLIP_VERTICAL, fv);
//    }
//
//    bool getUseCurrentLayer() {
//        return getProperty(RS_ACTION_EDIT_PASTE_CURRENT_LAYER).toBool();
//    }
//
//    void setUseCurrentLayer(bool cl) {
//        RS_PropertyOwner::setProperty(RS_ACTION_EDIT_PASTE_CURRENT_LAYER, cl);
//    }
//
//
//protected:
//    RS_PasteData data;
//
//private:
//    static const char* propertyRotation;
//    static const char* propertyFactor;
//    static const char* propertyFlipHorizontal;
//    static const char* propertyFlipVertical;
//    static const char* propertyCurrentLayer;
};

#endif
