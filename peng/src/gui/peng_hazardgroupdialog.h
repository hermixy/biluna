/*****************************************************************
 * $Id: peng_fluiddialog.h 2146 2014-05-23 20:27:06Z rutger $
 * Created: 2014-06-16 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_HAZARDGROUPDIALOG_H
#define PENG_HAZARDGROUPDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "peng_modelfactory.h"


/**
 * Dialog to edit the hazard group list
 */
class PENG_HazardGroupDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PENG_HazardGroupDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("PENG_HazardGroupDialog");
    }

    virtual ~PENG_HazardGroupDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelHazardGroup);
        mModel->setRoot(PENG_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("PENG Edit hazard group[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of hazard groups"));
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

        this->setHelpSubject("PENG_HazardGroupDialog");
        this->readSettings();

        // Hidden columns only for tableviews
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

    }

    void on_pbAdd_clicked() {
        if (!mModel) return;
        DB_TableDialog::on_pbAdd_clicked();

        QModelIndex idx = tableView->currentIndex();
        if (!idx.isValid()) {
            return;
        }

        int col = mModel->fieldIndex("code");
        idx = mModel->index(idx.row(), col, QModelIndex());
        mModel->setData(idx, "<NEW>", Qt::EditRole);
    }

};

#endif // PENG_HAZARDGROUPDIALOG_H
