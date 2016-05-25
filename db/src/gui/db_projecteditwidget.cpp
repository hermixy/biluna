/*****************************************************************
 * $Id: db_projecteditdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#include "db_projecteditwidget.h"

#include "db_modelfactory.h"

DB_ProjectEditWidget::DB_ProjectEditWidget(QWidget* parent)
        : RB_ProjectEditWidget(parent) {

}

RB_String DB_ProjectEditWidget::getName() const {
    return "DB_ProjectEditWidget";
}

RB2::PerspectiveType DB_ProjectEditWidget::getPerspectiveType() const {
    return RB2::PerspectiveDB;
}

void DB_ProjectEditWidget::init() {
    //
    // 1. Set model with ID by implementation
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(""); // TODO: depending user rights

    setHelpSubject("DB_ProjectEditWidget");

    // Use all initiations from base class
    RB_ProjectEditWidget::init();
}
