/*****************************************************************
 * $Id: peng_insulationdialog.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Aug 18, 2005 - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_insulationdialog.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"


PENG_InsulationDialog::PENG_InsulationDialog(QWidget *parent)
    : RB_Dialog(parent) {

    setupUi(this);

    mModelCC = NULL;
    mModelHCI = NULL;
    mModelHCO = NULL;
    mModelPP = NULL;
}

PENG_InsulationDialog::~PENG_InsulationDialog() {
    delete mModelCC;
    delete mModelHCI;
    delete mModelHCO;
    delete mModelPP;
    RB_DEBUG->print("PENG_InsulationDialog::~PENG_InsulationDialog() OK");
}

void PENG_InsulationDialog::init() {

    setWindowTitle(tr("PENG Edit Insulation Tables[*]"));

    //
    // 1. Set model with ID
    //
    mModelCC = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelInsulationCC, false);
    mModelCC->setRoot(PENG_MODELFACTORY->getRootId());
    mModelHCI = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelInsulationHCI, false);
    mModelHCI->setRoot(PENG_MODELFACTORY->getRootId());
    mModelHCO = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelInsulationHCO, false);
    mModelHCO->setRoot(PENG_MODELFACTORY->getRootId());
    mModelPP = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelInsulationPP, false);
    mModelPP->setRoot(PENG_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    // mMapper = mModel->getMapper();
    // mMapper->addMapping(leNumber, mModel->fieldIndex("number"));

    //
    // 3. Execute SELECT on model
    //
    mModelCC->select();
    mModelHCI->select();
    mModelHCO->select();
    mModelPP->select();
    updateModels();

    //
    // 4. Connect model to main view
    //
    formatTableView(tvColdConservation, mModelCC);
    formatTableView(tvHeatConservationIndoor, mModelHCI);
    formatTableView(tvHeatConservationOutdoor, mModelHCO);
    formatTableView(tvPersonnelProtection, mModelPP);

    //
    // 5. Set toolbuttonbar
    //
    //    tbbPaint->initEdit(false, false, false);
    //    tvPaint->setToolButtonBar(tbbPaint);
    //    connect(tbbPaint, SIGNAL(addClicked()),
    //            this, SLOT(slotPbAdd_clicked()));

    readSettings();

    //
    // 5. Set visible columns in tableView
    //
    setVisibleColumns(mModelCC, tvColdConservation);
    setVisibleColumns(mModelHCI, tvHeatConservationIndoor);
    setVisibleColumns(mModelHCO, tvHeatConservationOutdoor);
    setVisibleColumns(mModelPP, tvPersonnelProtection);

    tvColdConservation->verticalHeader()->setVisible(false);
    tvHeatConservationIndoor->verticalHeader()->setVisible(false);
    tvHeatConservationOutdoor->verticalHeader()->setVisible(false);
    tvPersonnelProtection->verticalHeader()->setVisible(false);

    mModelCC->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, "Line Size");
    setColdTemperatureHeader(mModelCC);
    setWarmTemperatureHeader(mModelHCI);
    setWarmTemperatureHeader(mModelHCO);
    setWarmTemperatureHeader(mModelPP);
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool PENG_InsulationDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect) {
        if (mModelCC->submitAllAndSelect() && mModelHCI->submitAllAndSelect()
                && mModelHCO->submitAllAndSelect() && mModelPP->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else {
        if (mModelCC->submitAll() && mModelHCI->submitAll()
                && mModelHCO->submitAll() && mModelPP->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void PENG_InsulationDialog::fileRevert() {
    mModelCC->revert();
    mModelHCI->revert();
    mModelHCO->revert();
    mModelPP->revert();
    setWindowModified(false);
}

void PENG_InsulationDialog::updateModels() {
    if(mModelCC->rowCount() < 1) {
        addDefaultRows(mModelCC);
        setWindowModified(true);
    }

    if(mModelHCI->rowCount() < 1) {
        addDefaultRows(mModelHCI);
        setWindowModified(true);
    }

    if(mModelHCO->rowCount() < 1) {
        addDefaultRows(mModelHCO);
        setWindowModified(true);
    }

    if(mModelPP->rowCount() < 1) {
        addDefaultRows(mModelPP);
        setWindowModified(true);
    }
}

void PENG_InsulationDialog::addDefaultRows(RB_MmProxy* model) {
    // setDefaultRow(model, 0.125);
    // setDefaultRow(model, 0.25);
    // setDefaultRow(model, 0.375);
    setDefaultRow(model, 0.5);
    setDefaultRow(model, 0.750);
    setDefaultRow(model, 1.0);
    // setDefaultRow(model, 1.25);
    setDefaultRow(model, 1.500);
    setDefaultRow(model, 2.0);
    setDefaultRow(model, 2.5);
    setDefaultRow(model, 3.0);
    // setDefaultRow(model, 3.5);
    setDefaultRow(model, 4.0);
    // setDefaultRow(model, 5.0);
    setDefaultRow(model, 6.0);
    setDefaultRow(model, 8.0);
    setDefaultRow(model, 10.0);
    setDefaultRow(model, 12.0);
    setDefaultRow(model, 14.0);
    setDefaultRow(model, 16.0);
    setDefaultRow(model, 18.0);
    setDefaultRow(model, 20.0);
    setDefaultRow(model, 22.0);
    setDefaultRow(model, 24.0);
    setDefaultRow(model, 26.0);
    setDefaultRow(model, 28.0);
    setDefaultRow(model, 30.0);
    setDefaultRow(model, 32.0);
    setDefaultRow(model, 34.0);
    setDefaultRow(model, 36.0);
}

void PENG_InsulationDialog::setDefaultRow(RB_MmProxy* model, double lineSize) {
    int row = model->rowCount();
    model->insertRows(row, 1);
    QModelIndex idx = model->index(row, model->fieldIndex("linesize"));
    model->setData(idx, lineSize);
}

/**
 * Button help clicked
 */
