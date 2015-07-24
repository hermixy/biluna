/*****************************************************************
 * $Id: peng_typeeditdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 16, 2011 21:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPEEDITDIALOG_H
#define PENG_TYPEEDITDIALOG_H

#include <QtWidgets>
#include "peng_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"

class PENG_TypeEditDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PENG_TypeEditDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("PENG_TypeEditDialog");
    }

    virtual ~PENG_TypeEditDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipmentType, false);
        mModel->setRoot(PENG_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("Edit Equipment Type"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of equipment types"));
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

        this->setHelpSubject("PENG_TypeEditDialog");
        readSettings();

        for (int i = 0; i < RB2::HIDDENCOLUMNS; i++) {
            tableView->hideColumn(i);
        }
    }

    void on_pbAdd_clicked() {

        // always insert at the end
        int row = mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        //currencydefault
//        QModelIndex idx = mModel->index(row, mModel->fieldIndex("currencydefault"), QModelIndex());
//        mModel->setData(idx, "EUR", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("debtorsact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("creditorsact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("payrollact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("grnact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("retainedearnings_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("freightact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("exchangediffact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("purchasesexchangediffact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("pytdiscountact_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("gllink_debtors"), QModelIndex());
//        mModel->setData(idx, 0, Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("gllink_creditors"), QModelIndex());
//        mModel->setData(idx, 0, Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("gllink_stock"), QModelIndex());
//        mModel->setData(idx, 0, Qt::EditRole);
//        idx = mModel->index(row, mModel->fieldIndex("taxprovince_id"), QModelIndex());
//        mModel->setData(idx, 0, Qt::EditRole);
//        mModel->submit();
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
        tableView->scrollTo(tableView->currentIndex());
    }

};

#endif // PENG_TYPEEDITDIALOG_H
