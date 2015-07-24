/*****************************************************************
 * $Id: peng_linedetailwidget.cpp 2241 2015-05-22 10:22:19Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_linedetailwidget.h"

#include "peng.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_objectiterator.h"
#include "rb_widget.h"


PENG_LineDetailWidget::PENG_LineDetailWidget(QWidget *parent)
                    : RB_Widget(parent){
    setupUi(this);

    mModelLine = NULL;
    mSizeHint = QSize(580, 385);
}

PENG_LineDetailWidget::~PENG_LineDetailWidget() {
    // delete only the line mapper not the model
    if (mModelLine) {
        mModelLine->deleteMapper("PENG_LineDetailWidget"); // not the default
    }
}

void PENG_LineDetailWidget::init() {
    //
    // 1. Set model with ID
    //
    mModelLine = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLine);
    // mModelLine->setRoot(PENG_MODELFACTORY->getRootId()); already done

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapperLine = mModelLine->getMapper("PENG_LineDetailWidget");
    mMapperLine->addMapping(leLineNo, mModelLine->fieldIndex("number"));
    mMapperLine->addMapping(leRevision, mModelLine->fieldIndex("revision"));
    mMapperLine->addMapping(leDnSize, mModelLine->fieldIndex("sizedn"));
    leDnSize->setValidator(new QDoubleValidator(0.1, 9999.9, 2, this));
    RB_StringList items;
    items << tr("(none)") << tr("Process") << tr("Utility");
    mModelLine->setTextList(mModelLine->fieldIndex("processutility_id"), items);
    cbProcessUtility->setModel(new QStringListModel(items, this));
    mMapperLine->addMapping(cbProcessUtility, mModelLine->fieldIndex("processutility_id"),
                            "currentIndex");
    mMapperLine->addMapping(leFrom, mModelLine->fieldIndex("fromdesignation"));
    items.clear();
    items << tr("(none)") << tr("Atmosphere") << tr("Battery limit")
          << tr("Equipment") << tr("Line") << tr("Trim");
    mModelLine->setTextList(mModelLine->fieldIndex("fromtype_id"), items);
    cbFromType->setModel(new QStringListModel(items, this));
    mMapperLine->addMapping(cbFromType, mModelLine->fieldIndex("fromtype_id"),
                            "currentIndex");
    mMapperLine->addMapping(leFromSub, mModelLine->fieldIndex("fromsub")); // Nozzle number
    mMapperLine->addMapping(leTo, mModelLine->fieldIndex("todesignation"));
    mModelLine->setTextList(mModelLine->fieldIndex("totype_id"), items);
    cbToType->setModel(new QStringListModel(items, this));
    mMapperLine->addMapping(cbToType, mModelLine->fieldIndex("totype_id"),
                            "currentIndex");
    mMapperLine->addMapping(leToSub, mModelLine->fieldIndex("tosub")); // Nozzle number
    mMapperLine->addMapping(ileFluidCode, mModelLine->fieldIndex("fluid_idx"));
    mMapperLine->addMapping(ilePipeClass, mModelLine->fieldIndex("pipeclass_idx"));
    mMapperLine->addMapping(leTd, mModelLine->fieldIndex("designtemp"));
    leTd->setValidator(new QDoubleValidator(-999.9, 9999.9, 2, this));
    mMapperLine->addMapping(lePd, mModelLine->fieldIndex("designpress"));
    lePd->setValidator(new QDoubleValidator(-99.9, 999.9, 2, this));

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    QModelIndex idx = mModelLine->getProxyIndex();
    mMapperLine->setCurrentModelIndex(idx);

    // Separate models for detail line table data,
    // lazy loading as per other models CRM_CustomerWidget
    mModelLineClassif = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLineClassif);
    mMapperLineClassif = mModelLineClassif->getMapper("PENG_LineDetailWidget");
    mMapperLineClassif->addMapping(leClassNomOperPress, mModelLineClassif->fieldIndex("classifnomoperpress"));
    leClassNomOperPress->setValidator(new QDoubleValidator(-99.9, 999.9, 2, this));
    mMapperLineClassif->addMapping(leClassNomOperTemp, mModelLineClassif->fieldIndex("classifnomopertemp"));
    leClassNomOperTemp->setValidator(new QDoubleValidator(-999.9, 9999.9, 2, this));
    mMapperLineClassif->addMapping(leClassMaxDesPress, mModelLineClassif->fieldIndex("classifmaxdespress"));
    leClassMaxDesPress->setValidator(new QDoubleValidator(-99.9, 999.9, 2, this));
    mMapperLineClassif->addMapping(leClassMaxDesTemp, mModelLineClassif->fieldIndex("classifmaxdestemp"));
    leClassMaxDesTemp->setValidator(new QDoubleValidator(-999.9, 9999.9, 2, this));
    mMapperLineClassif->addMapping(leClassMinDesTemp, mModelLineClassif->fieldIndex("classifmindestemp"));
    leClassMinDesTemp->setValidator(new QDoubleValidator(-999.9, 9999.9, 2, this));
    mMapperLineClassif->addMapping(leClassAuthorRegul, mModelLineClassif->fieldIndex("classifauthorityregulation"));
    addComboBoxMapping(mModelLineClassif,"classiffluiddangergroup_id", "PENG_HazardGroup", "id",
                       "code", cbClassFluidDangerClassGroup, mMapperLineClassif);
    addComboBoxMapping(mModelLineClassif,"classifgasstability_id", "PENG_FluidStability", "id",
                       "code", cbClassGasStability, mMapperLineClassif);
    mMapperLineClassif->addMapping(leClassRegulClassGroup, mModelLineClassif->fieldIndex("classifregulationgroup"));
    mMapperLineClassif->addMapping(leClassRegulSubClassGroup, mModelLineClassif->fieldIndex("classifregulationsubgroup"));

    mModelLineCustom = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLineCustom);
    mMapperLineCustom = mModelLineCustom->getMapper("PENG_LineDetailWidget");
    mMapperLineCustom->addMapping(leCustValue1, mModelLineCustom->fieldIndex("customfield1value"));
    leCustValue1->setValidator(new QDoubleValidator(this));
    mMapperLineCustom->addMapping(leCustValue2, mModelLineCustom->fieldIndex("customfield2value"));
    leCustValue2->setValidator(new QDoubleValidator(this));
    mMapperLineCustom->addMapping(leCustValue3, mModelLineCustom->fieldIndex("customfield3value"));
    leCustValue3->setValidator(new QDoubleValidator(this));
    mMapperLineCustom->addMapping(leCustValue4, mModelLineCustom->fieldIndex("customfield4value"));
    leCustValue4->setValidator(new QDoubleValidator(this));
    mMapperLineCustom->addMapping(leCustValue5, mModelLineCustom->fieldIndex("customfield5value"));
    leCustValue5->setValidator(new QDoubleValidator(this));
    mMapperLineCustom->addMapping(leCustText6, mModelLineCustom->fieldIndex("customfield6text"));
    mMapperLineCustom->addMapping(leCustText7, mModelLineCustom->fieldIndex("customfield7text"));
    mMapperLineCustom->addMapping(leCustText8, mModelLineCustom->fieldIndex("customfield8text"));
    mMapperLineCustom->addMapping(leCustText9, mModelLineCustom->fieldIndex("customfield9text"));
    mMapperLineCustom->addMapping(leCustText10, mModelLineCustom->fieldIndex("customfield10text"));

    mModelLineEnginDes = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLineEnginDes);
    mMapperLineEnginDes = mModelLineEnginDes->getMapper("PENG_LineDetailWidget");
    mMapperLineEnginDes->addMapping(leDesPnidFrom, mModelLineEnginDes->fieldIndex("engindespnidfrom"));
    mMapperLineEnginDes->addMapping(leDesPnidTo, mModelLineEnginDes->fieldIndex("engindespnidto"));
    mMapperLineEnginDes->addMapping(leDesGaFrom, mModelLineEnginDes->fieldIndex("engindesgafrom"));
    mMapperLineEnginDes->addMapping(leDesGaTo, mModelLineEnginDes->fieldIndex("engindesgato"));
    mMapperLineEnginDes->addMapping(leDesIsoFrom, mModelLineEnginDes->fieldIndex("engindesisofrom"));
    mMapperLineEnginDes->addMapping(leDesIsoTo, mModelLineEnginDes->fieldIndex("engindesisoto"));
    items.clear();
    items << tr("(none)") << tr("Main Line") << tr("Start-up/Shutdown")
             << tr("Priming") << tr("Draining") << tr("Min. Flow Bypass")
                << tr("Secondary Line") << tr("Utility") << tr("Other");
    cbDesLineFunction->setModel(new QStringListModel(items, this));
    mMapperLineEnginDes->addMapping(cbDesLineFunction, mModelLineEnginDes->fieldIndex("engindeslinefunction_id"));
    items.clear();
    items << tr("(none)") << tr("Full Vacuum") << tr("Partial Vacuum")
             << tr("Jacketed Pipe") << tr("Other");
    cbDesExternalPressure->setModel(new QStringListModel(items, this));
    mMapperLineEnginDes->addMapping(cbDesExternalPressure, mModelLineEnginDes->fieldIndex("engindesexternalpress_id"));
    items.clear();
    items << tr("(none)") << tr("Electrical Tracer") << tr("Oil Jacket")
              << tr("Steam Tracer") << tr("Steam Jacket") << tr("Water Jacket")
                  << tr("Winterizing") << tr("other");
    cbDesTracingType->setModel(new QStringListModel(items, this));
    mMapperLineEnginDes->addMapping(cbDesTracingType, mModelLineEnginDes->fieldIndex("engindestracingtype_id"));
    leDesTracingNumberOf->setValidator(new QIntValidator(0, 99, this));
    mMapperLineEnginDes->addMapping(leDesTracingNumberOf, mModelLineEnginDes->fieldIndex("engindesnotracer"));
    items.clear();
    items << tr("(none)") << tr("Cold Conservation") << tr("Heat Conservation")
              << tr("Heat Cons. Indoor") << tr("Personnel Protection")
                  << tr("Other");
    cbDesInsulationType->setModel(new QStringListModel(items, this));
    mMapperLineEnginDes->addMapping(cbDesInsulationType, mModelLineEnginDes->fieldIndex("engindesinsulationtype_id"));
    leDesInsulationThickness->setValidator(new QDoubleValidator(0.0, 999.9, 2, this));
    mMapperLineEnginDes->addMapping(leDesInsulationThickness, mModelLineEnginDes->fieldIndex("engindesinsulationthickness"));
    mMapperLineEnginDes->addMapping(ckbDesPaintingAsPerSpec, mModelLineEnginDes->fieldIndex("engindesusepaintspec_id"));
    mMapperLineEnginDes->addMapping(leDesPaintingSystem, mModelLineEnginDes->fieldIndex("engindespaintsystem"));
    mMapperLineEnginDes->addMapping(ckbStressCalcRequired, mModelLineEnginDes->fieldIndex("enginstresscalcrequired_id"));
    mMapperLineEnginDes->addMapping(leStressCalcNumber, mModelLineEnginDes->fieldIndex("enginstresscalcnumber"));

    mModelLineProcess = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLineProcess);
    mMapperLineProcess = mModelLineProcess->getMapper("PENG_LineDetailWidget");
    items.clear();
    items << tr("(none)") << tr("Gas") << tr("Liquid") << tr("Vapour")
          << tr("Two Phase") << tr("With Solids");
    mModelLineProcess->setTextList(mModelLineProcess->fieldIndex("processphase_id"), items);
    cbProcessPhase->setModel(new QStringListModel(items, this));
    mMapperLineProcess->addMapping(cbProcessPhase, mModelLineProcess->fieldIndex("processphase_id"),
                                   "currentIndex");
    mMapperLineProcess->addMapping(leProcessActualFlow, mModelLineProcess->fieldIndex("processactualflowstd"));
    mMapperLineProcess->addMapping(leProcessActualFlowMax, mModelLineProcess->fieldIndex("processactualflowmax"));
    mMapperLineProcess->addMapping(leProcessBoilingPoint, mModelLineProcess->fieldIndex("processboilingpoint"));
    mMapperLineProcess->addMapping(leProcessDensityLiquid, mModelLineProcess->fieldIndex("processdensityliquid"));
    mMapperLineProcess->addMapping(leProcessDensityVapour, mModelLineProcess->fieldIndex("processdensityvapour"));
    mMapperLineProcess->addMapping(leProcessFluidDescr, mModelLineProcess->fieldIndex("processfluiddescription"));
    mMapperLineProcess->addMapping(leProcessMassFlowRate, mModelLineProcess->fieldIndex("processmassflowratestd"));
    mMapperLineProcess->addMapping(leProcessMassFlowRateMax, mModelLineProcess->fieldIndex("processmassflowratemax"));
    mMapperLineProcess->addMapping(leProcessMolWeight, mModelLineProcess->fieldIndex("processmolweight"));
    mMapperLineProcess->addMapping(leProcessOperPress, mModelLineProcess->fieldIndex("processoperatingpress"));
    mMapperLineProcess->addMapping(leProcessOperTemp, mModelLineProcess->fieldIndex("processoperatingtempstd"));
    mMapperLineProcess->addMapping(leProcessOperTempHigh, mModelLineProcess->fieldIndex("processoperatingtemphigh"));
    mMapperLineProcess->addMapping(leProcessOperTempLow, mModelLineProcess->fieldIndex("processoperatingtemplow"));
    mMapperLineProcess->addMapping(leProcessPressDrop, mModelLineProcess->fieldIndex("processpressdrop"));
    mMapperLineProcess->addMapping(leProcessSimulation, mModelLineProcess->fieldIndex("processsimulation"));
    mMapperLineProcess->addMapping(leProcessStandardVolumeFlow, mModelLineProcess->fieldIndex("processstandardvolumeflowstd"));
    mMapperLineProcess->addMapping(leProcessStandardVolumeFlowMax, mModelLineProcess->fieldIndex("processstandardvolumeflowmax"));
    mMapperLineProcess->addMapping(leProcessTestMedium, mModelLineProcess->fieldIndex("processtestmedium"));
    mMapperLineProcess->addMapping(leProcessTestPress, mModelLineProcess->fieldIndex("processtestpress"));
    mMapperLineProcess->addMapping(leProcessVapourPress, mModelLineProcess->fieldIndex("processvapourpress"));
    mMapperLineProcess->addMapping(leProcessVelocity, mModelLineProcess->fieldIndex("processvelocity"));
    mMapperLineProcess->addMapping(leProcessViscosityLiquid, mModelLineProcess->fieldIndex("processviscosityliquid"));
    mMapperLineProcess->addMapping(leProcessViscosityVapour, mModelLineProcess->fieldIndex("processviscosityvapour"));

    // Show detail row or add row if not exists
    connect(mModelLine, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));

    // Required because not setFormatTableView()
    connect(mModelLine, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));
    connect(mModelLine, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mModelLineClassif, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mModelLineCustom, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mModelLineEnginDes, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mModelLineProcess, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));

    //
    // 3. Set toolbuttonbar
    //
    tbbLineDetail->initDetailEdit();

    connect(tbbLineDetail, SIGNAL(previousClicked()),
            mMapperLine, SLOT(toPrevious()));
    connect(tbbLineDetail, SIGNAL(nextClicked()),
            mMapperLine, SLOT(toNext()));
    connect(tbbLineDetail, SIGNAL(saveClicked()),
            this, SLOT(slotFileSave()));
    connect(tbbLineDetail, SIGNAL(revertClicked()),
            this, SLOT(slotFileRevert()));

    slotUpdateSettings(PENG_DialogFactory::DialogSettings);
    connect(PENG_DIALOGFACTORY, SIGNAL(settingChanged(int)),
            this, SLOT(slotUpdateSettings(int)));
    readSettings();
}

bool PENG_LineDetailWidget::fileSave(bool withSelect) {
    if (withSelect) {
        mModelLineClassif->submitAllAndSelect();
        mModelLineCustom->submitAllAndSelect();
        mModelLineEnginDes->submitAllAndSelect();
        mModelLineProcess->submitAllAndSelect();
        mModelLine->submitAllAndSelect();
    } else {
        mModelLineClassif->submitAll();
        mModelLineCustom->submitAll();
        mModelLineEnginDes->submitAll();
        mModelLineProcess->submitAll();
        mModelLine->submitAll();
    }
    return true;
}

bool PENG_LineDetailWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

void PENG_LineDetailWidget::slotFileSave() {
    fileSave(true);
}

void PENG_LineDetailWidget::slotFileRevert() {
    mModelLineClassif->revertAll();
    mModelLineCustom->revertAll();
    mModelLineEnginDes->revertAll();
    mModelLineProcess->revertAll();
    mModelLine->revertAll();
    modificationChanged(false);
}

void PENG_LineDetailWidget::on_ileFluidCode_clicked() {
    if (!mModelLine->getProxyIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No line item selected.\n"
                                                    "Please select an line item first."));
        return;
    }

    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogFluid);
    int res = dlg->exec();
    RB_ObjectBase* obj = dlg->getCurrentObject();

    if (res == QDialog::Accepted && obj) {
        RB_String fluidIdx = obj->getId() + obj->getValue("code").toString();
        mModelLine->setCurrentValue("fluid_idx", fluidIdx, Qt::EditRole);

        if (mModelLineClassif->getCurrentIndex().isValid()) {
            mModelLineClassif->setCurrentValue("classiffluiddangergroup_id", obj->getValue("hazardgroup_id"), Qt::EditRole);
            mModelLineClassif->setCurrentValue("classifgasstability_id", obj->getValue("stability_id"), Qt::EditRole);
        }
    } else if (!obj) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                    "Data is unchanged."));
    }

    dlg->deleteLater();
}

void PENG_LineDetailWidget::on_ileFluidCode_clear() {
    if (!mModelLine->getProxyIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No line item selected.\n"
                                                    "Please select an line item first."));
        return;
    }

    mModelLine->setCurrentValue("fluid_idx", "0", Qt::EditRole);
}

void PENG_LineDetailWidget::on_ilePipeClass_clicked() {
    if (!mModelLine->getProxyIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No line item selected.\n"
                                                    "Please select an line item first."));
        return;
    }

    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogPipeClass);
    int res = dlg->exec();
    RB_ObjectBase* obj = dlg->getCurrentObject();

    if (res == QDialog::Accepted && obj) {
        RB_String pipeClassIdx = obj->getId() + obj->getValue("classnumber").toString();
        mModelLine->setCurrentValue("pipeclass_idx", pipeClassIdx, Qt::EditRole);
    } else if (!obj) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                    "Data is unchanged."));
    }

    dlg->deleteLater();
}

void PENG_LineDetailWidget::on_ilePipeClass_clear() {
    if (!mModelLine->getProxyIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No line item selected.\n"
                                                    "Please select an line item first."));
        return;
    }

    mModelLine->setCurrentValue("pipeclass_idx", "0", Qt::EditRole);
}

void PENG_LineDetailWidget::changeEvent(QEvent *e) {
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
 * Parent row changed in ACC_Customer. Timer
 * will fire single shot after 200 milliseconds to activate
 * slotHandleParentRowChanged(). This delay is set to prevent many SQL to
 * get from related table(s) or to create
 * rows that are only browsed very quickly by the user.
 *
 * Add row in related model(s) (CRM_CustomerDetail) if not exists and
 * set QDataWidgetMapper to relevant index.
 *
 * @param curr current model index not used
 * @param prev previous model index not used
 */
void PENG_LineDetailWidget::slotParentRowChanged(const QModelIndex& /*curr*/,
                                               const QModelIndex& /*prev*/) {
    mCurrentId = mModelLine->getCurrentId();
    QTimer::singleShot(200, this, SLOT(slotHandleParentRowChanged()));
}

/**
 * Add row in related model(s) (like CRM_CustomerDetail) if not exists. The
 * current ID needs to be the same as the set ID at slotAddRowIfNotExists().
 * Otherwise the user has apparently already selected a different row.
 */
