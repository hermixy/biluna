/*****************************************************************
 * $Id: acc_taxconfirmationdialog.cpp 1800 2012-11-13 20:22:10Z rutger $
 * Created: Mar 28, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxconfirmationdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_TaxConfirmationDialog::ACC_TaxConfirmationDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    setWindowTitle(tr("Tax Confirmation"));
    mTransDoc = NULL;
    mDebCredTransList = NULL;
    mIsSalesOrder = true;
    // Tax confirmation cannot be cancelled
    this->pbCancel->hide();
}

/**
 * Destructor
 */
ACC_TaxConfirmationDialog::~ACC_TaxConfirmationDialog() {
    // Nothing
    RB_DEBUG->print("ACC_TaxConfirmationDialog::~ACC_TaxConfirmationDialog() OK");
}

/**
 * Set debtor transaction list
 */
void ACC_TaxConfirmationDialog::setTransModel(RB_ObjectBase* transDoc,
                                              RB_ObjectContainer* debCredTransList,
                                              bool isSalesOrder) {
    mTransDoc = transDoc;
    mDebCredTransList = debCredTransList;
    mIsSalesOrder = isSalesOrder;
}

/**
 * Initialize dialog and models
 */
void ACC_TaxConfirmationDialog::init() {

    setWindowTitle(tr("Confirm Taxes"));

    if (!mDebCredTransList) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_TaxConfirmationDialog::init() "
                        "mDebCredTransList NULL ERROR");
        return;
    }

    setFields();
    readSettings();
}

void ACC_TaxConfirmationDialog::setFields() {
    tableWidget->clear();
    tableWidget->setEditTriggers(QAbstractItemView::EditKeyPressed |
                                 QAbstractItemView::DoubleClicked);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTaxMap.clear();

    tableWidget->setColumnCount(5);
    tableWidget->verticalHeader()->setDefaultSectionSize(20); // row height
    QTableWidgetItem* headerItem = new QTableWidgetItem(tr("Description"));
    tableWidget->setHorizontalHeaderItem(0, headerItem);
    headerItem = new QTableWidgetItem(tr("Amount"));
    tableWidget->setHorizontalHeaderItem(1, headerItem);
    headerItem = new QTableWidgetItem(tr("High Tax"));
    tableWidget->setHorizontalHeaderItem(2, headerItem);
    headerItem = new QTableWidgetItem(tr("Low Tax"));
    tableWidget->setHorizontalHeaderItem(3, headerItem);
    headerItem = new QTableWidgetItem(tr("Other Tax"));
    tableWidget->setHorizontalHeaderItem(4, headerItem);

    tableWidget->setRowCount(mDebCredTransList->objectCount()+1);

    int taxRow = 0;
    QTableWidgetItem* twItem = NULL;
    RB_ObjectMember* mem = NULL;
    double amount = 0.0;
    RB_ObjectIterator* iterDt = mDebCredTransList->createIterator();

    for (iterDt->first(); !iterDt->isDone(); iterDt->next()) {
        RB_ObjectBase* dt = iterDt->currentObject();

        twItem = new QTableWidgetItem(dt->getValue("description").toString());
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
        tableWidget->setItem((taxRow), 0, twItem);

        amount = dt->getValue("amount").toDouble();

        if (amount > 0.0) {
            twItem = new QTableWidgetItem(RB_String::number(amount));
        } else {
            twItem = new QTableWidgetItem(tr("(extra tax item"));
        }

        twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
        twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
        twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
        tableWidget->setItem((taxRow), 1, twItem);

        mem = dt->getMember("taxhighamt");
        amount = mem->getValue().toDouble();
        twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
        twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
        tableWidget->setItem((taxRow), 2, twItem);
        // Hack;
        mTaxMap[twItem] = mem;

        mem = dt->getMember("taxlowamt");
        amount = mem->getValue().toDouble();
        twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
        twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
        tableWidget->setItem((taxRow), 3, twItem);
        mTaxMap[twItem] = mem;

        mem = dt->getMember("taxotheramt");
        amount = mem->getValue().toDouble();
        twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
        twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
        tableWidget->setItem((taxRow), 4, twItem);
        mTaxMap[twItem] = mem;

        ++taxRow;
    }

    delete iterDt;

    twItem = new QTableWidgetItem(tr("TOTAL (incl. tax)"));
    twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
    tableWidget->setItem((taxRow), 0, twItem);

    if (mIsSalesOrder) {
        amount = mTransDoc->getValue("totalamountrec").toDouble();
    } else {
        amount = mTransDoc->getValue("totalamountpay").toDouble();
    }
    twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
    twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
    twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
    tableWidget->setItem((taxRow), 1, twItem);

    twItem = new QTableWidgetItem("");
    twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
    tableWidget->setItem((taxRow), 2, twItem);

    twItem = new QTableWidgetItem(tr("Total Tax"));
    twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
    twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
    tableWidget->setItem((taxRow), 3, twItem);

    amount = mTransDoc->getValue("totaltax").toDouble();
    twItem = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
    twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
    twItem->setTextAlignment((int)(Qt::AlignRight | Qt::AlignVCenter));
    tableWidget->setItem((taxRow), 4, twItem);

    return;
}

/**
 * Button Help clicked
 */
void ACC_TaxConfirmationDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button Select item clicked
 */
void ACC_TaxConfirmationDialog::on_pbOk_clicked() {
    updateTax();
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_TaxConfirmationDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_TaxConfirmationDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Update tax amounts
 */
void ACC_TaxConfirmationDialog::updateTax() {
    double totalAmount = 0.0;
    double totalTax = 0.0;
    double tax = 0.0;

    // Get nett total amount
    RB_ObjectIterator* iterDt = mDebCredTransList->createIterator();
    for (iterDt->first(); !iterDt->isDone(); iterDt->next()) {
        RB_ObjectBase* dt = iterDt->currentObject();
        totalAmount += dt->getValue("amount").toDouble();
    }
    delete iterDt;

    // Hack:
    std::map<QTableWidgetItem*, RB_ObjectMember*>::iterator iter = mTaxMap.begin();
    while (iter != mTaxMap.end()) {
        std::pair<QTableWidgetItem*, RB_ObjectMember*> element = *iter;
        tax = element.first->text().toDouble();
        element.second->setValue(tax);
        totalTax += tax;
        ++iter;
    }

    mTransDoc->setValue("totaltax", totalTax);

    if (mIsSalesOrder) {
        mTransDoc->setValue("totalamountrec", totalAmount + totalTax);
    } else {
        mTransDoc->setValue("totalamountpay", totalAmount + totalTax);
    }

    setFields();
}

/**
 * Set taxes and recalculate the total values
 */
void ACC_TaxConfirmationDialog::on_pbRefreshTax_clicked() {
    updateTax();
    setFields();
}
