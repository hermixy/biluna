/*****************************************************************
 * $Id: std_selectcomponentwidget.cpp 0001 2016-07-13T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_selectcomponentwidget.h"
#include "ui_std_selectcomponentwidget.h"

STD_SelectComponentWidget::STD_SelectComponentWidget(QWidget *parent) :
            RB_Widget(parent), ui(new Ui::STD_SelectComponentWidget),
            STD_DimensionTableHelper() {
    ui->setupUi(this);
}

STD_SelectComponentWidget::~STD_SelectComponentWidget() {
    delete ui;
}

RB_ObjectBase* STD_SelectComponentWidget::getCurrentObject() {
    if (ui->tvComponent->currentIndex().isValid()) {
        return mComponentModel->getCurrentObject();
    }

    return nullptr;
}
