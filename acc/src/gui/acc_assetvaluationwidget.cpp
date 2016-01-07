/*****************************************************************
 * $Id: acc_assetvaluationwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Nov 7, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetvaluationwidget.h"

#include <cmath>
#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_valueasset.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_sqlrelationaldelegate.h"
#include "thrd_chart.h"

/**
 * Constructor
 */
ACC_AssetValuationWidget::ACC_AssetValuationWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mAttrCustModel = NULL;
    mAttrCustMapper = NULL;
    mAttrSupplModel = NULL;
    mAttrSupplMapper = NULL;
    mAssetModel = NULL;
}

/**
 * Destructor
 */
ACC_AssetValuationWidget::~ACC_AssetValuationWidget() {
    delete mAttrCustModel;
    delete mAttrSupplModel;
    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue("discountrate", sbDiscountRate->value());
    RB_SETTINGS->endGroup();

    RB_DEBUG->print("ACC_AssetValuationWidget::~ACC_AssetValuationWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_AssetValuationWidget::init() {
    mAssetModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAsset);
    connect(mAssetModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotAssetRowChanged(QModelIndex,QModelIndex)));

    QStringList items;
    items << tr("Once") << tr("Month") << tr("Year");
    cbIntervalCust->addItems(items);
    cbIntervalSuppl->addItems(items);

    //
    // 1. Set model customer
    //
    mAttrCustModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAssetCustAttr);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mAttrCustMapper = mAttrCustModel->getMapper();
    mAttrCustMapper->addMapping(leSeqNoCust, mAttrCustModel->fieldIndex("seqno"));
    mAttrCustMapper->addMapping(leShortNameCust, mAttrCustModel->fieldIndex("shortname"));
    mAttrCustMapper->addMapping(leDescriptionCust, mAttrCustModel->fieldIndex("description"));
    mAttrCustMapper->addMapping(leUnitCust, mAttrCustModel->fieldIndex("unit"));
    mAttrCustMapper->addMapping(sbWeightCust, mAttrCustModel->fieldIndex("weight"));
    mAttrCustMapper->addMapping(leValueCust, mAttrCustModel->fieldIndex("value"));
    mAttrCustMapper->addMapping(leMultiplyWithCust, mAttrCustModel->fieldIndex("multiplywith"));
    mAttrCustMapper->addMapping(deStartDateCust, mAttrCustModel->fieldIndex("startdate"));
    mAttrCustMapper->addMapping(deEndDateCust, mAttrCustModel->fieldIndex("enddate"));
    mAttrCustMapper->addMapping(cbIntervalCust, mAttrCustModel->fieldIndex("interval"), "currentIndex");
    mAttrCustMapper->addMapping(ileCustName, mAttrCustModel->fieldIndex("customer_idx"));

    deStartDateCust->setDisplayFormat("yyyy-MM-dd");
    deEndDateCust->setDisplayFormat("yyyy-MM-dd");
    leValueCust->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
    leMultiplyWithCust->setValidator(new QIntValidator(0, 999999, this));

    //
    // 3. Select after relations have been set, only for database models
    //
    // mModel->select(); not here done by parent model
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvAttrCust, mAttrCustModel);
    RB_SETTINGS->beginGroup(objectName());
    sbDiscountRate->setValue(RB_SETTINGS->value("discountrate", 4.0).toDouble());
    RB_SETTINGS->endGroup();

    //
    // 1. Set model supplier
    //
    mAttrSupplModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAssetSupplAttr);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mAttrSupplMapper = mAttrSupplModel->getMapper();
    mAttrSupplMapper->addMapping(leSeqNoSuppl, mAttrSupplModel->fieldIndex("seqno"));
    mAttrSupplMapper->addMapping(leShortNameSuppl, mAttrSupplModel->fieldIndex("shortname"));
    mAttrSupplMapper->addMapping(leDescriptionSuppl, mAttrSupplModel->fieldIndex("description"));
    mAttrSupplMapper->addMapping(leUnitSuppl, mAttrSupplModel->fieldIndex("unit"));
    mAttrSupplMapper->addMapping(sbWeightSuppl, mAttrSupplModel->fieldIndex("weight"));
    mAttrSupplMapper->addMapping(leValueSuppl, mAttrSupplModel->fieldIndex("value"));
    mAttrSupplMapper->addMapping(leMultiplyWithSuppl, mAttrSupplModel->fieldIndex("multiplywith"));
    mAttrSupplMapper->addMapping(deStartDateSuppl, mAttrSupplModel->fieldIndex("startdate"));
    mAttrSupplMapper->addMapping(deEndDateSuppl, mAttrSupplModel->fieldIndex("enddate"));
    mAttrSupplMapper->addMapping(cbIntervalSuppl, mAttrSupplModel->fieldIndex("interval"), "currentIndex");
    mAttrSupplMapper->addMapping(ileSupplName, mAttrSupplModel->fieldIndex("supplier_idx"));

    deStartDateSuppl->setDisplayFormat("yyyy-MM-dd");
    deEndDateSuppl->setDisplayFormat("yyyy-MM-dd");
    leValueSuppl->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
    leMultiplyWithSuppl->setValidator(new QIntValidator(0, 999999, this));

    //
    // 3. Select after relations have been set, only for database models
    //
    // mModel->select(); not here done by parent model
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvAttrSuppl, mAttrSupplModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mAttrCustModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvAttrCust->hideColumn(i);
        } else {
            tvAttrCust->showColumn(i);

            if (tvAttrCust->columnWidth(i) < 5) {
                tvAttrCust->setColumnWidth(i, 100);
            }
        }
    }

    // Hidden columns only for tableviews
    colCount = mAttrSupplModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvAttrSuppl->hideColumn(i);
        } else {
            tvAttrSuppl->showColumn(i);

            if (tvAttrSuppl->columnWidth(i) < 5) {
                tvAttrSuppl->setColumnWidth(i, 100);
            }
        }
    }

    // tvAttrSuppl->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelAsset);
    mAttrCustModel->slotParentCurrentRowChanged(idx, QModelIndex());
    mAttrSupplModel->slotParentCurrentRowChanged(idx, QModelIndex());
    slotAssetRowChanged(idx, QModelIndex());
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_AssetValuationWidget::filePdf(QPrinter* printer) {
    if (tabWidget->currentIndex() != 1) {
        QPainter p(printer);
        QPixmap pm = QPixmap::grabWidget(this);
        p.drawPixmap(0, 0, pm);
    } else {
        teValuation->print(printer);
    }
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_AssetValuationWidget::filePrint(QPrinter* printer) {
    if (tabWidget->currentIndex() != 1) {
        QPainter p(printer);
        QPixmap pm = QPixmap::grabWidget(this);
        p.drawPixmap(0, 0, pm);
    } else {
        teValuation->print(printer);
    }
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_AssetValuationWidget::filePrintPreview(QPrinter* printer) {
    if (tabWidget->currentIndex() != 1) {
        QPainter p(printer);
        QPixmap pm = QPixmap::grabWidget(this);
        p.drawPixmap(0, 0, pm);
    } else {
        teValuation->print(printer);
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave(). Always does a select() for now.
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_AssetValuationWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        mAttrCustModel->submitAllAndSelect();
        mAttrSupplModel->submitAllAndSelect();

    } else {
        mAttrCustModel->submitAll();
        mAttrSupplModel->submitAll();
    }

    return true;
}

/**
 * Reverse all changes
 */
void ACC_AssetValuationWidget::fileRevert() {
    mAttrCustModel->revertAll();
    mAttrSupplModel->revertAll();
}

/**
 * Button save clicked
 */
void ACC_AssetValuationWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button add clicked
 */
void ACC_AssetValuationWidget::on_pbAddAttrCust_clicked() {
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelAsset);
    if (!idx.isValid()) return;

    if (!mAttrCustModel) return;
    mAttrCustModel->undoFilterSort();

    // always insert at the end, can be done only for smaller models
    int row = mAttrCustModel->rowCount();
    mAttrCustModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QDateTime today = QDateTime::currentDateTime();
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("weight"));
    mAttrCustModel->setData(idx, 100, Qt::EditRole);
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("value"));
    mAttrCustModel->setData(idx, 0.00, Qt::EditRole);
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("multiplywith"));
    mAttrCustModel->setData(idx, 0, Qt::EditRole);
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("startdate"));
    mAttrCustModel->setData(idx, today, Qt::EditRole);
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("enddate"));
    mAttrCustModel->setData(idx, today, Qt::EditRole);
    idx = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("interval"));
    mAttrCustModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tvAttrCust->setCurrentIndex(mAttrCustModel->index(row, RB2::HIDDENCOLUMNS));
    tvAttrCust->scrollTo(tvAttrCust->currentIndex());
    leSeqNoCust->setFocus();
    leSeqNoCust->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_AssetValuationWidget::on_pbDeleteAttrCust_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete attribute ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this attribute?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    if (!mAttrCustModel) return;

    QModelIndex idx = tvAttrCust->currentIndex();
    if (idx.isValid()) {
        mAttrCustModel->removeRows(idx.row(), 1, QModelIndex());
        mAttrCustModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button select customer clicked
 */
void ACC_AssetValuationWidget::on_ileCustName_clicked() {
    if (!tvAttrCust->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No attribute selected.\n"
                                                   "Please select an attribute first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCustBranch);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvAttrCust->currentIndex().row();
            QModelIndex index = mAttrCustModel->index(row, mAttrCustModel->fieldIndex("customer_idx"));
            mAttrCustModel->setData(index, obj->getId()
                                    + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No customer branch selected.\n"
                                                   "Customer is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button add clicked
 */
void ACC_AssetValuationWidget::on_pbAddAttrSuppl_clicked() {
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelAsset);
    if (!idx.isValid()) return;

    if (!mAttrSupplModel) return;
    mAttrSupplModel->undoFilterSort();

    // always insert at the end, can be done only for smaller models
    int row = mAttrSupplModel->rowCount();
    mAttrSupplModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QDateTime today = QDateTime::currentDateTime();
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("weight"));
    mAttrSupplModel->setData(idx, 100, Qt::EditRole);
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("value"));
    mAttrSupplModel->setData(idx, 0.00, Qt::EditRole);
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("multiplywith"));
    mAttrSupplModel->setData(idx, 0, Qt::EditRole);
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("startdate"));
    mAttrSupplModel->setData(idx, today, Qt::EditRole);
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("enddate"));
    mAttrSupplModel->setData(idx, today, Qt::EditRole);
    idx = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("interval"));
    mAttrSupplModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tvAttrSuppl->setCurrentIndex(mAttrSupplModel->index(row, RB2::HIDDENCOLUMNS));
    tvAttrSuppl->scrollTo(tvAttrSuppl->currentIndex());
    leSeqNoSuppl->setFocus();
    leSeqNoSuppl->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_AssetValuationWidget::on_pbDeleteAttrSuppl_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete attribute ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this attribute?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    if (!mAttrSupplModel) return;

    QModelIndex idx = tvAttrSuppl->currentIndex();
    if (idx.isValid()) {
        mAttrSupplModel->removeRows(idx.row(), 1, QModelIndex());
        mAttrSupplModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

void ACC_AssetValuationWidget::on_ileSupplName_clicked() {
    if (!tvAttrSuppl->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No attribute selected.\n"
                                                   "Please select an attribute first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectSupplier);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvAttrSuppl->currentIndex().row();
            QModelIndex index = mAttrSupplModel->index(row, mAttrSupplModel->fieldIndex("supplier_idx"));
            mAttrSupplModel->setData(index, obj->getId()
                               + obj->getValue("suppname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Supplier is unchanged."));
    }

    dlg->deleteLater();

}

/**
 * Button Refresh clicked.
 */
void ACC_AssetValuationWidget::on_pbRefresh_clicked() {
    if (!mAttrCustModel || !mAttrSupplModel) {
        RB_DEBUG->error("ACC_AssetValuationWidget::on_pbRefresh_clicked() NULL ERROR");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_ObjectContainer* inputList =
            new RB_ObjectContainer("", NULL, "ACC_AssetCustAttrList", ACC_OBJECTFACTORY);
    RB_String id = mAttrCustModel->getRoot()->getId();
    inputList->setValue("parent", id);
    inputList->dbReadList(ACC_MODELFACTORY->getDatabase());
    id = mAttrSupplModel->getRoot()->getId();
    inputList->setValue("parent", id);
    inputList->setName("ACC_AssetSupplAttrList");
    inputList->dbReadList(ACC_MODELFACTORY->getDatabase());

    RB_String htmlStr = "";
    int inputCount = inputList->objectCount();

    if (inputCount < 1) {
        htmlStr = "<p>" + tr("No input found.") + "</p>";
        teValuation->setHtml(htmlStr);
        delete inputList;
        QApplication::restoreOverrideCursor();
        return;
    }

    // set all cost value to minus
    RB_ObjectIterator* iter = inputList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getName() == "ACC_AssetSupplAttr") {
            double val = obj->getValue("value").toDouble();
            obj->setValue("value", val * (-1));
        }
    }

    delete iter;

    // create outputList
    RB_ObjectContainer* outputList =
            new RB_ObjectContainer("", NULL, "ACC_OutputList", ACC_OBJECTFACTORY);

    // create operation ACC_ValueAsset and execute
    ACC_ValueAsset* oper = new ACC_ValueAsset();
    if (!oper->execute(inputList, outputList)) {
        teValuation ->setHtml(oper->getErrorMessage());
        delete oper;
        delete inputList;
        delete outputList;
        QApplication::restoreOverrideCursor();
        return;
    }
    delete oper;

    // create report text
    htmlStr += "<h2>" + mAssetModel->getCurrentValue("assetcode").toString() + " "
             + mAssetModel->getCurrentValue("mname").toString() + "</h2>";
    htmlStr += "<h3>Based on bank lending/discount rate: "
            + RB_String::number(sbDiscountRate->value()) + "%</h3>";
    htmlStr += "<p>Created Date/Time: " + QDateTime::currentDateTime().toString() + "</p>";

    // Add cash flow barcharts
    htmlStr.append("<p>Cash Flow:</p>");
    setBarChart(htmlStr, outputList, ChartCashFlow);
    setValueTable(htmlStr, "Cash Flow Values:");

    htmlStr.append("<p>Cumulative Cash Flow:</p>");
    setBarChart(htmlStr, outputList, ChartCumulativeCF);
    setValueTable(htmlStr, "Cumulative Cash Flow Values:");

    // Add net present value barcharts
    htmlStr.append("<p>Net Present Value:</p>");
    setBarChart(htmlStr, outputList, ChartNettPresentValue);
    setValueTable(htmlStr, "Net Present Values:");

    htmlStr.append("<p>Cumulative Net Present Value:</p>");
    setBarChart(htmlStr, outputList, ChartCumulativeNPV);
    setValueTable(htmlStr, "Cumulative Net Present Values:");

    // intermediate values
//    htmlStr += "<table>";
//    iter = outputList->createIterator();

//    for (iter->first(); !iter->isDone(); iter->next()) {
//        RB_ObjectBase* obj = iter->currentObject();
//        htmlStr += "<tr><td>"
//                + obj->getValue("seqno").toString() + "</td><td>&nbsp;</td><td>"
//                + obj->getValue("shortname").toString() + "</td><td>&nbsp;</td><td>"
//                + obj->getValue("description").toString() + "</td><td>&nbsp;</td><td>"
//                + obj->getValue("startdate").toDate().toString(Qt::ISODate) + "</td><td>&nbsp;</td><td>"
//                + RB_String::number(obj->getValue("resultvalue").toDouble(), 'f', 2)
//                + "</td></tr>";
//    }

//    delete iter;
//    htmlStr += "</table>";

    teValuation->setHtml(htmlStr);
    delete inputList;
    delete outputList;

    QApplication::restoreOverrideCursor();
}

