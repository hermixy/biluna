/*****************************************************************
 * $Id: pcalc_en1591widget.cpp 2241 2015-05-22 10:22:19Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591widget.h"

#include <cmath>
#include <QTransform>

#include "en1591handler.h"
#include "pcalc_dialogfactory.h"
#include "pcalc_inputoutput.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "en1591_unittestfactory.h"
// #include "peng_graphicsview.h"


/**
 * Constructor
 */
PCALC_EN1591Widget::PCALC_EN1591Widget(QWidget *parent)
                                : RB_Widget(parent) {
    setupUi(this);

    mInputOutput = NULL;
}

/**
 * Destructor
 */
PCALC_EN1591Widget::~PCALC_EN1591Widget() {
    delete mInputOutput;
    mInputOutput = NULL;
}

RB_String PCALC_EN1591Widget::getSaveAsFileName() {
    return ""; // "pcalc_EN1591_" + mModel->getCurrentValue("number").toString();
}

void PCALC_EN1591Widget::init() {
    //
    // 0. Set button toolbar
    //
    tbbAssembly->initSlimTable(true, false);
    tvAssembly->setToolButtonBar(tbbAssembly);

    tbbLoadCase->initSlimTable(true, false);
    tvLoadCase->setToolButtonBar(tbbLoadCase);

    //
    // 1. Set model with ID
    //
    mAssemblyModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591Assembly);
    mAssemblyModel->setRoot(PCALC_MODELFACTORY->getRootId());
    mBoltNutWasherModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591BoltNutWasher);
    mFlangeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591Flange);
    mGasketModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591Gasket);
    mLoadCaseModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591LoadCase);
    mShellModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN1591Shell);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    // general
    //
    mAssemblyMapper = mAssemblyModel->getMapper();
    mAssemblyMapper->addMapping(leNumber, mAssemblyModel->fieldIndex("number"));
    mAssemblyMapper->addMapping(leDescription,
                                mAssemblyModel->fieldIndex("description"));
    mAssemblyMapper->addMapping(sbRevision,
                                mAssemblyModel->fieldIndex("revision"));
    mAssemblyMapper->addMapping(deDate,
                                mAssemblyModel->fieldIndex("date"));
    mAssemblyMapper->addMapping(leNameEngineer,
                                mAssemblyModel->fieldIndex("nameengineer"));

    mAssemblyModel->select();
    formatTableView(tvAssembly, mAssemblyModel);

    // Hide columns
    for (int i = 0; i < mAssemblyModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1) {
            tvAssembly->hideColumn(i);
        }
    }

    // flange
    //
    mFlangeMapper = mFlangeModel->getMapper();
    mFlangeMapper->addMapping(sbNoReassemblies,
                              mFlangeModel->fieldIndex("nr"));
    mFlangeMapper->addMapping(sbnB, mFlangeModel->fieldIndex("nb"));
    RB_StringList items;
    items << "Blind" << "Integral" << "Loose";
    cbTypeFlange_1->setModel(new QStringListModel(items, this));
