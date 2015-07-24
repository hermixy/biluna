/*****************************************************************
 * $Id: crm_selectcustomerdialog.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCUSTOMERDIALOG_H
#define ACC_SELECTCUSTOMERDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class CRM_SelectCustomerDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SelectCustomerDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("CRM_SelectCustomerDialog");
    }

    virtual ~CRM_SelectCustomerDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setProjectId(const QString& projectId) {
        mModel->setWhere("`acc_customer`.`crm_parent` = '" + projectId + "'", false);
        mModel->select();
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomer, false);
//        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroups", "id", "groupname"));
//        mModel->setRoot(CRM_MODELFACTORY->getRootId());
        mModel->setWhere("id <> '0'", false);
        mModel->setRoot("");
        mModel->select();



        setWindowTitle(tr("CRM Select customer"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Customer"));
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

        this->setHelpSubject("CRM_SelectCustomerDialog");
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
