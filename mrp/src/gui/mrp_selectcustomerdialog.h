/*****************************************************************
 * $Id: mrp_selectcustomerdialog.h 2191 2014-10-27 20:31:51Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SELECTCUSTOMERDIALOG_H
#define MRP_SELECTCUSTOMERDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "mrp_modelfactory.h"
#include "rb_sqlrelation.h"

class MRP_SelectCustomerDialog : public DB_TableDialog {

    Q_OBJECT

public:
    MRP_SelectCustomerDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("MRP_SelectCustomerDialog");
    }

    virtual ~MRP_SelectCustomerDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = MRP_MODELFACTORY->getModel(MRP_ModelFactory::ModelCustomer, false);
        mModel->setRoot(MRP_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("MRP Select customer"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Customers"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("MRP_SelectCustomerDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == RB2::HIDDENCOLUMNS || i == RB2::HIDDENCOLUMNS + 1) {
                tableView->showColumn(i);
            } else {
                tableView->hideColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }
    }
};

#endif // ACC_SELECTCUSTOMERDIALOG_H
