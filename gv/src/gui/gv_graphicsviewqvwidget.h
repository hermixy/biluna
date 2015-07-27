#ifndef GV_GRAPHICSVIEWQVWIDGET_H
#define GV_GRAPHICSVIEWQVWIDGET_H

#include "rb_widget.h"
#include "ui_gv_graphicsviewqvwidget.h"

class GV_EventHandlerQV;

class GV_GraphicsViewQVWidget : public RB_Widget, protected Ui::GV_GraphicsViewQVWidget {

    Q_OBJECT

public:
    explicit GV_GraphicsViewQVWidget(QWidget *parent = 0);
    ~GV_GraphicsViewQVWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "GraphicsView"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }

    virtual void init();

private:
    GV_EventHandlerQV* mEventHandler;
};

#endif // GV_GRAPHICSVIEWQVWIDGET_H
