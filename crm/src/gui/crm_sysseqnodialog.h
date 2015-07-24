/*****************************************************************
 * $Id: crm_sysseqnodialog.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 27, 2014 10:25:40 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SYSSEQNODIALOG_H
#define CRM_SYSSEQNODIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "crm_modelfactory.h"

class CRM_SysSeqNoDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SysSeqNoDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("CRM_SysSeqNoDialog");
    }

    virtual ~CRM_SysSeqNoDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelSysSeqNo);
        mModel->setRoot(CRM_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("CRM System Sequence Numbers[*]"));
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

        this->setHelpSubject("CRM_SysSeqNoDialog");
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

#endif // CRM_SYSSEQNODIALOG_H