//    mFlangeModel->setTextList(mFlangeModel->fieldIndex("typeflange1_id"),
//                              flangeItems);
    mFlangeMapper->addMapping(cbTypeFlange_1,
                              mFlangeModel->fieldIndex("typeflange1_id"),
                              "currentIndex");
    mFlangeMapper->addMapping(led0_1, mFlangeModel->fieldIndex("d01"));
    mFlangeMapper->addMapping(led3_1, mFlangeModel->fieldIndex("d31"));
    mFlangeMapper->addMapping(led4_1, mFlangeModel->fieldIndex("d41"));
    mFlangeMapper->addMapping(ledX_1, mFlangeModel->fieldIndex("dx1"));
    mFlangeMapper->addMapping(leeF_1, mFlangeModel->fieldIndex("ef1"));
    mFlangeMapper->addMapping(leeFt_1, mFlangeModel->fieldIndex("eft1"));
    mFlangeMapper->addMapping(leeP_1, mFlangeModel->fieldIndex("ep1"));
    mFlangeMapper->addMapping(leeRF_1, mFlangeModel->fieldIndex("erf1"));
    mFlangeMapper->addMapping(leeX_1, mFlangeModel->fieldIndex("ex1"));

    mFlangeMapper->addMapping(led9_1, mFlangeModel->fieldIndex("d91"));
    mFlangeMapper->addMapping(lee0_1, mFlangeModel->fieldIndex("e01"));

    mFlangeMapper->addMapping(led1_1, mFlangeModel->fieldIndex("d11"));
    mFlangeMapper->addMapping(led2_1, mFlangeModel->fieldIndex("d21"));

    mFlangeMapper->addMapping(lee1_1, mFlangeModel->fieldIndex("e11"));
    mFlangeMapper->addMapping(lee2_1, mFlangeModel->fieldIndex("e21"));
    mFlangeMapper->addMapping(lelH_1, mFlangeModel->fieldIndex("lh1"));

    mFlangeMapper->addMapping(leb0_1, mFlangeModel->fieldIndex("b01"));
    mFlangeMapper->addMapping(led6_1, mFlangeModel->fieldIndex("d61"));
    mFlangeMapper->addMapping(led8_1, mFlangeModel->fieldIndex("d81"));
    mFlangeMapper->addMapping(leeL_1, mFlangeModel->fieldIndex("el1"));
    mFlangeMapper->addMapping(ileMaterialFlange_1,
                              mFlangeModel->fieldIndex("materialflange1_idx"));
    mFlangeMapper->addMapping(ileMaterialLooseRing_1,
                              mFlangeModel->fieldIndex(
                                  "materialloosering1_idx"));

    mFlangeMapper->addMapping(chbFlange2Equal,
                              mFlangeModel->fieldIndex("flange2equal"));

    cbTypeFlange_2->setModel(new QStringListModel(items, this));
    mFlangeMapper->addMapping(cbTypeFlange_2,
                              mFlangeModel->fieldIndex("typeflange2_id"),
                              "currentIndex");
    mFlangeMapper->addMapping(led0_2, mFlangeModel->fieldIndex("d02"));
    mFlangeMapper->addMapping(led3_2, mFlangeModel->fieldIndex("d32"));
    mFlangeMapper->addMapping(led4_2, mFlangeModel->fieldIndex("d42"));
    mFlangeMapper->addMapping(ledX_2, mFlangeModel->fieldIndex("dx2"));
    mFlangeMapper->addMapping(leeF_2, mFlangeModel->fieldIndex("ef2"));
    mFlangeMapper->addMapping(leeFt_2, mFlangeModel->fieldIndex("eft2"));
    mFlangeMapper->addMapping(leeP_2, mFlangeModel->fieldIndex("ep2"));
    mFlangeMapper->addMapping(leeRF_2, mFlangeModel->fieldIndex("erf2"));
    mFlangeMapper->addMapping(leeX_2, mFlangeModel->fieldIndex("ex2"));

    mFlangeMapper->addMapping(led9_2, mFlangeModel->fieldIndex("d92"));
    mFlangeMapper->addMapping(lee0_2, mFlangeModel->fieldIndex("e02"));

    mFlangeMapper->addMapping(led1_2, mFlangeModel->fieldIndex("d12"));
    mFlangeMapper->addMapping(led2_2, mFlangeModel->fieldIndex("d22"));

    mFlangeMapper->addMapping(lee1_2, mFlangeModel->fieldIndex("e12"));
    mFlangeMapper->addMapping(lee2_2, mFlangeModel->fieldIndex("e22"));
    mFlangeMapper->addMapping(lelH_2, mFlangeModel->fieldIndex("lh2"));

    mFlangeMapper->addMapping(leb0_2, mFlangeModel->fieldIndex("b02"));
    mFlangeMapper->addMapping(led6_2, mFlangeModel->fieldIndex("d62"));
    mFlangeMapper->addMapping(led8_2, mFlangeModel->fieldIndex("d82"));
    mFlangeMapper->addMapping(leeL_2, mFlangeModel->fieldIndex("el2"));
    mFlangeMapper->addMapping(ileMaterialFlange_2,
                              mFlangeModel->fieldIndex("materialflange2_idx"));
    mFlangeMapper->addMapping(ileMaterialLooseRing_2,
                              mFlangeModel->fieldIndex(
                                  "materialloosering2_idx"));

    // gasket
    mGasketMapper = mGasketModel->getMapper();
    mGasketMapper->addMapping(ileTypeGasket,
                              mGasketModel->fieldIndex("materialcode_ix"));
    items.clear();
    items << "Flat" << "Curved Simple Contact" << "Curved Double Contact"
          << "Octagonal Double Contact";
    cbFormType->setModel(new QStringListModel(items, this));
    mGasketMapper->addMapping(cbFormType,
                              mGasketModel->fieldIndex("formtype_id"),
                              "currentIndex");
