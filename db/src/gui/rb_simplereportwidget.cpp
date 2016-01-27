/*****************************************************************
 * $Id: rb_simplereportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Sept 26, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_simplereportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_textedit.h"

/**
 * Constructor
 */
RB_SimpleReportWidget::RB_SimpleReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    // setupUi(this);
    // mTeReport = teReport; central widget

    mTeReport = NULL;

    mReportColCount = 3;
    mReportTitle = "";
    mReportSubTitle = "";
    mReportLimit = "";
    mEvenRow = true;
    mIsAlternatingRow = true;
    mIsFirstDataFullRow = false;

    mOnePixelTransparentImage =
            "<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; "
            "margin-right:0px; -qt-block-indent:0; text-indent:0px;'>"
            "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAY"
            "AAAAfFcSJAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRDb21tZW50AENyZWF0ZW"
            "Qgd2l0aCBHSU1QV4EOFwAAAA1JREFUCJljYGBgYAAAAAUAAYehTtQAAAAASUVORK5CYII=' "
            "height=\"1\" width=\"1\"/></p>";
}

/**
 * Destructor
 */
RB_SimpleReportWidget::~RB_SimpleReportWidget() {
    // nothing
}

/**
 * Initialize and create trial balance
 */
void RB_SimpleReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    RB_String html = "<p style=\"font-size:11pt;\">";
    html += tr("Select limits above and click refresh button ...");
    html += "</p>";

    //'Times New Roman', Times, serif
    mTeReport->setStyleSheet("font-family:'Times New Roman'; font-size:8pt;");
    mTeReport->setHtml(html);
}

/**
 * File Save, can be called by closeWidget().
 * Does nothing, returns false, to be overriden
 */
bool RB_SimpleReportWidget::fileSave(bool /*withSelect*/) {
    // nothing
    return false;
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool RB_SimpleReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}
/**
 * Slot File export PDF.
 * @param printer
 */