void PENG_LineDetailWidget::slotHandleParentRowChanged() {
    if (mCurrentId != mModelLine->getCurrentId()) {
        // User already selected another row
        return;
    }

    QModelIndex idx;
    int row = 0; // mCustModel->rowCount();

    if (mModelLineClassif->rowCount() < 1) {
        mModelLineClassif->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        idx = mModelLineClassif->index(row, mModelLineClassif->fieldIndex("classiffluiddangergroup_id"));
        mModelLineClassif->setData(idx, "0", Qt::EditRole);
        idx = mModelLineClassif->index(row, mModelLineClassif->fieldIndex("classifgasstability_id"));
        mModelLineClassif->setData(idx, "0", Qt::EditRole);
        // end NOTE

        mModelLineClassif->submitAll();
    }

    idx = mModelLineClassif->index(0, 0);
    mModelLineClassif->slotChangeCurrentRow(idx, QModelIndex());

    if (mModelLineCustom->rowCount() < 1) {
        mModelLineCustom->insertRows(row, 1, QModelIndex());
        mModelLineCustom->submitAll();
    }

    idx = mModelLineCustom->index(0, 0);
    mModelLineCustom->slotChangeCurrentRow(idx, QModelIndex());

    if (mModelLineEnginDes->rowCount() < 1) {
        mModelLineEnginDes->insertRows(row, 1, QModelIndex());
        mModelLineEnginDes->submitAll();
    }

    idx = mModelLineEnginDes->index(0, 0);
    mModelLineEnginDes->slotChangeCurrentRow(idx, QModelIndex());

    if (mModelLineProcess->rowCount() < 1) {
        mModelLineProcess->insertRows(row, 1, QModelIndex());
        mModelLineProcess->submitAll();
    }

    idx = mModelLineProcess->index(0, 0);
    mModelLineProcess->slotChangeCurrentRow(idx, QModelIndex());
}

/**
 * @brief PENG_LineDetailWidget::slotSettingsChanged
 * @param dialogWidgetType
 */
void PENG_LineDetailWidget::slotUpdateSettings(int dialogWidgetType) {
    if (dialogWidgetType != (int) PENG_DialogFactory::DialogSettings) {
        return;
    }

    RB_ObjectContainer* objC = new RB_ObjectContainer("", NULL, "PENG_SettingList");
    objC->setValue("parent", PENG_MODELFACTORY->getRootId());
    objC->dbReadList(PENG_MODELFACTORY->getDatabase());
    RB_ObjectIterator* iter = objC->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        int categoryType = obj->getValue("category_id").toInt();

        if (categoryType == (int) PENG2::SettingCatCustom) {
            int settingType = obj->getValue("setting_id").toInt();

            switch (settingType) {
            case PENG2::SettingCustomLabel1:
                leCustLabel1->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel2:
                leCustLabel2->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel3:
                leCustLabel3->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel4:
                leCustLabel4->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel5:
                leCustLabel5->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel6:
                leCustLabel6->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel7:
                leCustLabel7->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel8:
                leCustLabel8->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel9:
                leCustLabel9->setText(obj->getValue("value").toString());
                break;
            case PENG2::SettingCustomLabel10:
                leCustLabel10->setText(obj->getValue("value").toString());
                break;
            default:
                break;

            }
        }
    }

    delete iter;
    delete objC;
}






#if 0
#include "mainwindow.h"


#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QImage>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QMenuBar>
#include <QModelIndex>
#include <QPixmap>
#include <QtGui>
#include <QtSql>
#include <QToolBar>

#include "actiondialog.h"
#include "auth_permissiondialog.h"
#include "auth_projectselectdialog.h"
#include "db_browserdialog.h"
#include "db_connectiondialog.h"
#include "db_mappingdialog.h"
#include "filterdialog.h"
#include "fluiddialog.h"
#include "fluidreport.h"
#include "helpbrowser.h"
#include "inspectiondatareport.h"
#include "inspectiondialog.h"
#include "inspectionloopdialog.h"
#include "inspectionloopreport.h"
#include "insulationdialog.h"
#include "insulationreport.h"
#include "ldtxmlimportdialog.h"
#include "lineeditdelegate.h"
#include "mainwindelegate.h"
#include "jobmainwindow.h"
#include "paintdialog.h"
#include "paintreport.h"
#include "peddialog.h"
#include "pipeclassdialog.h"
#include "pipeclassreport.h"
#include "progressdialog.h"
#include "projectdialog.h"
#include "projectselectiondialog.h"
#include "tablereportdialog.h"
#include "tieindialog.h"
#include "tieinreport.h"
#include "ldt_actiondbexportxml.h"
#include "ldt_actiondbimportxml.h"
#include "ldt_filterxml.h"
#include "ldt_pccxml.h"
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_system.h"

const QString rsrcPath = ":/images";


