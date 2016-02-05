/*****************************************************************
 * $Id: pcalc_en13555gasketwidget.cpp 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en13555gasketwidget.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"
#include "ui_rb_databasetablewidget.h"

PCALC_EN13555GasketWidget::PCALC_EN13555GasketWidget(QWidget* parent)
    : RB_DatabaseTableWidget(parent) {

    mModel = nullptr;
}

PCALC_EN13555GasketWidget::~PCALC_EN13555GasketWidget() {
    RB_DEBUG->print("PCALC_EN13555GasketWidget::~PCALC_EN13555GasketWidget() OK");
}

void PCALC_EN13555GasketWidget::init() {
    QStringList items;
    items << tr("None") << "QminL" << "QsminL" << "PQR delta_eGC" << "EG eG";
    ui->cbProperty->addItems(items);

    connect(ui->cbProperty, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSetPropertyTable(int)));

    ui->tableView->setToolButtonBar(ui->tbbData);
    ui->cbProperty->setCurrentIndex(0);
    readSettings();
}

bool PCALC_EN13555GasketWidget::fileSave(bool withSelect) {
    bool success = true;

    if (mModel) {
        if (withSelect) {
            success = mModel->submitAllAndSelect();
        } else {
            success = mModel->submitAll();
        }
    }

    return success;
}

void PCALC_EN13555GasketWidget::fileRevert() {
    if (mModel) {
        mModel->revert();
    }
    return;
}

QString PCALC_EN13555GasketWidget::getHelpSubject() const {
    return objectName();
}

void PCALC_EN13555GasketWidget::on_pbSelectManuf_clicked() {
    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetEN13555Manuf);


    if (dlg->exec() == QDialog::Rejected) {
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();
    on_pbClearType_clicked();
    setCodeManufacturer(obj);
    dlg->deleteLater();
}

void PCALC_EN13555GasketWidget::on_pbClearManuf_clicked() {
    on_pbClearType_clicked();
    setCodeManufacturer(nullptr);
}

void PCALC_EN13555GasketWidget::on_pbSelectType_clicked() {
    // Is valid parent ID set for this dialog
    if (mCodeManufId.size() < 38) {
        PCALC_DIALOGFACTORY->requestInformationDialog(
                    tr("Not a valid manufacturer selected\n"));
        return;
    }

    // Set parent ID for dialog
    PCALC_MODELFACTORY->setStoredId(mCodeManufId);

    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetEN13555Type);


    if (dlg->exec() == QDialog::Rejected) {
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();
    slotSetPropertyTable(0);
    setType(obj);
    dlg->deleteLater();
}

void PCALC_EN13555GasketWidget::on_pbClearType_clicked() {
    slotSetPropertyTable(0);
    setType(nullptr);
    ui->cbProperty->setCurrentIndex(0);
}

void PCALC_EN13555GasketWidget::slotSetPropertyTable(int index) {
    // items << tr("None") << "QminL" << "QsminL" << "PQR delta_eGC" << "EG eG";
    QStringList items;

    switch (index) {
    case 0: { // None
        if (mModel) {
            fileSave(false);
            delete mModel;
            ui->tableView->setModel(nullptr);
        }

        break;
    }
    case 1: { // QminL
        fileSave(false);
        if (mModel) delete mModel;
        mModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelEN13555QminL, false);
        setModelTableView(mModel);

        items.clear();
        items << tr("None") << "testpressure" << "leakrate" << "qminl";
        ui->cbXcoord->addItems(items);
        ui->cbXcoord->setCurrentIndex(3);
        ui->cbYcoord->addItems(items);
        ui->cbYcoord->setCurrentIndex(2);
        setChartModel(mModel, ui->cbXcoord->currentText(),
                      ui->cbYcoord->currentText(), ScaleYLog);
        break;
    }
    case 2: { // QsminL
        fileSave(false);
        if (mModel) delete mModel;
        mModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelEN13555QsminL, false);
        setModelTableView(mModel);
        break;
    }
    case 3: { // PQR delta_eGC
        fileSave(false);
        if (mModel) delete mModel;
        mModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelEN13555PQRdeltaeGC, false);
        setModelTableView(mModel);
        break;
    }
    case 4: { // EG eG
        fileSave(false);
        if (mModel) delete mModel;
        mModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelEN13555EGeG, false);
        setModelTableView(mModel);
        break;
    }
    default:
        break;
    }

}

void PCALC_EN13555GasketWidget::setModelTableView(RB_MmProxy* model) {
    model->setRoot(mTypeId);
    model->select();
    formatTableView(ui->tableView, model);

    // Hide columns
    int colCount = model->columnCount();

    for (int i = 0; i < RB2::HIDDENCOLUMNS && i < colCount; ++i) {
        ui->tableView->hideColumn(i);
    }
}