void RB_SimpleReportWidget::filePdf(QPrinter* printer) {
    mTeReport->slotUpdateCursorPosition();
    mTeReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void RB_SimpleReportWidget::filePrint(QPrinter* printer) {
    mTeReport->slotUpdateCursorPosition();
    mTeReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void RB_SimpleReportWidget::filePrintPreview(QPrinter* printer) {
    mTeReport->slotUpdateCursorPosition();
    mTeReport->print(printer);
}

/**
 * Edit Cut.
 */
void RB_SimpleReportWidget::editCut() {
    mTeReport->cut();
}

/**
 * Edit Copy.
 */
void RB_SimpleReportWidget::editCopy() {
    mTeReport->copy();
}

/**
 * Edit Paste.
 */
void RB_SimpleReportWidget::editPaste() {
    mTeReport->paste();
}

/**
 * Edit Undo.
 */
void RB_SimpleReportWidget::editUndo() {
    mTeReport->undo();
}

/**
 * Edit Redo.
 */
void RB_SimpleReportWidget::editRedo() {
    mTeReport->redo();
}

/**
 * Select All.
 */
void RB_SimpleReportWidget::editSelectAll() {
    mTeReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void RB_SimpleReportWidget::editInsertImage(const QString& fn) {
    mTeReport->insertImage(fn);
}

/**
 * Zoom out is done by increasing the font points by factor,
 * for large documents this can take some time
 * @param factor to zoom in not used
 */
void RB_SimpleReportWidget::editZoomIn(double /*factor*/) {
    mTeReport->fontZoomIn(1);
}

/**
 * Zoom out is done by decreasing the font points by factor
 * @param factor to zoom out not used
 */
void RB_SimpleReportWidget::editZoomOut(double /*factor*/) {
    mTeReport->fontZoomOut(1);
}

/**
 * Format Bold
 */
void RB_SimpleReportWidget::formatBold() {
    mTeReport->slotTextBold();
}

/**
 * Format Italic
 */
void RB_SimpleReportWidget::formatItalic() {
    mTeReport->slotTextItalic();
}

/**
 * Format Underline
 */
void RB_SimpleReportWidget::formatUnderline() {
    mTeReport->slotTextUnderline();
}

/**
 * Format Strikethrough
 */
void RB_SimpleReportWidget::formatStrikethrough() {
    mTeReport->slotTextStrikeOut();
}

/**
 * Format Align Left
 */
void RB_SimpleReportWidget::formatAlignLeft() {
    mTeReport->slotTextAlign(RB_TextEdit::AlignLeft);
}

/**
 * Format Align Center
 */
void RB_SimpleReportWidget::formatAlignCenter() {
    mTeReport->slotTextAlign(RB_TextEdit::AlignCenter);
}

/**
 * Format Align Right
 */
void RB_SimpleReportWidget::formatAlignRight() {
    mTeReport->slotTextAlign(RB_TextEdit::AlignRight);
}

/**
 * Format Align Justify
 */
void RB_SimpleReportWidget::formatAlignJustify() {
    mTeReport->slotTextAlign(RB_TextEdit::AlignJustify);
}

/**
 * Format Increase Indent
 */
void RB_SimpleReportWidget::formatIncreaseIndent() {
    mTeReport->slotTextIndent(1);
}

/**
 * Format Decrease Indent
 */
void RB_SimpleReportWidget::formatDecreaseIndent() {
    mTeReport->slotTextIndent(-1);
}

/**
 * Format numbered list
 */
void RB_SimpleReportWidget::formatNumberedList() {
    mTeReport->slotTextStyle(RB_TextEdit::ListDecimal);
}

/**
 * Format bulleted list
 */
void RB_SimpleReportWidget::formatBulletedList() {
    mTeReport->slotTextStyle(RB_TextEdit::ListDisc);
}

/**
 * Format font family
 */
void RB_SimpleReportWidget::formatFontName() {
    mTeReport->slotTextFamily();
}

/**
 * Format font size
 */
void RB_SimpleReportWidget::formatFontSize() {
    mTeReport->slotTextSize();
}

/**
 * Format text color
 */
void RB_SimpleReportWidget::formatTextColor() {
    mTeReport->slotTextColor();
}

/**
 * Format background color
 */
void RB_SimpleReportWidget::formatBackgroundColor() {
    mTeReport->slotBackgroundColor();
}

/**
 * Remove formatting and style
 */
void RB_SimpleReportWidget::formatRemove() {
//    webView->triggerPageAction(QWebPage::RemoveFormat);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool RB_SimpleReportWidget::saveFile(const RB_String &fn) {
    mTeReport->slotUpdateCursorPosition();
    QFile file(fn + ".html");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
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

    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}



/**
 * Create report based on object list
 * @param html report HTML
 * @param list container with list of data objects
 */
void RB_SimpleReportWidget::createReport(RB_String& html,
                                    RB_ObjectContainer* list) {
    bool isFirst = true;
    RB_ObjectIterator* iter = list->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (isFirst) {
            setTableHeader(html, obj); // sets report header
            isFirst = false;
        }
        setDataRow(html, obj);
    }

    delete iter;

    setDataFooter(html);
    setTableFooter(html);
    setReportFooter(html);
}

/**
 * Create report based on database query
 * @param html report HTML
 * @param list container with list of data objects
 */
void RB_SimpleReportWidget::createReport(RB_String& html, QSqlQuery& query) {

    bool isFirst = true;

    while (query.next()) {
        QSqlRecord rec = query.record();
        if (isFirst) {
            setTableHeader(html, rec); // sets report header
            isFirst = false;
        }
        setDataRow(html, rec);
    }

    setDataFooter(html);
    setTableFooter(html);
    setReportFooter(html);
}

/**
 * Set the report header text. Always call at the start of
 * the report creation. The report header is a separate table.
 * Will be printed only on the first page of the report.
 * @param html HTML to be set
 */
void RB_SimpleReportWidget::setReportHeader(RB_String& html) {
    QDateTime dt = QDateTime::currentDateTime();
    html += "<table cellpadding=0 cellspacing=1 width='100%' "
            "style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
            "<thead><tr><td align=\"center\" style=\"font-size:11pt;\" "
            "colspan=\"" + RB_String::number(mReportColCount) + "\"><b>"
            + mReportTitle + " - " + mReportSubTitle + "</b></td></tr>"
            + "<tr><td align=\"center\" colspan=\"" + RB_String::number(mReportColCount) + "\">"
            + mReportLimit + "<br />"
            + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
            "<tr><td style='background-color:" + mTeReport->getTextColor().name()
            + ";' colspan=\""
            + RB_String::number(mReportColCount) + "\">"
            + mOnePixelTransparentImage + "</td></tr>";
}

/**
 * Set the table header text. This will contain the column
 * headings and will be printed at the top of each page.
 * @param html HTML to be set
 * @param obj data object
 */
void RB_SimpleReportWidget::setTableHeader(RB_String& html, RB_ObjectBase* obj) {
    setColumnProperties(obj->memberCount() - (int)RB2::HIDDENCOLUMNS);
    setReportHeader(html);

    mColRunner = 0;
    mRowRunner = 0;

    html += "<tr>";

    for (int col = 0; col < mMemCount; ++col) {
        setColumnName(html, obj->getMember((int)RB2::HIDDENCOLUMNS + col)->getName(),
                      col);
    }

    html += "</tr>"
            "<tr><td colspan=\"" + RB_String::number(mReportColCount)
            + "\" style='background-color:" + mTeReport->getTextColor().name() + ";'>"
            + mOnePixelTransparentImage + "</td></tr></thead><tbody>";
}

/**
 * Set the table header text. This will contain the column
 * headings and will be printed at the top of each page.
 * @param html HTML to be set
 * @param rec database query record
 */
void RB_SimpleReportWidget::setTableHeader(RB_String& html, const QSqlRecord& rec) {
    setColumnProperties(rec.count());
    setReportHeader(html);

    mColRunner = 0;
    mRowRunner = 0;

    html += "<tr>";

    for (int col = 0; col < mMemCount; ++col) {
        setColumnName(html, rec.fieldName(col), col);
    }

    html += "</tr>"
            "<tr><td colspan=\"" + RB_String::number(mReportColCount)
            + "\" style='background-color:" + mTeReport->getTextColor().name() + ";'>"
            + mOnePixelTransparentImage + "</td></tr></thead><tbody>";
}

/**
 * Set the data header text. This is used to  give a separate
 * title to the series of data rows that will follow. For example
 * in a tree data model the path of the tree will be displayed.
 * Does nothing.
 * @param html HTML to be set
 */
void RB_SimpleReportWidget::setDataHeader(RB_String& /* html */) {
    // nothing
}

/**
 * Set the actual data.
 * @param html HTML to be set
 * @param obj data object
 */
void RB_SimpleReportWidget::setDataRow(RB_String& html, RB_ObjectBase* obj) {
    mColRunner = 0;
    mRowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        setColumnData(html, obj->getValue((int)RB2::HIDDENCOLUMNS + col).toString(),
                      col);
    }

    html += "</tr>";
    mEvenRow = !mEvenRow;
}

/**
 * Set the actual data.
 * @param html HTML to be set
 * @param rec database query record
 */
void RB_SimpleReportWidget::setDataRow(QString &html, const QSqlRecord &rec) {
    mColRunner = 0;
    mRowRunner = 0;

    if (!mIsAlternatingRow || !mEvenRow) {
        html += "<tr>";
    } else {
        html += "<tr bgcolor=\"" + mTeReport->getAlternateBaseColor().name() + "\">";
    }

    for (int col = 0; col < mMemCount; ++col) {
        setColumnData(html, rec.value(col).toString(), col);
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
void RB_SimpleReportWidget::setDataFooter(RB_String& /* html */) {
    // nothing
}

/**
 * Set the table footer with closing tags of table.
 * Could include for examplethe overall data result.
 * This result can be the total sum of data values in
 * the report.
 * @param html HTML to be set
 */
void RB_SimpleReportWidget::setTableFooter(RB_String& html) {
    html += "</tbody></table>";
}

/**
 * Set the report footer with for example with the statement
 * "--- END OF REPORT ---".
 * @param html HTML to be set
 */
void RB_SimpleReportWidget::setReportFooter(RB_String& html) {
    html += "<p style=\"font-size:8pt;font-family:'Times New Roman',"
            " Times, serif;\" align=\"center\">--- END OF REPORT ---</p>";
}

/**
 * Set the table column properties based on the user settings
 * and the number of fields in the data set. In case the
 * user has requested less than 3 columns the column width(s)
 * are overriden by default values
 * @param dataFieldCount number of fields (columns) in the data set
 */
void RB_SimpleReportWidget::setColumnProperties(int dataFieldCount) {
    mReportColCount = mColumnWidthVector.size(); //  > 0 ? mColumnWidthVector.size() : 1;
    mMemCount = dataFieldCount; // rec.count() or obj->memberCount() - (int)RB2::HIDDENCOLUMNS

    if (mReportColCount > 1) {
        mNoDataRow = (int)((mMemCount - 1) / (mReportColCount - 1));

        if ((mMemCount - 1) % (mReportColCount - 1)) {
            ++mNoDataRow; // there is a rest
        }

        if (mIsFirstDataFullRow) {
            ++mNoDataRow; // there is a separate first row
        }
    } else {
        mNoDataRow = 1;
        mIsFirstDataFullRow = false;
        clearColumnWidths();
        int width = 100 / mMemCount;

        for (int i = 0; i < mMemCount; ++i) {
            setColumnWidth(width);
        }
    }
}

/**
 * Set the table column name
 * @param html html to be set
 * @param fieldName field name to be set as column header
 * @param mColRunner column counter/runner
 * @param mRowRunner row counter/runner
 */
void RB_SimpleReportWidget::setColumnName(RB_String& html,
                                          const RB_String& fieldName,
                                          int col) {
    if (mIsFirstDataFullRow && col == 0) {
        html += "<th align=\"left\" colspan=\"" + RB_String::number(mReportColCount)
                + "\">" + fieldName;
        mColRunner = mReportColCount - 1;
    } else {
        if (mRowRunner == 0 && !mIsFirstDataFullRow) {
            html += "<th width=\""
                    + RB_String::number(mColumnWidthVector.at(mColRunner)) + "%\"><b>"
                    + fieldName + "</b>";
        } else if (mRowRunner == 1 && mIsFirstDataFullRow) {
            html += "<th align=\"center\" width=\""
                    + RB_String::number(mColumnWidthVector.at(mColRunner-1)) + "%\"><b>"
                    + fieldName + "</b>";
        } else {
            html += "<th align=\"center\"><b>"
                    + fieldName + "</b>";
        }
    }

    html += "</th>";

    // new row
    if (mColRunner + 1 == mReportColCount && col < mMemCount - 1) {
        ++mRowRunner;
        html += "</tr><tr>";

        // skip first cell in new row
        mColRunner = 1;
        if (mRowRunner == mNoDataRow - 1) {
            html += "<th>&nbsp;</th>";
        } else {
            html += "<th align=\"center\">&nbsp;</th>";
        }
    } else if (col == mMemCount - 1) {
        // fill out last cells
        for (int i = 0; i < mReportColCount - 1 - mColRunner; ++i) {
            html += "<th>&nbsp;</th>";
        }

    } else {
        ++mColRunner;
    }
}

/**
 * Set the table column data
 * @param html html to be set
 * @param colData column data
 * @param mColRunner column counter/runner
 * @param mRowRunner row counter/runner
 */
void RB_SimpleReportWidget::setColumnData(RB_String& html,
                                          const RB_String& colData,
                                          int col) {
    if (mIsFirstDataFullRow && col == 0) {
        html += "<td colspan=\"" + RB_String::number(mReportColCount)
                + "\">" + colData + "</td>";
        mColRunner = mReportColCount - 1;
    } else {
        html += "<td>" + colData + "</td>";
    }

    // new row
    if (mColRunner + 1 == mReportColCount && col < mMemCount - 1) {
        ++mRowRunner;
        if (!mIsAlternatingRow || !mEvenRow) {
            html += "</tr><tr>";
        } else {
            html += "</tr><tr bgcolor=\""
                    + mTeReport->getAlternateBaseColor().name() + "\">";
        }

        // skip first cell in new row
        mColRunner = 1;
        html += "<td>&nbsp;</td>";
    } else if (col == mMemCount - 1) {
        // fill out last cells
        for (int i = 0; i < mReportColCount - 1 - mColRunner; ++i) {
            html += "<td>&nbsp;</td>";
        }

    } else {
        ++mColRunner;
    }
}

