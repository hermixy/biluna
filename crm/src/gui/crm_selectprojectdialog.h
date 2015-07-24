/*****************************************************************
 * $Id: crm_selectprojectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 19, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SELECTPROJECTDIALOG_H
#define CRM_SELECTPROJECTDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"


class CRM_SelectProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SelectProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("CRM_SelectProjectDialog");
    }

    virtual ~CRM_SelectProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelProject, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("CRM Select account[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of accounts (projects)"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditOnly();
//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("CRM_SelectProjectDialog");

        this->readSettings();
        int colCount = mModel->columnCount(QModelIndex());

        for (int i = 0; i < colCount; ++i) {
            if (i < RB2::HIDDENCOLUMNS) {
                tableView->hideColumn(i);
            } else {
                tableView->showColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }
    }

};

#endif // CRM_SELECTPROJECTDIALOG_H
