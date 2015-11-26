/*****************************************************************
 * $Id: mrp_selectprojectdialog.h 2191 2014-10-27 20:31:51Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SELECTPROJECTDIALOG_H
#define MRP_SELECTPROJECTDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"
#include "mrp_modelfactory.h"


class MRP_SelectProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    MRP_SelectProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("MRP_SelectProjectDialog");
    }

    virtual ~MRP_SelectProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = MRP_MODELFACTORY->getModel(MRP_ModelFactory::ModelProject, false);

        if (!DB_PERMISSIONHANDLER->isAdmin()) {
            // id set to "" will remove statement: parent='XXXX'
            mModel->setRoot("");

            QStringList projectIdList;
            DB_PERMISSIONHANDLER->getProjectIdList("MRP", projectIdList);
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

        setWindowTitle(tr("MRP Select company[*]"));
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

        this->setHelpSubject("MRP_SelectProjectDialog");

        this->readSettings();
        int colCount = mModel->columnCount(QModelIndex());

        for (int i = 0; i < colCount; ++i) {
            if (i != mModel->fieldIndex("coyname") && i != mModel->fieldIndex("regoffice1")
                    && i != mModel->fieldIndex("regoffice2") && i != mModel->fieldIndex("regoffice3")
                    && i != mModel->fieldIndex("regoffice4") && i != mModel->fieldIndex("regoffice5")
                    && i != mModel->fieldIndex("regoffice6")) {
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
        idx = mModel->index(row, mModel->fieldIndex("coyname"), QModelIndex());
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

#endif // MRP_SELECTPROJECTDIALOG_H