//    items.clear();
//    items << "Non Metalic" << "Grooved Steel" << "Spiral Wound"
//          << "Solid Metal" << "Covered Metal Jacketed" << "MetalJacketed";
//    cbMatType->setModel(new QStringListModel(items, this));
//    mGasketMapper->addMapping(cbMatType,
//                              mGasketModel->fieldIndex("materialtype_id"),
//                              "currentIndex");

    items.clear();
    // TODO: limit items based on clicked cbMatType
    items // Non metalic
         << "Rubber"
         << "Ptfe"
         << "ExpPtfe"
         << "ExpGraphWithoutIns"
         << "ExpGraphPerfMetal"
         << "ExpGraphAdhFlat"
         << "ExpGraphMetSheet"
         << "NonAsbetosSmaller1"
         << "NonAsbetosLargerEqual1"
         // Grooved steel gaskets with soft layers on both sides
         << "PtfeSoftSteel"
         << "PtfeStainless"
         << "GraphSoftSteel"
         << "GraphLowAlloy"
         << "GraphStainless"
         << "SilverLayer"
         // Spiral wound gaskets with soft filler
         << "SpiralPtfeFillOuterOnly"
         << "SpiralPtfeFillOuterInner"
         << "SpiralGraphFillOuterOnly"
         << "SpiralGraphFillOuterInner"
         // Solid metal gaskets
         << "AluminumSoft"
         << "CopperOrBrassSoft"
         << "IronSoft"
         << "SteelSoft"
         << "SteelLowAlloyHeatRes"
         << "StainlessSteel"
         << "StainLessSteelHeatRes"
         // Covered metal jacketed gaskets
         << "SoftIronOrSteelJackGraphFillCover"
         << "LowAlloyOrStainlessSteelGraphFillCover"
         << "StainlessSteelJacketPtfeFillCover"
         << "NickelAlloyPtfeFillCover"
         // Metal jacketed gaskets
         << "AluminumSoftGraphFill"
         << "CopperOrBrassSoftGraphFill"
         << "SoftIronOrSteelGraphFill"
         << "LowAlloyOrStainlessSteelGraphFill";
    cbInsType->setModel(new QStringListModel(items, this));
    mGasketMapper->addMapping(cbInsType,
                              mGasketModel->fieldIndex("insertfilltype_id"),
                              "currentIndex");
    mGasketMapper->addMapping(ledG0, mGasketModel->fieldIndex("dg0"));
    mGasketMapper->addMapping(ledG1, mGasketModel->fieldIndex("dg1"));
    mGasketMapper->addMapping(ledG2, mGasketModel->fieldIndex("dg2"));
    mGasketMapper->addMapping(ledG1_EN13555, mGasketModel->fieldIndex("dg1en13555"));
    mGasketMapper->addMapping(ledG2_EN13555, mGasketModel->fieldIndex("dg2en13555"));
    mGasketMapper->addMapping(leeGt, mGasketModel->fieldIndex("egt"));
    mGasketMapper->addMapping(leK, mGasketModel->fieldIndex("k"));
    mGasketMapper->addMapping(lephiG, mGasketModel->fieldIndex("phig"));
    mGasketMapper->addMapping(ler2, mGasketModel->fieldIndex("r2"));

    // bolt
    mBoltNutWasherMapper = mBoltNutWasherModel->getMapper();
    mBoltNutWasherMapper->addMapping(ileBoltSize,
                                     mBoltNutWasherModel->fieldIndex(
                                         "size_idx"));
    items.clear();
    items << "Hexagon Headed" << "Stud" << "Waisted Stud";
    cbBoltType->setModel(new QStringListModel(items, this));
    mBoltNutWasherMapper->addMapping(cbBoltType,
                                     mBoltNutWasherModel->fieldIndex(
                                         "bolttype_id"),
                                     "currentIndex");
    items.clear();
    items << "Manual Standard Ring" << "Manual Operator Feel" << "Impact"
         << "Torque Wrench" << "Tensioner Measure Hydraulic Pressure"
         << "Tensioner Measure Bolt Elongation" << "Wrench Measure Nut Turn"
         << "Wrench Measure Torque Plus Nut Turn";
    cbTensionerType->setModel(new QStringListModel(items, this));
    mBoltNutWasherMapper->addMapping(cbTensionerType,
                                     mBoltNutWasherModel->fieldIndex(
                                         "tensionertype_id"),
                                     "currentIndex");
    mBoltNutWasherMapper->addMapping(ledB0,
                                     mBoltNutWasherModel->fieldIndex("db0"));
    mBoltNutWasherMapper->addMapping(ledB2,
                                     mBoltNutWasherModel->fieldIndex("db2"));
    mBoltNutWasherMapper->addMapping(ledB4,
                                     mBoltNutWasherModel->fieldIndex("db4"));
    mBoltNutWasherMapper->addMapping(ledBe,
                                     mBoltNutWasherModel->fieldIndex("dbe"));
    mBoltNutWasherMapper->addMapping(ledBS,
                                     mBoltNutWasherModel->fieldIndex("dbs"));
    mBoltNutWasherMapper->addMapping(leeN,
                                     mBoltNutWasherModel->fieldIndex("en"));
    mBoltNutWasherMapper->addMapping(lel5t,
                                     mBoltNutWasherModel->fieldIndex("l5t"));
    mBoltNutWasherMapper->addMapping(lelS,
                                     mBoltNutWasherModel->fieldIndex("ls"));
    mBoltNutWasherMapper->addMapping(lemut,
                                     mBoltNutWasherModel->fieldIndex("mut"));
    mBoltNutWasherMapper->addMapping(leRuptureElongationA,
                                     mBoltNutWasherModel->fieldIndex(
                                         "ruptureelongationa"));
    mBoltNutWasherMapper->addMapping(ileMaterialBolt,
                                     mBoltNutWasherModel->fieldIndex(
                                         "materialbolt_idx"));
    // bolthole
    mBoltNutWasherMapper->addMapping(led5,
                                     mBoltNutWasherModel->fieldIndex("d5"));
    mBoltNutWasherMapper->addMapping(led5t,
                                     mBoltNutWasherModel->fieldIndex("d5t"));
    mBoltNutWasherMapper->addMapping(leeFb,
                                     mBoltNutWasherModel->fieldIndex("efb"));
    mBoltNutWasherMapper->addMapping(chbBlindHole,
                                     mBoltNutWasherModel->fieldIndex(
                                         "blindhole"));
    // washer
    mBoltNutWasherMapper->addMapping(leeW,
                                     mBoltNutWasherModel->fieldIndex("ew"));
    mBoltNutWasherMapper->addMapping(ledW1,
                                     mBoltNutWasherModel->fieldIndex("dw1"));
    mBoltNutWasherMapper->addMapping(ledW2,
                                     mBoltNutWasherModel->fieldIndex("dw2"));
    mBoltNutWasherMapper->addMapping(ileMaterialWasher,
                                     mBoltNutWasherModel->fieldIndex(
                                         "materialwasher_idx"));

    // shell
    mShellMapper = mShellModel->getMapper();
    items.clear();
    items << "Conical" << "Cylindrical" << "Spherical";
    cbTypeShell_1->setModel(new QStringListModel(items, this));
    mShellMapper->addMapping(cbTypeShell_1,
                             mShellModel->fieldIndex("typeshell1_id"),
                             "currentIndex");
    mShellMapper->addMapping(ledS_1, mShellModel->fieldIndex("ds1"));
    mShellMapper->addMapping(leeS_1, mShellModel->fieldIndex("es1"));
    mShellMapper->addMapping(lephiS_1, mShellModel->fieldIndex("phis1"));
    mShellMapper->addMapping(ileMaterialShell_1,
                             mShellModel->fieldIndex("materialshell1_idx"));

    mShellMapper->addMapping(chbShell2Equal,
                             mShellModel->fieldIndex("shell2equal"));

    cbTypeShell_2->setModel(new QStringListModel(items, this));
    mShellMapper->addMapping(cbTypeShell_2,
                             mShellModel->fieldIndex("typeshell2_id"),
                             "currentIndex");
    mShellMapper->addMapping(ledS_2, mShellModel->fieldIndex("ds2"));
    mShellMapper->addMapping(leeS_2, mShellModel->fieldIndex("es2"));
    mShellMapper->addMapping(lephiS_2, mShellModel->fieldIndex("phis2"));
    mShellMapper->addMapping(ileMaterialShell_2,
                             mShellModel->fieldIndex("materialshell2_idx"));

    // loadcase
    mLoadCaseMapper = mLoadCaseModel->getMapper();
    mLoadCaseMapper->addMapping(sbLoadCaseNo,
                                mLoadCaseModel->fieldIndex("loadcaseno"));
    // pressure temperature
    mLoadCaseMapper->addMapping(leP, mLoadCaseModel->fieldIndex("p"));
    mLoadCaseMapper->addMapping(leT0, mLoadCaseModel->fieldIndex("t0"));
    mLoadCaseMapper->addMapping(leTB, mLoadCaseModel->fieldIndex("tb"));
    mLoadCaseMapper->addMapping(leTG, mLoadCaseModel->fieldIndex("tg"));
    // flange 1
    mLoadCaseMapper->addMapping(leTF_1, mLoadCaseModel->fieldIndex("tf1"));
    mLoadCaseMapper->addMapping(leTL_1, mLoadCaseModel->fieldIndex("tl1"));
    mLoadCaseMapper->addMapping(leTW_1, mLoadCaseModel->fieldIndex("tw1"));
    // flange 2
    mLoadCaseMapper->addMapping(leTF_2, mLoadCaseModel->fieldIndex("tf2"));
    mLoadCaseMapper->addMapping(leTL_2, mLoadCaseModel->fieldIndex("tl2"));
    mLoadCaseMapper->addMapping(leTW_2, mLoadCaseModel->fieldIndex("tw2"));
    // force
    mLoadCaseMapper->addMapping(leFx, mLoadCaseModel->fieldIndex("fx"));
    mLoadCaseMapper->addMapping(leFy, mLoadCaseModel->fieldIndex("fy"));
    mLoadCaseMapper->addMapping(leFz, mLoadCaseModel->fieldIndex("fz"));
    mLoadCaseMapper->addMapping(leMx, mLoadCaseModel->fieldIndex("mx"));
    mLoadCaseMapper->addMapping(leMy, mLoadCaseModel->fieldIndex("my"));
    mLoadCaseMapper->addMapping(leMz, mLoadCaseModel->fieldIndex("mz"));
    // stress expansion elasticity
    mLoadCaseMapper->addMapping(lefB, mLoadCaseModel->fieldIndex("fb"));
    mLoadCaseMapper->addMapping(lefN, mLoadCaseModel->fieldIndex("fn"));

    // flange 1
    mLoadCaseMapper->addMapping(lefF_1, mLoadCaseModel->fieldIndex("ff1"));
    mLoadCaseMapper->addMapping(lefL_1, mLoadCaseModel->fieldIndex("fl1"));
    mLoadCaseMapper->addMapping(lefS_1, mLoadCaseModel->fieldIndex("fs1"));
    mLoadCaseMapper->addMapping(lefW_1, mLoadCaseModel->fieldIndex("fw1"));
    // flange 2
    mLoadCaseMapper->addMapping(lefF_2, mLoadCaseModel->fieldIndex("ff2"));
    mLoadCaseMapper->addMapping(lefL_2, mLoadCaseModel->fieldIndex("fl2"));
    mLoadCaseMapper->addMapping(lefS_2, mLoadCaseModel->fieldIndex("fs2"));
    mLoadCaseMapper->addMapping(lefW_2, mLoadCaseModel->fieldIndex("fw2"));

    mLoadCaseMapper->addMapping(lealphaB, mLoadCaseModel->fieldIndex("alphab"));
    mLoadCaseMapper->addMapping(lealphaG, mLoadCaseModel->fieldIndex("alphag"));
    // flange 1
    mLoadCaseMapper->addMapping(lealphaF_1, mLoadCaseModel->fieldIndex("alphaf1"));
    mLoadCaseMapper->addMapping(lealphaL_1, mLoadCaseModel->fieldIndex("alphal1"));
    mLoadCaseMapper->addMapping(lealphaW_1, mLoadCaseModel->fieldIndex("alphaw1"));
    // flange 2
    mLoadCaseMapper->addMapping(lealphaF_2, mLoadCaseModel->fieldIndex("alphaf2"));
    mLoadCaseMapper->addMapping(lealphaL_2, mLoadCaseModel->fieldIndex("alphal2"));
    mLoadCaseMapper->addMapping(lealphaW_2, mLoadCaseModel->fieldIndex("alphaw2"));

    mLoadCaseMapper->addMapping(leEB, mLoadCaseModel->fieldIndex("eb"));
    // flange 1
    mLoadCaseMapper->addMapping(leEF_1, mLoadCaseModel->fieldIndex("ef1"));
    mLoadCaseMapper->addMapping(leEL_1, mLoadCaseModel->fieldIndex("el1"));
    mLoadCaseMapper->addMapping(leEW_1, mLoadCaseModel->fieldIndex("ew1"));
    // flange 2
    mLoadCaseMapper->addMapping(leEF_2, mLoadCaseModel->fieldIndex("ef2"));
    mLoadCaseMapper->addMapping(leEL_2, mLoadCaseModel->fieldIndex("el2"));
    mLoadCaseMapper->addMapping(leEW_2, mLoadCaseModel->fieldIndex("ew2"));

    formatTableView(tvLoadCase, mLoadCaseModel);

    // Hide columns
    int colCount = mLoadCaseModel->columnCount();

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS || i > RB2::HIDDENCOLUMNS + 4) {
            tvLoadCase->hideColumn(i);
        } else {
            tvLoadCase->showColumn(i);

            if (tvLoadCase->columnWidth(i) < 5) {
                tvLoadCase->setColumnWidth(i, 100);
            }
        }
    }

    readSettings();

    connect(tbbAssembly, SIGNAL(addClicked()), this, SLOT(slotAssemblyAdded()));

    // Show detail row or add row if not exists
    connect(mAssemblyModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));
}

