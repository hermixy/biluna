/*****************************************************************
 * $Id: scan_scanselectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 28, 2011 21:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_SCANSELECTDIALOG_H
#define SCAN_SCANSELECTDIALOG_H

#include <QtWidgets>
#include "scan_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"

class SCAN_ScanSelectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SCAN_ScanSelectDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SCAN_ScanSelectDialog");
    }

    virtual ~SCAN_ScanSelectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelScan, false);
        mModel->setRoot(SCAN_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("Select Scan Type"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of scan types"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("SCAN_ScanSelectDialog");
        this->readSettings();

        for (int i = 0; i < RB2::HIDDENCOLUMNS; i++) {
            tableView->hideColumn(i);
        }

        // Make sure there is one ACC_Project
        if (mModel->rowCount() < 1) {
            on_pbAdd_clicked();
        }
    }
};

#endif // SCAN_SCANSELECTDIALOG_H
