/*****************************************************************
 * $Id: db_versionwidget.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 2, 2013 1:25:40 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_versionwidget.h"

#include "db_modelfactory.h"
#include "db_permissionhandler.h"

DB_VersionWidget::DB_VersionWidget(QWidget* parent)
        : RB_TableWidget(parent) {

}

DB_VersionWidget::~DB_VersionWidget() {
    delete mModel;
}

RB_String DB_VersionWidget::getName() const {
    return "DB_VersionWidget";
}

RB2::PerspectiveType DB_VersionWidget::getPerspectiveType() const {
    return RB2::PerspectiveDB;
}

void DB_VersionWidget::init() {
    // Set model, root="" is already set by modelFactory
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelVersion, false);
    mModel->setRoot("");
    mModel->setWhere("id <> '0'", false);
    mModel->select();

    // Set objectname for writing settings otherwise RB_TableWidget
    setObjectName("DB_VersionWidget");
    setWindowTitle(tr("View perspective database versions"));
    setHelpSubject("DB_VersionWidget");

    lblRoot->hide();
    leRoot->hide();
    gbTable->setTitle(tr("List of persectives"));

    // Must be false here due to rootId of model can be empty string
    // and corrupts filterOn/filterOff
    initEditSort(false);
//    this->initEditUpDown();
//    this->initSelectionOnly();
//    this->initTest();

    this->readSettings();
    int count = DB_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; ++i) {
        tableView->hideColumn(i);
    }
}
