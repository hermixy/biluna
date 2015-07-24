/*****************************************************************
 * $Id: srm_selectsupplierdialog.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTSUPPLIERDIALOG_H
#define ACC_SELECTSUPPLIERDIALOG_H

#include <QtWidgets>
#include "srm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class SRM_SelectSupplierDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SRM_SelectSupplierDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SRM_SelectSupplierDialog");
    }

    virtual ~SRM_SelectSupplierDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setProjectId(const QString& projectId) {
        mModel->setWhere("`acc_supplier`.`srm_parent` = '" + projectId + "'", false);
        mModel->select();
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSupplier, false);
//        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroups", "id", "groupname"));
//        mModel->setRoot(SRM_MODELFACTORY->getRootId());
        mModel->setWhere("`acc_supplier`.`id` <> '0'", false);
        mModel->setRoot("");
        mModel->select();



        setWindowTitle(tr("SRM Select supplier"));
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

        this->setHelpSubject("SRM_SelectSupplierDialog");
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

#endif // ACC_SELECTSUPPLIERDIALOG_H
