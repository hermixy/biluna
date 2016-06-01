/*****************************************************************
 * $Id: srm_projecteditwidget.cpp 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_projecteditwidget.h"
#include "srm_modelfactory.h"

SRM_ProjectEditWidget::SRM_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

SRM_ProjectEditWidget::~SRM_ProjectEditWidget() {
    delete mModel;
}

RB_String SRM_ProjectEditWidget::getName() const {
    return "SRM_ProjectEditWidget";
}

RB2::PerspectiveType SRM_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectiveCRM;
}

void SRM_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("SRM_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
