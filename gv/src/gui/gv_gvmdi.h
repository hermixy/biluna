/*****************************************************************
 * $Id: gv_gvmdi.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Mar 24, 2008 11:11:05 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_GVMDI_H
#define GV_GVMDI_H

#include "gv_graphicsview.h"
#include "gv_grid.h"

class QGraphicsScene;


class GV_GvMdi : public GV_GraphicsView {

	Q_OBJECT
	
public:
	GV_GvMdi(QWidget* parent = 0);
	virtual ~GV_GvMdi();
	
	virtual void setScene(QGraphicsScene* drawing);
	
	virtual int getHeight();
	virtual int getWidth();

public slots:
	void slotSliderValueChanged(int value);
	
protected:
	virtual void scaleView(double scaleFactor);
	virtual void dragEnterEvent(QDragEnterEvent* e);
	virtual void dragMoveEvent(QDragMoveEvent* e);
	virtual void dropEvent(QDropEvent* e);
	
private:
	bool mBeingScaled;

};

#endif /*GV_GRAPHICSVIEW_H*/
