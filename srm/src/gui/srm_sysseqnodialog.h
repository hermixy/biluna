/*****************************************************************
 * $Id: srm_sysseqnodialog.h 2074 2014-01-26 12:21:13Z rutger $
 * Created: Jan 26, 2014 10:25:40 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SYSSEQNODIALOG_H
#define SRM_SYSSEQNODIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "srm_modelfactory.h"

class SRM_SysSeqNoDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SRM_SysSeqNoDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SRM_SysSeqNoDialog");
    }

    virtual ~SRM_SysSeqNoDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSysSeqNo);
        mModel->setRoot(SRM_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("SRM System Sequence Numbers[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of system sequence numbers"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initEditOnly();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("SRM_SysSeqNoDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; ++i) {
            if (i == (RB2::HIDDENCOLUMNS + 1) || i == (RB2::HIDDENCOLUMNS + 2)) {
                tableView->showColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            } else {
                tableView->hideColumn(i);
            }
        }

    }
};

#endif // SRM_SYSSEQNODIALOG_H
