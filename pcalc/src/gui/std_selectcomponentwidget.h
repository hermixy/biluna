/*****************************************************************
 * $Id: std_selectcomponentwidget.h 0001 2016-07-13T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_SELECTCOMPONENTWIDGET_H
#define STD_SELECTCOMPONENTWIDGET_H

#include "rb_widget.h"

namespace Ui {
class STD_SelectComponentWidget;
}

class STD_SelectComponentWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit STD_SelectComponentWidget(QWidget *parent = 0);
    ~STD_SelectComponentWidget();

    RB_ObjectBase* getCurrentObject();
    void initWidgets();

protected:
    RB_MmProxy* mComponentModel;
    RB_MmProxy* mDimensionModel;
    RB_MmProxy* mRatingModel;
    RB_MmProxy* mSerieModel;
    RB_MmProxy* mTypeModel;

    Ui::STD_SelectComponentWidget* ui;
};

#endif // STD_SELECTCOMPONENTWIDGET_H
