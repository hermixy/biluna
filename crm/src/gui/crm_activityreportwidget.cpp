/*****************************************************************
 * $Id: crm_activityreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jun 8, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_activityreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
CRM_ActivityReportWidget::CRM_ActivityReportWidget(QWidget *parent)
                        : CRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
CRM_ActivityReportWidget::~CRM_ActivityReportWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void CRM_ActivityReportWidget::init() {
    CRM_ReportWidget::init();

    setLists();
    cbActivityType->setCurrentIndex(0);
    QDate dt = QDate::currentDate();
    dtFromStartDate->setDate(dt);
    cbFilterByAccount->setChecked(true);
}

/**
 * File Save, can be called by closeWidget()
 */
bool CRM_ActivityReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool CRM_ActivityReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void CRM_ActivityReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void CRM_ActivityReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void CRM_ActivityReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void CRM_ActivityReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void CRM_ActivityReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void CRM_ActivityReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void CRM_ActivityReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void CRM_ActivityReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void CRM_ActivityReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void CRM_ActivityReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief CRM_ActivityReportWidget::editZoomIn
 */
void CRM_ActivityReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief CRM_ActivityReportWidget::editZoomOut
 */
void CRM_ActivityReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool CRM_ActivityReportWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        CRM_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        CRM_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teReport->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    CRM_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    emit modificationChanged(false);
    return true;
}

/**
 * Initialize and create trial balance
 */
void CRM_ActivityReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Contact List
    clearColumnWidths();
    setColumnWidth(8); // activitytype_id
    setColumnWidth(8); // customerno
    setColumnWidth(20); // activitycode
    setColumnWidth(28); // subject
    setColumnWidth(10); // mstart
    setColumnWidth(10); // mend
    setColumnWidth(8); // priority_id
    setColumnWidth(8); // status_id
    // setAlternatingRow(true);
    // setFirstDataFullRow(true);

    setReportTitle(tr("Activity List"));
    setCompanyName(CRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Activity: ") + cbActivityType->currentText();
    setReportLimit(limit);

    int type = cbActivityType->currentIndex();
    RB_String activityType;

    switch (type) {
    case 0 :
        activityType = "crm_activity.id <> '0' ";
        break;
    case 1 :
        activityType = "activitytype_id = 0 ";
        break;
    case 2 :
        activityType = "activitytype_id = 1 ";
        break;
    case 3 :
        activityType = "activitytype_id = 2 ";
        break;
    case 4 :
        activityType = "activitytype_id = 3 ";
        break;
    case 5 :
        activityType = "activitytype_id = 4 ";
        break;
    case 6 :
        activityType = "activitytype_id = 5 ";
        break;
    case 7 :
        activityType = "activitytype_id = 6 ";
        break;
    case 8 :
        activityType = "activitytype_id = 7 ";
        break;
    case 9 :
        activityType = "activitytype_id = 8 ";
        break;
    case 10 :
        activityType = "activitytype_id = 9 ";
        break;
    default :
        activityType = " ";
        break;
    }

    QSqlQuery q(CRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT activitytype_id as type, acc_customer.customerno as custcode, "
            "activitycode, subject, SUBSTR(mstart,1,10) as start, SUBSTR(mend,1,10) as end, priority_id, status_id "
            "FROM crm_activity LEFT OUTER JOIN acc_customer ON acc_customer.id=crm_activity.acc_customer_id ";
    str += "WHERE mend > '" + dtFromStartDate->date().toString(Qt::ISODate) + "' ";
    str += "AND " + activityType;

    if (!chbShowCompleted->isChecked()) {
        str += "AND status_id < 4 ";
    }

    if (cbFilterByAccount->isChecked()) {
        str += "AND crm_activity.parent='";
        str += CRM_MODELFACTORY->getRootId() + "' ";
    }

    str += "ORDER BY custcode, mend;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->error("CRM_ActivityReportWidget::on_pbRefresh_clicked() "
                        + q.lastError().text() + " ERROR");
        QApplication::restoreOverrideCursor();
        return;
    }

    RB_String html;
    createReport(html, q);
    mTeReport->setHtml(html);

    QApplication::restoreOverrideCursor();
}

/**
 * Set the actual data.
 * @param html HTML to be set
 * @param obj data object
 */
void CRM_ActivityReportWidget::setDataRow(RB_String& html, RB_ObjectBase* obj) {
    int colRunner = 0;
    int rowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mAlternateBaseColor.name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        if (mIsFirstDataFullRow && col == 0) {
            html += "<td colspan=\"" + RB_String::number(mReportColCount)
                    + "\">" + obj->getValue((int)RB2::HIDDENCOLUMNS + col).toString() + "</td>";
            colRunner = mReportColCount - 1;
        } else {
            html += "<td>" + obj->getValue((int)RB2::HIDDENCOLUMNS + col).toString() + "</td>";
        }

        // new row
        if (colRunner + 1 == mReportColCount && col < mMemCount - 1) {
            ++rowRunner;
            if (!mIsAlternatingRow || !mEvenRow) {
                html += "</tr><tr>";
            } else {
                html += "</tr><tr bgcolor=\"" + mAlternateBaseColor.name() + "\">";
            }

            // skip first cell in new row
            colRunner = 1;
            html += "<td>&nbsp;</td>";
        } else if (col == mMemCount - 1) {
            // fill out last cells
            for (int i = 0; i < mReportColCount - 1 - colRunner; ++i) {
                html += "<td>&nbsp;</td>";
            }

        } else {
            ++colRunner;
        }
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;
}

