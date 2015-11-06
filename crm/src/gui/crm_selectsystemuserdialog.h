/*****************************************************************
 * $Id: crm_selectsystemuserdialog.h 2073 2014-01-25 17:07:56Z rutger $
 * Created: Nov 4, 2015 12:08:25 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SELECTSYSTEMUSERDIALOG_H
#define CRM_SELECTSYSTEMUSERDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"


class CRM_SelectSystemUserDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SelectSystemUserDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("CRM_SelectSystemUserDialog");
    }

    virtual ~CRM_SelectSystemUserDialog() {
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
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelSystemUser, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("CRM Select System User"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("System Users"));
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

        this->setHelpSubject("CRM_SelectSystemUserDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == RB2::HIDDENCOLUMNS || i == RB2::HIDDENCOLUMNS + 1
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

#endif // CRM_SELECTSYSTEMUSERDIALOG_H