/**
 * Menu file save
 */
bool PCALC_EN1591Widget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        mLoadCaseModel->submitAll();
        mAssemblyModel->submitAllAndSelect();
    } else {
        mLoadCaseModel->submitAll();
        mAssemblyModel->submitAll();
    }

    setWindowModified(false);
    return true;
}

/**
 * Menu file save as, not applicable for this widget
 */
bool PCALC_EN1591Widget::fileSaveAs() {
//    PCALC_DIALOGFACTORY->requestWarningDialog(tr("Menu File Save As is\n"
//                                               "not applicable for this window"));
    return false;
}

bool PCALC_EN1591Widget::fileRevert() {
    mLoadCaseModel->revertAll();
    mAssemblyModel->revertAll();

    setWindowModified(false);
    return true;
}

/**
 * Menu file PDF
 */
void PCALC_EN1591Widget::filePdf(QPrinter* /*pr*/) {

}

/**
 * On pushbutton add weldolet clicked.
 */
void PCALC_EN1591Widget::filePrint(QPrinter* /*pr*/) {

}

/**
 * On pushbutton add weldolet clicked.
 */
void PCALC_EN1591Widget::filePrintPreview(QPrinter* /*pr*/) {

}

void PCALC_EN1591Widget::changeEvent(QEvent *e) {
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
 * @returns Help subject
 */
RB_String PCALC_EN1591Widget::getHelpSubject() const {
    return objectName();
}

QTextEdit *PCALC_EN1591Widget::getTextEdit() {
    return this->teCalculationReport;
}

void PCALC_EN1591Widget::slotAssemblyAdded() {
    QModelIndex idx = mAssemblyModel->index(
                mAssemblyModel->getCurrentIndex().row(),
                mAssemblyModel->fieldIndex("date"));
    QDate date = QDateTime::currentDateTime().date();
    mAssemblyModel->setData(idx, date.toString(Qt::ISODate));
    idx = mAssemblyModel->index(
                mAssemblyModel->getCurrentIndex().row(),
                mAssemblyModel->fieldIndex("nr"));
    mAssemblyModel->setData(idx, 1);

    leNumber->setFocus();
    leNumber->selectAll();
}

void PCALC_EN1591Widget::on_pbCalculate_clicked() {
    if (!mAssemblyModel->getCurrentIndex().isValid()) {
        PCALC_DIALOGFACTORY->requestWarningDialog("No flange assembly selected");
        return;
    }

    QString po = "<p>";
    QString pc = "</p>";
    QString tbo = "<table border='0'>";
    QString tbc = "</table>";
    QString tro = "<tr>";
    QString trc = "</tr>";
    QString tdo = "<td>";
    QString tdc = "</td>";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    getTextEdit()->clear();
    setInputObject();

    QString dateTimeStr = QDateTime::currentDateTime().toString(Qt::ISODate);

    // continue here with first calculation
    // second the material, allow=>1, qual service=>1

    RedBag::Calc::EN1591::EN1591Handler handler(mInputOutput, NULL, NULL, NULL);
    handler.exec();

    // Create output report

    QString outputStr;
    outputStr.append(po + "-- Start calculation: " + dateTimeStr + pc);
    outputStr.append(tbo);
    outputStr.append(tro + "<td width='15%'>&nbsp;</td>"
                           "<td width='10%'><strong>INPUT</strong></td>"
                           "<td width='10%'>&nbsp;</td>"
                           "<td width='65%'>&nbsp;</td>"
                     + trc);
    // input
    RB_ObjectContainer* inList = mInputOutput->getContainer("PCALC_InputList");
    RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");
    int memberCount = in->countMember();

    for (int i = RB2::HIDDENCOLUMNS; i < memberCount; ++i) {
        RB_ObjectMember* mem = in->getMember(i);
        outputStr.append(tro + tdo + "Input:" + tdc
                    + tdo + mem->getName() + " = " + tdc
                    + tdo + mem->getValue().toString() + tdc + tdo + tdc + trc);

    }

    outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc + tdo + tdc + trc);
    outputStr.append(tro + tdo + tdc + tdo + "<strong>LOADCASES</strong>"
                     + tdc + tdo + tdc + tdo + tdc + trc);

    RB_ObjectContainer* loadCaseList =
            mInputOutput->getContainer("PCALC_LoadCaseList");
    loadCaseList->sort(0, RB2::SortOrderAscending, RB2::MemberInteger);
    RB_ObjectIterator* iterLoad = loadCaseList->createIterator();

    for (iterLoad->first(); !iterLoad->isDone(); iterLoad->next()) {
        in = iterLoad->currentObject();
        memberCount = in->countMember();

        for (int i = 0; i < memberCount; ++i) {
            RB_ObjectMember* mem = in->getMember(i);
            outputStr.append(tro + tdo + "Input:" + tdc
                        + tdo + mem->getName() + " = " + tdc
                        + tdo + mem->getValue().toString()) + tdc + trc;
        }
    }

    delete iterLoad;

    outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc + tdo + tdc + trc);
    outputStr.append(tro + tdo + tdc + tdo + "<strong>OUTPUT</strong>"
                     + tdc + tdo + tdc + tdo + tdc + trc);

    // output
    RB_ObjectContainer* outList = mInputOutput->getContainer("PCALC_OutputList");
    RB_ObjectIterator* iter = outList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        outputStr.append(tro
                + tdo + obj->getValue("formulanumber").toString()
                         + " [" + obj->getValue("loadcaseno").toString() + "]" + tdc
                + tdo + obj->getValue("variablename").toString() + " = " + tdc
                + tdo + obj->getValue("result").toString() + tdc
                + tdo + " = " + obj->getValue("formula").toString() + tdc + trc
                    /*+ " " + obj->getValue("unit").toString()
                    + " " + obj->getValue("loadcaseno").toString()
                    + " " + obj->getValue("note").toString()*/);

        QString str = obj->getValue("formulavalues").toString();
        if (!str.isEmpty()) {
            outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc
                             + tdo + " = " + str + tdc + trc);
        }
    }

    delete iter;
    outputStr.append(tbc);
    outputStr.append(po + "-- End calculation:"
                     + QDateTime::currentDateTime().toString(Qt::ISODate) + pc);
    getTextEdit()->setHtml(outputStr);
    QApplication::restoreOverrideCursor();
}

