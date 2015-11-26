/*****************************************************************
 * $Id: acc_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 21, 2009 21:25:40 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PROJECTDIALOG_H
#define ACC_PROJECTDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"


class ACC_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("ACC_ProjectDialog");
    }

    virtual ~ACC_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelProject, false);

        if (!DB_PERMISSIONHANDLER->isAdmin()) {
            // id set to "" will remove statement: parent='XXXX'
            mModel->setRoot("");

            QStringList projectIdList;
            DB_PERMISSIONHANDLER->getProjectIdList("ACC", projectIdList);
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

        setWindowTitle(tr("ACC Select company[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of companies (projects)"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_ProjectDialog");

        this->readSettings();
        int colCount = mModel->columnCount(QModelIndex());

        for (int i = 0; i < colCount; ++i) {
            if (i != mModel->fieldIndex("number")
                    && i != mModel->fieldIndex("description")
                    && i != mModel->fieldIndex("company")
                    && i != mModel->fieldIndex("location")) {
                tableView->hideColumn(i);
            } else {
                tableView->showColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }

        // Make sure there is one ACC_Project
        if (mModel->rowCount() < 1) {
            on_pbAdd_clicked();
        }
    }

    void on_pbAdd_clicked() {
        if (!mModel)  return;
        mModel->undoFilterSort();

        // insert at the beginning
        int row = 0; // mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        QModelIndex idx;
        idx = mModel->index(row, mModel->fieldIndex("company"), QModelIndex());
        mModel->setData(idx, "MyCompany", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("currencydefault_id"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("payrollact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("grnact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("retainedearnings_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("freightact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("exchangediffact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("purchasesexchangediffact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("pytdiscountact_idx"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        idx = mModel->index(row, mModel->fieldIndex("taxprovince_id"), QModelIndex());
        mModel->setData(idx, "0", Qt::EditRole);
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
        tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS));
    }

};

#endif // ACC_PROJECTDIALOG_H