/**
 *  Constructs a mainWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	RB_DEBUG->print("MainWindow::MainWindow()");
	
	modelLine = NULL;
	modelInspectionItem = NULL;    
	selectionModel = NULL;
	modelLineImport = NULL;
	modelLineExport = NULL;
	findDialog = NULL;
    
    fluidReport = NULL;
    inspectionDataReport = NULL;
    inspectionLoopReport = NULL;
    insulationReport = NULL;
    jobMainWindow = NULL;
    paintReport = NULL;
    pipeClassReport = NULL;
    tieInReport = NULL;

	// TODO: debug only
	spy = NULL;

    // implement MainWindow widget
    setupUi(this);

    createFileActions();
    createEditActions();
    createViewActions();
    createLineActions();
    createSpecificationActions();
//    createToolBarActions();
	createJobActions();
	createDatabaseActions();
	createReportActions();
    createHelpActions();
    createModel();
    createConnections();
    
    formatLineFields();
    
    initStatusBar();
    readSettings();
    
   	setWindowTitle(tr("LDT - Biluna Line Designation Table"));
    setWindowIcon(QIcon(rsrcPath + "/biluna16.png"));
    
    LDT_Project* activeProject = (LDT_Project*)modelLine->getRoot();
	setProjectFields(*activeProject);
	
	textEditRemProcessChanged = false;
	textEditRemEnginDesChanged = false;
	textEditRemClassificationChanged = false;
	textEditRemOperMaintChanged = false;
	textEditRemCustomChanged = false;

	// yellow
    // revPalette.setColor(QPalette::Active, QPalette::Base, QColor(255, 255, 153));
    // lightblue CCFFFF
    revPalette.setColor(QPalette::Active, QPalette::Base, QColor(204, 255, 255));
    
    // connect assistant (help documentation) TODO: create RB_System
	RB_String appPath = QCoreApplication::applicationDirPath();
	assistantClient = new QAssistantClient(appPath + "/bin", this);
    
    QStringList arguments;
    arguments << "-profile" << appPath + QDir::separator() 
    									+ QString("doc") + QDir::separator() 
    									+ QString("help") + QDir::separator() 
    									+ QString("ldt_help.adp");
    assistantClient->setArguments(arguments);
}


/*
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow() {
	RB_DEBUG->print("MainWindow::~MainWindow()");
    // no need to delete child widgets, Qt does it all for us
    delete modelInspectionItem;
    delete modelLine;
    
    if (assistantClient)
        assistantClient->closeAssistant();
        
	LDT_OBJECTFACTORY->deleteInstance();
        
    // test possible memory leakage1
    RB_DEBUG->printObjectCreated();
    RB_DEBUG->printObjectDeleted();
    RB_DEBUG->printMemberCreated();
    RB_DEBUG->printMemberDeleted();
    RB_DEBUG->printObjectList();
    
	RB_DEBUG->print("MainWindow::~MainWindow() OK");
}


void MainWindow::createFileActions() {

    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("File Actions"));
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(menu);

    QAction *a;

    fileNewAction = a = new QAction(QIcon(rsrcPath + "/filenew.png"), tr("&New..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/fileprojectedit.png"), tr("Project &Edit..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(a, SIGNAL(triggered()), this, SLOT(fileProjectEdit()));
    tb->addAction(a);
    menu->addAction(a);

    fileOpenAction = a = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("&Open..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    fileSaveAction = a = new QAction(QIcon(rsrcPath + "/filesave.png"), tr("&Save..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSave()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    fileSaveAsAction = a = new QAction(tr("Save &As..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    
    menu->addSeparator();
/*
    a = new QAction(QIcon(rsrcPath + "/fileprint.png"), tr("&Print..."), this);
    a->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(tr("&Close"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_W);
    connect(a, SIGNAL(triggered()), this, SLOT(fileClose()));
    menu->addAction(a);
*/
    QMenu* importSubMenu = new QMenu(tr("I&mport"), this);
	menu->addMenu(importSubMenu);

    fileImportLdtAction = a = new QAction(tr("Import &LDT"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileImportLdt()));
    a->setEnabled(false);
    importSubMenu->addAction(a);
	
    fileImportPccAction = a = new QAction(tr("Import &PCC"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileImportPcc()));
    a->setEnabled(true);
    importSubMenu->addAction(a);
	
    QMenu* exportSubMenu = new QMenu(tr("Ex&port"), this);
	menu->addMenu(exportSubMenu);

    fileExportLdtAction = a = new QAction(tr("Export &LDT"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(fileExportLdt()));
    a->setEnabled(false);
    exportSubMenu->addAction(a);
	
    a = new QAction(tr("E&xit"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    // connect(a, SIGNAL(triggered()), this, SLOT(fileExit()));
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(a);
}
    
    
void MainWindow::createEditActions() {

	QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Edit Actions"));
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(menu);

    QAction *a;
/*    a = editUndoAction = new QAction(QIcon(rsrcPath + "/editundo.png"), tr("&Undo"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Z);
//    connect(a, SIGNAL(triggered()), this, SLOT(editUndo()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    a = editRedoAction = new QAction(QIcon(rsrcPath + "/editredo.png"), tr("&Redo"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Y);
//    connect(a, SIGNAL(triggered()), this, SLOT(editRedo()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
*/
    a = editCutAction = new QAction(QIcon(rsrcPath + "/editcut.png"), tr("Cu&t"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_X);
    connect(a, SIGNAL(triggered()), this, SLOT(editCut()));
    tb->addAction(a);
    menu->addAction(a);

    a = editCopyAction = new QAction(QIcon(rsrcPath + "/editcopy.png"), tr("&Copy"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(a, SIGNAL(triggered()), this, SLOT(editCopy()));
    tb->addAction(a);
    menu->addAction(a);

    a = editPasteAction = new QAction(QIcon(rsrcPath + "/editpaste.png"), tr("&Paste"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_V);
    connect(a, SIGNAL(triggered()), this, SLOT(editPaste()));
    a->setEnabled(!QApplication::clipboard()->text(QClipboard::Clipboard).isEmpty());
    tb->addAction(a);
    menu->addAction(a);
    
    editPasteAction->setEnabled(!QApplication::clipboard()->text().isEmpty());
    
    menu->addSeparator();

    a = editFindAction = new QAction(QIcon(rsrcPath + "/editfind.png"), tr("&Find"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_F);
    connect(a, SIGNAL(triggered()), this, SLOT(editFind()));
    tb->addAction(a);
    menu->addAction(a);

    a = editFilterOnAction = new QAction(QIcon(rsrcPath + "/editfilteron.png"), tr("Filter On"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(editFilterOn()));
    tb->addAction(a);
    menu->addAction(a);

    a = editFilterOffAction = new QAction(QIcon(rsrcPath + "/editfilteroff.png"), tr("Filter Off"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(editFilterOff()));
    a->setEnabled(false);
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    a = editCommitChangesAction = new QAction(tr("Commit C&hanges"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(editCommitChanges()));
    menu->addAction(a);
}

void MainWindow::createViewActions() {
    QMenu *menu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = viewProcessAction = new QAction(tr("&Process Lines"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a, SIGNAL(triggered()), this, SLOT(viewProcess()));
    menu->addAction(a);

    a = viewUtilityAction = new QAction(tr("&Utility Lines"), this);
    a->setCheckable(true);
    a->setChecked(true);
    connect(a, SIGNAL(triggered()), this, SLOT(viewUtility()));
    menu->addAction(a);

    menu->addSeparator();

    a = viewChangesAction = new QAction(tr("Sho&w Changes"), this);
    a->setCheckable(true);
    a->setChecked(false);
    connect(a, SIGNAL(triggered()), this, SLOT(viewChanges()));
    menu->addAction(a);
}


void MainWindow::createLineActions() {

	QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Line Actions"));
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("&Line"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = lineNewAction = new QAction(QIcon(rsrcPath + "/componentnew.png"), tr("New &Line"), this);
    // a->setShortcut(Qt::CTRL + Qt::Key_Z);
    connect(a, SIGNAL(triggered()), this, SLOT(lineNew()));
    tb->addAction(a);
    menu->addAction(a);

    a = lineDeleteAction = new QAction(QIcon(rsrcPath + "/componentdelete.png"), tr("&Delete Line"), this);
    // a->setShortcut(Qt::CTRL + Qt::Key_Y);
    connect(a, SIGNAL(triggered()), this, SLOT(lineDelete()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();

    a = lineExecuteAction = new QAction(QIcon(rsrcPath + "/componentexecute.png"), tr("Exec&ute"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_U);
    connect(a, SIGNAL(triggered()), this, SLOT(lineExecute()));
    tb->addAction(a);
	menu->addAction(a);

    a = new QAction(tr("Classificati&on"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(lineClassification()));
    menu->addAction(a);

    a = new QAction(tr("&Tie-in"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(lineTieIn()));
    menu->addAction(a);
}


void MainWindow::createSpecificationActions() {

    QMenu *menu = new QMenu(tr("&Specification"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = new QAction(tr("&Fluids"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationFluid()));
    menu->addAction(a);

    a = new QAction(tr("&Insulation"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationInsulation()));
    menu->addAction(a);

    a = new QAction(tr("P&ainting"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationPainting()));
    menu->addAction(a);

    a = new QAction(tr("&Pipe Class"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationPipeClass()));
    menu->addAction(a);

//    a = new QAction(tr("Classificati&on"), this);
//    connect(a, SIGNAL(triggered()), this, SLOT(specificationClassification()));
//    menu->addAction(a);

    menu->addSeparator();

    a = new QAction(tr("Inspection &Loop"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationInspectionLoop()));
	menu->addAction(a);

    a = new QAction(tr("&Tie-in List"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(specificationTieIn()));
    menu->addAction(a);
}

	
void MainWindow::createJobActions() {
    QMenu *menu = new QMenu(tr("&Job"), this);
    menuBar()->addMenu(menu);

    QAction *a;
//    a = jobWorkAction = new QAction(tr("Work"), this);
//    connect(a, SIGNAL(triggered()), this, SLOT(jobWork()));
//    menu->addAction(a);

//    menu->addSeparator();
    
//    a = jobResourceAction = new QAction(tr("Resource"), this);
//    connect(a, SIGNAL(triggered()), this, SLOT(jobResource()));
//    menu->addAction(a);

    a = jobTaskAction = new QAction(tr("Task"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(jobTask()));
    menu->addAction(a);

//    a = jobResourceUseAction = new QAction(tr("Resource Use"), this);
//    connect(a, SIGNAL(triggered()), this, SLOT(databaseResourceUse()));
//    a->setEnabled(false);
//    menu->addAction(a);
}


void MainWindow::createDatabaseActions() {
    QMenu *menu = new QMenu(tr("&Database"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = dbConnectAction = new QAction(tr("Connect"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseConnect()));
    menu->addAction(a);

    menu->addSeparator();
    
    a = dbSelectProjectAction = new QAction(tr("Select Project"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseSelectProject()));
    a->setEnabled(false);
    menu->addAction(a);
    
    QMenu* adminSubMenu = new QMenu(tr("&Admin"), this);
	menu->addMenu(adminSubMenu);

    a = dbCreateAction = new QAction(tr("Create Tables"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseCreateTables()));
    a->setEnabled(true); // todo: after testing set to false
    adminSubMenu->addAction(a);

//    a = dbCheckAction = new QAction(tr("Check Tables"), this);
//    connect(a, SIGNAL(triggered()), this, SLOT(databaseCheck()));
//    a->setEnabled(true); // todo: after testing set to false
//    menu->addAction(a);

    a = dbSettingsAction = new QAction(tr("Connection Settings"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseSettings()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbBrowseAction = new QAction(tr("Browse Database"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseBrowse()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbPermissionAction = new QAction(tr("Database Permission"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databasePermission()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbImportCsvAction = new QAction(tr("Import CSV file"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseImportCsv()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbImportXmlAction = new QAction(tr("Import XML file"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseImportXml()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbExportXmlAction = new QAction(tr("Export XML file"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseExportXml()));
    a->setEnabled(false);
    adminSubMenu->addAction(a);

    a = dbDisconnectAction = new QAction(tr("Disconnect"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(databaseDisconnect()));
    a->setEnabled(false);
    menu->addAction(a);
}


void MainWindow::createReportActions() {

//	QToolBar *tb = new QToolBar(this);
//    tb->setWindowTitle(tr("Report Actions"));
//    addToolBar(tb);

    QMenu *menu = new QMenu(tr("&Report"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = /*reportTableAction =*/ new QAction(tr("&Line Designation Table"), this);
    // a->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(a, SIGNAL(triggered()), this, SLOT(reportTable()));
    // tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
    
    a = new QAction(tr("&Fluid definition list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportFluid()));
    menu->addAction(a);
	
    a = new QAction(tr("In&sulation list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportInsulation()));
    menu->addAction(a);
	
    a = new QAction(tr("P&aint System list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportPaint()));
    menu->addAction(a);
	
    a = new QAction(tr("&Pipe Class list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportPipeClass()));
    menu->addAction(a);
	
    menu->addSeparator();
    
    a = new QAction(tr("&Inspection loop list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportInspectionLoop()));
    menu->addAction(a);
	
    a = new QAction(tr("Inspection &Data Report"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportInspectionData()));
    menu->addAction(a);
	
    a = new QAction(tr("&Tie-in list"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(reportTieIn()));
    menu->addAction(a);
}

void MainWindow::createHelpActions() {

    QMenu *menu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = new QAction(tr("Index"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(helpIndex()));
    menu->addAction(a);
	
    a = new QAction(tr("Contents"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(helpContents()));
    menu->addAction(a);

    a = new QAction(tr("About"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(helpAbout()));
    menu->addAction(a);

	menu->addSeparator();
	
	// TODO: follow is for debugging only
    QMenu* debugSubMenu = new QMenu(tr("&Debug"), this);
	menu->addMenu(debugSubMenu);

    a = new QAction(tr("Spy signal/slot"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(helpDebugSpy()));
    debugSubMenu->addAction(a);

    a = new QAction(tr("Export model"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(helpDebugModel()));
    debugSubMenu->addAction(a);
}


void MainWindow::createModel() {
	RB_DEBUG->print("MainWindow::createModel()");
	
	// test only
//	databaseConnect();
//	return;
	
	// creates only the root object LDT_Project, others are created as required
	modelLine = LDT_MmLine::getInstance();
	modelLine->setRoot();

	MainWinDelegate* delegate = new MainWinDelegate;
    tableView->setItemDelegate(delegate);

	tableView->setModel(modelLine);
	tableView->setAlternatingRowColors(true);
	tableView->setEditTriggers(QAbstractItemView::EditKeyPressed |
								QAbstractItemView::DoubleClicked);
	tableView->horizontalHeader()->setClickable(true);

	selectionModel = tableView->selectionModel();
//    connect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
//    		this, SLOT(changeCurrent(const QModelIndex&, const QModelIndex&)));	
//	connect(modelLine, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
//			this, SLOT(modelDataChanged(const QModelIndex&, const QModelIndex&)));
//	
//    // Used in findDialog
//    connect(modelLine, SIGNAL(clearSelection()), tableView, SLOT(clearSelection()));
//	connect(modelLine, SIGNAL(selectTableRow(int)), this, SLOT(selectTableRow(int)));

// remove	
//	modelLine->setSelectionModel(selectionModel);

	formatTableView();	
    updateMenus();

	/**
	 * LDT_MmInspectionItem is child model of LDT_MmLine. Note:
	 * - modelInspectionItem->setModel(modelLine) with changeCurrent()
	 * - tvInspectionItem->setModel(modelInspectionItem) with changeCurrent()
	 */
	modelInspectionItem = LDT_MmInspectionItem::getInstance(modelLine);
	RB_ObjectContainer* objC = (RB_ObjectContainer*)modelLine->getCurrentObject();
	modelInspectionItem->setModel(objC, "LDT_InspectionItemList", modelLine);
	
	LineEditDelegate* leDelegate = new LineEditDelegate;
    tvInspectionItem->setItemDelegate(leDelegate);
	
	tvInspectionItem->setAlternatingRowColors(true);
	tvInspectionItem->setEditTriggers(QAbstractItemView::EditKeyPressed |
							QAbstractItemView::DoubleClicked);
	tvInspectionItem->setSelectionBehavior(QAbstractItemView::SelectItems);
	
	tvInspectionItem->setModel(modelInspectionItem);
	selectionInspectionModel = tvInspectionItem->selectionModel();
	
	formatTvInspectionItem();
}

/**
 * Create additional user interface connections
 */
void MainWindow::createConnections() {
	RB_DEBUG->print("MainWindow::createConnections()");
	
	// signal from model change to line edits (lineEdits signal individually)
	connect(modelLine, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(modelDataChanged(const QModelIndex&, const QModelIndex&)));
	
	// signal from selection model that current row or selection changed
    connect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		this, SLOT(changeCurrent(const QModelIndex&, const QModelIndex&)));	
    connect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelLine, SLOT(changeCurrentRow(const QModelIndex&, const QModelIndex&)));	
    connect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelInspectionItem, SLOT(parentCurrentRowChanged(const QModelIndex&, const QModelIndex&)));	
    connect(selectionInspectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelInspectionItem, SLOT(changeCurrentRow(const QModelIndex&, const QModelIndex&)));	
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
    		modelLine, SLOT(changeSelection(const QItemSelection&, const QItemSelection&)));
    
    // Used in findDialog
    connect(modelLine, SIGNAL(clearSelection()), tableView, SLOT(clearSelection()));
	connect(modelLine, SIGNAL(selectTableRow(int)), this, SLOT(selectTableRow(int)));

	// set column sort
	connect(tableView->horizontalHeader(), SIGNAL(sectionClicked(int)),
			tableView, SLOT(sortByColumn(int)));
	connect(tvInspectionItem->horizontalHeader(), SIGNAL(sectionClicked(int)),
			tvInspectionItem, SLOT(sortByColumn(int)));
			
	// set column width of tableViews
	connect(tableView->horizontalHeader(), SIGNAL(sectionResized(int, int, int)),
			this, SLOT(setColumnTableView(int, int, int)));
	connect(tvInspectionItem->horizontalHeader(), SIGNAL(sectionResized(int, int, int)),
			this, SLOT(setColumnTvInspectionItem(int, int, int)));
	
	if (findDialog) {
	    connect(findDialog, SIGNAL(findNext(int, const QString&, bool)),
	            modelLine, SLOT(findNext(int, const QString&, bool)));
	    connect(findDialog, SIGNAL(findPrevious(int, const QString&, bool)),
	            modelLine, SLOT(findPrevious(int, const QString&, bool)));
	}	
}

void MainWindow::removeConnections() {
	// signal from model change to line edits (lineEdits signal individually)
	disconnect(modelLine, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(modelDataChanged(const QModelIndex&, const QModelIndex&)));
	
	// signal from selection model that current row changed
    disconnect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		this, SLOT(changeCurrent(const QModelIndex&, const QModelIndex&)));	
    disconnect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelLine, SLOT(changeCurrentRow(const QModelIndex&, const QModelIndex&)));	
    disconnect(selectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelInspectionItem, SLOT(parentCurrentRowChanged(const QModelIndex&, const QModelIndex&)));	
    disconnect(selectionInspectionModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), 
    		modelInspectionItem, SLOT(changeCurrentRow(const QModelIndex&, const QModelIndex&)));	

    // Used in findDialog
    disconnect(modelLine, SIGNAL(clearSelection()), tableView, SLOT(clearSelection()));
	disconnect(modelLine, SIGNAL(selectTableRow(int)), this, SLOT(selectTableRow(int)));

	// set column sort
	disconnect(tableView->horizontalHeader(), SIGNAL(sectionClicked(int)),
			tableView, SLOT(sortByColumn(int)));
	disconnect(tvInspectionItem->horizontalHeader(), SIGNAL(sectionClicked(int)),
			tvInspectionItem, SLOT(sortByColumn(int)));
			
	// set column width of tableViews
	disconnect(tableView->horizontalHeader(), SIGNAL(sectionResized(int, int, int)),
			this, SLOT(setColumnTableView(int, int, int)));
	disconnect(tvInspectionItem->horizontalHeader(), SIGNAL(sectionResized(int, int, int)),
			this, SLOT(setColumnTvInspectionItem(int, int, int)));
	
	if (findDialog) {
	    disconnect(findDialog, SIGNAL(findNext(int, const QString&, bool)),
	            modelLine, SLOT(findNext(int, const QString&, bool)));
	    disconnect(findDialog, SIGNAL(findPrevious(int, const QString&, bool)),
	            modelLine, SLOT(findPrevious(int, const QString&, bool)));
	}	
}

void MainWindow::formatTableView() {
	// RB_DEBUG->print("MainWindow::formatTableView()");
	
	if (modelLine) {
		int columnCount = modelLine->columnCount();

		for (int i = 0; i < columnCount; i++) {
			if (i != 3 && i != 4 && i != 5 && i != 7 && i != 10 && i != 13 
											&& i != 14 && i != 15 && i != 16 ) {
				tableView->setColumnHidden(i, true);
			}
		}
	}
	
	// set sort indicator, is also used for determining the sort order
	tableView->horizontalHeader()->setSortIndicatorShown(true);

	RB_SETTINGS->beginGroup("mainwindow/tableview");
	tableView->setColumnWidth(3, RB_SETTINGS->value("col3", 100).toInt());
	tableView->setColumnWidth(4, RB_SETTINGS->value("col4", 40).toInt());
	tableView->setColumnWidth(5, RB_SETTINGS->value("col5", 40).toInt());
	tableView->setColumnWidth(7, RB_SETTINGS->value("col7", 75).toInt());
	tableView->setColumnWidth(10, RB_SETTINGS->value("col10", 75).toInt());
	tableView->setColumnWidth(13, RB_SETTINGS->value("col13", 75).toInt());
	tableView->setColumnWidth(14, RB_SETTINGS->value("col14", 75).toInt());
	tableView->setColumnWidth(15, RB_SETTINGS->value("col15", 45).toInt());
	tableView->setColumnWidth(16, RB_SETTINGS->value("col16", 45).toInt());
	RB_SETTINGS->endGroup();
}

/**
 * Format the tableView tvInspectionItem, hides relevant columns and sets the 
 * column width as last changed by the user
 */
void MainWindow::formatTvInspectionItem() {
	// RB_DEBUG->print("MainWindow::formatTvInspectionItem");
	
	if (modelInspectionItem) {
		int columnCount = modelInspectionItem->columnCount();
	
	    for (int i = 0; i < columnCount; i++) {
			if (i != 3 && i != 4) {
				tvInspectionItem->setColumnHidden(i, true);
			}
		}
	}
	
	// set sort indicator, is also used for determining the sort order
	tvInspectionItem->horizontalHeader()->setSortIndicatorShown(true);

	RB_SETTINGS->beginGroup("mainwindow/tvInspectionItem");
	tvInspectionItem->setColumnWidth(3, RB_SETTINGS->value("col3", 40).toInt());
	tvInspectionItem->setColumnWidth(4, RB_SETTINGS->value("col4", 95).toInt());
	RB_SETTINGS->endGroup();
}


/**
 * Slot receives signal when column of the main tableView is resized
 * @param column column number, also called logicalIndex in Qt
 * @param oldSize column width before resize, not used
 * @papam newSize column width after resize
 */
void MainWindow::setColumnTableView(int column, int /* oldSize */, int newSize) {
	RB_String colStr = "col";
	colStr += RB_String::number(column);
	
	RB_SETTINGS->beginGroup("mainWindow/tableview");
	RB_SETTINGS->setValue(colStr, newSize);
	RB_SETTINGS->endGroup();
}

/**
 * Slot receives signal when column of the inspection tableView is resized
 * @param column column number, also called logicalIndex in Qt
 * @param oldSize column width before resize, not used
 * @papam newSize column width after resize
 */
void MainWindow::setColumnTvInspectionItem(int column, int /* oldSize */, int newSize) {
	RB_String colStr = "col";
	colStr += RB_String::number(column);
	
	RB_SETTINGS->beginGroup("mainWindow/tvInspectionItem");
	RB_SETTINGS->setValue(colStr, newSize);
	RB_SETTINGS->endGroup();
}


/**
 * Initializes the status bar at the bottom.
 */
void MainWindow::initStatusBar() {
    // RB_DEBUG->print("MainWindow::initStatusBar()");

    statusBar()->showMessage("Line Designation Table - Ready", 4000);
}


//void MainWindow::resizeEvent(QResizeEvent* /* event */) {
//	int extraWidth = width() - minimumWidth();
//	int extraHeight = height() - minimumHeight();
//	
//    stackedWidget->setGeometry(QRect(10, 10, 961 + extraWidth, 641 + extraHeight));
//    tableView->setGeometry(QRect(390, 10, 550 + extraWidth, 620 - 30 + extraHeight));	
//}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainWindow::languageChange() {
    setWindowTitle(tr("Line Designation Table"));
}


void MainWindow::fileNew() {
	RB_DEBUG->print("MainWindow::fileNew()");
    
    // current model data to be saved?
    if (!maybeSave()) {
    	return;
    }
    
    // clear all lineEdits, set checkboxes and comboboxes to default
    clearProjectFields();
    clearLineFields();
    currentFileName.clear();
    
	// disconnect signals
	removeConnections();
	
    // deleted existing model and create new root (LDT_Project)
    modelLine->setRoot();
    modelLine->modelIsModified(false);
	tableView->setModel(modelLine);
	
	selectionModel = tableView->selectionModel();
    
	createConnections();

//	modelLine->setSelectionModel(selectionModel);

    // modal insulation dialog, with new and show() it would be non modal
	ProjectDialog dialog(this);
	dialog.exec();
	// LDT_Project* activeProject = (LDT_Project*)modelLine->getRoot();
	setProjectFields(*modelLine->getRoot());
}


void MainWindow::fileOpen() {
	RB_DEBUG->print("MainWindow::fileOpen()");

    if (maybeSave()) {
 		RB_SETTINGS->beginGroup("paths");
	    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
		RB_SETTINGS->endGroup();
    	
        QString fn = QFileDialog::getOpenFileName (this, 
        		"LDT - open file", 
		        openPath); 
//		        const QString & filter = QString(), 
//		        QString * selectedFilter = 0, 
//		        Options options = 0)

		// clear all lineEdits, listView and set Combo boxes to default
		clearProjectFields();
		clearLineFields();

		// load file and set model
		if (!fn.isEmpty()) {
			modelLineImport = modelLine;
            loadFile(fn, "LDT_XML");
			modelLineImport = NULL;
			
			editFilterOff();

		    setCurrentFileName(fn);
		    RB_String openPath = QFileInfo(fn).absolutePath();
			RB_SETTINGS->beginGroup("paths");
		    RB_SETTINGS->setValue("openPath", openPath);
			RB_SETTINGS->endGroup();
        } else {
        	return;	
		}

		setProjectFields(*(LDT_Project*)modelLine->getRoot());
    }
}


void MainWindow::fileProjectEdit() {
	RB_DEBUG->print("MainWindow::fileProjectEdit()");

    ProjectDialog dialog(this);
	dialog.exec();
	LDT_Project* activeProject = (LDT_Project*)modelLine->getRoot();
	setProjectFields(*activeProject);
	
    updateMenus();
}


bool MainWindow::fileSave() {
	RB_DEBUG->print("MainWindow::fileSave()");

	// save possible changes of textEdits
	if (tableView->currentIndex().isValid()) {
		int row = tableView->currentIndex().row();
		saveTextEditRemProcess(row);
		saveTextEditRemEnginDes(row);
		saveTextEditRemClassification(row);
		saveTextEditRemOperMaint(row);
		saveTextEditRemCustom(row);
	}

    if (currentFileName.isEmpty()) {
        return fileSaveAs();
    } else {
    	modelLineExport = modelLine;
    	bool fileIsSaved = saveFile(currentFileName);
    	modelLineExport = NULL;
        return fileIsSaved;
    }
}


bool MainWindow::fileSaveAs() {
	RB_DEBUG->print("MainWindow::fileSaveAs()");

	RB_SETTINGS->beginGroup("paths");
    RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
	RB_SETTINGS->endGroup();
    	
    QFileDialog dialog(this,"LDT - save file as", savePath);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    
    if (dialog.exec() == QDialog::Accepted) {
    	RB_StringList fileNames = dialog.selectedFiles();
		currentFileName = fileNames.at(0);

//    	currentFileName = QFileDialog::getSaveFileName(this, 
//       										"LDT - save file as", 
//	        								savePath); 
//	        								const QString & filter = QString(), 
//	        								QString * selectedFilter = 0, 
//	        								Options options = 0 )
    } else {
    	return false;
    }

    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, tr("LDT Save As ..."),
                             tr("File not saved,\nfile name was empty."));
        return false;
    }

	modelLineExport = modelLine;
	bool fileIsSaved = saveFile(currentFileName);
	modelLineExport = NULL;
    return fileIsSaved;
}


bool MainWindow::maybeSave() {
	RB_DEBUG->print("MainWindow::maybeSave()");
	
    if (modelLine->isModelModified() && !modelLine->database().isOpen()) {
        int ret = QMessageBox::warning(this, tr("Line Designation Table"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
		if (ret == QMessageBox::Yes) {
            if (fileSave()) {
            	updateMenus();
            	return true;
            }
			return false;
		} else if (ret == QMessageBox::Cancel) {
            return false;
		}
    }
    return true;
}


//void MainWindow::filePrint() {
//	
//}

/**
 * Import line and optional project data in the database from a XML file 
 * generated by LDT. This function is only active when connected to a database.
 */
void MainWindow::fileImportLdt() {
    RB_DEBUG->print("MainWindow::fileImportLdt()");
    
	RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
	RB_SETTINGS->endGroup();
    	
    QString fn = QFileDialog::getOpenFileName(this, "Import LDT XML file", 
    											openPath); 
	if (fn.isEmpty()) {
        QMessageBox::warning(this, tr("LDT Import"),
                             tr("File not imported,\nfile name was empty."));
        return;
	}

    LdtXmlImportDialog dialog(this);
	dialog.exec();

	// create temporary import model
	if (modelLineImport) delete modelLineImport;
	modelLineImport	= LDT_MmLine::getInstance();
	modelLineImport->setObjectFactory(LDT_OBJECTFACTORY);

	// import LDT file in separate in memory model
    loadFile(fn, "LDT_XML");

	// create action
	LDT_ActionDbImportXml* action = new LDT_ActionDbImportXml();
	RB_String* result = new RB_String();
	
	// execute import action
	action->execute(modelLine, modelLineImport, dialog.actionSelected, result);

    QMessageBox::information(this, tr("LDT import"), *result);

	if (modelLineImport) {
		delete modelLineImport;
		modelLineImport = NULL;
	}
	
	if (action) delete action;
	if (result) delete result;

//	updateMenus();
}

/**
 * Import LDT relevant data from a XML file generated by PCC
 */
void MainWindow::fileImportPcc() {
    RB_DEBUG->print("MainWindow::fileImportPcc()");
    
	RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
	RB_SETTINGS->endGroup();
    	
    QString fn = QFileDialog::getOpenFileName (this, "LDT - import PCC file", 
	        									openPath); 
	// import PCC file and create model
	if (!fn.isEmpty()) {
		modelLineImport = modelLine;
        loadFile(fn, "PCC_XML");
		modelLineImport = NULL;
    } else {
       	return;	
	}

    PipeClassDialog dialog(this);
	dialog.exec();
	updateMenus();
}

/**
 * Export a XML file from the database with project and line data only.
 * This function is only active when connected to a database.
 */
void MainWindow::fileExportLdt() {
    RB_DEBUG->print("MainWindow::fileExportPcc()");

	RB_SETTINGS->beginGroup("paths");
    RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
	RB_SETTINGS->endGroup();
    	
    RB_String fileName = QFileDialog::getSaveFileName (this, 
   												"LDT - save file as", savePath); 
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("LDT Export"),
                             tr("File not exported,\nfile name was empty."));
        return;
    }
    
    // commit all changes first
    editCommitChanges();

	// create temporary export model
	modelLineExport	= LDT_MmLine::getInstance();
	modelLineExport->setObjectFactory(LDT_OBJECTFACTORY);

	// create action
	LDT_ActionDbExportXml* action = new LDT_ActionDbExportXml();
	RB_String* result = new RB_String();
	
	// execute export action
	action->execute(modelLine, modelLineExport, RB2::ActionLdtFileExportLdt, result);

	// export LDT file to disk
    if (saveFile(fileName)) {
	    QMessageBox::information(this, tr("LDT export"), *result);
    } else {
	    QMessageBox::warning(this, tr("LDT export"), 
	    					tr("File not exported,\nfile writing error."));
    }

	if (modelLineExport) {
		delete modelLineExport;
		modelLineExport = NULL;
	}
	
	if (action) delete action;
	if (result) delete result;

//	updateMenus();
}


/**
 * Load XML file in model
 * 
 * @fileName name of XML file to import
 * @fileType type of file, for example LDT, PCC
 * @import file is a LDT XML file to be imported in the database
 */
void MainWindow::loadFile(const QString &fileName, const QString& fileType) {
	RB_DEBUG->print("MainWindow::loadFile() type = %s", 
					fileType.toStdString().c_str());
	
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("LDT"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    RB_String openPath = "";
	
	if (fileType == "LDT_XML") {
		LDT_FilterXml filter;
		
		bool success = false;
		success = filter.fileImport(modelLineImport, fileName);
			
		if (!success) {
	        QApplication::restoreOverrideCursor();

	        // error do nothing except message and reset model
	        QMessageBox::information(this, ("Error"),
	                     ("Error\nCould not open the file\n%1\nPlease "
	                      "check the parse message below.\n"
	                      + filter.getParseMessage())
	                      .arg(fileName),
	                      QMessageBox::Ok);
	        
	        modelLineImport->setRoot();
	        modelLineImport->reset();
			setProjectFields(*modelLine->getRoot());

			return;
	  	}
	
	    openPath = QFileInfo(fileName).absolutePath();
		RB_SETTINGS->beginGroup("paths");
	    RB_SETTINGS->setValue("openPath", openPath);
		RB_SETTINGS->endGroup();

	} else if (fileType == "PCC_XML") {
		LDT_PccXml filter;	
//		QFileInfo fi(fileName);
				
		// set progress dialog if file > 1 MB
//		ProgressDialog* progress = NULL;
//		
//		if (fi.size() > qint64(1000000)) {
//			progress = new ProgressDialog(this);
//			progress->setWindowTitle(tr("LDT Progress"));
//			
//	    	QString str = "Import file: " + fi.fileName();
//			progress->setSubjectName(str);
//			progress->setModal(false);
//			progress->show();
//
//			for (int i = 0; i < 10000 ; i++) {
//				progress->setRow("Start reading ...");
//			}
//
//			qApp->processEvents();
//			filter.setProgressDialog(progress);
//		}
	
		// load file to model	
		if (!filter.fileImport(modelLineImport, fileName)) {
	        // error do nothing except message
	        QMessageBox::information(this, ("Error"),
	                     ("Error\nCould not open the file\n%1\nPlease "
	                      "check the parse message below.\n"
	                      + filter.getParseMessage())
	                      .arg(fileName),
	                      QMessageBox::Ok);
			QApplication::restoreOverrideCursor();
			return;
	  	}
		
//		if (fi.size() > qint64(1000000)) {
//			delete progress;
//			progress = NULL;
//		}	  	
	}

	// create tableView set filter off
	resetModel();
	// editFilterOff();
	
	QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("File loaded"), 2000);
}


bool MainWindow::saveFile(const QString &fileName) {
	RB_DEBUG->print("MainWindow::saveFile()");

    QFile file(fileName);
    if (!modelLineExport || !file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("LDT"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    LDT_FilterXml filter;
    
	if (!filter.fileExport(modelLineExport, fileName)) {
        // error: duplication of above?
        QMessageBox::information(this, QMessageBox::tr("Warning"),
        			tr("Cannot save the file\n%1\nPlease "
                    "check the permissions.")
                    .arg(fileName),
                    QMessageBox::Ok);
		return false;
    }
    
    QApplication::restoreOverrideCursor();

    RB_String savePath = QFileInfo(fileName).absolutePath();
	RB_SETTINGS->beginGroup("paths");
    RB_SETTINGS->setValue("savePath", savePath);
	RB_SETTINGS->endGroup();

    setCurrentFileName(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


void MainWindow::setCurrentFileName(const QString &fileName) {
	RB_DEBUG->print("MainWindow::setCurrentFileName()");
	
    currentFileName = fileName;
    modelLine->modelIsModified(false);
    updateMenus();

    if (currentFileName.isEmpty())
        setWindowTitle(tr("LDT"));
    else
        setWindowTitle(tr("%1 - %2[*]").arg(tr("LDT"))
        							.arg(strippedFileName(currentFileName)));
}


QString MainWindow::strippedFileName(const QString &fullFileName) {
	RB_DEBUG->print("MainWindow::strippedFileName()");
	
    return QFileInfo(fullFileName).fileName();
}


void MainWindow::readSettings() {
	// RB_DEBUG->print("MainWindow::readSettings()");
	
	RB_SETTINGS->beginGroup("mainWindow");
    QPoint pos = RB_SETTINGS->value("pos", QPoint(100, 100)).toPoint();
    QSize size = RB_SETTINGS->value("size", QSize(700, 757)).toSize();
	RB_SETTINGS->endGroup();
	move(pos);
	resize(size);
	
	RB_SETTINGS->beginGroup("mainWindow/tableView");
	tableView->setColumnWidth(3, RB_SETTINGS->value("col3", 100).toInt());
	tableView->setColumnWidth(4, RB_SETTINGS->value("col4", 40).toInt());
	tableView->setColumnWidth(5, RB_SETTINGS->value("col5", 40).toInt());
	tableView->setColumnWidth(7, RB_SETTINGS->value("col7", 75).toInt());
	tableView->setColumnWidth(10, RB_SETTINGS->value("col10", 75).toInt());
	tableView->setColumnWidth(13, RB_SETTINGS->value("col13", 75).toInt());
	tableView->setColumnWidth(14, RB_SETTINGS->value("col14", 75).toInt());
	tableView->setColumnWidth(15, RB_SETTINGS->value("col15", 45).toInt());
	tableView->setColumnWidth(16, RB_SETTINGS->value("col16", 45).toInt());
	RB_SETTINGS->endGroup();

	RB_SETTINGS->beginGroup("mainWindow/tvInspectionItem");
	tvInspectionItem->setColumnWidth(3, RB_SETTINGS->value("col3", 40).toInt());
	tvInspectionItem->setColumnWidth(4, RB_SETTINGS->value("col4", 95).toInt());
	RB_SETTINGS->endGroup();
}


void MainWindow::writeSettings() {
	// RB_DEBUG->print("MainWindow::writeSettings()");
	
   	RB_SETTINGS->beginGroup("mainWindow");
    RB_SETTINGS->setValue("pos", pos());
    RB_SETTINGS->setValue("size", size());
	RB_SETTINGS->endGroup();
}



void MainWindow::closeEvent(QCloseEvent* event) {
	RB_DEBUG->print("MainWindow::closeEvent()");
	
	if (maybeSave()) {
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}


void MainWindow::editUndo() {
	
}


void MainWindow::editRedo() {
	
}


void MainWindow::editCut() {
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(QApplication::focusWidget());
	if (lineEdit) {
		lineEdit->cut();
	}

	QTextEdit* textEdit = qobject_cast<QTextEdit*>(QApplication::focusWidget());
	if (textEdit) {
		textEdit->cut();
	}
}


void MainWindow::editCopy() {
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(QApplication::focusWidget());
	if (lineEdit) {
		lineEdit->copy();
	}

	QTextEdit* textEdit = qobject_cast<QTextEdit*>(QApplication::focusWidget());
	if (textEdit) {
		textEdit->copy();
	}
}


void MainWindow::editPaste() {
	RB_DEBUG->print("MainWindow::editPaste()");
	
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(QApplication::focusWidget());
	if (lineEdit) {
		lineEdit->paste();
	}

	QTextEdit* textEdit = qobject_cast<QTextEdit*>(QApplication::focusWidget());
	if (textEdit) {
		textEdit->paste();
	}
}


void MainWindow::editFind() {
	RB_DEBUG->print("MainWindow::editFind()");
	
	if (findDialog == NULL) {
		findDialog = new FindDialog(this);

	    connect(findDialog, SIGNAL(findNext(int, const QString&, bool)),
	            modelLine, SLOT(findNext(int, const QString&, bool)));
	    connect(findDialog, SIGNAL(findPrevious(int, const QString&, bool)),
	            modelLine, SLOT(findPrevious(int, const QString&, bool)));
	}

	findDialog->show();
	findDialog->raise();
}


void MainWindow::editFilterOn() {
	RB_DEBUG->print("MainWindow::editFilterOn()");
	
	// for database, otherwise changes remain in cache only and are lost
	editCommitChanges();
	
    FilterDialog dialog(this);
    int ret = dialog.exec();

	if (ret == QDialog::Accepted) {
		viewProcessAction->setChecked(true);
		viewUtilityAction->setChecked(true);

		modelLine->reset();
		formatTableView();

		// Hack: only required for the database connection
		if (modelLine->database().isOpen()) {
			tableView->setModel(modelLine);	
			formatTableView();
		}
		
		updateMenus();
	} else if (ret == QDialog::Rejected) {
		// do nothing
		statusBar()->showMessage("No records match the filter criteria - Ready", 4000);
		// resetModel(); // reset all
	}
}


void MainWindow::editFilterOff() {
	RB_DEBUG->print("MainWindow::editFilterOff()");

	// for database, otherwise changes remain in cache only and are lost
	editCommitChanges();
	
	// model reset
	viewProcessAction->setChecked(true);
	viewUtilityAction->setChecked(true);

	resetModel();
	updateMenus();
}


/**
 * This function allows the user to commit the changes to the database 
 * which might still be stored in the cache.
 */
void MainWindow::editCommitChanges() {
	RB_DEBUG->print("MainWindow::editCommitChanges()");
	
	// Redundant because function is not activated if not connected to db
	if(modelLine->database().isOpen()) {
		modelLine->submitAll();
	}
}


void MainWindow::clipboardDataChanged() {
	RB_DEBUG->print("MainWindow::clipboardDataChanged()");
    editPasteAction->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void MainWindow::viewProcess() {
	if (!viewProcessAction->isChecked() && !viewUtilityAction->isChecked()) {
		viewUtilityAction->setChecked(true);
	}
	
	modelLine->setProcessUtility(viewProcessAction->isChecked(), 
										viewUtilityAction->isChecked());

	// Hack: only required for the database connection
	if (modelLine->database().isOpen()) {
		tableView->setModel(modelLine);	
		formatTableView();
	}
	
	updateMenus();
}

void MainWindow::viewUtility() {
	if (!viewProcessAction->isChecked() && !viewUtilityAction->isChecked()) {
		viewProcessAction->setChecked(true);
	}
	
	modelLine->setProcessUtility(viewProcessAction->isChecked(), 
										viewUtilityAction->isChecked());	

	// Hack: only required for the database connection
	if (modelLine->database().isOpen()) {
		tableView->setModel(modelLine);	
		formatTableView();
	}
	
	updateMenus();
}

void MainWindow::viewChanges() {
	// Note: the check mark is changed first in viewChangesAction->isChecked()
	// and than this function is executed

	setLineFields();
	// refreshTableWidgetInspItem();

	// resetModel();
	// updateMenus();
}

void MainWindow::lineNew() {
	RB_DEBUG->print("MainWindow::lineNew()");

	// save possible changes of textEdits
	if (tableView->currentIndex().isValid()) {
		int row = tableView->currentIndex().row();
		saveTextEditRemProcess(row);
		saveTextEditRemEnginDes(row);
		saveTextEditRemClassification(row);
		saveTextEditRemOperMaint(row);
		saveTextEditRemCustom(row);
		// modelLine->submitAll();
	}

	// always insert at the end
	int row = modelLine->rowCount();
	
	// demo version only, maximum number of lines is 10
//	if (row > 9) {
//        QMessageBox::information(this, ("LDT demonstration version"),
//	                     ("This is a demonstration version of LDT,\n"
//	                      "the maximum number of lines has been limited.\n"),
//	                      QMessageBox::Ok);
//		return;
//	}
	
	modelLine->insertRows(row, 1, QModelIndex());
	tableView->setCurrentIndex(modelLine->index(row, 3, QModelIndex()));
	
	// with first row the table view format is lost
	if (modelLine->rowCount() < 2)
		formatTableView();

	updateMenus();
}


void MainWindow::lineDelete() {
	RB_DEBUG->print("MainWindow::lineDelete()");
	
    int ret = QMessageBox::warning(this, tr("Delete Action"),
					tr("You will NOT be able to undo this action.\n"
                    "Are you sure you want to delete the item?"),
                 	QMessageBox::Yes | QMessageBox::Default,
                 	QMessageBox::No | QMessageBox::Escape);
	if (ret == QMessageBox::No) {
		return;
	}

	// first clear lines otherwise the next line will become empty
	clearLineFields();
	
	if (tableView->currentIndex().isValid()) {
		modelLine->removeRows(tableView->currentIndex().row(), 1, QModelIndex());
	}
	
	// only to prevent crash, is 0 for in-memory model
	// if (modelLine->rowCount() == 0) {
		// tableView->setModel(modelLine);
	// }

	// for database second clear required
	clearLineFields();
	
	updateMenus();
}


void MainWindow::lineExecute() {
	RB_DEBUG->print("MainWindow::lineExecute()");
    
	ActionDialog dialog(this);
	dialog.exec();
	updateMenus();
}

/**
 * Calculate and review the details of the PED line classification
 */
void MainWindow::lineClassification() {
	RB_DEBUG->print("MainWindow::specificationClassification()");

	if (tableView->currentIndex().isValid()) {
	    PedDialog dialog(this, tableView->currentIndex());
		dialog.exec();
	} else {
        QMessageBox::information(this, ("Classification"),
	                     ("A line needs to be selected\n"
	                      "before a relevant classification can be done.\n"),
	                      QMessageBox::Ok);
		statusBar()->showMessage("No line selected - Ready", 2000);
	}
	
	updateMenus();
}


/**
 * View and edit tie-ins for the selected line
 */
void MainWindow::lineTieIn() {
	RB_DEBUG->print("MainWindow::lineTieIn()");
    
	if (tableView->currentIndex().isValid()) {
		
		bool showAllTieIns = false;
	
		TieInDialog dialog(this, showAllTieIns);
		dialog.exec();
		updateMenus();
	} else {
        QMessageBox::information(this, ("Tie-in"),
	                     ("A line needs to be selected\n"
	                      "before a relevant Tie-In can be edited.\n"),
	                      QMessageBox::Ok);
		statusBar()->showMessage("No line selected - Ready", 2000);
	}
}


/**
 * Open fluid (medium) specification dialog
 */
void MainWindow::specificationFluid() {
	RB_DEBUG->print("MainWindow::specificationFluid()");
    
	FluidDialog dialog(this);
	dialog.exec();
	updateMenus();
}


/**
 * Open insulation specification dialog
 * Open modal insulation dialog, with new and show() it would be non modal,
 * not cloned model, all data in dialog QTableWidget which acts as a clone
 */
void MainWindow::specificationInsulation() {
	RB_DEBUG->print("MainWindow::specificationInsulation()");
    
	InsulationDialog dialog(this);
	dialog.exec();
	updateMenus();
}

/**
 * Open (modal) paint dialog. 
 * The paint ModelViewControler is created in the dialog function createModel().
 * The cloned model is created at the dialog construction. The cloned model
 * is required to allow cancelation of the changes and closing the dialog
 */
void MainWindow::specificationPainting() {
	RB_DEBUG->print("MainWindow::specificationPainting()");
	
    PaintDialog dialog(this);
	dialog.exec();
	updateMenus();
}


void MainWindow::specificationPipeClass() {
	RB_DEBUG->print("MainWindow::specificationPipeClass()");

    PipeClassDialog dialog(this);
	dialog.exec();
	updateMenus();
}


void MainWindow::specificationInspectionLoop() {
	RB_DEBUG->print("MainWindow::lineInspectionLoop()");
    
    InspectionLoopDialog dialog(this);
	dialog.exec();
	updateMenus();
}


/**
 * View and edit tie-ins
 */
void MainWindow::specificationTieIn() {
	RB_DEBUG->print("MainWindow::specificationTieIn()");
    
	bool showAllTieIns = true;

	TieInDialog dialog(this, showAllTieIns);
	dialog.exec();
	updateMenus();
}


void MainWindow::jobTask() {
	RB_DEBUG->print("MainWindow::jobTask()");
	
	if (!jobMainWindow) {
		jobMainWindow = new JobMainWindow(this);
	} else {
		// jobMainWindow->refresh(this);	
	}

	int posX = qMax(0, (this->width() - jobMainWindow->width())/2);
	int posY = qMax(0, (this->height() - jobMainWindow->height())/2);
	
	// non modal, to be able to select the line from the LDT main window
	jobMainWindow->move(posX, posY);
	jobMainWindow->show();
	jobMainWindow->raise();
}


void MainWindow::databaseConnect() {
	RB_DEBUG->print("MainWindow::databaseConnect()");

	// current in memory model data to be saved?
    if (!maybeSave()) {
    	updateMenus();
    	return;
    }

    // clear all lineEdits, set checkboxes and comboboxes to default
    clearLineFields();
	tableView->setCurrentIndex(QModelIndex());
    

    // check for previous valid instance and database connection
    if (modelLine != NULL && modelLine->database().isOpen()) { // ==> TODO: db.open(), after connecting twice program crashes
        QMessageBox::information(this, tr("Already connected to database"),
             tr("Please disconnect from current database first \n"
             "before selecting another database connection."));
		return;
    }

	RB_SETTINGS->beginGroup("databaseConnection");
	RB_String driverName = RB_SETTINGS->value("driverName", "QODBC").toString();
	RB_String databaseName = RB_SETTINGS->value("databaseName", "biluna").toString();
	RB_String hostName = RB_SETTINGS->value("hostName", "localhost").toString();
	RB_String userName = RB_SETTINGS->value("userName", "user").toString();
	RB_String password = RB_SETTINGS->value("password", "password").toString();
	RB_String port = RB_SETTINGS->value("port", "Default").toString();
	RB_String loginUser = RB_SETTINGS->value("loginUser", "loginUser").toString();
	RB_String localDriverName = RB_SETTINGS->value("localDriverName", "QODBC").toString();
	RB_String localDatabaseName = RB_SETTINGS->value("localDatabaseName", "biluna").toString();
	RB_SETTINGS->endGroup();

	QSqlDatabase db;
    
    if (db.isOpen()) {
        QMessageBox::information(this, tr("Already connected to database"),
             tr("Please disconnect from this database first \n"
             "before selecting another database connection."));
		return;
    }

	DB_ConnectionDialog dlgDbConn(this, 
								RB2::driverNameToEnum(driverName), 
								databaseName, 
								userName, 
								password, 
								hostName, 
								port, 
								loginUser, 	// not the password
								RB2::driverNameToEnum(localDriverName),
								localDatabaseName);

    if (dlgDbConn.exec() != QDialog::Accepted) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    RB_String strDbFn = "";

	// create connection, 'dbConn' is a random name for this connection
    static int cCount = 0;
    RB_String connection = RB_String("dbConn%1").arg(++cCount);
    
    if (dlgDbConn.localDriverName() == RB2::DatabaseNone) {
        db = QSqlDatabase::addDatabase(
        					RB2::enumToDriverName(dlgDbConn.driverName()),
        					RB_String("connection"));
	    db.setDatabaseName(dlgDbConn.databaseName());
	    db.setHostName(dlgDbConn.hostName());
	    db.setUserName(dlgDbConn.userName());
	    db.setPassword(dlgDbConn.password());
	    db.setPort(dlgDbConn.port());
    } else if (dlgDbConn.localDriverName() == RB2::DatabaseAccess) {
        // check if we can write to that directory:
#ifndef Q_OS_WIN
    	RB_String path = QFileInfo(dlgDbConn.localDatabaseName()).absolutePath();
        if (QFileInfo(path).isWritable() == false) {
            RB_DEBUG->print("MainWindow::addConnection(): can't write file: "
                            "no permission");
            return;
        }
#endif

        db = QSqlDatabase::addDatabase("QODBC",
                                            QString("DbBrowserDialog"));
        strDbFn = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=";
        strDbFn += dlgDbConn.localDatabaseName(); // is absolute file path
    	db.setDatabaseName(strDbFn); 
    } else if (dlgDbConn.localDriverName() == RB2::DatabaseSqlite) {
        // check if we can write to that directory:
#ifndef Q_OS_WIN
    	RB_String path = QFileInfo(dlgDbConn.localDatabaseName()).absolutePath();
        if (QFileInfo(path).isWritable() == false) {
            RB_DEBUG->print("MainWindow::addConnection(): can't write file: "
                            "no permission");
            return;
        }
#endif
        // if database does not exists it will be created
        db = QSqlDatabase::addDatabase("QSQLITE",
                                            QString("DbBrowserDialog"));
    	db.setDatabaseName(dlgDbConn.localDatabaseName());// is absolute file path
    }
    
    // check if database is open otherwise return to in memory model
    if (!db.open()) {
        QMessageBox::warning(this, tr("Unable to open database"), 
        	tr("An error occured while opening the connection: ") 
        	+ db.lastError().text());
    
        // there should be a in memory model but for safety create new if not.
        if (!modelLine) {
    		createModel();
    	}
    	
	    QApplication::restoreOverrideCursor();	    	
    	updateMenus();
    	return;
    }

    QApplication::restoreOverrideCursor();

    // set database permissions in model view controler (model manager)
	LDT_DmLine* tmpModelLine = LDT_DmLine::getInstance(this, db);
//	tmpModelLine->setObjectFactory(LDT_OBJECTFACTORY);
	
	// set current connection name
	tmpModelLine->setCurrentConnection(connection);
	
	if (tmpModelLine->setPermissions(dlgDbConn.loginUser(), 
										dlgDbConn.loginPassword())) {

		// TODO: continue here with setting the permission singleton


	} else {
        QMessageBox::warning(this, tr("Unable to open database"), 
	        	tr("invalid login user and/or password \n") 
	        	+ db.lastError().text());
		
		db.close();    	
	    QApplication::restoreOverrideCursor();	    	
    	updateMenus();
    	return;
	}

	// disconnect the model signal/slots if already existed with the in memory model
	removeConnections();

    // select project from database or create new. If cancel, create in memory model
	ProjectSelectionDialog dialogProject(this, db);

    QApplication::restoreOverrideCursor();
	
	if (dialogProject.exec() == QDialog::Rejected) { 
		if (modelLine && modelLine->database().isOpen()) {
			QStringList strL = QSqlDatabase::connectionNames();
			for (int i = 0; i < strL.size(); ++i) {
				// TODO: this still gives a warning about connection in 
				//       use and all queries cease to work
				QSqlDatabase::database(strL.at(i), false).close();
				QSqlDatabase::removeDatabase(strL.at(i)); 
			}

	    	delete modelLine;
	    	modelLine = NULL;
		}

    	if (!modelLine) {
    		createModel();
    	}
    	
	    // clear all lineEdits, set checkboxes and comboboxes to default
	    clearProjectFields();
	    clearLineFields();
	    currentFileName.clear();

    	updateMenus();
    	return;
	}
		
	// keep successful settings 
	QStringList strL = QSqlDatabase::connectionNames();

	for (int i = 0; i < strL.size(); ++i) {
		if (i == 0) {
			QSqlDatabase db = QSqlDatabase::database(strL.at(i));

			RB_SETTINGS->beginGroup("databaseConnection");
			RB_SETTINGS->setValue("driverName", db.driverName());
			RB_SETTINGS->setValue("databaseName", db.databaseName());
			RB_SETTINGS->setValue("hostName", db.hostName());
			RB_SETTINGS->setValue("userName", db.userName());
			RB_SETTINGS->setValue("password", db.password());

			if (db.port() < 0) {
				RB_SETTINGS->setValue("port", "default");
			} else {
				RB_SETTINGS->setValue("port", RB_String::number(db.port()));
			}

			RB_SETTINGS->setValue("loginUser", dlgDbConn.loginUser());
			RB_SETTINGS->setValue("localDriverName", 
							RB2::enumToDriverName(dlgDbConn.localDriverName()));
			RB_SETTINGS->setValue("localDatabaseName",
							dlgDbConn.localDatabaseName());

			RB_SETTINGS->endGroup();
		}
	}
	
	
	// First delete LDT_DmInspectionItem which is child model of LDT_DmLine
	// - modelInspectionItem->setModel(modelLine) with changeCurrent()
	// - tvInspectionItem->setModel(modelInspectionItem) with changeCurrent()
	if (modelInspectionItem)
		delete modelInspectionItem;

	// Second delete modelLine otherwise modelRoot in LDT_DmInspectionItem gone 
	// - set modelLine to database model and give database connection as 
	//   parameter
	delete modelLine;
	modelLine = tmpModelLine;
	tmpModelLine = NULL;
	
	modelLine->setCurrentConnection(connection);
    modelLine->setRoot(dialogProject.getProjectId());
    modelLine->modelIsModified(false);
	tableView->setModel(modelLine);

	// required to reset the selectionModel and the changeCurrentRow
	// due to tableView->setModel(modelLine);
	selectionModel = tableView->selectionModel();
	formatTableView();

	setProjectFields(*(LDT_Project*)modelLine->getRoot());
    currentFileName.clear();

	modelInspectionItem = LDT_DmInspectionItem::getInstance(this, db);
	RB_ObjectContainer* objC = (RB_ObjectContainer*)modelLine->getCurrentObject();
	modelInspectionItem->setModel(objC, "LDT_InspectionItemList", modelLine);

	tvInspectionItem->setModel(modelInspectionItem);
	selectionInspectionModel = tvInspectionItem->selectionModel();
	formatTvInspectionItem();

    createConnections();

    // emit statusbar and update menu
    statusBar()->showMessage("Connected to database - Ready", 4000);
	updateMenus();

    setWindowTitle(tr("%1 - %2").arg(tr("LDT"))
    							.arg(tr("database")));
	
	RB_DEBUG->print("MainWindow::databaseConnect() - OK");
}

 
/**
 * Select a new project from the database. The database connection was 
 * established previously in databaseConnect().
 */
void MainWindow::databaseSelectProject() {
	// select project from database or create new
    if (modelLine != NULL && !modelLine->database().isOpen()) {
        QMessageBox::information(this, tr("Not connected to database"),
             tr("Please connect to database first \n"
             		"before selecting a project."));
		return;
    }

    // select project from database or if cancel close dialog
	ProjectSelectionDialog dialogProject(this, modelLine->database());

	if (dialogProject.exec() == QDialog::Rejected) { 
    	return;
	}

    // get new root (LDT_Project) based on selected id in dialog
    modelLine->setRoot(dialogProject.getProjectId());
	tableView->setModel(modelLine);
    modelLine->reset();

	setProjectFields(*(LDT_Project*)modelLine->getRoot());

    modelLine->modelIsModified(false);				// todo: not necessary?
	
    // emit statusbar and update menu
    statusBar()->showMessage("Connected to database - Ready", 4000);
	formatTableView();
	updateMenus();
}

	
void MainWindow::databaseCreateTables() {
	RB_DEBUG->print("MainWindow::databaseCreateTables()");
	
    // check for previous valid instance and database connection
    if (modelLine == NULL && !modelLine->database().isOpen()) {
        QMessageBox::information(this, tr("Create tables"),
             tr("Could not connect to database, please select\n"
             "another database or database connection."));
		return;
    }
	
    if (!modelLine->createTables()) {
        QMessageBox::warning(this, tr("Create database tables"),
             tr("Could not create database tables. \n")
             + modelLine->database().lastError().text());
    }
}

/**
 * Show database connection settings only. For a new connection, disconnect from
 * the database first and select from the menu Database > Connect
 */
void MainWindow::databaseSettings() {
	RB_DEBUG->print("MainWindow::databaseSettings()");

	RB_SETTINGS->beginGroup("databaseConnection");
	RB_String driverName = RB_SETTINGS->value("driverName", "QODBC").toString();
	RB_String databaseName = RB_SETTINGS->value("databaseName", "biluna").toString();
	RB_String hostName = RB_SETTINGS->value("hostName", "localhost").toString();
	RB_String userName = RB_SETTINGS->value("userName", "user").toString();
	RB_String password = RB_SETTINGS->value("password", "password").toString();
	RB_String port = RB_SETTINGS->value("port", "Default").toString();
	RB_String loginUser = RB_SETTINGS->value("loginUser", "loginUser").toString();
	RB_String localDriverName = RB_SETTINGS->value("localDriverName", "QODBC").toString();
	RB_String localDatabaseName = RB_SETTINGS->value("localDatabaseName", "biluna").toString();
	RB_SETTINGS->endGroup();

	DB_ConnectionDialog dlgDbConn(this, 
								RB2::driverNameToEnum(driverName), 
								databaseName, 
								userName, 
								password, 
								hostName, 
								port, 
								loginUser, 	// not the password
								RB2::driverNameToEnum(localDriverName),
								localDatabaseName);

    dlgDbConn.setWindowTitle(tr("Database Connection Settings"));
    dlgDbConn.exec();
}

void MainWindow::databaseBrowse() {
	RB_DEBUG->print("MainWindow::databaseBrowse()");
	
	DB_BrowserDialog dialog(this);
	dialog.exec();
	
	RB_DEBUG->print("MainWindow::databaseBrowse() OK");
}


void MainWindow::databasePermission() {
	RB_DEBUG->print("MainWindow::databasePermission()");
	
	ProjectSelectionDialog projDialog(this, QSqlDatabase::database());
	
	if (projDialog.exec() == QDialog::Rejected) { 
    	return;
	}

	AUTH_PermissionDialog dialog(this, projDialog.getProjectId());
	dialog.exec();

	// updateMenus();
}


void MainWindow::databaseImportCsv() {
	RB_DEBUG->print("MainWindow::databaseImportCsv()");
	
	DB_MappingDialog dialog(this);
	dialog.setParent("LDT_Line", modelLine->getRoot()->getId(), "parent", true);
	dialog.exec();

	// updateMenus();
}


/**
 * Import all data from XML file generated by XML.  This function is only 
 * active when connected to a database.
 */
void MainWindow::databaseImportXml() {
	RB_DEBUG->print("MainWindow::databaseImportXml()");
    
    if (!modelLine->database().isOpen()) {
        QMessageBox::information(this, tr("Not connected to database"),
             tr("Please connect to database first \n"
             		"before importing a project."));
		return;
    }

	RB_SETTINGS->beginGroup("paths");
    RB_String openPath = RB_SETTINGS->value("openPath", "").toString();
	RB_SETTINGS->endGroup();
    	
    QString fn = QFileDialog::getOpenFileName (this, "Import LDT project", 
    											openPath); 
	if (fn.isEmpty()) {
        QMessageBox::warning(this, tr("LDT Import"),
                             tr("File not imported,\nfile name was empty."));
        return;
	}

	// create temporary import model
	if (modelLineImport) delete modelLineImport;
	modelLineImport	= LDT_MmLine::getInstance();
	modelLineImport->setObjectFactory(LDT_OBJECTFACTORY);

	// import LDT file in separate in memory model
    loadFile(fn, "LDT_XML");

	// create action
	LDT_ActionDbImportXml* action = new LDT_ActionDbImportXml();
	RB_String* result = new RB_String();
	
	// execute import action
	action->execute(modelLine, modelLineImport, RB2::ActionLdtDbImport, result);

    QMessageBox::information(this, tr("LDT import"), *result);

	if (modelLineImport) {
		delete modelLineImport;
		modelLineImport = NULL;
	}
	
	if (action) delete action;
	if (result) delete result;

//	updateMenus();
}


/**
 * Export all data from database project to XML file.  This function is only 
 * active when connected to a database.
 */
void MainWindow::databaseExportXml() {
	RB_DEBUG->print("MainWindow::databaseExportXml()");
	
	RB_SETTINGS->beginGroup("paths");
    RB_String savePath = RB_SETTINGS->value("savePath", "").toString();
	RB_SETTINGS->endGroup();
    	
    RB_String fileName = QFileDialog::getSaveFileName (this, 
   												"LDT - save file as", savePath); 
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("LDT Export"),
                             tr("File not exported,\nfile name was empty."));
        return;
    }
    
    // commit all changes first
    editCommitChanges();

	// create temporary export model
	modelLineExport	= LDT_MmLine::getInstance();
	modelLineExport->setObjectFactory(LDT_OBJECTFACTORY);

	// create action
	LDT_ActionDbExportXml* action = new LDT_ActionDbExportXml();
	RB_String* result = new RB_String();
	
	// execute export action
	action->execute(modelLine, modelLineExport, RB2::ActionLdtDbExport, result);

	// export LDT file to disk
    if (saveFile(fileName)) {
	    QMessageBox::information(this, tr("LDT export"), *result);
    } else {
	    QMessageBox::warning(this, tr("LDT export"), 
	    					tr("File not exported,\nfile writing error."));
    }

	if (modelLineExport) {
		delete modelLineExport;
		modelLineExport = NULL;
	}
	
	if (action) delete action;
	if (result) delete result;

//	updateMenus();
}


void MainWindow::databaseDisconnect() {
	RB_DEBUG->print("MainWindow::databaseDisconnect()");

	// save possible changes of textEdits
	if (tableView->currentIndex().isValid()) {
		int row = tableView->currentIndex().row();
		saveTextEditRemProcess(row);
		saveTextEditRemEnginDes(row);
		saveTextEditRemClassification(row);
		saveTextEditRemOperMaint(row);
		saveTextEditRemCustom(row);
	}

    clearProjectFields();
    clearLineFields();
	
	removeConnections();

	// first: delete LDT_DmInspectionItem and LDT_DmLine
	modelInspectionItem->submitAll();
	modelLine->submitAll();

	delete modelInspectionItem;
	delete modelLine;

	modelInspectionItem = NULL;
	modelLine = NULL;

    createModel();
	createConnections();	

	LDT_Project* activeProject = (LDT_Project*)modelLine->getRoot();
	setProjectFields(*activeProject);

	// second: clear database connection
	QStringList strL = QSqlDatabase::connectionNames();

	for (int i = 0; i < strL.size(); ++i) {
		QSqlDatabase::database(strL.at(i), false).close();
		QSqlDatabase::removeDatabase(strL.at(i));
	}

	updateMenus();
}


void MainWindow::reportTable() {
	RB_DEBUG->print("MainWindow::reportTable()");
	
    TableReportDialog dialog(this);
	dialog.exec();
	updateMenus();
}


void MainWindow::reportFluid() {
	RB_DEBUG->print("MainWindow::reportFluid()");
	
	if (!fluidReport) {
		fluidReport = new FluidReport(this);
	} else {
		fluidReport->refresh(this);	
	}

	fluidReport->show();
	fluidReport->raise();
}


void MainWindow::reportInspectionData() {
	RB_DEBUG->print("MainWindow::reportInspectionData()");
	
	if (!tvInspectionItem->currentIndex().isValid()) {
	    QMessageBox::warning(this, tr("Inspection Item Data Report"),
						tr("An inspection item needs to be selected\n"
	                    "before you can view the report."));
	    return;
	}
	
	// set current selected item
//	int row = tvInspectionItem->currentIndex().row();
//	if (row >= 0 && row < tvInspectionItem->model()->rowCount()) {
//		modelInspectionItem->changeCurrent(row);
//	} else {
//		statusBar()->showMessage("No inspection item selected - Ready", 2000);
//	}
	
	if (!inspectionDataReport) {
		inspectionDataReport = new InspectionDataReport(this);
	} else {
		inspectionDataReport->refresh(this);	
	}

	inspectionDataReport->show();
	inspectionDataReport->raise();
}


void MainWindow::reportInspectionLoop() {
	RB_DEBUG->print("MainWindow::reportInspectionLoop()");
	
	if (!inspectionLoopReport) {
		inspectionLoopReport = new InspectionLoopReport(this);
	} else {
		inspectionLoopReport->refresh(this);	
	}

	inspectionLoopReport->show();
	inspectionLoopReport->raise();
}


void MainWindow::reportInsulation() {
	RB_DEBUG->print("MainWindow::reportInsulation()");
	
	if (!insulationReport) {
		insulationReport = new InsulationReport(this);
	} else {
		insulationReport->refresh(this);	
	}

	insulationReport->show();
	insulationReport->raise();
}


void MainWindow::reportPaint() {
	RB_DEBUG->print("MainWindow::reportPaint()");
	
	if (!paintReport) {
		paintReport = new PaintReport(this);
	} else {
		paintReport->refresh(this);	
	}

	paintReport->show();
	paintReport->raise();
}


void MainWindow::reportPipeClass() {
	RB_DEBUG->print("MainWindow::reportPipeClass()");
	
	if (!pipeClassReport) {
		pipeClassReport = new PipeClassReport(this);
	} else {
		pipeClassReport->refresh(this);	
	}

	pipeClassReport->show();
	pipeClassReport->raise();
}


void MainWindow::reportTieIn() {
	RB_DEBUG->print("MainWindow::reportTieIn()");
	
	if (!tieInReport) {
		tieInReport = new TieInReport(this);
	} else {
		tieInReport->refresh(this);	
	}

	tieInReport->show();
	tieInReport->raise();
}


/**
 * Index of help documentation
 */
void MainWindow::helpIndex() {
	RB_DEBUG->print("MainWindow::helpIndex()");
	// HelpBrowser::showPage(this, "index.html");

	// TODO: create application paths in RB_System
    RB_String appPath = QCoreApplication::applicationDirPath();
    assistantClient->showPage(appPath + "/doc/help/index.html");
}

/**
 * Alphabetical contents of help
 */
void MainWindow::helpContents() {
	HelpBrowser::showPage(this, "contents.html");
}


void MainWindow::helpAbout() {
	QMessageBox mb(tr("About LDT"),
		tr("The <b>Biluna LDT</b> prototype demonstrates the "
           "handling of Line Designation Tables, as used by "
           "engineering contractors and process plant owners.<br/><br/>"
           "The LDT application is developed by Red-Bag. "
           "For more information visit: http://www.red-bag.com"),
         QMessageBox::Information,	// is overwritten with 'image'
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
	
	QPixmap image(rsrcPath + "/biluna32.png");
	mb.setIconPixmap (image);
	mb.exec();
}


/**
 * TODO: for debugging only of signal/slot only
 */
void MainWindow::helpDebugSpy() {
  	// Setup the spy, cw is the widget to be checked for the signal and slots 
  	//   with copies (signals slots) send to SignalSpyDialog (ssd)
//	if (!spy) {
//		spy = new Q4puGenericSignalSpy(this);
//		spy->spyOn(tableView);
//	} else {
//		delete spy;
//		spy = new Q4puGenericSignalSpy(this);
//		spy->spyOn(tableView);
//	}
//	
//	// dialog to receive the signal and slots from genericsignalspy
//	ssd = new RB_SignalSpyDialog(this);
//
//	// connect the signal and slots
//	QObject::connect(spy, SIGNAL(caughtSignal(const QString&)), 
//						ssd, SLOT(append(const QString&)));
//	QObject::connect(spy, SIGNAL(caughtSlot(const QString&)), 
//						ssd, SLOT(append(const QString&)));
//
//	ssd->show();
	
	RB_SignalSpyDialog* ssd;
	ssd = new RB_SignalSpyDialog(this, tableView);
	ssd->show();

}


/**
 * TODO: for debugging only of model structure only
 */
void MainWindow::helpDebugModel() {
	// store the pointer of the main model and create model structure
	modelLine->setModelStructure();
	
	// export
	fileSaveAs();
	
	// restore model
	modelLine->unsetModelStructure();
}



/**
 * Slot receives warning messages from the application 
 * and shows a message to the user for information
 */
void MainWindow::messageWarning(const QString& str) {
	QMessageBox mb("LDT Application Warning", 
			str,
         	QMessageBox::Warning,
         	QMessageBox::Ok | QMessageBox::Default,
         	QMessageBox::NoButton, QMessageBox::NoButton);
	
	// QPixmap image(rsrcPath + "/biluna32.png");
	// mb.setIconPixmap (image);
	mb.exec();
}


/**
 * Slot receives critical error messages from the application 
 * and shows a message to the user for information
 */
void MainWindow::messageCritical(const QString& str) {
	QMessageBox mb("LDT Application Error", 
			str,
         	QMessageBox::Critical,
         	QMessageBox::Ok | QMessageBox::Default,
         	QMessageBox::NoButton, QMessageBox::NoButton);
	
	// QPixmap image(rsrcPath + "/biluna32.png");
	// mb.setIconPixmap (image);
	mb.exec();
}


/**
 * Slot receives signal from model, to update the lineFields if user has 
 * edited the tableView and to update the menus
 */
void MainWindow::modelDataChanged(const QModelIndex&, const QModelIndex&) {
	RB_DEBUG->print("MainWindow::modelDataChanged()");
	
	// if statement is workaround to prevent infinite loop with textEdits
	if (!textEditRemProcess->hasFocus() && !textEditRemEnginDes->hasFocus()
									&& !textEditRemClassification->hasFocus() 
									&& !textEditRemOperMaint->hasFocus() 
									&& !textEditRemCustom->hasFocus()) {
		setLineFields();
		// refreshTableWidgetInspItem();
	}
	updateMenus();	
}

void MainWindow::updateMenus() {
	RB_DEBUG->print("MainWindow::updateMenus()");
	
	bool dbIsOpen = modelLine->database().isOpen();
	bool lineListIsFiltered = modelLine->isListFiltered();
	bool modelModified = modelLine->isModelModified();
	bool processChecked = viewProcessAction->isChecked();
	bool utilityChecked = viewUtilityAction->isChecked();
	
	fileNewAction->setEnabled(!dbIsOpen);
	fileOpenAction->setEnabled(!dbIsOpen);
	fileSaveAction->setEnabled(!dbIsOpen && modelModified);
	fileSaveAsAction->setEnabled(!dbIsOpen);
	fileImportLdtAction->setEnabled(dbIsOpen);
	fileImportPccAction->setEnabled(!dbIsOpen);
	fileExportLdtAction->setEnabled(dbIsOpen);
	
//	editUndoAction;
//	editRedoAction;
//	editPasteAction;
	editFilterOnAction->setEnabled(!lineListIsFiltered);
	editFilterOffAction->setEnabled(lineListIsFiltered);
	editCommitChangesAction->setEnabled(dbIsOpen);

	// allow filter set by process/utility not by editFilterOn()
	lineNewAction->setEnabled(!lineListIsFiltered 
					|| (processChecked && !utilityChecked && lineListIsFiltered)
					|| (!processChecked && utilityChecked && lineListIsFiltered));
	lineDeleteAction->setEnabled(!lineListIsFiltered
					|| (processChecked && !utilityChecked && lineListIsFiltered)
					|| (!processChecked && utilityChecked && lineListIsFiltered));

	dbConnectAction->setEnabled(!dbIsOpen);	// official version
	// dbConnectAction->setEnabled(false);	// demo version non-database
	
	dbCreateAction->setEnabled(dbIsOpen); 	// TODO: activate after testing
	dbSelectProjectAction->setEnabled(dbIsOpen);
	dbSettingsAction->setEnabled(dbIsOpen);
	dbBrowseAction->setEnabled(dbIsOpen);
	dbPermissionAction->setEnabled(dbIsOpen);
	dbImportCsvAction->setEnabled(dbIsOpen);
	dbImportXmlAction->setEnabled(dbIsOpen);
	dbExportXmlAction->setEnabled(dbIsOpen);
	dbDisconnectAction->setEnabled(dbIsOpen);
}


void MainWindow::resetModel() {
	RB_DEBUG->print("MainWindow::resetModel()");
	
	if (modelLine->isListFiltered()) {
		modelLine->removeListFilter();
	}
	
	modelLine->setProcessUtility(viewProcessAction->isChecked(), 
								viewUtilityAction->isChecked());
	modelLine->reset(); // TODO: bring to modelLine->setProcessUtility() as in changeCurrent()  
	
	// Hack: only required for database
	if (modelLine->database().isOpen()) {
		tableView->setModel(modelLine);	
	}

	formatTableView();
	clearLineFields();
}

void MainWindow::formatLineFields() {
	RB_DEBUG->print("MainWindow::formatLineFields()");
	
    lineEditLineNo->setValidator(
    		new QIntValidator(0, 9999999, lineEditLineNo));
    lineEditLineRev->setValidator(
    		new QIntValidator(0, 999, lineEditLineRev));
    lineEditSize->setValidator(
    		new QDoubleValidator( 0.0, 999.0, 2, lineEditSize));
    
    // todo: bring the Process and Utility term to a central place in StringList
    comboBoxProcessUtility->insertItem(0, "(none)");
    comboBoxProcessUtility->insertItem(1, "Process");
    comboBoxProcessUtility->insertItem(2, "Utility");
    comboBoxProcessUtility->setCurrentIndex(0);
    // todo: bring the Atm., B.L., etc. terms to a central place in StringList    
    comboBoxFromType->insertItem(0, "(none)");
    comboBoxFromType->insertItem(1, "Atmosphere");
    comboBoxFromType->insertItem(2, "Battery limit");
    comboBoxFromType->insertItem(3, "Equipment");
    comboBoxFromType->insertItem(4, "Line");
    comboBoxFromType->insertItem(5, "Trim");
    comboBoxFromType->setCurrentIndex(0);
    // todo: bring the Atm., B.L., etc. terms to a central place in StringList    
    comboBoxToType->insertItem(0, "(none)");
    comboBoxToType->insertItem(1, "Atmosphere");
    comboBoxToType->insertItem(2, "Battery limit");
    comboBoxToType->insertItem(3, "Equipment");
    comboBoxToType->insertItem(4, "Line");
    comboBoxToType->insertItem(5, "Trim");
    comboBoxToType->setCurrentIndex(0);
    lineEditPd->setValidator( 
    		new QDoubleValidator(-1.0, 9999.0, 2, lineEditPd));
    lineEditTd->setValidator( 
    		new QDoubleValidator(-999.0, 9999.0, 2, lineEditTd));
    
    // tabs
	lineEditProcessOperPress->setValidator( 
    		new QDoubleValidator(lineEditProcessOperPress));
	lineEditProcessOperTemp->setValidator( 
    		new QDoubleValidator(lineEditProcessOperTemp));
	lineEditProcessOperTempLow->setValidator( 
    		new QDoubleValidator(lineEditProcessOperTempLow));
	lineEditProcessOperTempHigh->setValidator( 
    		new QDoubleValidator(lineEditProcessOperTempHigh));
	lineEditProcessTestPress->setValidator( 
    		new QDoubleValidator(lineEditProcessTestPress));
	lineEditProcessPressDrop->setValidator( 
    		new QDoubleValidator(lineEditProcessPressDrop));
	lineEditProcessMassFlowRate->setValidator( 
    		new QDoubleValidator(lineEditProcessMassFlowRate));
	lineEditProcessMassFlowRateMax->setValidator( 
    		new QDoubleValidator(lineEditProcessMassFlowRateMax));
	lineEditProcessVelocity->setValidator( 
    		new QDoubleValidator(lineEditProcessVelocity));
	lineEditProcessBoilingPoint->setValidator( 
    		new QDoubleValidator(lineEditProcessBoilingPoint));
	lineEditProcessVapourPress->setValidator( 
    		new QDoubleValidator(lineEditProcessVapourPress));
	lineEditProcessMolWeight->setValidator( 
    		new QDoubleValidator(lineEditProcessMolWeight));
	lineEditProcessDensityLiquid->setValidator( 
    		new QDoubleValidator(lineEditProcessDensityLiquid));
	lineEditProcessDensityVapour->setValidator( 
    		new QDoubleValidator(lineEditProcessDensityVapour));
	lineEditProcessViscosityLiquid->setValidator( 
    		new QDoubleValidator(lineEditProcessViscosityLiquid));
	lineEditProcessViscosityVapour->setValidator( 
    		new QDoubleValidator(lineEditProcessViscosityVapour));
	lineEditProcessStandardVolumeFlow->setValidator( 
    		new QDoubleValidator(lineEditProcessStandardVolumeFlow));
	lineEditProcessStandardVolumeFlowMax->setValidator( 
    		new QDoubleValidator(lineEditProcessStandardVolumeFlowMax));
	lineEditProcessActualFlow->setValidator( 
    		new QDoubleValidator(lineEditProcessActualFlow));
	lineEditProcessActualFlowMax->setValidator( 
    		new QDoubleValidator(lineEditProcessActualFlowMax));
	
	lineEditDesTracingNumberOf->setValidator( 
    		new QIntValidator(0, 9999999, lineEditDesTracingNumberOf));
	lineEditDesInsulationThickness->setValidator( 
    		new QIntValidator(0, 9999999, lineEditDesTracingNumberOf));
	lineEditClassNomOperPress->setValidator( 
    		new QDoubleValidator(lineEditClassNomOperPress));
	lineEditClassNomOperTemp->setValidator( 
    		new QDoubleValidator(lineEditClassNomOperTemp));
	lineEditClassMaxDesPress->setValidator( 
    		new QDoubleValidator(lineEditClassMaxDesPress));
	lineEditClassMaxDesTemp->setValidator( 
    		new QDoubleValidator(lineEditClassMaxDesTemp));
	lineEditClassMinDesTemp->setValidator( 
    		new QDoubleValidator(lineEditClassMinDesTemp));
   
    // todo: bring the terms to a central place in StringList    
   comboBoxClassFluidDangerClassGroup->insertItem(0, "(none)");
   comboBoxClassFluidDangerClassGroup->insertItem(1, "Group1");
   comboBoxClassFluidDangerClassGroup->insertItem(2, "Group2");
   comboBoxClassGasStability->insertItem(0, "(none)"); 
   comboBoxClassGasStability->insertItem(1, "Stable"); 
   comboBoxClassGasStability->insertItem(2, "Unstable"); 

    // todo: bring the terms to a central place in StringList    
    comboBoxProcessPhase->insertItem(0, "(none)");
    comboBoxProcessPhase->insertItem(1, "Gas");
    comboBoxProcessPhase->insertItem(2, "Liquid");
    comboBoxProcessPhase->insertItem(3, "Vapour");
    comboBoxProcessPhase->insertItem(4, "Two Phase");
    comboBoxProcessPhase->setCurrentIndex(0);
    // todo: bring the terms to a central place in StringList    
    comboBoxDesLineFunction->insertItem(0, "(none)");
    comboBoxDesLineFunction->insertItem(1, "Main Line");
    comboBoxDesLineFunction->insertItem(2, "Start-up/Shutdown");
    comboBoxDesLineFunction->insertItem(3, "Priming");
    comboBoxDesLineFunction->insertItem(4, "Draining");
    comboBoxDesLineFunction->insertItem(5, "Min. Flow Bypass");
    comboBoxDesLineFunction->setCurrentIndex(0);
    comboBoxDesExternalPressure->insertItem(0, "(none)");
    comboBoxDesExternalPressure->insertItem(1, "Full Vacuum");
    comboBoxDesExternalPressure->insertItem(2, "Jacketed Pipe");
    comboBoxDesExternalPressure->setCurrentIndex(0);
    comboBoxDesTracingType->insertItem(0, "(none)");
    comboBoxDesTracingType->insertItem(1, "Electrical Tracer");
    comboBoxDesTracingType->insertItem(2, "Oil Jacket");
    comboBoxDesTracingType->insertItem(3, "Steam Tracer");
    comboBoxDesTracingType->insertItem(4, "Steam Jacket");
    comboBoxDesTracingType->insertItem(5, "Water Jacket");
    comboBoxDesTracingType->insertItem(6, "Winterizing");
    comboBoxDesTracingType->setCurrentIndex(0);
    // bring terms to central place in QStringList
    comboBoxDesInsulationType->insertItem(0, "(none)");
    comboBoxDesInsulationType->insertItem(1, "Cold Conservation");
    comboBoxDesInsulationType->insertItem(2, "Heat Conservation");
    comboBoxDesInsulationType->insertItem(3, "Heat Cons. Indoor");
    comboBoxDesInsulationType->insertItem(4, "Personnel Protection");
    comboBoxDesInsulationType->setCurrentIndex(0);

	/* date */    
    dateEditOperInstallation->setDisplayFormat("yyyy-MM-dd");
	
	lineEditOperInspectionFrequency->setValidator( 
			new QIntValidator(0, 9999, lineEditOperInspectionFrequency));
    lineEditCustValue1->setValidator( 
    		new QDoubleValidator(lineEditCustValue1));
    lineEditCustValue2->setValidator( 
    		new QDoubleValidator(lineEditCustValue2)) ;
    lineEditCustValue3->setValidator( 
    		new QDoubleValidator(lineEditCustValue3));
    lineEditCustValue4->setValidator( 
    		new QDoubleValidator(lineEditCustValue4));
    lineEditCustValue5->setValidator( 
    		new QDoubleValidator(lineEditCustValue5));
}


void MainWindow::changeCurrent(const QModelIndex& current, 
								const QModelIndex& previous) {
	RB_DEBUG->print("MainWindow::changeCurrent()");
	
	if (modelInspectionItem->isModelModified()) {
		modelLine->modelIsModified(true);
	}

	saveTextEditRemProcess(previous.row());
	saveTextEditRemEnginDes(previous.row());
	saveTextEditRemClassification(previous.row());
	saveTextEditRemOperMaint(previous.row());
	saveTextEditRemCustom(previous.row());
	
	// workaround 2005-12-13, with database connection the tableView
	// sometimes returns an invalid current index, results in crash
	if (!current.isValid()) {
		RB_DEBUG->print("MainWindow::changeCurrent() - current is not valid");

		// workaround sometimes necessary for LDT_SqlLineManager
		tableView->setColumnHidden(0, true); 	// id
		tableView->setColumnHidden(1, true);	// parent
		tableView->setColumnHidden(2, true);	// name
		tableView->setColumnHidden(6, true);	// processUtility 
		tableView->setColumnHidden(8, true);	// fromType
		tableView->setColumnHidden(9, true);	// fromSub
		tableView->setColumnHidden(11, true);	// toType
		tableView->setColumnHidden(12, true);	// toSub
		
		// hide the remaining fields
		for (int i = 17; i < modelLine->columnCount(QModelIndex()); i++) {
			tableView->setColumnHidden(i, true);
		}
// TODO: next lines are not necessary but do result in a crash 
// when in database mode
//		for (int i = 3; i < 16; i++) {
//			tableView->resizeColumnToContents(i);
//		}
		
		clearLineFields();
		return;
	}
	
	// reset selection in modelManager, otherwise selection is not updated
	// TODO: is this correct or should there be a signal/slot alternative,done remove
//	modelLine->setSelectionModel(selectionModel);
	
	setLineFields();

	// set modelLine subObject to current row
//	modelLine->changeCurrent(current.row());
//	modelInspectionItem->submitAll();
//	
//	// set modelInspectionItem to subObject of modelLine and get relevant list
//	// modelInspectionItem->setModel(modelLine);
//	// alternative 1:
//	// modelInspectionItem->setSubModel(modelLine, "LDT_InspectionItemList");
//	// alternative 2:
//	RB_ObjectContainer* objC = (RB_ObjectContainer*)modelLine->getCurrentObject();
//	modelInspectionItem->setModel(objC, "LDT_InspectionItemList", modelLine);
//	
//	tvInspectionItem->setModel(modelInspectionItem);
//	formatTvInspectionItem();
	
	updateMenus();
}


void MainWindow::setProjectFields(const RB_ObjectContainer& activeProject) {
    RB_DEBUG->print("MainWindow::setProjectFields()");
	
    lineEditProjectNo->setText(activeProject.getValue("number").toString());
    // NOTE: revision is actually an integer!
    lineEditProjectRev->setText(activeProject.getValue("revision").toString());

   	// fields in tab custom
    lineEditCustLabel1->setText(activeProject.getValue("customLabel1").toString());
    lineEditCustLabel2->setText(activeProject.getValue("customLabel2").toString());
    lineEditCustLabel3->setText(activeProject.getValue("customLabel3").toString());
    lineEditCustLabel4->setText(activeProject.getValue("customLabel4").toString());
    lineEditCustLabel5->setText(activeProject.getValue("customLabel5").toString());
    lineEditCustUnit1->setText(activeProject.getValue("customUnit1").toString());
    lineEditCustUnit2->setText(activeProject.getValue("customUnit2").toString());
    lineEditCustUnit3->setText(activeProject.getValue("customUnit3").toString());
    lineEditCustUnit4->setText(activeProject.getValue("customUnit4").toString());
    lineEditCustUnit5->setText(activeProject.getValue("customUnit5").toString());
    lineEditCustLabel6->setText(activeProject.getValue("customLabel6").toString());
    lineEditCustLabel7->setText(activeProject.getValue("customLabel7").toString());
    lineEditCustLabel8->setText(activeProject.getValue("customLabel8").toString());
    lineEditCustLabel9->setText(activeProject.getValue("customLabel9").toString());
    lineEditCustLabel10->setText(activeProject.getValue("customLabel10").toString());
}


void MainWindow::setLineFields() {
	RB_DEBUG->print("MainWindow::setLineFields()");
	
	clearLineFields();

	if (!tableView->currentIndex().isValid()) {
		return;
	}
	
	if(viewChangesAction->isChecked())
		setLineFieldDecoration();
	
	int row = tableView->currentIndex().row();
	
	lineEditLineNo->setText(modelLine->data(modelLine->index(row, 3)).toString());
    lineEditLineRev->setText(modelLine->data(modelLine->index(row, 4)).toString());
    lineEditSize->setText(modelLine->data(modelLine->index(row, 5)).toString());
	
    QString str = modelLine->data(modelLine->index(row, 6)).toString();
    comboBoxProcessUtility->setCurrentIndex(comboBoxProcessUtility->findText(str));

    lineEditFrom->setText(modelLine->data(modelLine->index(row, 7)).toString());
	
    str = modelLine->data(modelLine->index(row, 8)).toString();
    comboBoxFromType->setCurrentIndex(comboBoxFromType->findText(str));

    lineEditFromNozzle->setText(modelLine->data(modelLine->index(row, 9)).toString());
    lineEditTo->setText(modelLine->data(modelLine->index(row, 10)).toString());
	
    str = modelLine->data(modelLine->index(row, 11)).toString();
    comboBoxToType->setCurrentIndex(comboBoxToType->findText(str));


    lineEditToNozzle->setText(modelLine->data(modelLine->index(row, 12)).toString());
    lineEditFluidID->setText(modelLine->data(modelLine->index(row, 13)).toString());
    lineEditPd->setText(modelLine->data(modelLine->index(row, 16)).toString());
    lineEditPipeClass->setText(modelLine->data(modelLine->index(row, 14)).toString());
    lineEditTd->setText(modelLine->data(modelLine->index(row, 15)).toString());
    
    // detail data
    lineEditProcessFluidDescr->setText(modelLine->data(modelLine->index(row, 17)).toString());
    lineEditProcessOperPress->setText(modelLine->data(modelLine->index(row, 18)).toString());
    lineEditProcessOperTemp->setText(modelLine->data(modelLine->index(row, 19)).toString());
    lineEditProcessOperTempLow->setText(modelLine->data(modelLine->index(row, 20)).toString());
    lineEditProcessOperTempHigh->setText(modelLine->data(modelLine->index(row, 21)).toString());
    lineEditProcessTestMedium->setText(modelLine->data(modelLine->index(row, 22)).toString());
    lineEditProcessTestPress->setText(modelLine->data(modelLine->index(row, 23)).toString());
    lineEditProcessPressDrop->setText(modelLine->data(modelLine->index(row, 24)).toString());
    lineEditProcessMassFlowRate->setText(modelLine->data(modelLine->index(row, 25)).toString());
	lineEditProcessMassFlowRateMax->setText(modelLine->data(modelLine->index(row, 26)).toString());
    lineEditProcessVelocity->setText(modelLine->data(modelLine->index(row, 27)).toString());

    str = modelLine->data(modelLine->index(row, 28)).toString();
    comboBoxProcessPhase->setCurrentIndex(comboBoxProcessPhase->findText(str));

    lineEditProcessBoilingPoint->setText(modelLine->data(modelLine->index(row, 29)).toString());
    lineEditProcessVapourPress->setText(modelLine->data(modelLine->index(row, 30)).toString());
    lineEditProcessMolWeight->setText(modelLine->data(modelLine->index(row, 31)).toString());
    lineEditProcessDensityLiquid->setText(modelLine->data(modelLine->index(row, 32)).toString());
    lineEditProcessDensityVapour->setText(modelLine->data(modelLine->index(row, 33)).toString());
    lineEditProcessViscosityLiquid->setText(modelLine->data(modelLine->index(row, 34)).toString());
    lineEditProcessViscosityVapour->setText(modelLine->data(modelLine->index(row, 35)).toString());
	lineEditProcessStandardVolumeFlow->setText(modelLine->data(modelLine->index(row, 36)).toString());
    lineEditProcessStandardVolumeFlowMax->setText(modelLine->data(modelLine->index(row, 37)).toString());
    lineEditProcessActualFlow->setText(modelLine->data(modelLine->index(row, 38)).toString());
    lineEditProcessActualFlowMax->setText(modelLine->data(modelLine->index(row, 39)).toString());
    lineEditProcessSimulation->setText(modelLine->data(modelLine->index(row, 40)).toString());

    lineEditDesPnidFrom->setText(modelLine->data(modelLine->index(row, 41)).toString());
    lineEditDesPnidTo->setText(modelLine->data(modelLine->index(row, 42)).toString());
    lineEditDesGaFrom->setText(modelLine->data(modelLine->index(row, 43)).toString());
    lineEditDesGaTo->setText(modelLine->data(modelLine->index(row, 44)).toString());
	lineEditDesIsoFrom->setText(modelLine->data(modelLine->index(row, 45)).toString());
    lineEditDesIsoTo->setText(modelLine->data(modelLine->index(row, 46)).toString());

    str = modelLine->data(modelLine->index(row, 47)).toString();
    comboBoxDesLineFunction->setCurrentIndex(comboBoxDesLineFunction->findText(str));

    str = modelLine->data(modelLine->index(row, 48)).toString();
    comboBoxDesExternalPressure->setCurrentIndex(comboBoxDesExternalPressure->findText(str));

    str = modelLine->data(modelLine->index(row, 49)).toString();
    comboBoxDesTracingType->setCurrentIndex(comboBoxDesTracingType->findText(str));

    lineEditDesTracingNumberOf->setText(modelLine->data(modelLine->index(row, 50)).toString());

    str = modelLine->data(modelLine->index(row, 51)).toString();
    comboBoxDesInsulationType->setCurrentIndex(comboBoxDesInsulationType->findText(str));

    lineEditDesInsulationThickness->setText(modelLine->data(modelLine->index(row, 52)).toString());

	str = modelLine->data(modelLine->index(row, 53)).toString();
    if (str == "true") {
	    checkBoxDesPaintingAsPerSpec->setChecked(true);
    } else {
    	checkBoxDesPaintingAsPerSpec->setChecked(false);
    }

    lineEditDesPaintingSystem->setText(modelLine->data(modelLine->index(row, 54)).toString());

    lineEditClassNomOperPress->setText(modelLine->data(modelLine->index(row, 55)).toString());
    lineEditClassNomOperTemp->setText(modelLine->data(modelLine->index(row, 56)).toString());
    lineEditClassMaxDesPress->setText(modelLine->data(modelLine->index(row, 57)).toString());
    lineEditClassMaxDesTemp->setText(modelLine->data(modelLine->index(row, 58)).toString());
    lineEditClassMinDesTemp->setText(modelLine->data(modelLine->index(row, 59)).toString());
    lineEditClassAuthorRegul->setText(modelLine->data(modelLine->index(row, 60)).toString());

    str = modelLine->data(modelLine->index(row, 61)).toString();
    comboBoxClassFluidDangerClassGroup->setCurrentIndex(comboBoxClassFluidDangerClassGroup->findText(str));
    
    str = modelLine->data(modelLine->index(row, 62)).toString();
    comboBoxClassGasStability->setCurrentIndex(comboBoxClassGasStability->findText(str));

    lineEditClassRegulClassGroup->setText(modelLine->data(modelLine->index(row, 63)).toString());
    lineEditClassRegulSubClassGroup->setText(modelLine->data(modelLine->index(row, 64)).toString());

    QDate date;
	dateEditOperInstallation->setDate(date.fromString(modelLine->data(modelLine->index(row, 65)).toString(), Qt::ISODate));

    str = modelLine->data(modelLine->index(row, 66)).toString();
	if (str == "true") {
		checkBoxOperLineOutOfService->setChecked(true);
	} else {
		checkBoxOperLineOutOfService->setChecked(false);
	}

    str = modelLine->data(modelLine->index(row, 67)).toString();
	if (str == "true") {
		checkBoxOperPeriodicInspectionRequired->setChecked(true);
	} else {
		checkBoxOperPeriodicInspectionRequired->setChecked(false);
	}

    lineEditOperInspectionLoopNumber->setText(modelLine->data(modelLine->index(row, 68)).toString());
    lineEditOperInspectionFrequency->setText(modelLine->data(modelLine->index(row, 69)).toString());

    textEditRemProcess->setPlainText(modelLine->data(modelLine->index(row, 70)).toString());
    textEditRemEnginDes->setPlainText(modelLine->data(modelLine->index(row, 71)).toString());
    textEditRemClassification->setPlainText(modelLine->data(modelLine->index(row, 72)).toString());
    textEditRemOperMaint->setPlainText(modelLine->data(modelLine->index(row, 73)).toString());
    textEditRemCustom->setPlainText(modelLine->data(modelLine->index(row, 74)).toString());

    lineEditCustValue1->setText(modelLine->data(modelLine->index(row, 75)).toString());
    lineEditCustValue2->setText(modelLine->data(modelLine->index(row, 76)).toString());
    lineEditCustValue3->setText(modelLine->data(modelLine->index(row, 77)).toString());
    lineEditCustValue4->setText(modelLine->data(modelLine->index(row, 78)).toString());
    lineEditCustValue5->setText(modelLine->data(modelLine->index(row, 79)).toString());
    lineEditCustText6->setText(modelLine->data(modelLine->index(row, 80)).toString());
    lineEditCustText7->setText(modelLine->data(modelLine->index(row, 81)).toString());
    lineEditCustText8->setText(modelLine->data(modelLine->index(row, 82)).toString());
    lineEditCustText9->setText(modelLine->data(modelLine->index(row, 83)).toString());
    lineEditCustText10->setText(modelLine->data(modelLine->index(row, 84)).toString());
}


void MainWindow::setLineFieldDecoration() {
	// RB_DEBUG->print("MainWindow::setLineFieldDecoration()");
	int row = tableView->currentIndex().row();
	
	RB_String changeStr = modelLine->data(modelLine->index(row, 3),
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditLineNo->setPalette(revPalette);
	} else {
	    lineEditLineNo->setPalette(this->palette());
	}

	changeStr = modelLine->data(modelLine->index(row, 4), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditLineRev->setPalette(revPalette);
	} else {
	    lineEditLineRev->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 5), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditSize->setPalette(revPalette);
	} else {
	    lineEditSize->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 6), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxProcessUtility->setPalette(revPalette);
	} else {
	    comboBoxProcessUtility->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 7), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditFrom->setPalette(revPalette);
	} else {
	    lineEditFrom->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 8), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxFromType->setPalette(revPalette);
	} else {
	    comboBoxFromType->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 9), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditFromNozzle->setPalette(revPalette);
	} else {
	    lineEditFromNozzle->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 10), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditTo->setPalette(revPalette);
	} else {
	    lineEditTo->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 11), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxToType->setPalette(revPalette);
	} else {
	    comboBoxToType->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 12), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditToNozzle->setPalette(revPalette);
	} else {
	    lineEditToNozzle->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 13), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditFluidID->setPalette(revPalette);
	} else {
	    lineEditFluidID->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 16), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditPd->setPalette(revPalette);
	} else {
	    lineEditPd->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 14), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditPipeClass->setPalette(revPalette);
	} else {
	    lineEditPipeClass->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 15), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditTd->setPalette(revPalette);
	} else {
	    lineEditTd->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 17), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessFluidDescr->setPalette(revPalette);
	} else {
	    lineEditProcessFluidDescr->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 18), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessOperPress->setPalette(revPalette);
	} else {
	    lineEditProcessOperPress->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 19), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessOperTemp->setPalette(revPalette);
	} else {
	    lineEditProcessOperTemp->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 20), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessOperTempLow->setPalette(revPalette);
	} else {
	    lineEditProcessOperTempLow->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 21), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessOperTempHigh->setPalette(revPalette);
	} else {
	    lineEditProcessOperTempHigh->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 22), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessTestMedium->setPalette(revPalette);
	} else {
	    lineEditProcessTestMedium->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 23), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessTestPress->setPalette(revPalette);
	} else {
	    lineEditProcessTestPress->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 24), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessPressDrop->setPalette(revPalette);
	} else {
	    lineEditProcessPressDrop->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 25), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessMassFlowRate->setPalette(revPalette);
	} else {
	    lineEditProcessMassFlowRate->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 26), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessMassFlowRateMax->setPalette(revPalette);
	} else {
	    lineEditProcessMassFlowRateMax->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 27), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessVelocity->setPalette(revPalette);
	} else {
	    lineEditProcessVelocity->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 28), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxProcessPhase->setPalette(revPalette);
	} else {
	    comboBoxProcessPhase->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 29), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessBoilingPoint->setPalette(revPalette);
	} else {
	    lineEditProcessBoilingPoint->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 30), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessVapourPress->setPalette(revPalette);
	} else {
	    lineEditProcessVapourPress->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 31), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessMolWeight->setPalette(revPalette);
	} else {
	    lineEditProcessMolWeight->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 32), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessDensityLiquid->setPalette(revPalette);
	} else {
	    lineEditProcessDensityLiquid->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 33), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessDensityVapour->setPalette(revPalette);
	} else {
	    lineEditProcessDensityVapour->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 34), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessViscosityLiquid->setPalette(revPalette);
	} else {
	    lineEditProcessViscosityLiquid->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 35), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessViscosityVapour->setPalette(revPalette);
	} else {
	    lineEditProcessViscosityVapour->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 36), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessStandardVolumeFlow->setPalette(revPalette);
	} else {
	    lineEditProcessStandardVolumeFlow->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 37), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessStandardVolumeFlowMax->setPalette(revPalette);
	} else {
	    lineEditProcessStandardVolumeFlowMax->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 38), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessActualFlow->setPalette(revPalette);
	} else {
	    lineEditProcessActualFlow->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 39), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessActualFlowMax->setPalette(revPalette);
	} else {
	    lineEditProcessActualFlowMax->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 40), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditProcessSimulation->setPalette(revPalette);
	} else {
	    lineEditProcessSimulation->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 41), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesPnidFrom->setPalette(revPalette);
	} else {
	    lineEditDesPnidFrom->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 42), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesPnidTo->setPalette(revPalette);
	} else {
	    lineEditDesPnidTo->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 43), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesGaFrom->setPalette(revPalette);
	} else {
	    lineEditDesGaFrom->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 44), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesGaTo->setPalette(revPalette);
	} else {
	    lineEditDesGaTo->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 45), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesIsoFrom->setPalette(revPalette);
	} else {
	    lineEditDesIsoFrom->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 46), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesIsoTo->setPalette(revPalette);
	} else {
	    lineEditDesIsoTo->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 47), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxDesLineFunction->setPalette(revPalette);
	} else {
	    comboBoxDesLineFunction->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 48), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxDesExternalPressure->setPalette(revPalette);
	} else {
	    comboBoxDesExternalPressure->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 49), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxDesTracingType->setPalette(revPalette);
	} else {
	    comboBoxDesTracingType->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 50), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesTracingNumberOf->setPalette(revPalette);
	} else {
	    lineEditDesTracingNumberOf->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 51), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxDesInsulationType->setPalette(revPalette);
	} else {
	    comboBoxDesInsulationType->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 52), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesInsulationThickness->setPalette(revPalette);
	} else {
	    lineEditDesInsulationThickness->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 53), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		checkBoxDesPaintingAsPerSpec->setPalette(revPalette);
	} else {
	    checkBoxDesPaintingAsPerSpec->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 54), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditDesPaintingSystem->setPalette(revPalette);
	} else {
	    lineEditDesPaintingSystem->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 55), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassNomOperPress->setPalette(revPalette);
	} else {
	    lineEditClassNomOperPress->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 56), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassNomOperTemp->setPalette(revPalette);
	} else {
	    lineEditClassNomOperTemp->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 57), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassMaxDesPress->setPalette(revPalette);
	} else {
	    lineEditClassMaxDesPress->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 58), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassMaxDesTemp->setPalette(revPalette);
	} else {
	    lineEditClassMaxDesTemp->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 59), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassMinDesTemp->setPalette(revPalette);
	} else {
	    lineEditClassMinDesTemp->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 60), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassAuthorRegul->setPalette(revPalette);
	} else {
	    lineEditClassAuthorRegul->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 61), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxClassFluidDangerClassGroup->setPalette(revPalette);
	} else {
	    comboBoxClassFluidDangerClassGroup->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 62), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		comboBoxClassGasStability->setPalette(revPalette);
	} else {
	    comboBoxClassGasStability->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 63), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassRegulClassGroup->setPalette(revPalette);
	} else {
	    lineEditClassRegulClassGroup->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 64), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditClassRegulSubClassGroup->setPalette(revPalette);
	} else {
	    lineEditClassRegulSubClassGroup->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 65), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		dateEditOperInstallation->setPalette(revPalette);
	} else {
	    dateEditOperInstallation->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 66), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		checkBoxOperLineOutOfService->setPalette(revPalette);
	} else {
	    checkBoxOperLineOutOfService->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 67), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		checkBoxOperPeriodicInspectionRequired->setPalette(revPalette);
	} else {
	    checkBoxOperPeriodicInspectionRequired->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 68), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditOperInspectionLoopNumber->setPalette(revPalette);
	} else {
	    lineEditOperInspectionLoopNumber->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 69), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditOperInspectionFrequency->setPalette(revPalette);
	} else {
	    lineEditOperInspectionFrequency->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 70), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		textEditRemProcess->setPalette(revPalette);
	} else {
	    textEditRemProcess->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 71), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		textEditRemEnginDes->setPalette(revPalette);
	} else {
	    textEditRemEnginDes->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 72), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		textEditRemClassification->setPalette(revPalette);
	} else {
	    textEditRemClassification->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 73), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		textEditRemOperMaint->setPalette(revPalette);
	} else {
	    textEditRemOperMaint->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 74), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		textEditRemCustom->setPalette(revPalette);
	} else {
	    textEditRemCustom->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 75), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustValue1->setPalette(revPalette);
	} else {
	    lineEditCustValue1->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 76), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustValue2->setPalette(revPalette);
	} else {
	    lineEditCustValue2->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 77), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustValue3->setPalette(revPalette);
	} else {
	    lineEditCustValue3->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 78), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustValue4->setPalette(revPalette);
	} else {
	    lineEditCustValue4->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 79), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustValue5->setPalette(revPalette);
	} else {
	    lineEditCustValue5->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 80), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustText6->setPalette(revPalette);
	} else {
	    lineEditCustText6->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 81), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustText7->setPalette(revPalette);
	} else {
	    lineEditCustText7->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 82), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustText8->setPalette(revPalette);
	} else {
	    lineEditCustText8->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 83), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustText9->setPalette(revPalette);
	} else {
	    lineEditCustText9->setPalette(this->palette());
	}
	changeStr = modelLine->data(modelLine->index(row, 84), 
							Qt::DecorationRole).toString();
	if (changeStr == "valueChanged") {
		lineEditCustText10->setPalette(revPalette);
	} else {
	    lineEditCustText10->setPalette(this->palette());
	}
}


