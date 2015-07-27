/*****************************************************************
 * $Id: gv_previewwidget.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Sep 30, 2008 12:02:07 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_previewwidget.h"

#include "gv_dialogfactory.h"
#include "gv_drawing.h"
//#include "gv_mmdrawing.h"
#include "gv_graphicsscene.h"

GV_PreviewWidget::GV_PreviewWidget(QWidget* parent) 
                                : RB_Widget(parent){
	
	setupUi(this);
	
}

GV_PreviewWidget::~GV_PreviewWidget() {
	
}


void GV_PreviewWidget::slotPreviewDrawing(const QModelIndex& current) {
	RB_DEBUG->print("GV_PreviewWidget::previewDrawing()");

	if (!gvPreview)  return;
	
//	// update preview
//	RB_ObjectBase* obj = GV_MMDRAWING->getObject(current);
//	GV_Drawing* dwg = dynamic_cast<GV_Drawing*>(obj);
//
//	// show and synchronize drawing entities only if not in a open window
//	// and scene is not already in preview widget
//	if (!GV_DIALOGFACTORY->isDrawingOpen(dwg) && gvPreview->scene() != dwg) {
//			dwg->synchronizeSceneData();
//			gvPreview->setScene(dwg);
//			gvPreview->fitInView(dwg->sceneRect(), Qt::KeepAspectRatio);
//	}
}

/**
 * Toggle view action if parent is a QDockWidget
 */
QAction* GV_PreviewWidget::toggleViewAction() {
	QDockWidget* wdgt = qobject_cast<QDockWidget*>(this->parent());
	
	if (wdgt) {
		return wdgt->toggleViewAction();
	} 
	
	return NULL;
}
