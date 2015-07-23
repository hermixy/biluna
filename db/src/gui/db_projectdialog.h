/*****************************************************************
 * $Id: db_projectdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 8, 2009 21:25:40 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PROJECTDIALOG_H
#define DB_PROJECTDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "db_modelfactory.h"

class DB_EXPORT DB_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    DB_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("DB_ProjectDialog");
    }

    virtual ~DB_ProjectDialog() {
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
        mModel = DB_MODELFACTORY->getModel(DB_MODELFACTORY->ModelProject, false);
        mModel->setRoot("none");
        mModel->select();

        setWindowTitle(tr("Select project"));
        lblRoot->setText(tr("Biluna root project"));
//        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of projects"));
//        pbAdd->setText(tr("&Add"));
//        pbDelete->setText(tr("&Delete"));
//        pbUp->setText(tr("&Up"));
//        pbDown->setText(tr("D&own"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("DB_ProjectDialog");

        this->readSettings();
        int count = DB_MODELFACTORY->hiddenColumnCount();

        for (int i = 0; i < count; ++i) {
            tableView->hideColumn(i);
        }
    }
};

#endif // DB_PROJECTDIALOG_H
