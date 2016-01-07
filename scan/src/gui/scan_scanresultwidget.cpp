/*****************************************************************
 * $Id: scan_scanresultwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: 2011-04-28 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_scanresultwidget.h"

#include <QStringListModel>
#include "db_actionfilesaveas.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"
#include "scan_dialogfactory.h"
#include "scan_ledelegate.h"
#include "scan_modelfactory.h"
#include "scan_objectfactory.h"
#include "scan_preparejrreport.h"
#include "scan_preparejrresult.h"
#include "thrd_chart.h" // test only

/**
 * Constructor
 */
SCAN_ScanResultWidget::SCAN_ScanResultWidget(QWidget *parent)
                        : RB_Widget(parent){

    setupUi(this);

    mScanModel = NULL;
    mQuestionModel = NULL;
    mAnswerModel = NULL;

    // Refer also RB_DataWidgetMapper
    mCbStyle = "QComboBox::drop-down {border-top-right-radius: 3px;"
               "border-bottom-right-radius: 3px;}";
}

/**
 * Destructor
 */
SCAN_ScanResultWidget::~SCAN_ScanResultWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mQuestionModel;
    delete mAnswerModel;
    delete mScanModel;
    RB_DEBUG->print("SCAN_ScanResultWidget::~SCAN_ScanResultWidget() OK");
}

/**
 * @returns name based on selected tab
 */
RB_String SCAN_ScanResultWidget::getName() const {
    if (1 == tabWidget->currentIndex()) {
        return "Score Result";
    }
    return "Scan Result";
}

/**
 * Initialize widget and models
 */
void SCAN_ScanResultWidget::init() {
    setWindowTitle(getName());

    // Set model and connect to table view
    mScanModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelScanResult);
    mScanModel->setRoot(SCAN_MODELFACTORY->getRootId());

    // Mappers
    mScanMapper = mScanModel->getMapper();
    mScanMapper->addMapping(leResultName, mScanModel->fieldIndex("title"));
    addComboBoxMapping(mScanModel,"scan_id", "SCAN_Scan", "id",
                       "title", cbScanType, mScanMapper);
    cbScanType->setStyleSheet(mCbStyle);
    cbScanType->setEnabled(false);
//    addComboBoxMapping(mScanModel,"contact_id", "ACC_Contact", "id",
//                       "lastname", cbPerson, mScanMapper);
    cbPerson->setStyleSheet(mCbStyle);
    cbPerson->setEnabled(false);

    // Set standard format and connection for model and view
    setFormatTableView(tvScan, mScanModel);

    // Hide columns
    for (int i = 0; i < mScanModel->columnCount(QModelIndex()); ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvScan->hideColumn(i);
        }
    }

    mQuestionModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelQuestionResult);
    mQuestionModel->setRoot(SCAN_MODELFACTORY->getRootId());

    // Set standard format and connection for model and view
    setFormatTableView(tvQuestion, mQuestionModel);
    tvQuestion->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Hide columns
    for (int i = 0; i < mQuestionModel->columnCount(); ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 2) {
            tvQuestion->hideColumn(i);
        }
    }

    mAnswerModel = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelAnswerResult);
    mAnswerModel->setRoot(SCAN_MODELFACTORY->getRootId());

    // Mapper for line edits etc.
    mAnswerMapper = mAnswerModel->getMapper();
    mAnswerMapper->addMapping(leAnswerNumber, mAnswerModel->fieldIndex("number"));
    leAnswerNumber->setEnabled(false);
    mAnswerMapper->addMapping(leValue, mAnswerModel->fieldIndex("mvalue"));
    leValue->setValidator(new QIntValidator(0, 999, leValue));

    // Set standard format and connection for model and view
    setFormatTableView(tvAnswer, mAnswerModel);
    tvAnswer->setItemDelegate(new SCAN_LeDelegate(tvAnswer));

    // Hide columns
    for (int i = 0; i < mAnswerModel->columnCount(); ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvAnswer->hideColumn(i);
        }
    }

    mScanModel->select();
    teScoreResult->setHtml(tr("Click refresh button ..."));
    teReport->setHtml(tr("Click refresh button ..."));
    QList<int> intList;
    intList << 200 << 400;
    splHorizontal->setSizes(intList);
    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool SCAN_ScanResultWidget::fileSave(bool withSelect) {
    if (!mScanModel || !mQuestionModel || !mAnswerModel) return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (withSelect) {
        mScanModel->submitAllAndSelect();
        mQuestionModel->submitAllAndSelect();
        mAnswerModel->submitAllAndSelect();

    } else {
        mScanModel->submitAll();
        mQuestionModel->submitAll();
        mAnswerModel->submitAll();
    }
    setWindowModified(false);

    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool SCAN_ScanResultWidget::fileSaveAs() {
    if (!mScanModel || !mQuestionModel || !mAnswerModel) return false;
    if (tabWidget->currentIndex() != 2) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("File Save As only available\n"
                                                    "when tab Report is selected"));
        return false;
    }

    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Reverse all changes
 */