void PENG_InsulationDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = PENG_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void PENG_InsulationDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void PENG_InsulationDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void PENG_InsulationDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void PENG_InsulationDialog::setVisibleColumns(RB_MmProxy* model, RB_TableView* tv) {
    int colCount = model->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tv->hideColumn(i);
        } else if (i == RB2::HIDDENCOLUMNS) {
            tv->showColumn(i);
            tv->setColumnWidth(i, 80);
        } else {
            tv->showColumn(i);
            tv->setColumnWidth(i, 50);
        }
    }

}

void PENG_InsulationDialog::setColdTemperatureHeader(RB_MmProxy* model) {
    int col = RB2::HIDDENCOLUMNS;
    model->setHeaderData(col, Qt::Horizontal, "Line Size");
    model->setHeaderData(++col,  Qt::Horizontal, "+5");
    model->setHeaderData(++col,  Qt::Horizontal, "0");
    model->setHeaderData(++col,  Qt::Horizontal, "-5");
    model->setHeaderData(++col,  Qt::Horizontal, "-10");
    model->setHeaderData(++col,  Qt::Horizontal, "-15");
    model->setHeaderData(++col,  Qt::Horizontal, "-20");
    model->setHeaderData(++col,  Qt::Horizontal, "-25");
    model->setHeaderData(++col,  Qt::Horizontal, "-30");
    model->setHeaderData(++col,  Qt::Horizontal, "-35");
    model->setHeaderData(++col,  Qt::Horizontal, "-40");
    model->setHeaderData(++col,  Qt::Horizontal, "-45");
    model->setHeaderData(++col,  Qt::Horizontal, "-50");
}

void PENG_InsulationDialog::setWarmTemperatureHeader(RB_MmProxy* model) {
    int col = RB2::HIDDENCOLUMNS;
    model->setHeaderData(col, Qt::Horizontal, "Line Size");
    model->setHeaderData(++col,  Qt::Horizontal, "50");
    model->setHeaderData(++col,  Qt::Horizontal, "100");
    model->setHeaderData(++col,  Qt::Horizontal, "150");
    model->setHeaderData(++col,  Qt::Horizontal, "200");
    model->setHeaderData(++col,  Qt::Horizontal, "250");
    model->setHeaderData(++col,  Qt::Horizontal, "300");
    model->setHeaderData(++col,  Qt::Horizontal, "350");
    model->setHeaderData(++col,  Qt::Horizontal, "400");
    model->setHeaderData(++col,  Qt::Horizontal, "450");
    model->setHeaderData(++col,  Qt::Horizontal, "500");
    model->setHeaderData(++col,  Qt::Horizontal, "550");
    model->setHeaderData(++col,  Qt::Horizontal, "600");
}