void PCALC_EN1591Widget::on_pbUnitTest_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    getTextEdit()->clear();
    createInputOutputObject();

    RedBag::Calc::EN1591::EN1591_UnitTestFactory testFactory(mInputOutput);
    testFactory.exec();

    RB_ObjectContainer* outList = mInputOutput->getContainer("PCALC_OutputList");
    RB_ObjectIterator* iter = outList->createIterator();
    RB_String str;

    getTextEdit()->append("-- Start UnitTest:"
                          + QDateTime::currentDateTime().toString(Qt::ISODate));
    int passedCount = 0;
    int failedCount = 0;

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        str = obj->getValue("note").toString();

        if (!str.isEmpty()) {
            getTextEdit()->append(str);

            if (str.contains("- test OK")) {
                ++passedCount;
            } else {
                ++failedCount;
            }
        }
    }

    getTextEdit()->append("Tests Passed: " + QString::number(passedCount));
    getTextEdit()->append("Tests Failed: " + QString::number(failedCount));
    getTextEdit()->append("-- End UnitTest:"
                          + QDateTime::currentDateTime().toString(Qt::ISODate));
    QApplication::restoreOverrideCursor();
}

void PCALC_EN1591Widget::slotParentRowChanged(const QModelIndex& /*curr*/,
                                              const QModelIndex& /*prev*/) {
    mCurrentId = mAssemblyModel->getCurrentId();
    QTimer::singleShot(200, this, SLOT(slotHandleParentRowChanged()));
}

