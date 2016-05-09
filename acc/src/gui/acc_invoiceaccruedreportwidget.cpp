/*****************************************************************
 * $Id: acc_invoiceaccruedreportwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 11, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_invoiceaccruedreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_invoiceaccrued.h"
#include "acc_modelfactory.h"
#include "acc_qachartmaster.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_InvoiceAccruedReportWidget::ACC_InvoiceAccruedReportWidget(QWidget *parent)
                        : RB_SimpleReportWidget(parent) {
    setupUi(this);
    mTeReport = teReport;
    mFiscalYearStart.setDate(1970,1,1);
    mFiscalYearEnd.setDate(1970, 1, 1);
    mAmountRec = 0.0;
    mAmountPay = 0.0;
    mAmountAlloc = 0.0;
    mTotalAmountRec = 0.0;
    mTotalAmountPay = 0.0;
    mPreviousTransDocNo = "";
}

/**
 * Destructor
 */
ACC_InvoiceAccruedReportWidget::~ACC_InvoiceAccruedReportWidget() {
    // nothing
}

/**
 * @returns widget name based on document setting Debtor or Creditor
 */
RB_String ACC_InvoiceAccruedReportWidget::getName() const {
    return "ACC Invoice Accrued List Report";
}

/**
 * Initialize
 */
void ACC_InvoiceAccruedReportWidget::init() {
    RB_SimpleReportWidget::init();
    setWidgets();
    readSettings();
}

/**
 * Initialize and software license report
 */
void ACC_InvoiceAccruedReportWidget::on_pbRefresh_clicked() {
    mAmountRec = 0.0;
    mAmountPay = 0.0;
    mAmountAlloc = 0.0;
    mTotalAmountRec = 0.0;
    mTotalAmountPay = 0.0;
    mPreviousTransDocNo = "";
    RB_ObjectContainer* invoiceAccruedList = NULL;

    try {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        setColumnWidthsAndTitle();

        invoiceAccruedList =
                new RB_ObjectContainer("", NULL, "ACC_InvoiceAccruedList");

        int yearDifference = sbYear->value() - mFiscalYearEnd.year();
        QDate endDate(mFiscalYearEnd.year() + yearDifference,
                        mFiscalYearEnd.month(), mFiscalYearEnd.day());
        ACC_InvoiceAccrued oper;
        oper.setEndDate(endDate);

        if (cbDebtorCreditor->currentIndex() == 0) {
            oper.setDebtorCreditor(true);
        } else {
            oper.setDebtorCreditor(false);
        }

        oper.execute(invoiceAccruedList);

        // Create report
        RB_String html;
        createReport(html, invoiceAccruedList);
        mTeReport->setHtml(html);

        delete invoiceAccruedList;
        QApplication::restoreOverrideCursor();
    } catch(std::exception& e) {
        delete invoiceAccruedList;
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(
             QString("ACC_InvoiceAccruedReportWidget::on_pbRefresh_clicked()\n")
             + QString("Exception: ") + QString(e.what()));
    } catch(...) {
        delete invoiceAccruedList;
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in "
                    "ACC_InvoiceAccruedReportWidget::on_pbRefresh_clicked()");
    }

}

/**
 * Set report column widths and title
 */
