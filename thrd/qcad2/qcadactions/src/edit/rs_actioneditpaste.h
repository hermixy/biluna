/****************************************************************************
** $Id: rs_actioneditpaste.h 10343 2008-04-06 17:43:02Z andrew $
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

#ifndef RS_ACTIONEDITPASTE_H
#define RS_ACTIONEDITPASTE_H

#include "rs_actionbase.h"

#include "rs_modification.h"
#include "rs_clipboard.h"

//#include "qg_dialogfactory.h"



#define RS_ACTION_EDIT_PASTE_ROTATION QObject::tr(RS_ActionEditPaste::propertyRotation)
#define RS_ACTION_EDIT_PASTE_FACTOR QObject::tr(RS_ActionEditPaste::propertyFactor)
#define RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL QObject::tr(RS_ActionEditPaste::propertyFlipHorizontal)
#define RS_ACTION_EDIT_PASTE_FLIP_VERTICAL QObject::tr(RS_ActionEditPaste::propertyFlipVertical)
#define RS_ACTION_EDIT_PASTE_CURRENT_LAYER QObject::tr(RS_ActionEditPaste::propertyCurrentLayer)



/**
 * This action class can handle user events for pasting entities from
 * the clipboard into the current document.
 *
 * @author Andrew Mustun
 */
class RS_ActionEditPaste : public RS_ActionBase {
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status {
        SetTargetPoint    /**< Setting the reference point. */
    };

public:
    RS_ActionEditPaste(RS_EntityContainer& container,
                       RS_GraphicView& graphicView);
    ~RS_ActionEditPaste();

    static RS_String getName() {
        return "Edit Paste";
    }

    virtual RS_String name() {
        return RS_ActionEditPaste::getName();
    }

    static RS_GuiAction* createGuiAction();
    static RS_ActionInterface* factory();

    virtual void init(int status=0);
    virtual void resume();
    
    virtual void finish() {
        RS_ActionBase::finish();
        RS_DIALOGFACTORY->updatePropertyEditor((RS_PropertyOwner*)NULL, false);
        RS_DIALOGFACTORY->updatePropertyEditorGui();
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

private:
    static const char* propertyRotation;
    static const char* propertyFactor;
    static const char* propertyFlipHorizontal;
    static const char* propertyFlipVertical;
    static const char* propertyCurrentLayer;
};

#endif