void MainWindow::clearProjectFields() {
    RB_DEBUG->print("mainWindow::clearProjectFields()");
	
    lineEditProjectNo->setText("");
    lineEditProjectRev->setText("");

	// fields in tab custom
    lineEditCustLabel1->setText("");
    lineEditCustLabel2->setText("");
    lineEditCustLabel3->setText("");
    lineEditCustLabel4->setText("");
    lineEditCustLabel5->setText("");
    lineEditCustLabel6->setText("");
    lineEditCustLabel7->setText("");
    lineEditCustLabel8->setText("");
    lineEditCustLabel9->setText("");
    lineEditCustLabel10->setText("");
    lineEditCustUnit1->setText("");
    lineEditCustUnit2->setText("");
    lineEditCustUnit3->setText("");
    lineEditCustUnit4->setText("");
    lineEditCustUnit5->setText("");
}


void MainWindow::clearLineFields() {
    RB_DEBUG->print("mainWindow::clearLineFields()");
	
    // lineEditLineNo->clear(); clear() replaced by setText("") 
    //   to prevent trigger of textEdited()
	lineEditLineNo->setText("");
	lineEditLineNo->setPalette(this->palette());
    lineEditLineRev->setText("");
    lineEditLineRev->setPalette(this->palette());
    lineEditSize->setText("");
    lineEditSize->setPalette(this->palette());
    comboBoxProcessUtility->setCurrentIndex(0);
    comboBoxProcessUtility->setPalette(this->palette());
    lineEditFrom->setText("");
    lineEditFrom->setPalette(this->palette());
    comboBoxFromType->setCurrentIndex(0);
    comboBoxFromType->setPalette(this->palette());
    lineEditFromNozzle->setText("");
    lineEditFromNozzle->setPalette(this->palette());
    lineEditTo->setText("");
    lineEditTo->setPalette(this->palette());
    comboBoxToType->setCurrentIndex(0);
    comboBoxToType->setPalette(this->palette());
    lineEditToNozzle->setText("");
    lineEditToNozzle->setPalette(this->palette());
    lineEditFluidID->setText("");
    lineEditFluidID->setPalette(this->palette());
    lineEditPd->setText("");
    lineEditPd->setPalette(this->palette());
    lineEditPipeClass->setText("");
    lineEditPipeClass->setPalette(this->palette());
    lineEditTd->setText("");
    lineEditTd->setPalette(this->palette());

    // detail data 
    lineEditProcessFluidDescr->setText("");
    lineEditProcessFluidDescr->setPalette(this->palette());
    lineEditProcessOperPress->setText("");
    lineEditProcessOperPress->setPalette(this->palette());
    lineEditProcessOperTemp->setText("");
    lineEditProcessOperTemp->setPalette(this->palette());
    lineEditProcessOperTempLow->setText("");
    lineEditProcessOperTempLow->setPalette(this->palette());
    lineEditProcessOperTempHigh->setText("");
    lineEditProcessOperTempHigh->setPalette(this->palette());
    lineEditProcessTestMedium->setText("");
    lineEditProcessTestMedium->setPalette(this->palette());
    lineEditProcessTestPress->setText("");
    lineEditProcessTestPress->setPalette(this->palette());
    lineEditProcessPressDrop->setText("");
    lineEditProcessPressDrop->setPalette(this->palette());
    lineEditProcessMassFlowRate->setText("");
    lineEditProcessMassFlowRate->setPalette(this->palette());
    lineEditProcessMassFlowRateMax->setText("");
    lineEditProcessMassFlowRateMax->setPalette(this->palette());
    lineEditProcessVelocity->setText("");
    lineEditProcessVelocity->setPalette(this->palette());
    comboBoxProcessPhase->setCurrentIndex(0);
    comboBoxProcessPhase->setPalette(this->palette());
    lineEditProcessBoilingPoint->setText("");
    lineEditProcessBoilingPoint->setPalette(this->palette());
    lineEditProcessVapourPress->setText("");
    lineEditProcessVapourPress->setPalette(this->palette());
    lineEditProcessMolWeight->setText("");
    lineEditProcessMolWeight->setPalette(this->palette());
    lineEditProcessDensityLiquid->setText("");
    lineEditProcessDensityLiquid->setPalette(this->palette());
    lineEditProcessDensityVapour->setText("");
    lineEditProcessDensityVapour->setPalette(this->palette());
    lineEditProcessViscosityLiquid->setText("");
    lineEditProcessViscosityLiquid->setPalette(this->palette());
    lineEditProcessViscosityVapour->setText("");
    lineEditProcessViscosityVapour->setPalette(this->palette());
    lineEditProcessStandardVolumeFlow->setText("");
    lineEditProcessStandardVolumeFlow->setPalette(this->palette());
    lineEditProcessStandardVolumeFlowMax->setText("");
    lineEditProcessStandardVolumeFlowMax->setPalette(this->palette());
    lineEditProcessActualFlow->setText("");
    lineEditProcessActualFlow->setPalette(this->palette());
    lineEditProcessActualFlowMax->setText("");
    lineEditProcessActualFlowMax->setPalette(this->palette());
    lineEditProcessSimulation->setText("");
    lineEditProcessSimulation->setPalette(this->palette());
    lineEditDesPnidFrom->setText("");
    lineEditDesPnidFrom->setPalette(this->palette());
    lineEditDesPnidTo->setText("");
    lineEditDesPnidTo->setPalette(this->palette());
    lineEditDesGaFrom->setText("");
    lineEditDesGaFrom->setPalette(this->palette());
    lineEditDesGaTo->setText("");
    lineEditDesGaTo->setPalette(this->palette());
    lineEditDesIsoFrom->setText("");
    lineEditDesIsoFrom->setPalette(this->palette());
    lineEditDesIsoTo->setText("");
    lineEditDesIsoTo->setPalette(this->palette());
    comboBoxDesLineFunction->setCurrentIndex(0);
    comboBoxDesLineFunction->setPalette(this->palette());
    comboBoxDesExternalPressure->setCurrentIndex(0);
    comboBoxDesExternalPressure->setPalette(this->palette());
    comboBoxDesTracingType->setCurrentIndex(0);
    comboBoxDesTracingType->setPalette(this->palette());
    lineEditDesTracingNumberOf->setText("");
    lineEditDesTracingNumberOf->setPalette(this->palette());
    comboBoxDesInsulationType->setCurrentIndex(0);
    comboBoxDesInsulationType->setPalette(this->palette());
    lineEditDesInsulationThickness->setText("");
    lineEditDesInsulationThickness->setPalette(this->palette());
    checkBoxDesPaintingAsPerSpec->setChecked(false);
    checkBoxDesPaintingAsPerSpec->setPalette(this->palette());
    lineEditDesPaintingSystem->setText("");
    lineEditDesPaintingSystem->setPalette(this->palette());
	comboBoxClassFluidDangerClassGroup->setCurrentIndex(0);
	comboBoxClassFluidDangerClassGroup->setPalette(this->palette());
    comboBoxClassGasStability->setCurrentIndex(0);
    comboBoxClassGasStability->setPalette(this->palette());
    lineEditClassRegulSubClassGroup->setText("");
    lineEditClassRegulSubClassGroup->setPalette(this->palette());
    lineEditClassRegulClassGroup->setText("");
    lineEditClassRegulClassGroup->setPalette(this->palette());
    lineEditClassAuthorRegul->setText("");
    lineEditClassAuthorRegul->setPalette(this->palette());
    lineEditClassMinDesTemp->setText("");
    lineEditClassMinDesTemp->setPalette(this->palette());
    lineEditClassMaxDesTemp->setText("");
    lineEditClassMaxDesTemp->setPalette(this->palette());
    lineEditClassMaxDesPress->setText("");
    lineEditClassMaxDesPress->setPalette(this->palette());
    lineEditClassNomOperTemp->setText("");
    lineEditClassNomOperTemp->setPalette(this->palette());
    lineEditClassNomOperPress->setText("");
    lineEditClassNomOperPress->setPalette(this->palette());
    dateEditOperInstallation->clear();
    dateEditOperInstallation->setPalette(this->palette());
    checkBoxOperPeriodicInspectionRequired->setChecked(false);
    checkBoxOperPeriodicInspectionRequired->setPalette(this->palette());
    checkBoxOperLineOutOfService->setChecked(false);
    checkBoxOperLineOutOfService->setPalette(this->palette());
    lineEditOperInspectionLoopNumber->setText("");
    lineEditOperInspectionLoopNumber->setPalette(this->palette());
    lineEditOperInspectionFrequency->setText("");
    lineEditOperInspectionFrequency->setPalette(this->palette());

	// tvInspectionItem->reset();
	// TODO: is the best workaround, it does not work well with in-memory model
	if (modelInspectionItem) {
		modelInspectionItem->setFilter("name = 'none'"); // name is alway LDT_..
		modelInspectionItem->select();
		tvInspectionItem->setModel(modelInspectionItem);
		formatTvInspectionItem();
	}

    textEditRemProcess->clear();
    textEditRemProcess->setPalette(this->palette());
    textEditRemEnginDes->clear();
    textEditRemEnginDes->setPalette(this->palette());
    textEditRemClassification->clear();
    textEditRemClassification->setPalette(this->palette());
    textEditRemOperMaint->clear();
    textEditRemOperMaint->setPalette(this->palette());
    textEditRemCustom->clear();
    textEditRemCustom->setPalette(this->palette());
    lineEditCustValue1->setText("");
    lineEditCustValue1->setPalette(this->palette());
    lineEditCustValue2->setText("");
    lineEditCustValue2->setPalette(this->palette());
    lineEditCustValue3->setText("");
    lineEditCustValue3->setPalette(this->palette());
    lineEditCustValue4->setText("");
    lineEditCustValue4->setPalette(this->palette());
    lineEditCustValue5->setText("");
    lineEditCustValue5->setPalette(this->palette());
    lineEditCustText6->setText("");
    lineEditCustText6->setPalette(this->palette());
    lineEditCustText7->setText("");
    lineEditCustText7->setPalette(this->palette());
    lineEditCustText8->setText("");
    lineEditCustText8->setPalette(this->palette());
    lineEditCustText9->setText("");
    lineEditCustText9->setPalette(this->palette());
    lineEditCustText10->setText("");
    lineEditCustText10->setPalette(this->palette());
}

