/*****************************************************************
 * $Id: db_projectselectwidget.cpp 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#include "db_projectselectwidget.h"

#include "db_modelfactory.h"
#include "db_permissionhandler.h"

DB_ProjectSelectWidget::DB_ProjectSelectWidget(QWidget* parent)
        : RB_TableWidget(parent) {

}

RB_String DB_ProjectSelectWidget::getName() const {
    return "DB_ProjectSelectWidget";
}

RB2::PerspectiveType DB_ProjectSelectWidget::getPerspectiveType() const {
    return RB2::PerspectiveDB;
}

void DB_ProjectSelectWidget::init() {
    // Set model, root="" is already set by modelFactory
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelProject, false);

    if (!DB_PERMISSIONHANDLER->isAdmin()) {
        // id set to "" will remove statement: parent='XXXX'
        mModel->setRoot("");

        QStringList dbIdList;
        DB_PERMISSIONHANDLER->getProjectIdList("DB", dbIdList);
        QString whereStatement = "INVALID_USER";

        int idCount = dbIdList.size();

        for (int i = 0; i < idCount; ++i) {
            if (i > 0) {
                whereStatement += " OR ";
            } else {
                whereStatement.clear();
            }

            whereStatement += "id='" + dbIdList.at(i) + "'";
        }

        RB_DEBUG->print(whereStatement);
        mModel->setWhere(whereStatement);
    } else if (DB_PERMISSIONHANDLER->isValidDbUser()) {
        // Single user
        mModel->setRoot("");
    } else {
        // Should not happen
        mModel->setRoot("INVALID_USER");
    }

    mModel->select();

    // Set objectname for writing settings otherwise RB_TableWidget
    setObjectName("DB_ProjectSelectWidget");
    setWindowTitle(tr("Select project"));
    setHelpSubject("DB_ProjectDialog");

    lblRoot->setText(tr("Biluna root project"));
    leRoot->hide();
    gbTable->setTitle(tr("List of projects"));

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