/**
 * Set the actual data.
 * @param html HTML to be set
 * @param rec database query record
 */
void CRM_ActivityReportWidget::setDataRow(QString& html, const QSqlRecord& rec) {
    int colRunner = 0;
    int rowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mAlternateBaseColor.name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        if (mIsFirstDataFullRow && col == 0) {
            // updated compared to CRM_ReportWidget
            if (col == 0) {
                html += "<td colspan=\"" + RB_String::number(mReportColCount)
                        + "\">" + mTypeVector.at(rec.value(col).toInt()) + "</td>";
            } else if (col == 5) {
                html += "<td colspan=\"" + RB_String::number(mReportColCount)
                        + "\">" + mPriorityVector.at(rec.value(col).toInt()) + "</td>";
            } else if (col == 6) {
                html += "<td colspan=\"" + RB_String::number(mReportColCount)
                        + "\">" + mStatusVector.at(rec.value(col).toInt()) + "</td>";
            } else {
                html += "<td colspan=\"" + RB_String::number(mReportColCount)
                        + "\">" + rec.value(col).toString() + "</td>";
            }
            colRunner = mReportColCount - 1;
        } else {
            // updated compared to CRM_ReportWidget
            if (col == 0) {
                html += "<td>" + mTypeVector.at(rec.value(col).toInt()) + "</td>";
            } else if (col == 6) {
                int priority = rec.value(col).toInt();
                if (priority == 1) {
                    html += "<td bgcolor=\"#CCFFCC\">" + mPriorityVector.at(rec.value(col).toInt()) + "</td>";
                } else if (priority == 2) {
                    html += "<td bgcolor=\"" + mAlternateBaseColor.name() + "\">" + mPriorityVector.at(rec.value(col).toInt()) + "</td>";
                } else if (priority == 3) {
                    html += "<td bgcolor=\"#FFCCCC\">" + mPriorityVector.at(rec.value(col).toInt()) + "</td>";
                } else {
                    html += "<td>" + mPriorityVector.at(rec.value(col).toInt()) + "</td>";
                }
            } else if (col == 7) {
                html += "<td>" + mStatusVector.at(rec.value(col).toInt()) + "</td>";
            } else {
                html += "<td>" + rec.value(col).toString() + "</td>";
            }
        }

        // new row
        if (colRunner + 1 == mReportColCount && col < mMemCount - 1) {
            ++rowRunner;
            if (!mIsAlternatingRow || !mEvenRow) {
                html += "</tr><tr>";
            } else {
                html += "</tr><tr bgcolor=\"" + mAlternateBaseColor.name() + "\">";
            }

            // skip first cell in new row
            colRunner = 1;
            html += "<td>&nbsp;</td>";
        } else if (col == mMemCount - 1) {
            // fill out last cells
            for (int i = 0; i < mReportColCount - 1 - colRunner; ++i) {
                html += "<td>&nbsp;</td>";
            }

        } else {
            ++colRunner;
        }
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_ActivityReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes and list.
 */
void CRM_ActivityReportWidget::setLists() {
    RB_StringList strL;
    strL << tr("All") << tr("Call") << tr("Case") << tr("Email") << tr("Log")
         << tr("Meeting") << tr("Note") << tr("Opportunity")
         << tr("Reminder") << tr("Solution") << tr("Task");
    cbActivityType->addItems(strL);

    RB_StringList typeItems;
    typeItems << tr("Call") << tr("Case") << tr("Email") << tr("Log")
              << tr("Meeting") << tr("Note") << tr("Opportunity")
              << tr("Reminder") << tr("Solution") << tr("Task");

    for (int i = 0; i < typeItems.count(); ++i) {
        mTypeVector.push_back(typeItems.at(i));
    }

    RB_StringList prioItems;
    prioItems << tr("None") << tr("Low") << tr("Medium") << tr("High");

    for (int i = 0; i < prioItems.count(); ++i) {
        mPriorityVector.push_back(prioItems.at(i));
    }

    RB_StringList statusItems;
    statusItems << tr("None") << tr("Not started") << tr("In progress")
                << tr("Pending input") << tr("Completed") << tr("Deferred");

    for (int i = 0; i < statusItems.count(); ++i) {
        mStatusVector.push_back(statusItems.at(i));
    }

}

/**
 * Change event such as language change
 */
void CRM_ActivityReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



