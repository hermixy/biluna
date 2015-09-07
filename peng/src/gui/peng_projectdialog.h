/*****************************************************************
 * $Id: peng_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 16, 2011 21:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PROJECTDIALOG_H
#define PENG_PROJECTDIALOG_H

#include <QtWidgets>
#include "peng_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "rb_dialogbase.h"

class PENG_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PENG_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("PENG_ProjectDialog");
    }

    virtual ~PENG_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelProject, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

//        for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
//            if (i != mModel->fieldIndex("coyname") && i != mModel->fieldIndex("regoffice1")
//                    && i != mModel->fieldIndex("regoffice2") && i != mModel->fieldIndex("regoffice3")
//                    && i != mModel->fieldIndex("regoffice4") && i != mModel->fieldIndex("regoffice5")
//                    && i != mModel->fieldIndex("regoffice6")) {
//                tableView->hideColumn(i);
//            }
//        }

        for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
            tableView->hideColumn(i);
        }

        setWindowTitle(tr("Select project[*]"));
        // lblRoot->setText(tr("Plant Engineering Project"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of projects"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("PENG_ProjectDialog");

        readSettings();
        int count = PENG_MODELFACTORY->hiddenColumnCount();

        for (int i = 0; i < count; i++) {
            tableView->hideColumn(i);
        }

        // Make sure there is one ACC_Project
        if (mModel->rowCount() < 1) {
            on_pbAdd_clicked();
        }
    }

    void on_pbAdd_clicked() {
        mModel->undoFilterSort();
        int row = 0; // mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        QModelIndex idx = mModel->index(row, mModel->fieldIndex("number"), QModelIndex());
        mModel->setData(idx, tr("<NEW>"), Qt::EditRole);
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, mModel->fieldIndex("number"), QModelIndex()));
        tableView->scrollTo(mModel->index(row, mModel->fieldIndex("number")));
    }

};

#endif // PENG_PROJECTDIALOG_H