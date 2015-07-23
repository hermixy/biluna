/*****************************************************************
 * $Id: db_simplereportwidget.cpp 1886 2013-01-24 17:49:24Z rutger $
 * Created: Jun 4, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_simplereportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_sqlhighlighter.h"


/**
 * Constructor
 */
DB_SimpleReportWidget::DB_SimpleReportWidget(QWidget *parent)
                        : RB_SimpleReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
DB_SimpleReportWidget::~DB_SimpleReportWidget() {
    // nothing
    delete mModel;
}

/**
 * Initialize widget
 */
void DB_SimpleReportWidget::init() {
    RB_SimpleReportWidget::init();

    //
    //  1. Set model, ID by parent model
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelSimpleReport);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leTitle, mModel->fieldIndex("title"));
    mMapper->addMapping(leSubtitle, mModel->fieldIndex("subtitle"));
    mMapper->addMapping(leColumnWidths, mModel->fieldIndex("columnwidths"));
    mMapper->addMapping(chbAlternatingRow, mModel->fieldIndex("alternatingrow"));
    mMapper->addMapping(chbFirstDataFull, mModel->fieldIndex("firstdatafull"));
    mMapper->addMapping(pteSql, mModel->fieldIndex("sqlstring"));

    //
    // 3. Select after relations have been set, initiated by parent model
    //
    mModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tv, mModel);
    readSettings();

    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tv->hideColumn(i);
        } else {
            tv->showColumn(i);

            if (tv->columnWidth(i) < 5) {
                tv->setColumnWidth(i, 100);
            }
        }
    }

    mCodeHighlighter = new DB_SqlHighlighter(pteSql->document());
    pteSql->setTabStopWidth(20); // in pixels, default is 80

    setComboBoxes();
    cbFromValue->setCurrentIndex(0);
    count = cbToValue->count();
    cbToValue->setCurrentIndex(count - 1);
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool DB_SimpleReportWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {

            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void DB_SimpleReportWidget::fileRevert() {
    mModel->revertAll();
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void DB_SimpleReportWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mModel->index(row, mModel->fieldIndex("title"));
    mModel->setData(idx, tr("<New>"), Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("alternatingrow"));
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("firstdatafull"));
    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tv->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tv->scrollTo(tv->currentIndex());
    leTitle->setFocus();
}

/**
 * Button delete clicked
 */
void DB_SimpleReportWidget::on_pbDelete_clicked() {
    QModelIndex index = tv->currentIndex();
    if (!index.isValid()) return;

    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete report ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this report?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    mModel->removeRows(index.row(), 1, QModelIndex());
    mModel->submitAllAndSelect();
    setWindowModified(false);
}

/**
 * Button save clicked
 */
void DB_SimpleReportWidget::on_pbSave_clicked() {
    fileSave(true);
}

void DB_SimpleReportWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`db_simplereport`.`title` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter, true); // do select
}

void DB_SimpleReportWidget::on_pbFilterOff_clicked() {
    fileSave(false);
    mModel->setWhere("", true); // do select
    leFilter->setText("");
}

/**
 * Initialize and create trial balance
 */
void DB_SimpleReportWidget::on_pbRefresh_clicked() {
    try {
        if (!tv || !tv->currentIndex().isValid()) {
            RB_String html = "<p style=\"font-size:11pt;\">";
            html += tr("No report selected.");
            html += "</p>";
            teReport->setHtml(html);
            return;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Test
        // throw("Error");

        // Set report data
        clearColumnWidths();
        setColumnWidthsAndTitle();

        // Prepare SQL statement
        RB_String fromValue = cbFromValue->currentText();
        RB_String toValue = cbToValue->currentText();
        RB_String sqlStr = mModel->getCurrentValue("sqlstring").toString();
        sqlStr = sqlStr.replace("{$valuefrom}", fromValue, Qt::CaseInsensitive);
        sqlStr = sqlStr.replace("{$valueto}", toValue, Qt::CaseInsensitive);

        // Execute query
        QSqlQuery q(DB_MODELFACTORY->getDatabase());

        if (!q.exec(sqlStr)) {
            // error
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "DB_SimpleReportWidget::on_pbRefresh_clicked() %s ERROR",
                            q.lastError().text().toStdString().c_str());
            teReport->setText(q.lastError().text());
            QApplication::restoreOverrideCursor();
            return;
        }

        // Create report
        RB_String html;
        createReport(html, q);
        mTeReport->setHtml(html);

        QApplication::restoreOverrideCursor();
    } catch(std::exception& e) {
        QApplication::restoreOverrideCursor();
        DB_DIALOGFACTORY->requestWarningDialog(e.what());
    } catch(...) {
        QApplication::restoreOverrideCursor();
        DB_DIALOGFACTORY->requestWarningDialog(
                    "Error <unkown> in DB_SimpleReportWidget::on_pbRefresh_clicked()");
    }
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_SimpleReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void DB_SimpleReportWidget::setComboBoxes() {
    RB_StringList strL;
    strL << "0" << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I"
            << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R"
            << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Zzzz";
    cbFromValue->addItems(strL);
    cbToValue->addItems(strL);
}

/**
 * Set report column widths and title
 */
void DB_SimpleReportWidget::setColumnWidthsAndTitle() {
    QModelIndex idx = mModel->getProxyIndex();

    if (!idx.isValid()) {
        return;
    }

    // Column widths
    RB_String str = mModel->getCurrentValue("columnwidths").toString();
    RB_StringList strL = str.split(";");

    for (int i = 0; i < strL.count(); ++i) {
        bool ok = false;
        int widthPercentage = strL.at(i).trimmed().toInt(&ok);

        if (ok) {
            setColumnWidth(widthPercentage);
        }
    }

    // Title
    setAlternatingRow(mModel->getCurrentValue("alternatingrow").toBool());
    setFirstDataFullRow(mModel->getCurrentValue("firstdatafull").toBool());
    setReportTitle(mModel->getCurrentValue("title").toString());
    setReportSubTitle(mModel->getCurrentValue("subtitle").toString());
    RB_String limit = tr("From: ") + cbFromValue->currentText();
    limit += tr(" To: ") + cbToValue->currentText();
    setReportLimit(limit);
}

/**
 * Change event such as language change
 */
void DB_SimpleReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



