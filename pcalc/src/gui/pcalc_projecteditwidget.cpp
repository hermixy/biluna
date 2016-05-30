/*****************************************************************
 * $Id: peng_projecteditwidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Apr 29, 2015 8:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_projecteditwidget.h"

#include "db_modelfactory.h"
#include "pcalc_modelfactory.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
PCALC_ProjectEditWidget::PCALC_ProjectEditWidget(QWidget *parent)
        : RB_ProjectEditWidget(parent) {
    // Write settings, otherwise RB_ProjectEditDialog
    this->setObjectName("PCALC_ProjectEditWidget");
}

/**
 * Destructor
 */
PCALC_ProjectEditWidget::~PCALC_ProjectEditWidget() {
    delete mModel;
    RB_DEBUG->print("PCALC_ProjectEditWidget::~PCALC_ProjectEditWidget() OK");
}

RB_String PCALC_ProjectEditWidget::getName() const {
    return "PCALC_ProjectEditWidget";
}

RB2::PerspectiveType PCALC_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}


/**
 * Initialize the dialog with model
 */
void PCALC_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId()); // TODO: depending user rights

    setHelpSubject("PCALC_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
