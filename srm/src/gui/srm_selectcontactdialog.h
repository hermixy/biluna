/*****************************************************************
 * $Id: srm_selectcontactdialog.h 2073 2014-01-25 17:07:56Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SELECTCONTACTDIALOG_H
#define SRM_SELECTCONTACTDIALOG_H

#include <QtWidgets>
#include "srm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class SRM_SelectContactDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SRM_SelectContactDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SRM_SelectContactDialog");
    }

    virtual ~SRM_SelectContactDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setSupplierId(const QString& supplierId) {
        mModel->setRoot(supplierId);
        mModel->select();
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelContact, false);

        // Show customer name for reference
        int relationColumn = 1; // mModel->fieldIndex("parent");
        mModel->setRelation(relationColumn,
                            RB_SqlRelation("acc_supplier", "id", "suppliercode"));
        mModel->setWhere("`acc_contact`.`id` <> '0'", false);
        mModel->setRoot("");
        mModel->select();
        // Example: to select per SRM parent
        // mModel->setQuery("SELECT acc_contact.id, acc_supplier.suppliercode,
        // acc_contact.firstname, acc_contact.lastname FROM acc_contact
        // INNER JOIN acc_supplier ON acc_supplier.id=acc_contact.parent
        // WHERE acc_supplier.srm_parent='{352601db-2b27-4937-a486-aa5b17428c67}';")

        setWindowTitle(tr("SRM Select contact"));
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

        this->setHelpSubject("SRM_SelectContactDialog");
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

#endif // SRM_SELECTCONTACTDIALOG_H
