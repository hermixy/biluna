/*****************************************************************
 * $Id: scan_chartwidget.h 1468 2011-10-12 23:23:42Z rutger $
 * Created: 2011-10-10 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_CHARTWIDGET_H
#define SCAN_CHARTWIDGET_H

#include "thrd_chartwidget.h"


class SCAN_ChartWidget : public THRD_ChartWidget {

    Q_OBJECT

public:
    explicit SCAN_ChartWidget(QWidget* parent = 0);
    virtual ~SCAN_ChartWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Review Charts"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveSCAN; }

    virtual void init() { }

};

#endif // SCAN_CHARTWIDGET_H
