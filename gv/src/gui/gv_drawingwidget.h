/*****************************************************************
 * $Id: gv_drawingwidget.h 1337 2010-12-27 23:20:25Z rutger $
 * Created: Apr 19, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_DRAWINGWIDGET_H
#define GV_DRAWINGWIDGET_H

#include <QTextEdit>
#include "gv_drawing.h"
#include "gv_graphicsview.h"
#include "rb_widget.h"
#include "ui_gv_drawingwidget.h"


/**
 * Widget with drawing
 */
class GV_DrawingWidget : public RB_Widget, protected Ui::GV_DrawingWidget {

    Q_OBJECT

public:
    GV_DrawingWidget(QWidget *parent = 0);
    virtual ~GV_DrawingWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Drawing"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }

    virtual void init();
    virtual bool fileSave();
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void formatBold();
    virtual void formatItalic();
    virtual void formatUnderline();
    virtual void formatAlignLeft();
    virtual void formatAlignCenter();
    virtual void formatAlignRight();
    virtual void formatFontAll();
    virtual void formatFontName();
    virtual void formatFontSize();
    virtual void formatTextColor();
    virtual void formatRemove();

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String& fn);
    virtual bool closeWidget();

    virtual GV_Drawing* getDrawing() { return mDrawing; }
    virtual GV_GraphicsView* getGraphicsView() { return mGraphicsView; }

public slots:
    virtual void adjustActions();
    virtual void slotMousePressed();

protected:
    bool maybeSave();
    void setCurrentFileName(const QString& fileName);
    void changeEvent(QEvent* e);

    //! File full name
    RB_String mFullFileName;
    //! Drawinglist model
    RB_MmProxy* mModel;
    //! Current drawing
    GV_Drawing* mDrawing;
    //! Graphicsview showing the drawing
    GV_GraphicsView* mGraphicsView;
    //! To use the QTextEdit text edit functions
    QTextEdit mTextEdit;

};

#endif // GV_DRAWINGWIDGET_H
