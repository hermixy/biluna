/*****************************************************************
 * $Id: crm_selectcontactdialog.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SELECTCONTACTDIALOG_H
#define CRM_SELECTCONTACTDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class CRM_SelectContactDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SelectContactDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("CRM_SelectContactDialog");
    }

    virtual ~CRM_SelectContactDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setCustomerId(const QString& customerId) {
        mModel->setRoot(customerId);
        mModel->select();
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelContact, false);

        // Show customer name for reference
        int relationColumn = 1; // mModel->fieldIndex("parent");
        mModel->setRelation(relationColumn,
                            RB_SqlRelation("acc_customer", "id", "customerno"));
        mModel->setWhere("`acc_contact`.`id` <> '0'", false);
        mModel->setRoot("");
        mModel->select();

        setWindowTitle(tr("CRM Select contact"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Contacts"));
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

        this->setHelpSubject("CRM_SelectContactDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == 1 || i == RB2::HIDDENCOLUMNS + 1
                    || i == RB2::HIDDENCOLUMNS + 2) {
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

#endif // CRM_SELECTCONTACTDIALOG_H
