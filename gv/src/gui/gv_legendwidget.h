/*****************************************************************
 * $Id: gv_legendwidget.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 1, 2008 7:00:13 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_LEGENDWIDGET_H
#define GV_LEGENDWIDGET_H

#include <QtWidgets>
#include "rb_widget.h"
#include "ui_gv_legendwidget.h"

class GV_Component;
class GV_Drawing;
class GV_GraphicsView;
class GV_GvLegend;
class GV_Scene;


/**
 * This is the widget for the legend of symbols that can be used in a drawing
 */
class GV_LegendWidget : public RB_Widget, private Ui::GV_LegendWidget {
	
	Q_OBJECT
	
	// for access to protected itemSelected()
	friend class GV_GvLegend; 
	
public:
	GV_LegendWidget(QWidget* parent);
	virtual ~GV_LegendWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return RB_String(); }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }
    virtual bool closeWidget() { return true; }

	virtual void init(GV_Drawing* dwg);
	virtual QAction* toggleViewAction();
	
	virtual GV_GvLegend* getGvLegend() {
		return mGvLegend;
	}
	// virtual GV_Component* getSelectedComponent();


//public slots:
//	void previewDrawing(const QModelIndex& current, 
//									const QModelIndex& previous);
	
	
private:
	GV_GvLegend* mGvLegend;
	GV_Drawing* mLegend;
};

#endif /*GV_LEGENDWIDGET_H*/