void PCALC_EN1591Widget::slotHandleParentRowChanged() {
    if (mCurrentId != mAssemblyModel->getCurrentId()) {
        // User already selected another row
        return;
    }

    QModelIndex idx;
    // always insert at the begin
    int row = 0; // mCustModel->rowCount();

    if (mBoltNutWasherModel->rowCount() < 1) {
        mBoltNutWasherModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
//        idx = mBoltNutWasherModel->index(row,
//          mBoltNutWasherModel->fieldIndex("classiffluiddangergroup_id"));
//        mBoltNutWasherModel->setData(idx, "0", Qt::EditRole);
//        idx = mBoltNutWasherModel->index(row,
//          mBoltNutWasherModel->fieldIndex("classifgasstability_id"));
//        mBoltNutWasherModel->setData(idx, "0", Qt::EditRole);
        // end NOTE

        mBoltNutWasherModel->submitAll();
    }

    idx = mBoltNutWasherModel->index(0, 0);
    mBoltNutWasherModel->slotChangeCurrentRow(idx, QModelIndex());

    if (mFlangeModel->rowCount() < 1) {
        int row = 0;
        mFlangeModel->insertRows(row, 1, QModelIndex());
        mFlangeModel->submitAll();
    }

    idx = mFlangeModel->index(0, 0);
    mFlangeModel->slotChangeCurrentRow(idx, QModelIndex());

    if (mGasketModel->rowCount() < 1) {
        mGasketModel->insertRows(row, 1, QModelIndex());
        mGasketModel->submitAll();
    }

    idx = mGasketModel->index(0, 0);
    mGasketModel->slotChangeCurrentRow(idx, QModelIndex());

    // LoadCase model is a child model

    if (mShellModel->rowCount() < 1) {
        mShellModel->insertRows(row, 1, QModelIndex());
        mShellModel->submitAll();
    }

    idx = mShellModel->index(0, 0);
    mShellModel->slotChangeCurrentRow(idx, QModelIndex());
}

