/*****************************************************************
 * $Id: scan_projecteditwidget.cpp 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_projecteditwidget.h"
#include "scan_modelfactory.h"

SCAN_ProjectEditWidget::SCAN_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

SCAN_ProjectEditWidget::~SCAN_ProjectEditWidget() {
    delete mModel;
}

RB_String SCAN_ProjectEditWidget::getName() const {
    return "SCAN_ProjectEditWidget";
}

RB2::PerspectiveType SCAN_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectiveSCAN;
}

void SCAN_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("SCAN_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