void ACC_InvoiceAccruedReportWidget::setColumnWidthsAndTitle() {
    clearColumnWidths();

    // 8 columns
    setColumnWidth(10);
    setColumnWidth(9);
    setColumnWidth(30);
    setColumnWidth(9);
    setColumnWidth(9);
    setColumnWidth(9);
    setColumnWidth(9);
    setColumnWidth(9);
    setColumnWidth(9);

    // Title
    setAlternatingRow(true);
    setFirstDataFullRow(false);

    RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();
    RB_String reportTitle = root->getValue("company").toString();
    setReportTitle(reportTitle);
    if (cbDebtorCreditor->currentIndex() == 0) {
        setReportSubTitle(" Debtor Accrued");
    } else {
        setReportSubTitle(" Creditor Accrued");
    }

    RB_String limit = tr("Year ending: ") + RB_String::number(sbYear->value())
            + lblMonthDay->text();
    setReportLimit(limit);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_InvoiceAccruedReportWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_InvoiceAccruedReportWidget::closeWidget() {
    return RB_Widget::closeWidget();
}

/**
 * Change event such as language change
 */
void ACC_InvoiceAccruedReportWidget::changeEvent(QEvent *e) {
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
 * Set the actual data, overrides RB_SimpleReportWidget
 * @param html HTML to be set
 * @param obj data object
 */
void ACC_InvoiceAccruedReportWidget::setDataRow(QString& html,
                                                RB_ObjectBase* obj) {
    mColRunner = 0;
    mRowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    double amount = 0.0;
    QString str = "";
    QString transDocNo = obj->getValue((int)RB2::HIDDENCOLUMNS + 1).toString();
    bool sameAsPreviousNo = mPreviousTransDocNo == transDocNo;

    for (int col = 0; col < mMemCount; ++col) {
        if (sameAsPreviousNo && col < 5) {
            str = "";
        } else if (col == 3 || col == 4 || col == 7 || col == 8){
            amount = obj->getValue((int)RB2::HIDDENCOLUMNS + col).toDouble();

            if (col == 3) {
                mAmountRec += amount;
                mTotalAmountRec += amount;
            } else if (col == 4) {
                mAmountPay += amount;
                mTotalAmountPay += amount;
            } else if (col == 7) {
                if (amount > 0) {
                    mAmountAlloc += amount;
                    mTotalAmountRec -= amount;
                } else if (amount < 0) {
                    mAmountAlloc += amount;
                    mTotalAmountPay += amount;
                }
            }

            str = "<p style=\"text-align: right;\">";
            str += QString::number(ACC2::roundMoney(amount), 'f', 2);
            str += "</p>";
        } else if (col == 5 || col == 6) {
            str = "<p style=\"text-align: right;\">";
            str += obj->getValue((int)RB2::HIDDENCOLUMNS + col).toString();
            str += "</p>";
        } else {
            str = obj->getValue((int)RB2::HIDDENCOLUMNS + col).toString();
        }

        setColumnData(html, str, col);
    }

    mPreviousTransDocNo = transDocNo;
    html += "</tr>";
    mEvenRow = !mEvenRow;
}

void ACC_InvoiceAccruedReportWidget::setDataFooter(QString &html) {
    QString startCell = "<td><p style=\"text-align: right;\"><strong>";
    QString endCell = "</strong></p></td>";

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        if (col == 2) {
            html += startCell + "Subtotals:" + endCell;
        } else if (col == 3) {
            html += startCell
                    + QString::number(ACC2::roundMoney(mAmountRec), 'f', 2)
                    + endCell;
        } else if (col == 4) {
            html += startCell
                    + QString::number(ACC2::roundMoney(mAmountPay), 'f', 2)
                    + endCell;
        } else if (col == 7) {
            html += startCell
                    + QString::number(ACC2::roundMoney(mAmountAlloc), 'f', 2)
                    + endCell;
        } else {
            html += "<td>&nbsp;</td>";

        }
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        if (col == 2) {
            html += startCell + "Total Open Amount:" + endCell;
        } else if (col == 3) {
            html += startCell
                    + QString::number(ACC2::roundMoney(mTotalAmountRec), 'f', 2)
                    + endCell;
        } else if (col == 4) {
            html += startCell
                    + QString::number(ACC2::roundMoney(mTotalAmountPay), 'f', 2)
                    + endCell;
        } else {
            html += "<td>&nbsp;</td>";

        }
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;
}

/**
 * Set widgets with limits
 */
void ACC_InvoiceAccruedReportWidget::setWidgets() {
    RB_StringList strL;
    strL << tr("Debtor Accrued") << tr("Creditor Accrued");
    this->cbDebtorCreditor->addItems(strL);

    QDate currentDate = QDate::currentDate();
    mFiscalYearStart = ACC_QACHARTMASTER->getFiscalYearStart(currentDate);
    mFiscalYearEnd = ACC_QACHARTMASTER->getFiscalYearEnd(currentDate);

    sbYear->setValue(mFiscalYearEnd.year() - 1);
    RB_String monthDay = mFiscalYearEnd.toString("-MM-dd");
    lblMonthDay->setText(monthDay);
}

