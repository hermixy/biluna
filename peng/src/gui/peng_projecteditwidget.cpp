/*****************************************************************
 * $Id: peng_projecteditwidget.cpp 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_projecteditwidget.h"
#include "peng_modelfactory.h"

PENG_ProjectEditWidget::PENG_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

PENG_ProjectEditWidget::~PENG_ProjectEditWidget() {
    delete mModel;
}

RB_String PENG_ProjectEditWidget::getName() const {
    return "PENG_ProjectEditWidget";
}

RB2::PerspectiveType PENG_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectivePENG;
}

void PENG_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("PENG_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
