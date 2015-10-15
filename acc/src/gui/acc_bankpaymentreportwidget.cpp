/*****************************************************************
 * $Id: sail_trackdistancereportwidget.cpp 1745 2012-08-22 17:57:33Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankpaymentreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include "db_actionfilesaveas.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_createbankpaymentlist.h"

/**
 * Constructor
 */
ACC_BankPaymentReportWidget::ACC_BankPaymentReportWidget(QWidget *parent)
                        : RB_SimpleReportWidget(parent) {
    setupUi(this);
    mTeReport = teReport;
}

/**
 * Destructor
 */
ACC_BankPaymentReportWidget::~ACC_BankPaymentReportWidget() {
    // nothing
}

/**
 * @returns widget name based on document setting Debtor or Creditor
 */
RB_String ACC_BankPaymentReportWidget::getName() const {
    return "ACC Bank Payment Report";
}

/**
 * Initialize
 */
void ACC_BankPaymentReportWidget::init() {
    RB_SimpleReportWidget::init();
    setSelectionWidgets();
    readSettings();
}

/**
 * Initialize and software license report
 */
void ACC_BankPaymentReportWidget::on_pbRefresh_clicked() {
    RB_ObjectContainer* paymentList = NULL;

    try {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        setColumnWidthsAndTitle();

        paymentList =
                new RB_ObjectContainer("", NULL, "ACC_BankPaymentList");

        ACC_CreateBankPaymentList oper;
//        oper.setLimits(cbField->currentText(),
//                       cbFrom->currentText(),
//                       cbTo->currentText());
//        oper.execute(paymentList);

        // Create report
        RB_String html;
        createReport(html, paymentList);
        mTeReport->setHtml(html);

        delete paymentList;
        QApplication::restoreOverrideCursor();
    } catch(std::exception& e) {
        delete paymentList;
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(e.what());
    } catch(...) {
        delete paymentList;
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in "
                    "ACC_BankPaymentReportWidget::on_pbRefresh_clicked()");
    }
}

/**
 * Set report column widths and title
 */
void ACC_BankPaymentReportWidget::setColumnWidthsAndTitle() {
    clearColumnWidths();

    // 8 columns (1+6+1)
    setColumnWidth(15);

    for (int i = 0; i < 6; ++i) {
        setColumnWidth(10);
    }

    setColumnWidth(25);

    // Title
    setAlternatingRow(true);
    setFirstDataFullRow(false);

    RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();
    RB_String reportTitle = root->getValue("coyname").toString();
    setReportTitle(reportTitle);
    setReportSubTitle(" Bank Payments");
    RB_String accountNumber = leBankAccount->text().isEmpty()
            ? "All accounts" : leBankAccount->text();
    RB_String limit = tr("Account: ") + accountNumber
            + tr(" - From: ") + deFrom->text()
            + tr(" To: ") + deTo->text();
    setReportLimit(limit);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_BankPaymentReportWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_BankPaymentReportWidget::closeWidget() {
    return RB_Widget::closeWidget();
}

/**
 * Change event such as language change
 */
void ACC_BankPaymentReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Set comboboxes with alphabeth.
 */
void ACC_BankPaymentReportWidget::setSelectionWidgets() {
    QDate date = QDate::currentDate();
    deTo->setDate(date);
    date = date.addMonths(-1);
    deFrom->setDate(date);

    RB_StringList strL;
    strL << "All Accounts" << "Selected Account";
    cbSelect->addItems(strL);
    cbSelect->setCurrentIndex(1);
}

