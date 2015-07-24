/*****************************************************************
 * $Id: acc_selectsupplierdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Sep 29, 2010 4:05:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTSUPPLIERDIALOG_H
#define ACC_SELECTSUPPLIERDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class ACC_SelectSupplierDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectSupplierDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SelectSupplierDialog");
    }

    virtual ~ACC_SelectSupplierDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier, false);
//        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroup", "id", "groupname"));
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Select supplier"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Supplier"));
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

        this->setHelpSubject("ACC_SelectSupplierDialog");
        this->readSettings();

        // Hidden columns only for tableviews
        int colCount = mModel->columnCount();
        for (int i = 0; i < colCount; ++i) {
            if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1) {
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

#endif // ACC_SELECTSUPPLIERDIALOG_H
