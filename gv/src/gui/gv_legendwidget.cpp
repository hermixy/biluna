/*****************************************************************
 * $Id: gv_legendwidget.cpp 1380 2011-03-07 20:47:51Z rutger $
 * Created: Oct 1, 2008 7:00:18 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_legendwidget.h"

#include "gv_actionbase.h"
#include "gv_gvlegend.h"
//#include "gv_gvmdi_old.h"
#include "rb_debug.h"


/**
 * Constructor
 */
GV_LegendWidget::GV_LegendWidget(QWidget* parent) : RB_Widget(parent) {
	RB_DEBUG->print("GV_LegendWidget::GV_LegendWidget()");
	setupUi(this);
	
	mLegend = NULL;
	mGvLegend = new GV_GvLegend(this);
	frLayout->addWidget(mGvLegend, 0, 0, 0);
}

/**
 * Destructor
 */
GV_LegendWidget::~GV_LegendWidget() {
	RB_DEBUG->print("GV_LegendWidget::~GV_LegendWidget()");
	// scene is delete with drawing
	mGvLegend->setScene(NULL);
	RB_DEBUG->print("GV_LegendWidget::~GV_LegendWidget() OK");
}

/**
 * Initialize the legend graphicsview with a drawing (scene)
 * @param dwg drawing with component legend 
 */
void GV_LegendWidget::init(GV_Drawing* dwg) {
	RB_DEBUG->print("GV_LegendWidget::init()");
	
	if (!dwg) {
		RB_DEBUG->print(RB_Debug::D_ERROR, 
				"GV_LegendWidget::init() drawing NULL ERROR");
		return;
	}
	
	mLegend = dwg;
	mLegend->synchronizeSceneData();
	mLegend->setDefaultAction(new GV_ActionBase(*mLegend, *mGvLegend));
//	mLegend->setDefaultAction(new GV_ActionSelectSingle(*mLegend, *mGvLegend));
	mGvLegend->setScene(mLegend);

    readSettings();
}

/**
 * Toggle view action if parent is a QDockWidget
 */
QAction* GV_LegendWidget::toggleViewAction() {
	QDockWidget* wdgt = qobject_cast<QDockWidget*>(this->parent());
	if (wdgt) return wdgt->toggleViewAction(); 
	return NULL;
}

/**
 * Get selected component from legend sheet
 */
//GV_Component* GV_LegendWidget::getSelectedComponent() {
//	GV_Drawing* legend = dynamic_cast<GV_Drawing*>(mGvLegend->scene());
//	return legend->getSelectedComponent();
//}
