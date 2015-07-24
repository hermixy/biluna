/*****************************************************************
 * $Id: peng_pedtable.h 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PEDTABLE_H
#define PENG_PEDTABLE_H

#include "rb_plotterwidget.h"

class PENG_PedTable : public RB_PlotterWidget
{
public:
    PENG_PedTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~PENG_PedTable();
	
	void setHorizontalLine(double pressure);
	void setVerticalLine(double sizeVolume);
	void setTable(int table = 0);

private:
	double toScale(double originalValue);
	double fromScale(double scaledValue);
};

#endif /*PENG_PEDTABLE_H*/