/**
 * add new inspection item for this line
 */
void MainWindow::on_pushButtonNewInspItem_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonNewInspItem_clicked()");

	if (!tableView->currentIndex().isValid()) {
		return;
	}

	if (tvInspectionItem->currentIndex().isValid()) {
		modelInspectionItem->insertRows(tvInspectionItem->currentIndex().row() + 1, 
													1, QModelIndex());
		tvInspectionItem->selectRow(tvInspectionItem->currentIndex().row() + 1);
	} else {
		// at the end of the container if no row is selected
		modelInspectionItem->insertRows(modelInspectionItem->rowCount(), 1, 
													QModelIndex());
		tvInspectionItem->selectRow(modelInspectionItem->rowCount() - 1);
	}
	
	// first item needs to set the table formating
	if (modelInspectionItem->rowCount() == 1)
		formatTvInspectionItem();
}


void MainWindow::on_pushButtonDeleteInspItem_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonDeleteInspItem_clicked()");

	if (!tableView->currentIndex().isValid()) {
		return;
	}

	if (tvInspectionItem->currentIndex().isValid()) {
	    int ret = QMessageBox::warning(this, tr("Delete Action"),
						tr("You will NOT be able to undo this action.\n"
	                    "Are you sure you want to delete the item?"),
	                 	QMessageBox::Yes | QMessageBox::Default,
	                 	QMessageBox::No | QMessageBox::Escape);
		if (ret == QMessageBox::No) {
			return;
		}
		modelInspectionItem->removeRows(tvInspectionItem->currentIndex().row(), 
															1, QModelIndex());
	}
}


