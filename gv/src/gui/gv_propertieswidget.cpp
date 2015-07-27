/*****************************************************************
 * $Id: gv_propertieswidget.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 12:05:17 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_propertieswidget.h"

#include "rb_debug.h"

GV_PropertiesWidget::GV_PropertiesWidget(QWidget* parent) : RB_Widget(parent) {
	
	setupUi(this);
	
}

GV_PropertiesWidget::~GV_PropertiesWidget() {
	
}

/**
 * Toggle view action if parent is a QDockWidget
 */
QAction* GV_PropertiesWidget::toggleViewAction() {
	QDockWidget* wdgt = qobject_cast<QDockWidget*>(this->parent());
	
	if (wdgt) {
		return wdgt->toggleViewAction();
	} 
	
	return NULL;
}
