/*****************************************************************
 * $Id: db_actionfileprintpreview.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEPRINTPREVIEW_H
#define DB_ACTIONFILEPRINTPREVIEW_H

#include "rb_action.h"

/**
 * Default action for print preview.
 */
class DB_EXPORT DB_ActionFilePrintPreview : public RB_Action {

    Q_OBJECT

public:
    /**
     * Action States.
     */
    enum Status {
        Neutral,
        Moving
    };

public:
    DB_ActionFilePrintPreview();
    virtual ~DB_ActionFilePrintPreview();

    static RB_String getName() {
        return "File Print Preview";
    }

    virtual RB_String name() {
        return DB_ActionFilePrintPreview::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

public slots:
    void slotFilePrintPreview(QPrinter* pr);

/*
    virtual void init(int status=0);

    virtual void trigger();

    virtual void mouseMoveEvent(RB_MouseEvent* e);
    virtual void mousePressEvent(RB_MouseEvent* e);
    virtual void mouseReleaseEvent(RB_MouseEvent* e);

    virtual void coordinateEvent(RB_CoordinateEvent* e);
    virtual void commandEvent(DB_CommandEvent* e);
    virtual RB_StringList getAvailableCommands();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    void center();
    void fit();
    void setScale(double f);
    double getScale();

    void setBlackWhite(bool bw);
    //bool isBlackWhite() {
    //    return blackWhite;
    //}
    RS2::Unit getUnit();

protected:
    //bool blackWhite;
    RVector v1;
    RVector v2;
*/
};

#endif
