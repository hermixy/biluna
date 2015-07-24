/*****************************************************************
 * $Id: acc_holdreasondialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 18, 2010 12:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_HOLDREASONDIALOG_H
#define ACC_HOLDREASONDIALOG_H

#include <QtWidgets>
#include "rb_cmbdelegate.h"
#include "db_tabledialog.h"
#include "acc_modelfactory.h"

/**
 * Edit dialog for hold reasons or credit status of customer
 */
class ACC_HoldReasonDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_HoldReasonDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_HoldReasonDialog");
    }

    virtual ~ACC_HoldReasonDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelHoldReason);
        mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);
        mModel->select();

        setWindowTitle(tr("ACC Edit Credit Status[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of customer credit status"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_HoldReasonDialog");

        RB_StringList items;
        items << tr("Invoice OK") << tr("NO INVOICING");
        mModel->setTextList(mModel->fieldIndex("dissallowinvoices"), items);
        RB_CmbDelegate* del = new RB_CmbDelegate(this, items);
        int col = mModel->fieldIndex("dissallowinvoices");
        tableView->setItemDelegateForColumn(col, del);

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

#endif // ACC_HOLDREASONDIALOG_H
