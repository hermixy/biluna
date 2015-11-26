/*****************************************************************
 * $Id: db_permissionreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 21, 2015 7:00:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_sqlcommonfunctions.h"


/**
 * Constructor
 */
DB_PermissionReportWidget::DB_PermissionReportWidget(QWidget *parent)
                       : RB_SimpleReportWidget(parent) {
    setupUi(this);
    mTeReport = tePermissionList;
}

/**
 * Destructor
 */
DB_PermissionReportWidget::~DB_PermissionReportWidget() {
    // nothing
}

/**
 * Initialize and create allocation widget
 */
void DB_PermissionReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    // From DB_PermissionWidget
    mProjectStatusList.clear();
    mProjectStatusList << tr("Hidden") << tr("Locked")
                       << tr("Live") << tr("Test");

    // From DB_SystemGroupDialog
    mCrudxList.clear();
    mCrudxList << tr("None") << "[R]" << "[RU]" << "[CRUD]" << "[CRUDX]";

    QString html;
    html = "<p>";
    html += tr("Select from/to period and click refresh button ...");
    html += "</p>";

    tePermissionList->setHtml(html);
    readSettings();
}

void DB_PermissionReportWidget::setDataRow(QString& html,
                                           const QSqlRecord& rec) {
    mColRunner = 0;
    mRowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        switch (col) {
        case 6:
            setColumnData(html, mCrudxList.at(
                              rec.value(col).toInt()), col);
            break;
        case 10:
            setColumnData(html, mProjectStatusList.at(
                              rec.value(col).toInt()), col);
            break;
        default:
            setColumnData(html, rec.value(col).toString(), col);
            break;
        }
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;
}


/**
 * Initialize and create allocation report
 */
void DB_PermissionReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    setColumnWidthsAndTitle();

    QSqlQuery query = QSqlQuery(DB_MODELFACTORY->getDatabase());
    DB_SqlCommonFunctions f;
    f.getPermissionReportList(query);

    RB_String html;
    createReport(html, query);
    mTeReport->setHtml(html);

    QApplication::restoreOverrideCursor();
}

/**
 * Change event such as language change
 */
void DB_PermissionReportWidget::changeEvent(QEvent *e) {
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
void DB_PermissionReportWidget::setColumnWidthsAndTitle() {
    clearColumnWidths();

    for (int i = 0; i < 7; ++i) {
        setColumnWidth(7);
    }

    setColumnWidth(5);
    setColumnWidth(10);
    setColumnWidth(15);

    for (int i = 0; i < 3; ++i) {
        setColumnWidth(7);
    }

    // Title
    setAlternatingRow(true);
    setFirstDataFullRow(false);

    RB_ObjectBase* root = DB_MODELFACTORY->getRoot();
    RB_String reportTitle = root->getValue("number").toString()
            + " - " + root->getValue("company").toString();
    setReportTitle(reportTitle);
    setReportSubTitle(tr("DB Permission List Report"));
    RB_String limit = "Period: all";
    setReportLimit(limit);
}

/**
 * Set comboboxes and other limit widgets
 */
void DB_PermissionReportWidget::setWidgets() {
//    QDateTime dt;
//    int year = dt.currentDateTime().date().year();
//    int startMonth = dt.currentDateTime().date().month();
//    int endMonth = startMonth;
//    int day = dt.currentDateTime().date().day();
//    if (startMonth > 1) {
//        --startMonth; // one full month before
//    }
//    QDate dateStart(year, startMonth, 1);
//    dtFrom->setDate(dateStart);
//    QDate dateEnd(year, endMonth, day);
//    dtTo->setDate(dateEnd);
}
