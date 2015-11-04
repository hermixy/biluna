/*****************************************************************
 * $Id: acc_areadialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 21, 2010 11:25:40 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_AREADIALOG_H
#define ACC_AREADIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "acc_modelfactory.h"
#include "db_modelfactory.h"

class ACC_AreaDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_AreaDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_AreaDialog");
    }

    virtual ~ACC_AreaDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelArea);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Edit Sales/Purchase Area[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of sales and purchase area"));
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

        this->setHelpSubject("ACC_AreaDialog");
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
};

#endif // ACC_AREADIALOG_H
