/*****************************************************************
 * $Id: sail_trackdistancereportwidget.cpp 1745 2012-08-22 17:57:33Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_softwarelicensereportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include "db_actionfilesaveas.h"
#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"
#include "mrp_softwarelicenseoperation.h"

/**
 * Constructor
 */
MRP_SoftwareLicenseReportWidget::MRP_SoftwareLicenseReportWidget(QWidget *parent)
                        : RB_SimpleReportWidget(parent) {
    setupUi(this);
    mTeReport = teReport;
}

/**
 * Destructor
 */
MRP_SoftwareLicenseReportWidget::~MRP_SoftwareLicenseReportWidget() {
    // nothing
}

/**
 * @returns widget name based on document setting Debtor or Creditor
 */
RB_String MRP_SoftwareLicenseReportWidget::getName() const {
    return "MRP Software License Report";
}

/**
 * Initialize
 */
void MRP_SoftwareLicenseReportWidget::init() {
    RB_SimpleReportWidget::init();
    setComboBoxes();
    readSettings();
}

/**
 * Initialize and software license report
 */
void MRP_SoftwareLicenseReportWidget::on_pbRefresh_clicked() {
    RB_ObjectContainer* licenseList = NULL;

    try {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        setColumnWidthsAndTitle();

        licenseList =
                new RB_ObjectContainer("", NULL, "MRP_SoftwareLicenseList");

        MRP_SoftwareLicenseOperation oper;
        oper.setLimits(cbField->currentText(),
                       cbFrom->currentText(),
                       cbTo->currentText());
        oper.execute(licenseList);

        // Create report
        RB_String html;
        createReport(html, licenseList);
        mTeReport->setHtml(html);

        delete licenseList;
        QApplication::restoreOverrideCursor();
    } catch(std::exception& e) {
        delete licenseList;
        QApplication::restoreOverrideCursor();
        MRP_DIALOGFACTORY->requestWarningDialog(e.what());
    } catch(...) {
        delete licenseList;
        QApplication::restoreOverrideCursor();
        MRP_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in "
                    "MRP_SoftwareLicenseReportWidget::on_pbRefresh_clicked()");
    }
}

/**
 * Set report column widths and title
 */
void MRP_SoftwareLicenseReportWidget::setColumnWidthsAndTitle() {
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

    RB_ObjectBase* root = MRP_MODELFACTORY->getRoot();
    RB_String reportTitle = root->getValue("coyname").toString();
    setReportTitle(reportTitle);
    setReportSubTitle(" Software Licenses");
    RB_String limit = tr("Field: ") + cbField->currentText()
            + tr(" - From: ") + cbFrom->currentText()
            + tr(" To: ") + cbTo->currentText();
    setReportLimit(limit);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String MRP_SoftwareLicenseReportWidget::getHelpSubject() const {
    return objectName();
}

bool MRP_SoftwareLicenseReportWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
}

/**
 * Change event such as language change
 */
void MRP_SoftwareLicenseReportWidget::changeEvent(QEvent *e) {
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
void MRP_SoftwareLicenseReportWidget::setComboBoxes() {
    RB_StringList strL;
    strL << "End Date" << "Last Name" << "Modified";
    cbField->addItems(strL);

    strL.clear();
    strL << "0" << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I"
            << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R"
            << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Zzzz";
    cbFrom->addItems(strL);
    cbTo->addItems(strL);

    cbFrom->setCurrentIndex(0);
    int count = cbTo->count();
    cbTo->setCurrentIndex(count - 1);
}