void SCAN_ScanResultWidget::fileRevert() {
    if (!mScanModel || !mQuestionModel || !mAnswerModel) return;

    mScanModel->revert();
    mQuestionModel->revert();
    mAnswerModel->revert();
    setWindowModified(false);
}

/**
 * File export PDF.
 * @param pr printer
 */
void SCAN_ScanResultWidget::filePdf(QPrinter* pr) {
    printWidget(pr);
}

/**
 * File Print.
 * @param pr printer
 */
void SCAN_ScanResultWidget::filePrint(QPrinter* pr) {
    printWidget(pr);
}

/**
 * File Print Preview.
 * @param pr printer
 */
void SCAN_ScanResultWidget::filePrintPreview(QPrinter* pr) {
    printWidget(pr);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool SCAN_ScanResultWidget::saveFile(const RB_String &fn) {
    QTextEdit* te = NULL;
    int tabNo = tabWidget->currentIndex();

    switch (tabNo) {
    case 0 :
    {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No save action available \n"
                                                    "for selected tab"));
        return false;
        break;
    }
    case 1 :
        te = teScoreResult;
        break;
    case 2 :
        te = teReport;
        break;
    default :
        return false;
        break;
    }

    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        SCAN_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        SCAN_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (fn.endsWith(".htm", Qt::CaseInsensitive)
        || fn.endsWith(".html", Qt::CaseInsensitive)
        || fn.endsWith(".xml", Qt::CaseInsensitive)) {
        out << te->toHtml();
    } else {
        out << te->toPlainText();
    }

    QApplication::restoreOverrideCursor();

//    setCurrentFileName(fn);
    SCAN_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Print widget of selected tab
 * @param pr printer
 */
void SCAN_ScanResultWidget::printWidget(QPrinter* pr) {
    int tabNo = tabWidget->currentIndex();

    switch (tabNo) {
    case 0 :
    {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No print available \n"
                                                    "for selected tab"));
        break;
    }
    case 1 :
        pr->setPageMargins(25,15,15,15,QPrinter::Millimeter);
        teScoreResult->print(pr);
        break;
    case 2 :
        pr->setPageMargins(25,15,15,15,QPrinter::Millimeter);
        teReport->print(pr);
        break;
    default :
        break;
    }
}
/**
 * Button add (scan) clicked
 */
void SCAN_ScanResultWidget::on_pbAddScan_clicked() {
    if (!mScanModel) return;

    if (isWindowModified()) {
        fileSave(true);
    }

    // Clear sorting and filtering of proxyModel
    mScanModel->undoFilterSort();
    int row = 0; // mScanModel->rowCount(); // 0;
    mScanModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mScanModel->index(row, mScanModel->fieldIndex("title"));
    mScanModel->setData(index, "New", Qt::EditRole);

    tvScan->setCurrentIndex(mScanModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvScan->scrollTo(mScanModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
}

/**
 * Button delete (scan) clicked
 */
void SCAN_ScanResultWidget::on_pbDeleteScan_clicked() {
    if (!mScanModel) return;

    int res = SCAN_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvScan->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mScanModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);

        if (row > 0) {
            tvScan->setCurrentIndex(mScanModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvScan->scrollTo(mScanModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

/**
 * Button save (scan) clicked
 */
void SCAN_ScanResultWidget::on_pbSaveScan_clicked() {
    fileSave(true);
}

/**
 * Button revert (scan) clicked
 */
void SCAN_ScanResultWidget::on_pbRevertScan_clicked() {
    fileRevert();
}

/**
 * Button select scan type clicked
 */
void SCAN_ScanResultWidget::on_pbSelectScanType_clicked() {
    if (!mScanModel->getCurrentIndex().isValid()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    // HACK: Refresh relation model before selecting, if required
    // TODO: use signal slot between models to notify changes
    //    QSqlTableModel* relModel = mModel->relationModel(mModel->fieldIndex("type_id"));
    //    relModel->select();

    RB_Dialog* dlg = SCAN_DIALOGFACTORY->getDialog(SCAN_DialogFactory::DialogScanSelect);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setRelationalModelData(mScanModel, "scan_id", obj);
        } else {
            SCAN_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                        "Data unchanged."));
        }
    }

    dlg->deleteLater();

}

/**
 * Button select (contact) person clicked
 */
void SCAN_ScanResultWidget::on_pbSelectPerson_clicked() {
    SCAN_DIALOGFACTORY->requestWarningDialog(tr("Not yet implemented."));
}

/**
 * Button add (questions from scan) clicked
 */
void SCAN_ScanResultWidget::on_pbAddQuestion_clicked() {
    QModelIndex index = tvScan->currentIndex();
    if (!index.isValid()) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No scan selected."));
        return;
    }

    if (tvQuestion->model()->rowCount() > 0) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("Scan already loaded."));
        return;
    }

    // Get scan type ID
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    RB_String id = m->hiddenData(m->index(index.row(), m->fieldIndex("scan_id"), index.parent()),
                                 RB2::RoleOrigData).toString();

    // Load scan content from database
    RB_ObjectBase* scan = SCAN_OBJECTFACTORY->newSingleObject("SCAN_Scan");
    scan->setId(id);
    scan->dbRead(SCAN_MODELFACTORY->getDatabase(), RB2::ResolveAll);

    // Upload scan to result tables
    if (!uploadScan(scan)) {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("Could not upload scan data,\n"
                                                    "data already available,\n"
                                                    "anwsers not complete,\n"
                                                    "or database ERROR"));
    }

    delete scan;
}

