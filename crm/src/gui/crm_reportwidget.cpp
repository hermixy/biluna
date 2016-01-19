/*****************************************************************
 * $Id: crm_reportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jun 1, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_reportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
//#include "acc_creategltrialbalance.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
CRM_ReportWidget::CRM_ReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    // setupUi(this);

    // mTeReport = teReport;
    mTeReport = NULL;

    mReportColCount = 3;
    mReportTitle = "";
    mCompanyName = "";
    mReportLimit = "";
    mDataList = NULL;
    mEvenRow = true;
    mIsAlternatingRow = true;
    mIsFirstDataFullRow = false;
}

/**
 * Destructor
 */
CRM_ReportWidget::~CRM_ReportWidget() {
    // nothing
}

/**
 * Initialize and create trial balance
 */
void CRM_ReportWidget::init() {
    RB_String html = "<p style=\"font-size:11pt;\">";
    html += tr("Select limits above and click refresh button ...");
    html += "</p>";

    //'Times New Roman', Times, serif
    mTeReport->setStyleSheet("font-family:'Times New Roman'; font-size:8pt;");
    mTeReport->setHtml(html);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool CRM_ReportWidget::fileSave(bool /*withSelect*/) {
    // if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    // }
    // return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool CRM_ReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}
/**
 * Slot File export PDF.
 * @param printer
 */
void CRM_ReportWidget::filePdf(QPrinter* printer) {
    mTeReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void CRM_ReportWidget::filePrint(QPrinter* printer) {
    mTeReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void CRM_ReportWidget::filePrintPreview(QPrinter* printer) {
    mTeReport->print(printer);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool CRM_ReportWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        CRM_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        CRM_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (fn.endsWith(".htm", Qt::CaseInsensitive)
        || fn.endsWith(".html", Qt::CaseInsensitive)
        || fn.endsWith(".xhtml", Qt::CaseInsensitive)) {
        out << mTeReport->toHtml();
    } else {
        out << mTeReport->toPlainText();
    }

    QApplication::restoreOverrideCursor();

    CRM_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}



/**
 * Create report based on object list
 * @param html report HTML
 * @param list container with list of data objects
 */
void CRM_ReportWidget::createReport(RB_String& html,
                                    RB_ObjectContainer* list) {
    setReportHeader(html);
    bool isFirst = true;
    RB_ObjectIterator* iter =  list->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (isFirst) {
            setTableHeader(html, obj);
            isFirst = false;
        }
        setDataRow(html, obj);
    }

    delete iter;

    setTableFooter(html);
    setReportFooter(html);
}

/**
 * Create report based on database query
 * @param html report HTML
 * @param list container with list of data objects
 */
void CRM_ReportWidget::createReport(RB_String& html, QSqlQuery& query) {

    setReportHeader(html);
    bool isFirst = true;

    while (query.next()) {
        QSqlRecord rec = query.record();
        if (isFirst) {
            setTableHeader(html, rec);
            isFirst = false;
        }
        setDataRow(html, rec);
    }

    setTableFooter(html);
    setReportFooter(html);
}

/**
 * Set the report header text. Always call at the start of
 * the report creation. The report header is a separate table.
 * Will be printed only on the first page of the report.
 * @param html HTML to be set
 */
void CRM_ReportWidget::setReportHeader(RB_String& html) {
    QDateTime dt = QDateTime::currentDateTime();
    html += "<table width=\"100%\" border=\"0\">"
            "<tr><td align=\"center\" style=\"font-size:11pt;\"><b>"
            + mReportTitle + " - " + mCompanyName + "</b></td></tr>"
            + "<tr><td align=\"center\">" + mReportLimit + "<br />"
            + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr></table>"
            "<hr style=\"border-style:solid;\" />";
}

/**
 * Set the table header text. This will contain the column
 * headings and will be printed at the top of each page.
 * @param html HTML to be set
 * @param obj data object
 */
void CRM_ReportWidget::setTableHeader(RB_String& html, RB_ObjectBase* obj) {
    mReportColCount = mColumnWidthVector.size();
    mMemCount = obj->memberCount() - (int)RB2::HIDDENCOLUMNS;
    mNoDataRow = (int)((mMemCount - 1) / (mReportColCount - 1));

    if ((mMemCount - 1) % (mReportColCount - 1)) {
        ++mNoDataRow; // there is a rest
    }

    if (mIsFirstDataFullRow) {
        ++mNoDataRow; // there is a separate first row
    }

    int colRunner = 0;
    int rowRunner = 0;

    html += "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\"><thead><tr>";

    for (int col = 0; col < mMemCount; ++col) {
        if (mIsFirstDataFullRow && col == 0) {
            html += "<th align=\"left\" colspan=\"" + RB_String::number(mReportColCount)
                    + "\">" + obj->getMember((int)RB2::HIDDENCOLUMNS + col)->getName();
            colRunner = mReportColCount - 1;
        } else {
            if (rowRunner == 0 && !mIsFirstDataFullRow) {
                html += "<th width=\""
                        + RB_String::number(mColumnWidthVector.at(col)) + "%\"><b>"
                        + obj->getMember((int)RB2::HIDDENCOLUMNS + col)->getName() + "</b>";
            } else if (rowRunner == 1 && mIsFirstDataFullRow) {
                html += "<th align=\"center\" width=\""
                        + RB_String::number(mColumnWidthVector.at(col-1)) + "%\"><b>"
                        + obj->getMember((int)RB2::HIDDENCOLUMNS + col)->getName() + "</b>";
            } else {
                html += "<th align=\"center\"><b>"
                        + obj->getMember((int)RB2::HIDDENCOLUMNS + col)->getName() + "</b>";
            }
        }

        // draw line if last row
        if (rowRunner == mNoDataRow - 1) {
            html += "<hr style=\"border-style:solid;height:5pt;\" />";
        }

        html += "</th>";

        // new row
        if (colRunner + 1 == mReportColCount && col < mMemCount - 1) {
            ++rowRunner;
            html += "</tr><tr>";

            // skip first cell in new row
            colRunner = 1;
            if (rowRunner == mNoDataRow - 1) {
                html += "<th>&nbsp;<hr style=\"border-style:solid;height:5pt;\" /></th>";
            } else {
                html += "<th align=\"center\">&nbsp;</th>";
            }
        } else if (col == mMemCount - 1) {
            // fill out last cells
            for (int i = 0; i < mReportColCount - 1 - colRunner; ++i) {
                html += "<th>&nbsp;<hr style=\"border-style:solid;height:5pt;\" /></th>";
            }

        } else {
            ++colRunner;
        }
    }

    html += "</tr>"
            "</thead>"
            "<tbody>";
}

/**
 * Set the table header text. This will contain the column
 * headings and will be printed at the top of each page.
 * @param html HTML to be set
 * @param rec database query record
 */
void CRM_ReportWidget::setTableHeader(RB_String& html, const QSqlRecord& rec) {
    mReportColCount = mColumnWidthVector.size();
    mMemCount = rec.count(); // obj->memberCount() - (int)RB2::HIDDENCOLUMNS;
    mNoDataRow = (int)((mMemCount - 1) / (mReportColCount - 1));

    if ((mMemCount - 1) % (mReportColCount - 1)) {
        ++mNoDataRow; // there is a rest
    }

    if (mIsFirstDataFullRow) {
        ++mNoDataRow; // there is a separate first row
    }

    int colRunner = 0;
    int rowRunner = 0;

    html += "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\"><thead><tr>";

    for (int col = 0; col < mMemCount; ++col) {
        if (mIsFirstDataFullRow && col == 0) {
            html += "<th align=\"left\" colspan=\"" + RB_String::number(mReportColCount)
                    + "\">" + rec.fieldName(col);
            colRunner = mReportColCount - 1;
        } else {
            if (rowRunner == 0 && !mIsFirstDataFullRow) {
                html += "<th width=\""
                        + RB_String::number(mColumnWidthVector.at(col)) + "%\"><b>"
                        + rec.fieldName(col) + "</b>";
            } else if (rowRunner == 1 && mIsFirstDataFullRow) {
                html += "<th align=\"center\" width=\""
                        + RB_String::number(mColumnWidthVector.at(col-1)) + "%\"><b>"
                        + rec.fieldName(col) + "</b>";
            } else {
                html += "<th align=\"center\"><b>"
                        + rec.fieldName(col) + "</b>";
            }
        }

        // draw line if last row
        if (rowRunner == mNoDataRow - 1) {
            html += "<hr style=\"border-style:solid;height:5pt;\" />";
        }

        html += "</th>";

        // new row
        if (colRunner + 1 == mReportColCount && col < mMemCount - 1) {
            ++rowRunner;
            html += "</tr><tr>";

            // skip first cell in new row
            colRunner = 1;
            if (rowRunner == mNoDataRow - 1) {
                html += "<th>&nbsp;<hr style=\"border-style:solid;height:5pt;\" /></th>";
            } else {
                html += "<th align=\"center\">&nbsp;</th>";
            }
        } else if (col == mMemCount - 1) {
            // fill out last cells
            for (int i = 0; i < mReportColCount - 1 - colRunner; ++i) {
                html += "<th>&nbsp;<hr style=\"border-style:solid;height:5pt;\" /></th>";
            }

        } else {
            ++colRunner;
        }
    }

    html += "</tr>"
            "</thead>"
            "<tbody>";
}

/**
 * Set the data header text. This is used to  give a separate
 * title to the series of data rows that will follow. For example
 * in a tree data model the path of the tree will be displayed.
 * Does nothing.
 * @param html HTML to be set
 */
void CRM_ReportWidget::setDataHeader(RB_String& html) {
    // nothing
}

/**
 * Set the actual data.
 * @param html HTML to be set
 * @param obj data object
 */
void CRM_ReportWidget::setDataRow(RB_String& html, RB_ObjectBase* obj) {
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
void CRM_ReportWidget::setDataRow(QString &html, const QSqlRecord &rec) {
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
                    + "\">" + rec.value(col).toString() + "</td>";
            colRunner = mReportColCount - 1;
        } else {
            html += "<td>" + rec.value(col).toString() + "</td>";
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
 * Set the data footer of the previous data serie. This footer
 * can be used for example to give an intermediate result or
 * or sum of values in the data rows. Does nothing.
 * @param html HTML to be set
 */
void CRM_ReportWidget::setDataFooter(RB_String& html) {
    // nothing
}

/**
 * Set the table footer with closing tags of table.
 * Could include for examplethe overall data result.
 * This result can be the total sum of data values in
 * the report.
 * @param html HTML to be set
 */
void CRM_ReportWidget::setTableFooter(RB_String& html) {
    html += "</tbody></table>";
}

/**
 * Set the report footer with for example with the statement
 * "--- END OF REPORT ---".
 * @param html HTML to be set
 */
void CRM_ReportWidget::setReportFooter(RB_String& html) {
    html += "<p align=\"center\">--- END OF REPORT ---</p>";
}

