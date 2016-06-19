/*****************************************************************
 * $Id: std_selectmaterialwidget.h 0 2016-06-01 15:23:20Z rutger $
 * Created: June 18, 2016 15:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef STD_SELECTMATERIALWIDGET_H
#define STD_SELECTMATERIALWIDGET_H

#include "rb_widget.h"

namespace Ui {
class STD_SelectMaterialWidget;
}

class STD_SelectMaterialWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit STD_SelectMaterialWidget(QWidget *parent = 0);
    ~STD_SelectMaterialWidget();

    RB_String getName() const;
    RB2::PerspectiveType getPerspectiveType() const;
    void init();

    bool fileSave(bool) { return true; }
    RB_ObjectBase* getCurrentObject();

private:
    void initModelMapping();

    Ui::STD_SelectMaterialWidget* ui;
    RB_MmProxy* mModel;
};

#endif // STD_SELECTMATERIALWIDGET_H