void MainWindow::on_pushButtonUpInspItem_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonUpInspItem_clicked()");

	if (!tableView->currentIndex().isValid()) {
		return;
	}

	QModelIndex index = tvInspectionItem->currentIndex();
	if (index.isValid() && index.row() > 0) {
		modelInspectionItem->swapObject(index.row(), index.row() - 1);
		tvInspectionItem->selectRow(index.row() - 1);
	}
}


void MainWindow::on_pushButtonDownInspItem_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonDownInspItem_clicked()");

	if (!tableView->currentIndex().isValid()) {
		return;
	}

	QModelIndex index = tvInspectionItem->currentIndex();
	if (index.isValid() && index.row() < modelInspectionItem->rowCount()) {
		modelInspectionItem->swapObject(index.row(), index.row() + 1);
		tvInspectionItem->selectRow(index.row() + 1);
	}
}

/**
 * Open (modal) inspection item dialog, with new and show() it would be non modal.
 * A clone of the relevant part of the modelLine is created in dialog 
 */
void MainWindow::on_pushButtonEditInspItem_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonEditInspItem_clicked()");
	if (!tableView->currentIndex().isValid()) {
		statusBar()->showMessage("No line selected - Ready", 2000);
		return;
	}

	if (tvInspectionItem->model()->rowCount() == 0) {
		statusBar()->showMessage("No inspection items - Ready", 2000);
		return;
	}

