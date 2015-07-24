/*****************************************************************
 * $Id: sail_selectmapdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SELECTORDERDIALOG_H
#define MRP_SELECTORDERDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "mrp_modelfactory.h"
#include "rb_sqlrelation.h"

class MRP_SelectOrderDialog : public DB_TableDialog {

    Q_OBJECT

public:
    MRP_SelectOrderDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("MRP_SelectOrderDialog");
        mParentId = "";
    }

    virtual ~MRP_SelectOrderDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setParentId(const RB_String& parentId) {
        mParentId = parentId;

        // Set model, root is already set by modelFactory
        mModel = MRP_MODELFACTORY->getModel(MRP_ModelFactory::ModelSalesOrder, false);
        mModel->setRoot(mParentId);
        mModel->select();

        this->initSelectionOnly();
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

    void init() {
        setWindowTitle(tr("MRP Select order"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Orders"));
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
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("MRP_SelectOrderDialog");
    }

private:
    RB_String mParentId;
};

#endif // ACC_SELECTORDERDIALOG_H