void PCALC_EN1591Widget::setInputObject() {
    createInputOutputObject();

    RB_ObjectContainer* inList = mInputOutput->getContainer("PCALC_InputList");
    RB_ObjectAtomic* objIn = new RB_ObjectAtomic("", inList, "PCALC_Input");
    inList->addObject(objIn);

    addObjectMemberVariable(objIn, "number", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "description", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "revision", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "date", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "nameengineer", "-", mAssemblyModel);

    // flanges
    addObjectMemberVariable(objIn, "nr", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "nb", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "typeflange1_id", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d01", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d31", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d41", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "dx1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ef1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "eft1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ep1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erf1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ex1", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "d91", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e01", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "d11", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d21", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "e11", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e21", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "lh1", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "b01", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d61", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d81", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "el1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialflange1_idx", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialloosering1_idx", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "flange2equal", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "typeflange2_id", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d02", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d32", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d42", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "dx2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ef2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "eft2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ep2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erf2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ex2", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "d92", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e02", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "d12", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d22", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "e12", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e22", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "lh2", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "b02", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d62", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d82", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "el2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialflange2_idx", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialloosering2_idx", "-", mFlangeModel);

    // gasket
    addObjectMemberVariable(objIn, "materialcode_ix", "-", mGasketModel);
    addObjectMemberVariable(objIn, "formtype_id", "-", mGasketModel);
    addObjectMemberVariable(objIn, "insertfilltype_id", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg0", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg1", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg2", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg1en13555", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg2en13555", "-", mGasketModel);
    addObjectMemberVariable(objIn, "egt", "-", mGasketModel);
    addObjectMemberVariable(objIn, "k", "-", mGasketModel);
    addObjectMemberVariable(objIn, "phig", "-", mGasketModel);
    addObjectMemberVariable(objIn, "r2", "-", mGasketModel);

    // bolt
    addObjectMemberVariable(objIn, "size_idx", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "bolttype_id", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "tensionertype_id", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "db0", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "db2", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "db4", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "dbe", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "dbs", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "en", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "l5t", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "ls", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "mut", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "ruptureelongationa", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "materialbolt_idx", "-", mBoltNutWasherModel);
    // bolthole
    addObjectMemberVariable(objIn, "d5", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "d5t", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "efb", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "blindhole", "-", mBoltNutWasherModel);
    // washer
    addObjectMemberVariable(objIn, "ew", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "dw1", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "dw2", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "materialwasher_idx", "-", mBoltNutWasherModel);

    // shell
    addObjectMemberVariable(objIn, "typeshell1_id", "-", mShellModel);
    addObjectMemberVariable(objIn, "ds1", "-", mShellModel);
    addObjectMemberVariable(objIn, "es1", "-", mShellModel);
    addObjectMemberVariable(objIn, "phis1", "-", mShellModel);
    addObjectMemberVariable(objIn, "materialshell1_idx", "-", mShellModel);
    addObjectMemberVariable(objIn, "shell2equal", "-", mShellModel);
    addObjectMemberVariable(objIn, "typeshell2_id", "-", mShellModel);
    addObjectMemberVariable(objIn, "ds2", "-", mShellModel);
    addObjectMemberVariable(objIn, "es2", "-", mShellModel);
    addObjectMemberVariable(objIn, "phis2", "-", mShellModel);
    addObjectMemberVariable(objIn, "materialshell2_idx", "-", mShellModel);

    // loadcase
    RB_ObjectContainer* loadCaseList;
    loadCaseList = mInputOutput->getContainer("PCALC_LoadCaseList");
    int rowCount = mLoadCaseModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        RB_ObjectAtomic* loadCase = new RB_ObjectAtomic("", loadCaseList,
                                                        "PCALC_LoadCase",
                                                        NULL, true);
        loadCaseList->addObject(loadCase);

        addLoadCaseVariable(loadCase, "loadcaseno", "-", mLoadCaseModel, row);
        // pressure temperature
        addLoadCaseVariable(loadCase, "p", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "t0", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tb", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tg", "-", mLoadCaseModel, row);
        // flange 1
        addLoadCaseVariable(loadCase, "tf1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tl1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tw1", "-", mLoadCaseModel, row);
        // flange 2
        addLoadCaseVariable(loadCase, "tf2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tl2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "tw2", "-", mLoadCaseModel, row);
        // force
        addLoadCaseVariable(loadCase, "fx", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fy", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fz", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "mx", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "my", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "mz", "-", mLoadCaseModel, row);
        // stress expansion elasticity
        addLoadCaseVariable(loadCase, "fb", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fn", "-", mLoadCaseModel, row);

        // flange 1
        addLoadCaseVariable(loadCase, "ff1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fl1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fs1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fw1", "-", mLoadCaseModel, row);
        // flange 2
        addLoadCaseVariable(loadCase, "ff2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fl2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fs2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "fw2", "-", mLoadCaseModel, row);

        addLoadCaseVariable(loadCase, "alphab", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "alphag", "-", mLoadCaseModel, row);
        // flange 1
        addLoadCaseVariable(loadCase, "alphaf1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "alphal1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "alphaw1", "-", mLoadCaseModel, row);
        // flange 2
        addLoadCaseVariable(loadCase, "alphaf2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "alphal2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "alphaw2", "-", mLoadCaseModel, row);

        addLoadCaseVariable(loadCase, "eb", "-", mLoadCaseModel, row);
        // flange 1
        addLoadCaseVariable(loadCase, "ef1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "el1", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "ew1", "-", mLoadCaseModel, row);
        // flange 2
        addLoadCaseVariable(loadCase, "ef2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "el2", "-", mLoadCaseModel, row);
        addLoadCaseVariable(loadCase, "ew2", "-", mLoadCaseModel, row);
    }
}

void PCALC_EN1591Widget::createInputOutputObject() {
    if (mInputOutput) {
        delete mInputOutput;
    }

    mInputOutput = new PCALC_InputOutput();
    RB_ObjectContainer* inList;
    inList = mInputOutput->getContainer("PCALC_InputList");

    // Add setting object to input list
    RB_ObjectAtomic* obj = new RB_ObjectAtomic("", inList, "PCALC_Setting");
    obj->addMember("lastvalue", "-", cbLastValuesOnly->isChecked() ? 1 : 0,
                   RB2::MemberInteger);
    obj->addMember("formulafrom", "-", sbFormulaFrom->value(),
                   RB2::MemberInteger);
    obj->addMember("formulato", "-", sbFormulaTo->value(),
                   RB2::MemberInteger);
    inList->addObject(obj);
}

void PCALC_EN1591Widget::addObjectMemberVariable(RB_ObjectBase* obj,
                                            const QString& variableName,
                                            const QString& unit,
                                            RB_MmProxy* model) {
    QModelIndex idx = model->index(
                model->getCurrentIndex().row(),
                model->fieldIndex(variableName));
    obj->addMember(variableName, unit, model->data(idx));
}

void PCALC_EN1591Widget::addLoadCaseVariable(RB_ObjectBase* loadCase,
                                             const QString& variableName,
                                             const QString& unit,
                                             RB_MmProxy* model,
                                             int currentRow) {
    QModelIndex idx = model->index(
                currentRow,
                model->fieldIndex(variableName));
    loadCase->addMember(variableName, unit, model->data(idx));
}

