#ifndef GV_GRAPHICSVIEWQTWIDGET_H
#define GV_GRAPHICSVIEWQTWIDGET_H

#include "rb_widget.h"
#include "ui_gv_graphicsviewqtwidget.h"

class GV_EventHandlerQt;

class GV_GraphicsViewQtWidget : public RB_Widget, protected Ui::GV_GraphicsViewQtWidget {

    Q_OBJECT

public:
    explicit GV_GraphicsViewQtWidget(QWidget *parent = 0);
    ~GV_GraphicsViewQtWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "GraphicsViewQt"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }

    virtual void init();


private:
    GV_EventHandlerQt* mEventHandler;

};

#endif // GV_GRAPHICSVIEWQTWIDGET_H
