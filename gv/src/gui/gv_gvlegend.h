/*****************************************************************
 * $Id: gv_gvlegend.h 911 2009-01-15 21:12:49Z rutger $
 * Created: Oct 18, 2008 3:29:01 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_GVLEGEND_H
#define GV_GVLEGEND_H

#include "gv_graphicsview.h"

class GV_Component;


/**
 * Legend graphicsview class handling selection of item, symbols or line types
 */
class GV_GvLegend : public GV_GraphicsView {
	
	Q_OBJECT
	
public:
	GV_GvLegend(QWidget* parent = 0);
	virtual ~GV_GvLegend();

	virtual GV_Component* getSelectedComponent();

	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* event);

	virtual void dragSymbol(GV_Component* comp);
	virtual void dragLine(GV_Component* comp);
private:
	QPoint mDragStartPosition;
};

#endif /*GV_GVLEGEND_H*/