/**
 * Button delete (all questions) clicked
 */
void SCAN_ScanResultWidget::on_pbDeleteQuestion_clicked() {
    if (!mQuestionModel) return;

    int res = SCAN_DIALOGFACTORY->requestYesNoDialog(tr("Delete items"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the items?"));
    if (res != QMessageBox::Yes) return;

    int rowCount = mQuestionModel->rowCount();
    mQuestionModel->removeRows(0, rowCount, QModelIndex());
    mQuestionModel->submitAllAndSelect();
    // setWindowModified(false); parent could have changed
}

/**
 * Button delete (all questions) clicked
 */
void SCAN_ScanResultWidget::on_pbRefreshScore_clicked() {
    teScoreResult->setHtml(tr("Click refresh button to generate result ..."));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QModelIndex index = tvScan->currentIndex();
    if (!index.isValid()) {
        QApplication::restoreOverrideCursor();
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No scan selected."));
        return;
    }

    if (tvQuestion->model()->rowCount() < 1) {
        QApplication::restoreOverrideCursor();
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("Scan result not completed."));
        return;
    }

    // Get scan result ID
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    RB_String id = m->hiddenData(m->index(index.row(), 0, index.parent()),
                                 RB2::RoleOrigData).toString();

    // Load scan user result from database
    RB_ObjectBase* scanRes = SCAN_OBJECTFACTORY->newSingleObject("SCAN_ScanResult");
    scanRes->setId(id);
    scanRes->dbRead(SCAN_MODELFACTORY->getDatabase(), RB2::ResolveAll);

    // Prepare HTML and set text in textedit
    SCAN_PrepareJrResult* oper = new SCAN_PrepareJrResult();
    oper->execute(scanRes);
    teScoreResult->setHtml(oper->getResultHtml());

    delete oper;
    delete scanRes;

    QApplication::restoreOverrideCursor();
}

/**
 * Button delete (all questions) clicked
 */
void SCAN_ScanResultWidget::on_pbRefreshReport_clicked() {
    teReport->setHtml(tr("Click refresh button to generate report ..."));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QModelIndex index = tvScan->currentIndex();
    if (!index.isValid()) {
        QApplication::restoreOverrideCursor();
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("No scan selected."));
        return;
    }

    if (tvQuestion->model()->rowCount() < 1) {
        QApplication::restoreOverrideCursor();
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("Scan result not completed."));
        return;
    }

    // Get scan result ID
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    RB_String id = m->hiddenData(m->index(index.row(), 0, index.parent()),
                                 RB2::RoleOrigData).toString();

    // Load scan user result from database
    RB_ObjectBase* scanRes = SCAN_OBJECTFACTORY->newSingleObject("SCAN_ScanResult");
    scanRes->setId(id);
    scanRes->dbRead(SCAN_MODELFACTORY->getDatabase(), RB2::ResolveAll);

    // Prepare result
    SCAN_PrepareJrResult* operResult = new SCAN_PrepareJrResult();
    operResult->execute(scanRes);

    // Get originating scan ID
    id = m->hiddenData(m->index(index.row(), m->fieldIndex("scan_id"), index.parent()),
                       RB2::RoleOrigData).toString();

    // Load scan content (text blocks) from database
    RB_ObjectBase* scanReport = SCAN_OBJECTFACTORY->newSingleObject("SCAN_Scan");
    scanReport->setId(id);
    scanReport->dbRead(SCAN_MODELFACTORY->getDatabase(), RB2::ResolveAll);

    // Generate report
    SCAN_PrepareJrReport* operReport = new SCAN_PrepareJrReport();
    teReport->setHtml(operReport->getReport(scanReport, operResult));


    // test
    // Create image
