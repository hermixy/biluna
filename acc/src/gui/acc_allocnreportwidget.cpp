/*****************************************************************
 * $Id: acc_allocnreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jun 30, 2011 9:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_allocnreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_createalloclist.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_AllocnReportWidget::ACC_AllocnReportWidget(QWidget *parent)
                       : RB_SimpleReportWidget(parent) {
    setupUi(this);
    mTeReport = teAlloc;
}

/**
 * Destructor
 */
ACC_AllocnReportWidget::~ACC_AllocnReportWidget() {
    // nothing
}

/**
 * Initialize and create allocation widget
 */
void ACC_AllocnReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    setComboBoxes();

    QString html;
    html = "<p>";
    html += tr("Select from/to period and click refresh button ...");
    html += "</p>";

    teAlloc->setHtml(html);
    readSettings();
}


/**
 * Initialize and create allocation report
 */
void ACC_AllocnReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    setColumnWidthsAndTitle();

    ACC_CreateAllocList* oper = new ACC_CreateAllocList();
    RB_ObjectContainer* allocList = new RB_ObjectContainer("", NULL, "ACC_AllocnList");

    oper->setPeriod(dtFrom->date(), dtTo->date());
    oper->execute(allocList);

    RB_String html;
    createReport(html, allocList);
    mTeReport->setHtml(html);

    delete allocList;
    delete oper;

    teAlloc->setHtml(html);

    QApplication::restoreOverrideCursor();
}

/**
 * Change event such as language change
 */
void ACC_AllocnReportWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Set report column widths and title
 */
void ACC_AllocnReportWidget::setColumnWidthsAndTitle() {
    clearColumnWidths();

    // 8 columns (1+6+1)
    setColumnWidth(30);

    for (int i = 0; i < 7; ++i) {
        setColumnWidth(10);
    }

    // Title
    setAlternatingRow(true);
    setFirstDataFullRow(false);

    RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();
    RB_String reportTitle = root->getValue("company").toString();
    setReportTitle(reportTitle);
    setReportSubTitle(tr("Allocation Report"));
    RB_String limit = tr("Period: ") + dtFrom->date().toString()
            + " - " + dtTo->date().toString();
    setReportLimit(limit);
}

/**
 * Set comboboxes with alphabeth.
 */
void ACC_AllocnReportWidget::setComboBoxes() {
    QDateTime dt;
    int year = dt.currentDateTime().date().year();
    int startMonth = dt.currentDateTime().date().month();
    int endMonth = startMonth;
    int day = dt.currentDateTime().date().day();
    if (startMonth > 1) {
        --startMonth; // one full month before
    }
    QDate dateStart(year, startMonth, 1);
    dtFrom->setDate(dateStart);
    QDate dateEnd(year, endMonth, day);
    dtTo->setDate(dateEnd);
}
