/*****************************************************************
 * $Id: scan_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 27, 2011 21:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_PROJECTDIALOG_H
#define SCAN_PROJECTDIALOG_H

#include <QtWidgets>
#include "scan_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"

class SCAN_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SCAN_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("SCAN_ProjectDialog");
    }

    virtual ~SCAN_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelProject, false);

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


        setWindowTitle(tr("Select project"));
        lblRoot->setText(tr("Scan Project"));
        // lblRoot->hide();
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

        this->setHelpSubject("SCAN_ProjectDialog");

        readSettings();
        int count = SCAN_MODELFACTORY->hiddenColumnCount();

        for (int i = 0; i < count; i++) {
            tableView->hideColumn(i);
        }
    }

    void on_pbAdd_clicked() {
        mModel->undoFilterSort();
        int row = 0; // mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        QModelIndex idx = mModel->index(row, mModel->fieldIndex("number"), QModelIndex());
        mModel->setData(idx, tr("New"), Qt::EditRole);
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, mModel->fieldIndex("number"), QModelIndex()));
        tableView->scrollTo(mModel->index(row, mModel->fieldIndex("number")));
    }

};

#endif // SCAN_PROJECTDIALOG_H