/*    int imgHeight = 600;
    int imgWidth = 600;
    int margin = 50;
    QImage img(QSize(imgWidth,imgHeight), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    THRD_Chart chart;
    //{Bar,Line,Pie2D,Pie3D,StackedBar}
    chart.setChartType(THRD_Chart::Pie3D);
    //{AxesAngled,AxesHoriz,Horizontal,Round,Vertical}
    chart.setLegendType(THRD_Chart::Vertical);

    chart.setChartCoords(margin, margin, img.width()/1.5, img.height()/3.0);
    chart.setLegendCoords(400, 100);
    chart.addChartItem("Item1",QColor(200,10,50),34);
    chart.addChartItem("Item2",Qt::green,27);
    chart.addChartItem("Item3",Qt::cyan,14);
    chart.addChartItem("Item4",Qt::yellow,7);
    chart.addChartItem("Item5",Qt::blue,4);
    chart.draw(&painter);
    chart.drawLegend(&painter);

    // Insert image
    QTextCursor cursor = teReport->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertImage(img);
 */
    // end test


    delete operResult;
    delete scanReport;
    delete operReport;
    delete scanRes;

    QApplication::restoreOverrideCursor();
}

/**
 * Change event, for example translation
 */
void SCAN_ScanResultWidget::changeEvent(QEvent *e) {
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
 * Upload data (questions and answers) from selected scan type
 * @param scan downloaded data from scan type
 * @return true on success
 */
bool SCAN_ScanResultWidget::uploadScan(RB_ObjectBase* scan) {
    // Clear sorting and filtering of proxyModel
    mQuestionModel->undoFilterSort();

    RB_ObjectContainer* qList = scan->getContainer("SCAN_QuestionList");
    if (qList->objectCount() < 1) return false;

    RB_ObjectIterator* qIter = qList->createIterator();
    QModelIndex index;

    for (qIter->first(); !qIter->isDone(); qIter->next()) {
        RB_ObjectBase* question = qIter->currentObject();

        int row = mQuestionModel->rowCount(); // 0;
        mQuestionModel->insertRows(row, 1, QModelIndex());

        index = mQuestionModel->index(row, mQuestionModel->fieldIndex("number"));
        mQuestionModel->setData(index, question->getValue("number").toInt(), Qt::EditRole);
        index = mQuestionModel->index(row, mQuestionModel->fieldIndex("type"));
        mQuestionModel->setData(index, question->getValue("type").toInt(), Qt::EditRole);
        index = mQuestionModel->index(row, mQuestionModel->fieldIndex("question"));
        mQuestionModel->setData(index, question->getValue("question").toString(), Qt::EditRole);

        tvQuestion->setCurrentIndex(mQuestionModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
        tvQuestion->scrollTo(mQuestionModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));

        RB_ObjectContainer* aList = question->getContainer("SCAN_AnswerList");
        if (aList->objectCount() < 1) return false;

        RB_ObjectIterator* aIter = aList->createIterator();

        for (aIter->first(); !aIter->isDone(); aIter->next()) {
            RB_ObjectBase* answer = aIter->currentObject();

            int row = mAnswerModel->rowCount(); // 0;
            mAnswerModel->insertRows(row, 1, QModelIndex());

            index = mAnswerModel->index(row, mAnswerModel->fieldIndex("number"));
            mAnswerModel->setData(index, answer->getValue("number").toInt(), Qt::EditRole);
            index = mAnswerModel->index(row, mAnswerModel->fieldIndex("answer"));
            mAnswerModel->setData(index, answer->getValue("answer").toString(), Qt::EditRole);

            tvAnswer->setCurrentIndex(mAnswerModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvAnswer->scrollTo(mAnswerModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
        }

        delete aIter;
        mAnswerModel->submitAll();
    }

    delete qIter;
    mQuestionModel->submitAll();

    return true;
}
