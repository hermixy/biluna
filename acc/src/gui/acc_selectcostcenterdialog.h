/*****************************************************************
 * $Id: acc_selectcostcenterdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jun 21, 2012 15:08:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCOSTCENTERDIALOG_H
#define ACC_SELECTCOSTCENTERDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"

class ACC_SelectCostCenterDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectCostCenterDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SelectCostCenterDialog");
    }

    virtual ~ACC_SelectCostCenterDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostCenter, false);
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Select cost center"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Cost Centers"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_SelectCostCenterDialog");
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

        // tableView->sortByColumn(mModel->fieldIndex("accountcode"), Qt::AscendingOrder);
    }
};

#endif // ACC_SELECTCOSTCENTERDIALOG_H
