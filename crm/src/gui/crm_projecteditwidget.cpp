/*****************************************************************
 * $Id: crm_projecteditwidget.cpp 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_projecteditwidget.h"
#include "crm_modelfactory.h"

CRM_ProjectEditWidget::CRM_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

CRM_ProjectEditWidget::~CRM_ProjectEditWidget() {
    delete mModel;
}

RB_String CRM_ProjectEditWidget::getName() const {
    return "CRM_ProjectEditWidget";
}

RB2::PerspectiveType CRM_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectiveCRM;
}

void CRM_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("CRM_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
