/*****************************************************************
 * $Id: sail_projecteditwidget.cpp 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_projecteditwidget.h"
#include "sail_modelfactory.h"

SAIL_ProjectEditWidget::SAIL_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

SAIL_ProjectEditWidget::~SAIL_ProjectEditWidget() {
    delete mModel;
}

RB_String SAIL_ProjectEditWidget::getName() const {
    return "SAIL_ProjectEditWidget";
}

RB2::PerspectiveType SAIL_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectiveSAIL;
}

void SAIL_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("SAIL_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
