/*****************************************************************
 * $Id: srm_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_PROJECTDIALOG_H
#define SRM_PROJECTDIALOG_H

#include <QtWidgets>
#include "srm_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"


class SRM_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SRM_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("SRM_ProjectDialog");
    }

    virtual ~SRM_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelProject, false);

        if (!DB_PERMISSIONHANDLER->isAdmin()) {
            // id set to "" will remove statement: parent='XXXX'
            mModel->setRoot("");

            QStringList projectIdList;
            DB_PERMISSIONHANDLER->getProjectIdList("SRM", projectIdList);
            QString whereStatement = "INVALID_USER";

            int idCount = projectIdList.size();

            for (int i = 0; i < idCount; ++i) {
                if (i > 0) {
                    whereStatement += " OR ";
                } else {
                    whereStatement.clear();
                }

                whereStatement += "id='" + projectIdList.at(i) + "'";
            }

            mModel->setWhere(whereStatement);
        } else {
            // Single user
            mModel->setRoot(DB_MODELFACTORY->getRootId());
        }

        mModel->select();

        setWindowTitle(tr("SRM Select account[*]"));
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
        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("SRM_ProjectDialog");

        this->readSettings();
        int colCount = mModel->columnCount();
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

        // Make sure there is one SRM_Project
        if (mModel->rowCount() < 1) {
            on_pbAdd_clicked();
        }
    }

    void on_pbAdd_clicked() {
        // insert at the beginning
        int row = 0; // mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());
        mModel->undoFilterSort();

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
//        QModelIndex idx;
//        idx = mModel->index(row, mModel->fieldIndex("currencydefault_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        mModel->submit();
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
        tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS));
    }

};

#endif // SRM_PROJECTDIALOG_H
