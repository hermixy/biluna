/*****************************************************************
 * $Id: pcalc_en13555gasketwidget.cpp 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en13555gasketwidget.h"

PCALC_EN13555GasketWidget::PCALC_EN13555GasketWidget(QWidget* parent)
    : RB_DatabaseTableWidget(parent) {
    // TODO
}

void PCALC_EN13555GasketWidget::init() {

}

bool PCALC_EN13555GasketWidget::fileSave(bool withSelect) {
    return true;
}

QString PCALC_EN13555GasketWidget::getHelpSubject() const {
    return objectName();
}