//	int row = tvInspectionItem->currentIndex().row();
//	if (row >= 0 && row < tvInspectionItem->model()->rowCount()) {
//		// current row of inspection item needs to be updated here
//		modelInspectionItem->changeCurrent(row);
//		
	    InspectionDialog dialog(this, modelInspectionItem);
		dialog.exec();
		
//		if (modelInspectionItem->isModelModified()) {
//			modelLine->modelIsModified(true);
//		}
//		
//		updateMenus();
//		
//	} else {
//		statusBar()->showMessage("No inspection item selected - Ready", 2000);
//	}
}


/**
 * slots for changes in lineEdit, textEdit, date and checkBox
 */
void MainWindow::on_lineEditLineNo_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 3, QModelIndex()), lineEditLineNo->text());
}

void MainWindow::on_lineEditLineRev_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 4, QModelIndex()), lineEditLineRev->text());
}

void MainWindow::on_lineEditSize_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 5, QModelIndex()), lineEditSize->text());
}

void MainWindow::on_comboBoxProcessUtility_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 6, QModelIndex()), 
								comboBoxProcessUtility->currentText());
}

void MainWindow::on_lineEditFrom_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 7, QModelIndex()), lineEditFrom->text());
}

void MainWindow::on_comboBoxFromType_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 8, QModelIndex()), 
								comboBoxFromType->currentText());
}
	
