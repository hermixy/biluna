/*****************************************************************
 * $Id: gv_previewwidget.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Sep 30, 2008 12:01:42 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PREVIEWWIDGET_H
#define GV_PREVIEWWIDGET_H

#include <QtWidgets>
#include "rb_widget.h"
#include "ui_gv_previewwidget.h"


/**
 * This is the widget for the preview of a drawing
 */
class GV_PreviewWidget  : public RB_Widget, private Ui::GV_PreviewWidget {
	
	Q_OBJECT
	
public:
	GV_PreviewWidget(QWidget* parent);
	virtual ~GV_PreviewWidget();
	
    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return RB_String(); }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }
    virtual bool closeWidget() { return true; }

	virtual QAction* toggleViewAction();
	
//	QGraphicsView* getGvPreview() { return gvPreview; }

public slots:
	void slotPreviewDrawing(const QModelIndex& current);
};

#endif /*GV_PREVIEWWIDGET_H*/
