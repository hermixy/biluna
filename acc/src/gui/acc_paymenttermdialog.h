/*****************************************************************
 * $Id: acc_paymenttermdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 17, 2010 17:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PAYMENTTERMDIALOG_H
#define ACC_PAYMENTTERMDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_cmbdelegate.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Edit payment terms dialog
 */
class ACC_PaymentTermDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_PaymentTermDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_PaymentTermDialog");
    }

    virtual ~ACC_PaymentTermDialog() {
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPaymentTerm);
        mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);
        mModel->select();

        setWindowTitle(tr("ACC Edit Payment Terms[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of payment terms"));
//        pbAdd->setText(tr("&Add"));
//        pbDelete->setText(tr("&Delete"));
//        pbUp->setText(tr("&Up"));
//        pbDown->setText(tr("D&own"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_PaymentTermDialog");

        // TODO: create separate ACC_PaymentTermDelegate table, now 0, 1 etc is shown
        RB_StringList items;
        items << tr("Number of days") << tr("Day in following month");
        mModel->setTextList(mModel->fieldIndex("dayinfollowingmonth"), items);
        RB_CmbDelegate* del = new RB_CmbDelegate(this, items);
        int col = mModel->fieldIndex("dayinfollowingmonth");
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


#endif // ACC_PAYMENTTERMDIALOG_H