void MainWindow::on_lineEditFromNozzle_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 9, QModelIndex()), lineEditFromNozzle->text());
}
	
void MainWindow::on_lineEditTo_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 10, QModelIndex()), lineEditTo->text());
}
	
void MainWindow::on_comboBoxToType_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 11, QModelIndex()), 
								comboBoxToType->currentText());
}
	
void MainWindow::on_lineEditToNozzle_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 12, QModelIndex()), lineEditToNozzle->text());
}
	
void MainWindow::on_lineEditFluidID_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 13, QModelIndex()), lineEditFluidID->text());
}
	
void MainWindow::on_lineEditPipeClass_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 14, QModelIndex()), lineEditPipeClass->text());
}
	
void MainWindow::on_lineEditTd_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 15, QModelIndex()), lineEditTd->text());
}
	
void MainWindow::on_lineEditPd_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 16, QModelIndex()), lineEditPd->text());
}
	
	
	// detail data
void MainWindow::on_lineEditProcessFluidDescr_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 17, QModelIndex()), lineEditProcessFluidDescr->text());
}
	
void MainWindow::on_lineEditProcessOperPress_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 18, QModelIndex()), lineEditProcessOperPress->text());
}
	
void MainWindow::on_lineEditProcessOperTemp_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 19, QModelIndex()), lineEditProcessOperTemp->text());
}
	
void MainWindow::on_lineEditProcessOperTempLow_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 20, QModelIndex()), lineEditProcessOperTempLow->text());
}
	
void MainWindow::on_lineEditProcessOperTempHigh_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 21, QModelIndex()), lineEditProcessOperTempHigh->text());
}
	
void MainWindow::on_lineEditProcessTestMedium_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 22, QModelIndex()), lineEditProcessTestMedium->text());
}
	
void MainWindow::on_lineEditProcessTestPress_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 23, QModelIndex()), lineEditProcessTestPress->text());
}
	
void MainWindow::on_lineEditProcessPressDrop_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 24, QModelIndex()), lineEditProcessPressDrop->text());
}
	
void MainWindow::on_lineEditProcessMassFlowRate_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 25, QModelIndex()), lineEditProcessMassFlowRate->text());
}
	
void MainWindow::on_lineEditProcessMassFlowRateMax_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 26, QModelIndex()), lineEditProcessMassFlowRateMax->text());
}
	
void MainWindow::on_lineEditProcessVelocity_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 27, QModelIndex()), lineEditProcessVelocity->text());
}
	
void MainWindow::on_comboBoxProcessPhase_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 28, QModelIndex()), comboBoxProcessPhase->currentText());
}
	
void MainWindow::on_lineEditProcessBoilingPoint_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 29, QModelIndex()), lineEditProcessBoilingPoint->text());
}
	
void MainWindow::on_lineEditProcessVapourPress_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 30, QModelIndex()), lineEditProcessVapourPress->text());
}
	
void MainWindow::on_lineEditProcessMolWeight_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 31, QModelIndex()), lineEditProcessMolWeight->text());
}
	
void MainWindow::on_lineEditProcessDensityLiquid_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 32, QModelIndex()), lineEditProcessDensityLiquid->text());
}
	
void MainWindow::on_lineEditProcessDensityVapour_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 33, QModelIndex()), lineEditProcessDensityVapour->text());
}
	
void MainWindow::on_lineEditProcessViscosityLiquid_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 34, QModelIndex()), lineEditProcessViscosityLiquid->text());
}
	
void MainWindow::on_lineEditProcessViscosityVapour_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 35, QModelIndex()), lineEditProcessViscosityVapour->text());
}
	
void MainWindow::on_lineEditProcessStandardVolumeFlow_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 36, QModelIndex()), lineEditProcessStandardVolumeFlow->text());
}
	
void MainWindow::on_lineEditProcessStandardVolumeFlowMax_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 37, QModelIndex()), lineEditProcessStandardVolumeFlowMax->text());
}
	
void MainWindow::on_lineEditProcessActualFlow_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 38, QModelIndex()), lineEditProcessActualFlow->text());
}
	
void MainWindow::on_lineEditProcessActualFlowMax_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 39, QModelIndex()), lineEditProcessActualFlowMax->text());
}
	
void MainWindow::on_lineEditProcessSimulation_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 40, QModelIndex()), lineEditProcessSimulation->text());
}
	
void MainWindow::on_lineEditDesPnidFrom_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 41, QModelIndex()), lineEditDesPnidFrom->text());
}
	
void MainWindow::on_lineEditDesPnidTo_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 42, QModelIndex()), lineEditDesPnidTo->text());
}
	
void MainWindow::on_lineEditDesGaFrom_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 43, QModelIndex()), lineEditDesGaFrom->text());
}
	
void MainWindow::on_lineEditDesGaTo_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 44, QModelIndex()), lineEditDesGaTo->text());
}
	
void MainWindow::on_lineEditDesIsoFrom_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 45, QModelIndex()), lineEditDesIsoFrom->text());
}
	
void MainWindow::on_lineEditDesIsoTo_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 46, QModelIndex()), lineEditDesIsoTo->text());
}
	
void MainWindow::on_comboBoxDesLineFunction_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 47, QModelIndex()), comboBoxDesLineFunction->currentText());
}
	
void MainWindow::on_comboBoxDesExternalPressure_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 48, QModelIndex()), comboBoxDesExternalPressure->currentText());
}
	
void MainWindow::on_comboBoxDesTracingType_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 49, QModelIndex()), comboBoxDesTracingType->currentText());
}
	
void MainWindow::on_lineEditDesTracingNumberOf_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 50, QModelIndex()), lineEditDesTracingNumberOf->text());
}
	
void MainWindow::on_comboBoxDesInsulationType_activated(const QString&) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 51, QModelIndex()), comboBoxDesInsulationType->currentText());
}
	
void MainWindow::on_lineEditDesInsulationThickness_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 52, QModelIndex()), lineEditDesInsulationThickness->text());
}
	
void MainWindow::on_checkBoxDesPaintingAsPerSpec_released() {  
	if (checkBoxDesPaintingAsPerSpec->isChecked()) {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 53, QModelIndex()), "true");
	} else {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 53, QModelIndex()), "false");
	}
}
	
void MainWindow::on_lineEditDesPaintingSystem_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 54, QModelIndex()), lineEditDesPaintingSystem->text());
}
	
void MainWindow::on_lineEditClassNomOperPress_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 55, QModelIndex()), lineEditClassNomOperPress->text());
}
	
void MainWindow::on_lineEditClassNomOperTemp_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 56, QModelIndex()), lineEditClassNomOperTemp->text());
}
	
void MainWindow::on_lineEditClassMaxDesPress_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 57, QModelIndex()), lineEditClassMaxDesPress->text());
}
	
void MainWindow::on_lineEditClassMaxDesTemp_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 58, QModelIndex()), lineEditClassMaxDesTemp->text());
}
	
void MainWindow::on_lineEditClassMinDesTemp_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 59, QModelIndex()), lineEditClassMinDesTemp->text());
}
	
void MainWindow::on_lineEditClassAuthorRegul_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 60, QModelIndex()), lineEditClassAuthorRegul->text());
}

void MainWindow::on_comboBoxClassFluidDangerClassGroup_activated(const QString& /*str*/) {
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 61, QModelIndex()), comboBoxClassFluidDangerClassGroup->currentText());
}

void MainWindow::on_comboBoxClassGasStability_activated(const QString& /*str*/) {
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 62, QModelIndex()), comboBoxClassGasStability->currentText());
}


void MainWindow::on_lineEditClassRegulClassGroup_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 63, QModelIndex()), lineEditClassRegulClassGroup->text());
}
	
void MainWindow::on_lineEditClassRegulSubClassGroup_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 64, QModelIndex()), lineEditClassRegulSubClassGroup->text());
}
	
void MainWindow::on_dateEditOperInstallation_dateChanged(const QDate& /*date*/) {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 65, QModelIndex()), 
					dateEditOperInstallation->text()); //date.toString(Qt::ISODate));
}
	
void MainWindow::on_checkBoxOperLineOutOfService_released() {  
	if (checkBoxOperLineOutOfService->isChecked()) {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 66, QModelIndex()), "true");
	} else {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 66, QModelIndex()), "false");
	}
}
	
void MainWindow::on_checkBoxOperPeriodicInspectionRequired_released() {  
	if (checkBoxOperPeriodicInspectionRequired->isChecked()) {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 67, QModelIndex()), "true");
	} else {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 67, QModelIndex()), "false");
	}
}
	
void MainWindow::on_pushButtonLoopNumber_clicked() {  
	RB_DEBUG->print("MainWindow::on_pushButtonLoopNumber_clicked()");
	if (!tableView->currentIndex().isValid()) {
		return;
	}
    
    // modelLine->createActiveModel(NULL, NULL, "LDT_InspectionLoopList", "");
    InspectionLoopDialog dialog(this);

	if (dialog.exec() == QDialog::Accepted && dialog.loopNumber >= 0) {
		QString strLoopNumber = QString::number(dialog.loopNumber);
		lineEditOperInspectionLoopNumber->setText(strLoopNumber);
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 68, 
						QModelIndex()), strLoopNumber);
	}

	updateMenus();
	// modelLine->deleteActiveModel();
}


void MainWindow::on_pushButtonClearNumber_clicked() {
	RB_DEBUG->print("MainWindow::on_pushButtonClearNumber_clicked()");
	if (!tableView->currentIndex().isValid()) {
		return;
	}
    
	int ret = QMessageBox::warning(this, tr("LDT Inspection Loop Number"),
                     tr("Are you sure you want to remove the loop number?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		modelLine->setData(modelLine->index(tableView->currentIndex().row(), 68, 
						QModelIndex()), 0);
	}
}

	
void MainWindow::on_lineEditOperInspectionFrequency_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 69, QModelIndex()), 
					lineEditOperInspectionFrequency->text());
}
	
	
void MainWindow::on_textEditRemProcess_cursorPositionChanged() {  
	// second workaround to prevent infinite loop on textEdits
	if (!textEditRemProcess->hasFocus()) {
		return;
	}
	
	modelLine->modelIsModified(true);
	textEditRemProcessChanged = true;
}
	
void MainWindow::saveTextEditRemProcess(int row) {  
	if (textEditRemProcessChanged) {
		modelLine->setData(modelLine->index(row, 70, QModelIndex()), 
							textEditRemProcess->toPlainText());
		textEditRemProcessChanged = false;
	}
}
	
void MainWindow::on_textEditRemEnginDes_cursorPositionChanged() {  
	if (!textEditRemEnginDes->hasFocus()) {
		return;
	}

	modelLine->modelIsModified(true);
	textEditRemEnginDesChanged = true;
}
	
void MainWindow::saveTextEditRemEnginDes(int row) {  
	if (textEditRemEnginDesChanged) {
		modelLine->setData(modelLine->index(row, 71, QModelIndex()), 
							textEditRemEnginDes->toPlainText());
		textEditRemEnginDesChanged = false;
	}
}
	
void MainWindow::on_textEditRemClassification_cursorPositionChanged() {  
	if (!textEditRemClassification->hasFocus()) {
		return;
	}

	modelLine->modelIsModified(true);
	textEditRemClassificationChanged = true;
}
	
void MainWindow::saveTextEditRemClassification(int row) {  
	if (textEditRemClassificationChanged) {
		modelLine->setData(modelLine->index(row, 72, QModelIndex()), 
							textEditRemClassification->toPlainText());
		textEditRemClassificationChanged = false;
	}
}
	
void MainWindow::on_textEditRemOperMaint_cursorPositionChanged() {  
	if (!textEditRemOperMaint->hasFocus()) {
		return;
	}

	modelLine->modelIsModified(true);
	textEditRemOperMaintChanged = true;
}
	
void MainWindow::saveTextEditRemOperMaint(int row) {  
	if (textEditRemOperMaintChanged) {
		modelLine->setData(modelLine->index(row, 73, QModelIndex()), 
							textEditRemOperMaint->toPlainText());
		textEditRemOperMaintChanged = false;
	}
}
	
void MainWindow::on_textEditRemCustom_cursorPositionChanged() {  
	if (!textEditRemCustom->hasFocus()) {
		return;
	}
	
	modelLine->modelIsModified(true);
	textEditRemCustomChanged = true;
}

void MainWindow::saveTextEditRemCustom(int row) {  
	if (textEditRemCustomChanged) {
		modelLine->setData(modelLine->index(row, 74, QModelIndex()), 
							textEditRemCustom->toPlainText());
		textEditRemCustomChanged = false;
	}
}

void MainWindow::on_lineEditCustValue1_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 75, QModelIndex()), lineEditCustValue1->text());
}
	
void MainWindow::on_lineEditCustValue2_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 76, QModelIndex()), lineEditCustValue2->text());
}
	
void MainWindow::on_lineEditCustValue3_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 77, QModelIndex()), lineEditCustValue3->text());
}
	
void MainWindow::on_lineEditCustValue4_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 78, QModelIndex()), lineEditCustValue4->text());
}
	
void MainWindow::on_lineEditCustValue5_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 79, QModelIndex()), lineEditCustValue5->text());
}
	
void MainWindow::on_lineEditCustText6_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 80, QModelIndex()), lineEditCustText6->text());
}
	
void MainWindow::on_lineEditCustText7_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 81, QModelIndex()), lineEditCustText7->text());
}
	
void MainWindow::on_lineEditCustText8_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 82, QModelIndex()), lineEditCustText8->text());
}
	
void MainWindow::on_lineEditCustText9_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 83, QModelIndex()), lineEditCustText9->text());
}
	
void MainWindow::on_lineEditCustText10_editingFinished() {  
	modelLine->setData(modelLine->index(tableView->currentIndex().row(), 84, QModelIndex()), lineEditCustText10->text());
}
	
#endif


