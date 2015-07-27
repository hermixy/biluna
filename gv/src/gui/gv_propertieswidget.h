/*****************************************************************
 * $Id: gv_propertieswidget.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 2, 2008 12:05:10 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PROPERTIESWIDGET_H
#define GV_PROPERTIESWIDGET_H

#include <QtWidgets>
#include "rb_widget.h"
#include "ui_gv_propertieswidget.h"


/**
 * This is the widget for the properties of the selected component
 */
class GV_PropertiesWidget : public RB_Widget, private Ui::GV_PropertiesWidget {
	
	Q_OBJECT
	
public:
	GV_PropertiesWidget(QWidget* parent);
	virtual ~GV_PropertiesWidget();
	
    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return RB_String(); }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }
    virtual bool closeWidget() { return true; }

	virtual QAction* toggleViewAction();
	
	QTableWidget* getTwProperties() { return twProperties; }

};

#endif /*GV_PROPERTIESWIDGET_H*/
