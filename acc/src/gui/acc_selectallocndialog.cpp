/*****************************************************************
 * $Id: acc_selectallocndialog.cpp 2202 2015-01-11 19:39:36Z rutger $
 * Created: Oct 4, 2010 21:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_selectallocndialog.h"
#include "acc_qachartmaster.h"


ACC_SelectAllocnDialog::ACC_SelectAllocnDialog(QWidget* parent)
                                : DB_TableDialog(parent) {
    // set combobox
    cbSourceModelFilter->addItem(tr("Customer"), (int)ACC2::AllocnCustomer);
    cbSourceModelFilter->addItem(tr("Supplier"), (int)ACC2::AllocnSupplier);
}

ACC_SelectAllocnDialog::~ACC_SelectAllocnDialog() {
    // Dialogs create and delete models by themselves, shared with
    if (mModel) {
        delete mModel;
        mModel = NULL;
    }
}

void ACC_SelectAllocnDialog::init() {
    // false will create a new model and to be delete here in dialog
    mModel = ACC_MODELFACTORY->getModel(ACC_MODELFACTORY->ModelTransDoc, false);
    // mModel->select();
    mModel->setRoot("none"); // because parent is ACC_ChartMaster not ACC_Project

    RB_String qStr = "SELECT acc_transdoc.* FROM acc_transdoc INNER JOIN acc_chartmaster "
                        "ON acc_transdoc.parent=acc_chartmaster.id "
                        "WHERE acc_chartmaster.parent='";
    qStr += ACC_MODELFACTORY->getRootId() + "' ";
    qStr += " AND acc_transdoc.settled=0";
    qStr += " AND (acc_transdoc.totalamountrec<-0.005 OR acc_transdoc.totalamountrec>0.005";
    qStr += " OR acc_transdoc.totalamountpay<-0.005 OR acc_transdoc.totalamountpay>0.005)";
    qStr += " AND (acc_transdoc.parent='" + ACC_QACHARTMASTER->getAccPayId() + "'";
    qStr += " OR acc_transdoc.parent='" + ACC_QACHARTMASTER->getAccRecId() + "'";
    qStr += " OR acc_transdoc.parent='" + ACC_QACHARTMASTER->getAccDebtorAccruedId() + "'";
    qStr += " OR acc_transdoc.parent='" + ACC_QACHARTMASTER->getAccCreditorAccruedId() + "')";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    query.exec(qStr);
    mModel->setQuery(query);

    //    this->initEditSort();
    //    this->initEditUpDown();
    this->initSelectionOnly();
    //    this->initTest();

    setWindowTitle(tr("ACC Select document[*]"));
    lblRoot->setText(tr("Document type"));
    // lblRoot->hide();
    leRoot->hide();
    cbSourceModelFilter->show();

    gbTable->setTitle(tr("List of documents"));
    //    pbAdd->setText(tr("&Add"));
    //    pbDelete->setText(tr("&Delete"));
    //    pbUp->setText(tr("&Up"));
    //    pbDown->setText(tr("D&own"));
    pbFilter->hide(); // filter is already used
    pbFilterClear->hide();
    pbHelp->setText(tr("&Help"));
    pbOk->setText(tr("&OK"));
    pbCancel->setText(tr("&Cancel"));
    lblTvInfo->setText(tr("Click item to select"));

    setHelpSubject("ACC_SelectAllocnDialog");

    // Set current index at model
    connect(tableView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            mModel, SLOT(slotChangeCurrentRow(const QModelIndex&,const QModelIndex&)));

    // Change filter at combobox index change
    connect(this->cbSourceModelFilter, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSourceFilterChange(int)));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i != mModel->fieldIndex("transdate") && i != mModel->fieldIndex("transno")
                && i != mModel->fieldIndex("description") && i != mModel->fieldIndex("refno")
                && i != mModel->fieldIndex("alloc")) {
            tableView->hideColumn(i);
        } else {
            tableView->showColumn(i);

            if (tableView->columnWidth(i) < 5) {
                tableView->setColumnWidth(i, 100);
            }
        }
    }
}

void ACC_SelectAllocnDialog::setFilterType(ACC2::AllocnType type) {
    setFilter(type);
}

/**
 * Update allocation amounts with current yet unsaved allocation
 * @param transDocList current transaction documents being edited
 */
void ACC_SelectAllocnDialog::updateAllocns(RB_ObjectContainer* transDocList) {
    int rowCount = mModel->rowCount();
    QModelIndex idx;

    RB_ObjectIterator* iter = transDocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transDoc = iter->currentObject();

        for (int row = 0; row < rowCount; ++row) {
            idx = mModel->index(row, 0);

            if (idx.data(Qt::EditRole).toString() ==  transDoc->getId()) {
                idx = mModel->index(row, mModel->fieldIndex("alloc"));
                mModel->setData(idx, transDoc->getValue("alloc").toDouble());
            }
        }
    }

    delete iter;

    // prevent yes/no save
    setWindowModified(false);
}

void ACC_SelectAllocnDialog::setFilter(ACC2::AllocnType type) {
    int number = cbSourceModelFilter->findData(type);
    cbSourceModelFilter->setCurrentIndex(number);

    if (type == ACC2::AllocnCustomer) {
        mModel->setUserFilter(ACC_QACHARTMASTER->getAccRecId(), "parent", false);
        int col = mModel->fieldIndex("totalamountpay");
        tableView->hideColumn(col);
        col = mModel->fieldIndex("totalamountrec");
        tableView->showColumn(col);
    } else if (type == ACC2::AllocnSupplier) {
        mModel->setUserFilter(ACC_QACHARTMASTER->getAccPayId(), "parent", false);
        int col = mModel->fieldIndex("totalamountrec");
        tableView->hideColumn(col);
        col = mModel->fieldIndex("totalamountpay");
        tableView->showColumn(col);
    }
}

void ACC_SelectAllocnDialog::slotSourceFilterChange(int number) {
    ACC2::AllocnType type = static_cast<ACC2::AllocnType>(
                cbSourceModelFilter->itemData(number).toInt());
    setFilter(type);
}



