/*****************************************************************
 * $Id: acc_salesmandialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 20, 2010 12:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESMANDIALOG_H
#define ACC_SALESMANDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "acc_modelfactory.h"

class ACC_SalesManDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SalesManDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SalesManDialog");
    }

    virtual ~ACC_SalesManDialog() {
        // Dialogs create and delete models by themselves
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//        mf->removeModel(mModel->getModelType());
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesMan);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Edit Salesmen[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of salesman"));
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

        this->setHelpSubject("ACC_SalesManDialog");
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

#endif // ACC_SALESMANDIALOG_H
