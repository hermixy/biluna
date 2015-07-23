/*****************************************************************
 * $Id: db_versiondialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 2, 2013 1:25:40 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_VERSIONDIALOG_H
#define DB_VERSIONDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "db_modelfactory.h"

class DB_EXPORT DB_VersionDialog : public DB_TableDialog {

    Q_OBJECT

public:
    DB_VersionDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("DB_VersionDialog");
    }

    virtual ~DB_VersionDialog() {
        // Dialogs create and delete models by themselves
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//        mf->removeModel(mModel->getModelType());
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory
        mModel = DB_MODELFACTORY->getModel(DB_MODELFACTORY->ModelVersion, false);
        mModel->setRoot("");
        mModel->setWhere("id <> '0'", false);
        mModel->select();

        setWindowTitle(tr("View perspective database versions"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of perspective versions"));
//        pbAdd->setText(tr("&Add"));
//        pbDelete->setText(tr("&Delete"));
//        pbUp->setText(tr("&Up"));
//        pbDown->setText(tr("D&own"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        pbCancel->hide();
//        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));
        lblTvInfo->hide();

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("DB_VersionDialog");

        this->readSettings();
        int count = DB_MODELFACTORY->hiddenColumnCount();

        for (int i = 0; i < count; ++i) {
            tableView->hideColumn(i);
        }
    }
};

#endif // DB_PROJECTDIALOG_H
