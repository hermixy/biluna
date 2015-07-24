/*****************************************************************
 * $Id: acc_systypedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 8, 2012 8:25:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SYSTYPEDIALOG_H
#define ACC_SYSTYPEDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "acc_modelfactory.h"

class ACC_SysTypeDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SysTypeDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SysTypeDialog");
    }

    virtual ~ACC_SysTypeDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSysType);
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC System Type Numbers[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of system type numbers"));
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

        this->setHelpSubject("ACC_SysTypeDialog");
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

#endif // ACC_SYSTYPEDIALOG_H