bool ACC_AssetValuationWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Slot parent (contact) model row changed, updates leSelectedBy
 * @param current current index (not used)
 * @param previous previous index (not used)
 */
void ACC_AssetValuationWidget::slotAssetRowChanged(const QModelIndex& /* current */,
                                                   const QModelIndex& /* previous */) {
    if (!mAssetModel)  return;
    leAssetCode->setText(mAssetModel->getCurrentValue("assetcode").toString());
}

/**
 * Set barchart in content
 * @param content report content to set barchart
 * @param outputList output list sorted by date with the data for the barchart
 */
void ACC_AssetValuationWidget::setBarChart(RB_String& content,
                                           RB_ObjectBase* outputList, ChartType type) {
    int startPeriod = 0; //  = year * 100 + month
    int endPeriod = 0;
    int runnerPeriod = 0;
    int currentPeriod = 0;
    double minResultValue = 0.0;
    double maxResultValue = 0.0;
    double currentResultValue = 0.0;
    double cumulativeResultValue = 0.0;
    double discountRate = sbDiscountRate->value() / 100;

    mResultMap.clear();

    RB_ObjectContainer* objC = dynamic_cast<RB_ObjectContainer*>(outputList);
    objC->sort("startdate", RB2::SortOrderAscending, RB2::MemberDateTime);

    int i = 0;
    RB_ObjectIterator* iter = objC->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        runnerPeriod = obj->getValue("startdate").toDate().year() * 100
                + obj->getValue("startdate").toDate().month();

        if (currentPeriod > 0 && runnerPeriod != currentPeriod) {
            // add total resultvalue to chart items
            if (type == ChartCashFlow) {
                cumulativeResultValue = currentResultValue; // only for min/max
                mResultMap[currentPeriod] = currentResultValue;
            } else if (type == ChartCumulativeCF) {
                cumulativeResultValue += currentResultValue;
                mResultMap[currentPeriod] = cumulativeResultValue;
            } else if (type == ChartNettPresentValue) {
                cumulativeResultValue = currentResultValue; // only for min/max
                mResultMap[currentPeriod] = currentResultValue
                        / pow((1 + discountRate), (double(i) + 0.5));
                ++i;
            } else if (type == ChartCumulativeNPV) {
                cumulativeResultValue += currentResultValue
                        / pow((1 + discountRate), (double(i) + 0.5));
                mResultMap[currentPeriod] = cumulativeResultValue;
                ++i;
            }
            currentResultValue = 0.0;
        }

        currentPeriod = runnerPeriod;

        if (startPeriod == 0 || startPeriod > runnerPeriod) {
            startPeriod = runnerPeriod;
        }

        if (endPeriod < runnerPeriod) {
            endPeriod = runnerPeriod;
        }

        currentResultValue += obj->getValue("resultvalue").toDouble();

        if (minResultValue == 0 || minResultValue > cumulativeResultValue) {
            minResultValue = cumulativeResultValue;
        }

        if (maxResultValue < cumulativeResultValue) {
            maxResultValue = cumulativeResultValue;
        }
    }

    delete iter;

    // add last result value and period
    if (type == ChartCashFlow) {
        cumulativeResultValue = currentResultValue; // only for min/max
        mResultMap[currentPeriod] = currentResultValue;
    } else if (type == ChartCumulativeCF) {
        cumulativeResultValue += currentResultValue;
        mResultMap[currentPeriod] = cumulativeResultValue;
    } else if (type == ChartNettPresentValue) {
        cumulativeResultValue = currentResultValue; // only for min/max
        mResultMap[currentPeriod] = currentResultValue
                / pow((1 + discountRate), (double(i) + 0.5));
        ++i;
    } else if (type == ChartCumulativeNPV) {
        cumulativeResultValue += currentResultValue
                / pow((1 + discountRate), (double(i) + 0.5));
        mResultMap[currentPeriod] = cumulativeResultValue;
        ++i;
    }

    if (minResultValue == 0 || minResultValue > cumulativeResultValue) {
        minResultValue = cumulativeResultValue;
    }

    if (maxResultValue < cumulativeResultValue) {
        maxResultValue = cumulativeResultValue;
    }

    // Chart
    THRD_Chart chart;
    //{Bar,Line,Pie2D,Pie3D,StackedBar}
    chart.setChartType(THRD_Chart::Bar);
    //{AxesAngled,AxesHoriz,Horizontal,Round,Vertical}
    chart.setLegendType(THRD_Chart::AxesAngled);
    chart.setShadow(true);
    chart.setValueInChart(false);

    // size in pixels
    QPixmap pxm(675, 420);
    pxm.fill(); // white
    QPainter painter(&pxm);

    chart.setChartCoords(50, 10, 600, 400); // x, y, w, h
    chart.setLegendCoords(50, 50);

    // chart.setFont(QFont(strList.at(11).trimmed()));
    // chart.setPieStartAngle(strList.at(12).trimmed().toDouble());
    chart.setLabelBefore("EUR");
    chart.setLabelAfter(""); // otherwise %
    chart.setLengthYaxis(std::max(std::abs(maxResultValue),
                                  std::abs(minResultValue))); // based on outputlist

    // int stackCount = 17 + strList.at(16).trimmed().toInt();

    // for (int i = 17; i < stackCount; ++i) {
    //     chart.addStackName(strList.at(i).trimmed());
    // }

    // int itemCount = stackCount + 1 + strList.at(stackCount).trimmed().toInt() * 3;

    // for (int i = stackCount + 1; i < itemCount; /*++i*/) {
    //     chart.addChartItem(strList.at(i).trimmed(), QColor(strList.at(i+1).trimmed()),
    //                        result->getResult(strList.at(i+2).trimmed()));
    //     i += 3;
    // }

    // Cash flow
    std::map<int, double>::iterator itr;
    double val =  0.0;

    for (itr = mResultMap.begin(); itr != mResultMap.end(); ++itr) {
        val = itr->second;

        if (val >= 0.0) {
            chart.addChartItem(RB_String::number(itr->first), QColor("#336699"), val);
        } else {
            chart.addChartItem(RB_String::number(itr->first), QColor("#CC0000"), val);
        }
    }

    chart.draw(&painter);
    chart.drawLegend(&painter);

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);

    if (pxm.save(&imageBuffer, "PNG")) {
        content.append("<p><img src=\"data:image/png;base64,");
        content.append(QString(QLatin1String(imageBuffer.buffer().toBase64())));
        content.append("\" /></p>");
    }

    imageBuffer.close();
}

/**
 * Set values after the bar chart has been painted
 * @param content HTML content
 * @param heading heading or title of the value table
 */
void ACC_AssetValuationWidget::setValueTable(RB_String& content,
                                             const RB_String& heading) {
    content.append("<h3>");
    content.append(heading);
    content.append("</h3>");

    // Add cash flow calculated values
    content += "<table>";
    std::map<int, double>::iterator itr;

    for (itr = mResultMap.begin(); itr != mResultMap.end(); ++itr) {
        content += "<tr><td>"
                + RB_String::number(itr->first) + "</td><td>&nbsp;</td><td align=\"right\">"
                + RB_String::number(itr->second, 'f', 2)
                + "</td></tr>";
    }

    content += "</table>";
}

/**
 * Change event, for example translation
 */
void ACC_AssetValuationWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
