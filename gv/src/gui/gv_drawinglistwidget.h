/*****************************************************************
 * $Id: gv_drawinglistwidget.h 1450 2011-08-29 17:40:53Z rutger $
 * Created: Apr 20, 2010 4:47:46 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_DRAWINGLISTWIDGET_H
#define GV_DRAWINGLISTWIDGET_H

#include "rb_widget.h"
#include "ui_gv_drawinglistwidget.h"

class RB_MmProxy;

/**
 * Widget for the GV_Drawing list
 */
class GV_DrawingListWidget : public RB_Widget, private Ui::GV_DrawingListWidget {

    Q_OBJECT

public:
    GV_DrawingListWidget(QWidget *parent = 0);
    virtual ~GV_DrawingListWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Drawing List"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }

    virtual void init();
    virtual bool closeWidget() { return true; }

//    QTableView* getTvManager() { return tvDrawingList; }
//    QItemSelectionModel* getSelectionModel() { return mSelectionModel; } // TODO: not required refer RB_MmProxy

public slots:
    virtual void on_pbAddDrawing_clicked();
    virtual void on_pbDeleteDrawing_clicked();

    virtual void slotDrawingListDoubleClicked(const QModelIndex& idx);

protected:
    void changeEvent(QEvent *e);

private:
    //! Data model
    RB_MmProxy* mModel;

//    QItemSelectionModel* mSelectionModel; // required?
};

#endif // GV_DRAWINGLISTWIDGET_H

