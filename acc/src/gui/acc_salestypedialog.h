/*****************************************************************
 * $Id: acc_salestypedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jan 20, 2010 21:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESTYPEDIALOG_H
#define ACC_SALESTYPEDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"

class ACC_SalesTypeDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SalesTypeDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SalesTypeDialog");
    }

    virtual ~ACC_SalesTypeDialog() {
        // Dialogs create and delete models by themselves
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//        mf->removeModel(mModel->getModelType());
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesType);
        mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);
        mModel->select();

        setWindowTitle(tr("ACC Edit Sales Types[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Sales types or price lists"));
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

        this->setHelpSubject("ACC_SalesTypeDialog");
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

#endif // ACC_SALESTYPEDIALOG_H
