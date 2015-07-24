/*****************************************************************
 * $Id: acc_selecttaxprovincedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 28, 2010 12:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTTAXPROVINCEDIALOG_H
#define ACC_SELECTTAXPROVINCEDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"

class ACC_SelectTaxProvinceDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectTaxProvinceDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SelectTaxProvinceDialog");
    }

    virtual ~ACC_SelectTaxProvinceDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxProvince, false);
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("Select tax province"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("ACC Tax Provinces"));
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

        this->setHelpSubject("ACC_SelectTaxProvinceDialog");
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

#endif // ACC_SELECTTAXPROVINCEDIALOG_H
