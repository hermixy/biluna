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
#include "en1591_unittestfactory.h"
#include "pcalc.h"
#include "pcalc_dialogfactory.h"
#include "pcalc_inputoutput.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_report.h"
#include "rb_dialogwindow.h"
#include "rb_idxlineedit.h"
#include "std_materialutility.h"
#include "std_unittestfactory.h"
// #include "peng_graphicsview.h"


/**
 * Constructor
 */
PCALC_EN1591Widget::PCALC_EN1591Widget(QWidget *parent)
                                : RB_Widget(parent) {
    setupUi(this);

    mAssemblyModel = NULL;
    mBoltNutWasherModel = NULL;
    mFlangeModel = NULL;
    mGasketModel = NULL;
    mLoadCaseModel = NULL;
    mShellModel = NULL;

    mIsUpdateMaterial = true;
}

/**
 * Destructor
 */
PCALC_EN1591Widget::~PCALC_EN1591Widget() {
    // store selected report type
//    RB_SETTINGS->beginGroup(objectName());
//    RB_SETTINGS->setValue("glno", cbLedgerAccount->currentIndex());
//    RB_SETTINGS->endGroup();

    // non-shared models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mAssemblyModel;
    delete mBoltNutWasherModel;
    delete mFlangeModel;
    delete mGasketModel;
    delete mLoadCaseModel;
    delete mShellModel;

    RB_DEBUG->print("PCALC_EN1591Widget::~PCALC_EN1591Widget() OK");
}

RB_String PCALC_EN1591Widget::getSaveAsFileName() {
    QString name = mAssemblyModel->getCurrentValue("number").toString();
    name.replace(" ", "_");
    name += "_rev";
    int revision = mAssemblyModel->getCurrentValue("revision").toInt();
    name += QString::number(revision);

    QString type = this->cbCalculationReportType->currentText().toLower();
    type.replace(" ", "_");

    return "pcalc_EN1591_" + name + "_" + type + ".odt";
}

void PCALC_EN1591Widget::init() {
    initMapping();
    initConnections();
    readSettings();
}

void PCALC_EN1591Widget::initConnections() {
    // Set date and set focus
    connect(tbbAssembly, SIGNAL(addClicked()), this, SLOT(slotAssemblyAdded()));

    // Show detail row or add row if not exists
    connect(mAssemblyModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));

    // Connections for models not formated via setFormatListView
    // and catch bolt, (loose)flange, shell or washer material changed
    connect(mBoltNutWasherModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mFlangeModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mGasketModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mShellModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mLoadCaseModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
}

void PCALC_EN1591Widget::initMapping() {
    //
    // 0. Set button toolbar
    //
    tbbAssembly->initSlimTable(true, false, true);
    tvAssembly->setToolButtonBar(tbbAssembly);

    tbbLoadCase->initSlimTable(true, false, true);
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
    mFlangeMapper->addMapping(leF_Bspec, mFlangeModel->fieldIndex("f_bspecified"));
    mFlangeMapper->addMapping(sbnB, mFlangeModel->fieldIndex("nb"));

    mFlangeMapper->addMapping(ileStandardFlange_1,
                              mFlangeModel->fieldIndex("standardflange1_idx"));
    // Note: diplayname will be added by dialog to provide pretty text,
    // standard name, pressure class and nominal size
    // TODO: solve additional setting of data with function pointer or lambda function
//    ileStandardFlange_1->setDefaultDialog(PCALC_DIALOGFACTORY,
//                                    PCALC_DialogFactory::WidgetSelectFlange,
//                                    "standardflange1_idx", "displayname");
    // catch signal for setting of details
    connect(ileStandardFlange_1, SIGNAL(clicked(bool)),
            this, SLOT(slotIleStandardFlange_1Clicked()));

    QStringList items;
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
    mFlangeMapper->addMapping(ledRF_1, mFlangeModel->fieldIndex("drf1"));
    mFlangeMapper->addMapping(ledREC_1, mFlangeModel->fieldIndex("drec1"));
    mFlangeMapper->addMapping(leeFb_1, mFlangeModel->fieldIndex("efb1"));
    mFlangeMapper->addMapping(leeRF_1, mFlangeModel->fieldIndex("erf1"));
    mFlangeMapper->addMapping(leeREC_1, mFlangeModel->fieldIndex("erec1"));
    mFlangeMapper->addMapping(leeQ_1, mFlangeModel->fieldIndex("eq1"));
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

    ileMaterialFlange_1->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialflange1_idx", "mname");
    ileMaterialLooseRing_1->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialloosering1_idx", "mname");
    mFlangeMapper->addMapping(ileMaterialFlange_1,
                              mFlangeModel->fieldIndex("materialflange1_idx"));
    mFlangeMapper->addMapping(ileMaterialLooseRing_1,
                              mFlangeModel->fieldIndex(
                                  "materialloosering1_idx"));
    // bolthole
    mFlangeMapper->addMapping(led5_1, mFlangeModel->fieldIndex("d51"));
    mFlangeMapper->addMapping(chbBlindHole_1,
                              mFlangeModel->fieldIndex("blindhole1"));
    mFlangeMapper->addMapping(led5t_1, mFlangeModel->fieldIndex("d5t1"));
    mFlangeMapper->addMapping(lel5t_1, mFlangeModel->fieldIndex("l5t1"));

    mFlangeMapper->addMapping(chbFlange2Equal,
                              mFlangeModel->fieldIndex("flange2equal"));

    mFlangeMapper->addMapping(ileStandardFlange_2,
                              mFlangeModel->fieldIndex("standardflange2_idx"));
    // Note: displayname will be added by dialog to provide pretty text,
    // standard name, pressure class and nominal size
//    ileStandardFlange_2->setDefaultDialog(PCALC_DIALOGFACTORY,
//                                    PCALC_DialogFactory::WidgetSelectFlange,
//                                    "standardflange2_idx", "displayname");
    connect(ileStandardFlange_2, SIGNAL(clicked(bool)),
            this, SLOT(slotIleStandardFlange_2Clicked()));

    cbTypeFlange_2->setModel(new QStringListModel(items, this));
    mFlangeMapper->addMapping(cbTypeFlange_2,
                              mFlangeModel->fieldIndex("typeflange2_id"),
                              "currentIndex");
    mFlangeMapper->addMapping(led0_2, mFlangeModel->fieldIndex("d02"));
    mFlangeMapper->addMapping(led3_2, mFlangeModel->fieldIndex("d32"));
    mFlangeMapper->addMapping(led4_2, mFlangeModel->fieldIndex("d42"));
    mFlangeMapper->addMapping(ledX_2, mFlangeModel->fieldIndex("dx2"));
    mFlangeMapper->addMapping(ledRF_2, mFlangeModel->fieldIndex("drf2"));
    mFlangeMapper->addMapping(ledREC_2, mFlangeModel->fieldIndex("drec2"));
    mFlangeMapper->addMapping(leeFb_2, mFlangeModel->fieldIndex("efb2"));
    mFlangeMapper->addMapping(leeRF_2, mFlangeModel->fieldIndex("erf2"));
    mFlangeMapper->addMapping(leeREC_2, mFlangeModel->fieldIndex("erec2"));
    mFlangeMapper->addMapping(leeQ_2, mFlangeModel->fieldIndex("eq2"));
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
    ileMaterialFlange_2->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialflange2_idx", "mname");
    ileMaterialLooseRing_2->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialloosering2_idx", "mname");
    mFlangeMapper->addMapping(ileMaterialFlange_2,
                              mFlangeModel->fieldIndex("materialflange2_idx"));
    mFlangeMapper->addMapping(ileMaterialLooseRing_2,
                              mFlangeModel->fieldIndex(
                                  "materialloosering2_idx"));
    // bolthole
    mFlangeMapper->addMapping(led5_2, mFlangeModel->fieldIndex("d52"));
    mFlangeMapper->addMapping(chbBlindHole_2,
                              mFlangeModel->fieldIndex("blindhole2"));
    mFlangeMapper->addMapping(led5t_2, mFlangeModel->fieldIndex("d5t2"));
    mFlangeMapper->addMapping(lel5t_2, mFlangeModel->fieldIndex("l5t2"));

    // gasket
    mGasketMapper = mGasketModel->getMapper();
    mGasketMapper->addMapping(ileTypeGasket,
                              mGasketModel->fieldIndex("gaskettype_idx"));
    ileTypeGasket->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetEN1591SelectGasket,
                                    "gaskettype_idx", "mname");

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
         << "PTFE"
         << "Expanded PTFE"
         << "Exp. graphite without ins."
         << "Exp. graph. perf. metal"
         << "Exp. graph. adh. flat metal"
         << "Exp. graph. metal sheet"
         << "Non asbetos fiber < 1 mm"
         << "Non asbetos fiber >= 1 mm"
         // Grooved steel gaskets with soft layers on both sides
         << "PTFE layers on soft steel"
         << "PTFE layers on stainless steel"
         << "Graphite layers on soft steel"
         << "Graph. layers on low alloy steel"
         << "Graph. layers on stainless steel"
         << "Silver layers on heat resist. SS"
         // Spiral wound gaskets with soft filler
         << "Spiral w. PTFE filler outer"
         << "Spiral w. PTFE filler out/in"
         << "Spiral w. graph. fill outer"
         << "Spiral w. graph. fill out/in"
         // Solid metal gaskets
         << "Aluminum soft"
         << "Copper of brass soft"
         << "Iron soft"
         << "Steel soft"
         << "Steel low alloy heat resist."
         << "Stainless steel"
         << "Stainless steel heat resist."
         // Covered metal jacketed gaskets
         << "Soft iron or st. jack. graph. fill. cover."
         << "Low alloy or SS graph. fill. cover."
         << "SS jacket PTFE fill. cover."
         << "Nickel alloy PTFE fill. cover."
         // Metal jacketed gaskets
         << "Aluminum soft. graph. fill."
         << "Copper or brass soft graph. fill."
         << "Soft iron or steel. graph. fill."
         << "Low alloy or SS graph. fill.";
    cbInsType->setModel(new QStringListModel(items, this));
    mGasketMapper->addMapping(cbInsType,
                              mGasketModel->fieldIndex("insertfilltype_id"),
                              "currentIndex");
    mGasketMapper->addMapping(sbNoReassemblies,
                              mGasketModel->fieldIndex("nr"));
    items.clear();
    items << "10e0" << "10e-1" << "10e-2" << "10e-3" << "10e-4" << "10e-5"
              << "10e-6" << "10e-7" << "10e-8";
    cbLeakageRate->setModel(new QStringListModel(items, this));
    mGasketMapper->addMapping(cbLeakageRate,
                              mGasketModel->fieldIndex("leakagerate"),
                              "currentIndex");
    mGasketMapper->addMapping(ledG0, mGasketModel->fieldIndex("dg0"));
    mGasketMapper->addMapping(ledGin, mGasketModel->fieldIndex("dgin"));
    mGasketMapper->addMapping(ledGout, mGasketModel->fieldIndex("dgout"));
    mGasketMapper->addMapping(leeGt, mGasketModel->fieldIndex("egt"));
    mGasketMapper->addMapping(lephiG, mGasketModel->fieldIndex("phig"));
    mGasketMapper->addMapping(ler2, mGasketModel->fieldIndex("r2"));

    // bolt
    mBoltNutWasherMapper = mBoltNutWasherModel->getMapper();
    mBoltNutWasherMapper->addMapping(ileBoltSize,
                                     mBoltNutWasherModel->fieldIndex(
                                         "size_idx"));
    connect(ileBoltSize, SIGNAL(clicked(bool)),
            this, SLOT(slotIleBoltSizeClicked()));
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
    mBoltNutWasherMapper->addMapping(
                cbTensionerType,
                mBoltNutWasherModel->fieldIndex("tensionertype_id"),
                "currentIndex");
    items.clear();
    items << "Summary Report" << "Detail Report" << "Material Report"
          << "Last Iteration Validation" << "All Iteration Validation"
          << "Unit Test Summary Report" << "Unit Test Detail Report";
    cbCalculationReportType->setModel(new QStringListModel(items, this));
    connect(cbCalculationReportType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotDisableFormulaWidgets(int)));
    cbCalculationReportType->setCurrentIndex(0);
    slotDisableFormulaWidgets(cbCalculationReportType->currentIndex());

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
    mBoltNutWasherMapper->addMapping(ledn,
                                     mBoltNutWasherModel->fieldIndex("dn"));
    mBoltNutWasherMapper->addMapping(leeN,
                                     mBoltNutWasherModel->fieldIndex("en"));
    mBoltNutWasherMapper->addMapping(lelS,
                                     mBoltNutWasherModel->fieldIndex("ls"));
    mBoltNutWasherMapper->addMapping(lemun,
                                     mBoltNutWasherModel->fieldIndex("mun"));
    mBoltNutWasherMapper->addMapping(lemut,
                                     mBoltNutWasherModel->fieldIndex("mut"));
    mBoltNutWasherMapper->addMapping(lept,
                                     mBoltNutWasherModel->fieldIndex("pt"));
    mBoltNutWasherMapper->addMapping(leRuptureElongationA,
                                     mBoltNutWasherModel->fieldIndex(
                                         "ruptureelongationa"));
    ileMaterialBolt->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialbolt_idx", "mname");
    mBoltNutWasherMapper->addMapping(ileMaterialBolt,
                                     mBoltNutWasherModel->fieldIndex(
                                         "materialbolt_idx"));
    // washer
    mBoltNutWasherMapper->addMapping(leeW,
                                     mBoltNutWasherModel->fieldIndex("ew"));
    mBoltNutWasherMapper->addMapping(ledW1,
                                     mBoltNutWasherModel->fieldIndex("dw1"));
    mBoltNutWasherMapper->addMapping(ledW2,
                                     mBoltNutWasherModel->fieldIndex("dw2"));
    ileMaterialWasher->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialwasher_idx", "mname");
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
    ileMaterialShell_1->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialshell1_idx", "mname");
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
    ileMaterialShell_2->setDefaultDialog(PCALC_DIALOGFACTORY,
                                    PCALC_DialogFactory::WidgetSelectMaterial,
                                    "materialshell2_idx", "mname");
    mShellMapper->addMapping(ileMaterialShell_2,
                             mShellModel->fieldIndex("materialshell2_idx"));

    // Update properties:
    // all requirements
    // exclude very stringent requirement EN13445-3 11.4.3 for bolts
    // use minimum safety factors, use test safety factors for assembly condition
    items.clear();
    items << "Yes (default)" << "Exclude 11.4.3" << "Min. safety factors";
    cbDisregardBoltRequirement->setModel(new QStringListModel(items, this));

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
    mLoadCaseMapper->addMapping(lerelaxB, mLoadCaseModel->fieldIndex("relaxb"));
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

    teCalculationReport->setHtml(
                "<p>" + tr("Select report type and click "
                           "calculate to generate report") + "</p>");
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

void PCALC_EN1591Widget::fileRevert() {
    mLoadCaseModel->revertAll();
    mAssemblyModel->revertAll();

    setWindowModified(false);
}

/**
 * Menu file PDF
 */
void PCALC_EN1591Widget::filePdf(QPrinter* pr) {
    filePrint(pr);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PCALC_EN1591Widget::filePrint(QPrinter* pr) {
    teCalculationReport->print(pr);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PCALC_EN1591Widget::filePrintPreview(QPrinter* pr) {
    filePrint(pr);
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

bool PCALC_EN1591Widget::saveFile(const QString& fn) {
    // copied from DB_TextWidget, make part of RB_TextWidget
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        PCALC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        PCALC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (fn.endsWith(".htm", Qt::CaseInsensitive)
            || fn.endsWith(".html", Qt::CaseInsensitive)
            || fn.endsWith(".xhtml", Qt::CaseInsensitive)) {
        RB_String html = teCalculationReport->toHtml();
        // teCalculationReport->saveHtmlEmbeddedImage(html);
        QTextStream out(&file);
        out << html;
    } else if (fn.endsWith(".odt", Qt::CaseInsensitive)) {
        QTextDocumentWriter writer;
        writer.setFileName(fn);
        writer.setFormat("odf");
        writer.write(teCalculationReport->document());
    } else {
        QTextStream out(&file);
        out << teCalculationReport->toPlainText();
    }

    QApplication::restoreOverrideCursor();

    //    setCurrentFileName(fn);
    //    setIsNewWidget(false);
    //    richTextEdit->document()->setModified(false);
    //    emit modificationChanged(false);
    PCALC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;

}

void PCALC_EN1591Widget::slotDataIsChanged(const QModelIndex& topLeft,
                                           const QModelIndex& bottomRight) {
    RB_Widget::slotDataIsChanged(topLeft, bottomRight);

    int row = mFlangeModel->getCurrentIndex().row();
    QModelIndex index = mFlangeModel->index(
                row, mFlangeModel->fieldIndex("flange2equal"));
    bool flange2equal = 1 == index.data().toInt();
    mFlangeModel->index(
                row, mFlangeModel->fieldIndex("typeflange1_id"));
    bool flange1blind = 0 == index.data().toInt();
    bool flange1loose = 2 == index.data().toInt();
    mFlangeModel->index(
                row, mFlangeModel->fieldIndex("typeflange2_id"));
    bool flange2blind = 0 == index.data().toInt();
    bool flange2loose = 2 == index.data().toInt();
    index = mShellModel->index(
                row, mShellModel->fieldIndex("shell2equal"));
    bool shell2equal = 1 == index.data().toInt();

    if (topLeft.model() == mFlangeModel) {
        if (topLeft.column() == mFlangeModel->fieldIndex("materialflange1_idx")) {
            // name PCALC_DIALOGFACTORY->requestInformationDialog(
            //              topLeft.data(Qt::DisplayRole).toString());
            // id + name PCALC_DIALOGFACTORY->requestInformationDialog(
            //              topLeft.data(Qt::EditRole).toString());
            // id PCALC_DIALOGFACTORY->requestInformationDialog(
            //              topLeft.data(RB2::RoleOrigData).toString());

            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tf1", "ff1", STD2::CompFlange);
            updateElasModul(materialId, "tf1", "ef1");
            updateThermExp(materialId, "tf1", "alphaf1");
        } else if (topLeft.column() == mFlangeModel->fieldIndex("materialloosering1_idx")) {
            if (!flange1loose) {
                return;
            }
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tl1", "fl1", STD2::CompFlange);
            updateElasModul(materialId, "tf1", "el1");
            updateThermExp(materialId, "tf1", "alphal1");
        } else if (topLeft.column() == mFlangeModel->fieldIndex("materialflange2_idx")) {
            if (flange2equal) {
                return;
            }
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tf2", "ff2", STD2::CompFlange);
            updateElasModul(materialId, "tf2", "ef2");
            updateThermExp(materialId, "tf2", "alphaf2");
        } else if (topLeft.column() == mFlangeModel->fieldIndex("materialloosering2_idx")) {
            if (flange2equal || !flange2loose) {
                return;
            }
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tl2", "fl2", STD2::CompFlange);
            updateElasModul(materialId, "tf2", "el2");
            updateThermExp(materialId, "tf2", "alphal2");
        }
    } else if (topLeft.model() == mBoltNutWasherModel) {
        if (topLeft.column() == mBoltNutWasherModel->fieldIndex("materialbolt_idx")){
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tb", "fb", STD2::CompBolt);
            updateAllowStress(materialId, "tb", "fn", STD2::CompBolt); // no separate material yet
            updateElasModul(materialId, "tb", "eb");
            updateThermExp(materialId, "tb", "alphab");
        } else if (topLeft.column() == mBoltNutWasherModel->fieldIndex("materialwasher_idx")) {
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tw1", "fw1", STD2::CompWasher);
            updateElasModul(materialId, "tw1", "ew1");
            updateThermExp(materialId, "tw1", "alphaw1");
            updateAllowStress(materialId, "tw2", "fw2", STD2::CompWasher);
            updateElasModul(materialId, "tw2", "ew2");
            updateThermExp(materialId, "tw2", "alphaw2");
        }
    } else if (topLeft.model() == mShellModel) {
        if (topLeft.column() == mShellModel->fieldIndex("materialshell1_idx")){
            if (flange1blind) {
                return;
            }
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tf1", "fs1", STD2::CompCylinder);
        } else if (topLeft.column() == mShellModel->fieldIndex("materialshell2_idx")) {
            if (shell2equal || flange2blind) {
                return;
            }
            QString materialId = topLeft.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tf2", "fs2", STD2::CompCylinder);
        }
    } else if (topLeft.model() == mLoadCaseModel) {
        if (topLeft.column() == mLoadCaseModel->fieldIndex("t0")){
            syncTemperatures(topLeft.row());
        }
    }
}

void PCALC_EN1591Widget::slotAssemblyAdded() {
    QModelIndex idx = mAssemblyModel->index(
                mAssemblyModel->getCurrentIndex().row(),
                mAssemblyModel->fieldIndex("date"));
    QDate date = QDateTime::currentDateTime().date();
    mAssemblyModel->setData(idx, date.toString(Qt::ISODate));

    leNumber->setFocus();
    leNumber->selectAll();
}

void PCALC_EN1591Widget::on_pbCalculate_clicked() {
    if (!tvAssembly->currentIndex().isValid()) {
        PCALC_DIALOGFACTORY->requestWarningDialog("No flange assembly selected");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    switch (cbCalculationReportType->currentIndex()) {
    case 0:
        createSummaryReport();
        break;
    case 1:
        createDetailReport();
        break;
    case 2:
        createMaterialReport();
        break;
    case 3:
        createValidationReport();
        break;
    case 4:
        createValidationReport();
        break;
    case 5:
        createUnitTestSummary();
        break;
    case 6:
        createUnitTestDetail();
        break;
    default:
        teCalculationReport->setHtml("<p>Invalid report type</p>");
        break;
    }
    QApplication::restoreOverrideCursor();
}

void PCALC_EN1591Widget::on_pbRefreshProperty_clicked() {
    mIsUpdateMaterial = true;
    refreshAllProperties();
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

void PCALC_EN1591Widget::slotDisableFormulaWidgets(int index) {
    sbFormulaFrom->setEnabled(index > 1 && index < 5);
    sbFormulaTo->setEnabled(index > 1 && index < 5);
}

void PCALC_EN1591Widget::slotIleStandardFlange_1Clicked() {
    // This function instead of:
    //    ileStandardFlange_1->setDefaultDialog(PCALC_DIALOGFACTORY,
    //                                    PCALC_DialogFactory::WidgetSelectFlange,
    //                                    "standardflange1_idx", "displayname");
    RB_DialogWindow* dlgW = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetSelectFlange);

    if (dlgW->exec() != QDialog::Accepted) {
        dlgW->deleteLater();
        return;
    }

    RB_ObjectBase* flangeObj = dlgW->getCurrentObject();
    RB_ObjectBase* facingObj = dlgW->getCurrentChild1Object();
    RB_ObjectBase* flangeTypeObj = dlgW->getCurrentChild2Object();

    if (!flangeObj || !facingObj || !flangeTypeObj) {
        PCALC_DIALOGFACTORY->requestWarningDialog(tr("No valid item selected,\n"
                                                     "data unchanged."));
        dlgW->deleteLater();
        return;
    } else {
        int result = PCALC_DIALOGFACTORY->requestYesNoDialog(
                    tr("Flange dimensions"),
                    tr("Do you want to replace\nthe current flange data?"));
        if (result != QMessageBox::Yes) {
            return;
        }
    }

    QString displayName = flangeObj->getValue("displayname").toString();
    QString str = flangeObj->getId() + displayName;
    QModelIndex idx = mFlangeModel->index(mFlangeModel->getCurrentIndex().row(),
                                    mFlangeModel->fieldIndex("standardflange1_idx"));
    mFlangeModel->setData(idx, str);

    if (displayName.startsWith("EN")) {
        // TODO: implement
    } else if (displayName.startsWith("ASME")) {
        if (flangeTypeObj->getValue("type").toString() == "WN") { // TODO: change to type_id!
            setIntegralFlange1AsmeData(flangeObj, facingObj);

            // update bolt with UNC thread and nut dimensions


        }

        // TODO: other types
    }


    dlgW->deleteLater();
}

void PCALC_EN1591Widget::slotIleStandardFlange_2Clicked() {

}

void PCALC_EN1591Widget::slotIleBoltSizeClicked() {
    // This function instead of standard dialog which sets the idx field only
    RB_DialogWindow* dlgW = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetSelectBolt);

    if (dlgW->exec() != QDialog::Accepted) {
        dlgW->deleteLater();
        return;
    }

    RB_ObjectBase* boltObj = dlgW->getCurrentObject();
//    RB_ObjectBase* boltTypeObj = dlgW->getCurrentChild1Object();
//    RB_ObjectBase* nutObj = dlgW->getCurrentChild2Object();

    if (!boltObj /*|| !boltTypeObj || !nutObj*/) {
        PCALC_DIALOGFACTORY->requestWarningDialog(tr("No valid item selected,\n"
                                                     "data unchanged."));
        dlgW->deleteLater();
        return;
    } else {
        int result = PCALC_DIALOGFACTORY->requestYesNoDialog(
                    tr("Bolt/nut dimensions"),
                    tr("Do you want to replace\nthe current bolt/nut data?"));
        if (result != QMessageBox::Yes) {
            return;
        }
    }

    QString displayName = boltObj->getValue("displayname").toString();
    QString str = boltObj->getId() + displayName;
    QModelIndex idx = mBoltNutWasherModel->index(
                mBoltNutWasherModel->getCurrentIndex().row(),
                mBoltNutWasherModel->fieldIndex("size_idx"));
    mBoltNutWasherModel->setData(idx, str);

    if (displayName.startsWith("EN")) {
        setBoltEnData(boltObj/*, nutObj*/);

    } else if (displayName.startsWith("ASME")) {
//        if (boltTypeObj->getValue("type").toString() == "HHD") { // TODO: change to type id!
        setBoltAsmeData(boltObj/*, nutObj*/);
//        }

        // TODO: other types
    }


    dlgW->deleteLater();

}

void PCALC_EN1591Widget::setInput() {
    RB_ObjectContainer* inList
            = PR->getInOutContainer()->getContainer("PCALC_InputList");
    RB_ObjectAtomic* objIn = new RB_ObjectAtomic("", inList, "PCALC_Input");
    inList->addObject(objIn);

    addObjectMemberVariable(objIn, "number", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "description", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "revision", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "date", "-", mAssemblyModel);
    addObjectMemberVariable(objIn, "nameengineer", "-", mAssemblyModel);

    // flanges
    addObjectMemberVariable(objIn, "nb", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "f_bspecified", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "typeflange1_id", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d31", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d41", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "dx1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "drf1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "drec1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "efb1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erf1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erec1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "eq1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ex1", "-", mFlangeModel);
    // bolthole
    addObjectMemberVariable(objIn, "d51", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "blindhole1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d5t1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "l5t1", "-", mFlangeModel);
    // blind only
    addObjectMemberVariable(objIn, "d91", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e01", "-", mFlangeModel);
    // integral, loose only
    addObjectMemberVariable(objIn, "d01", "-", mFlangeModel);
    // integral, loose if hub only
    addObjectMemberVariable(objIn, "d11", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d21", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e11", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e21", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "lh1", "-", mFlangeModel);
    // loose onlye
    addObjectMemberVariable(objIn, "b01", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d61", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d81", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "el1", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialflange1_idx", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialloosering1_idx", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "flange2equal", "-", mFlangeModel);

    addObjectMemberVariable(objIn, "typeflange2_id", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d32", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d42", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "dx2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "drf2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "drec2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "efb2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erf2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "erec2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "eq2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "ex2", "-", mFlangeModel);
    // bolthole
    addObjectMemberVariable(objIn, "d52", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "blindhole2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d5t2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "l5t2", "-", mFlangeModel);
    // blind only
    addObjectMemberVariable(objIn, "d92", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e02", "-", mFlangeModel);
    // integral, loose only
    addObjectMemberVariable(objIn, "d02", "-", mFlangeModel);
    // integral, loose if hub only
    addObjectMemberVariable(objIn, "d12", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d22", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e12", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "e22", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "lh2", "-", mFlangeModel);
    // loose only
    addObjectMemberVariable(objIn, "b02", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d62", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "d82", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "el2", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialflange2_idx", "-", mFlangeModel);
    addObjectMemberVariable(objIn, "materialloosering2_idx", "-", mFlangeModel);

    // gasket
    addObjectMemberVariable(objIn, "gaskettype_idx", "-", mGasketModel);
    addObjectMemberVariable(objIn, "formtype_id", "-", mGasketModel);
    addObjectMemberVariable(objIn, "insertfilltype_id", "-", mGasketModel);
    addObjectMemberVariable(objIn, "nr", "-", mGasketModel);
    addObjectMemberVariable(objIn, "leakagerate", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dg0", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dgin", "-", mGasketModel);
    addObjectMemberVariable(objIn, "dgout", "-", mGasketModel);
    addObjectMemberVariable(objIn, "egt", "-", mGasketModel);
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
    addObjectMemberVariable(objIn, "dn", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "en", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "ls", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "mun", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "mut", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "pt", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "ruptureelongationa", "-", mBoltNutWasherModel);
    addObjectMemberVariable(objIn, "materialbolt_idx", "-", mBoltNutWasherModel);

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
    loadCaseList = PR->getInOutContainer()->getContainer("PCALC_LoadCaseList");
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
        addLoadCaseVariable(loadCase, "relaxb", "-", mLoadCaseModel, row);
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

void PCALC_EN1591Widget::setReportSettings() {
    PR->setReportSettings(cbCalculationReportType->currentIndex(),
                          sbFormulaFrom->value(), sbFormulaTo->value());
}

void PCALC_EN1591Widget::createSummaryReport() {
    createReport(":/reports/EN1591_summary_report.html");
}

void PCALC_EN1591Widget::createDetailReport() {
    createReport(":/reports/EN1591_detail_report.html");
}

void PCALC_EN1591Widget::createReport(const QString& reportTemplate) {
    PR->clear();
    getTextEdit()->clear();
    setReportSettings();
    setInput();
    Biluna::Calc::EN1591::EN1591Handler handler;
    handler.exec();


    // get report template
    QFile file(reportTemplate);
    file.open(QIODevice::ReadOnly);
    QString report = file.readAll();
    file.flush();

    // parse html template and enter relevant values
    //    <td id="{$pB(1)}">&nbsp;</td><td id="{$pB(2)}">&nbsp;</td>
    //    existObj->setValue("variablename", variableName);
    //    existObj->setValue("result", result);

    // input
    RB_ObjectContainer* inList
            = PR->getInOutContainer()->getContainer("PCALC_InputList");
    RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");
    insertReportInputData(report, in);

    // loadcase
    RB_ObjectContainer* loadCaseList;
    loadCaseList = PR->getInOutContainer()->getContainer("PCALC_LoadCaseList");
    RB_ObjectIterator* lcIter = loadCaseList->createIterator();

    for (lcIter->first(); !lcIter->isDone(); lcIter->next()) {
        RB_ObjectBase* lcObj = lcIter->currentObject();
        insertReportLoadCaseData(report, lcObj);
    }

    delete lcIter;

    // output
    RB_ObjectContainer* outList
            = PR->getInOutContainer()->getContainer("PCALC_OutputList");
    RB_ObjectIterator* iter = outList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* outObj = iter->currentObject();
        insertReportCalculationData(report, outObj);
    }

    delete iter;

    teCalculationReport->setHtml(report);
}

void PCALC_EN1591Widget::insertReportInputData(QString& report,
                                               RB_ObjectBase* obj) {
    QString varName = "";
    double varDouble = 0.0;
    QString varData = "";
    int memberCount = obj->memberCount();
    // TODO: RB2::HIDDENCOLUMNS inconsistent with insertReportLoadCaseData
    for (int i = RB2::HIDDENCOLUMNS; i < memberCount; ++i) {
        RB_ObjectMember* mem = obj->getMember(i);
        varName = mem->getName();
        bool success = false;
        varDouble = mem->getValue().toDouble(&success);

        if (success && !varName.endsWith("_id")) {
            varData = QString::number(varDouble);
        } else {
            // variable value is a string such as a material name
            varData = mem->getValue().toString();

            if (varName.endsWith("_idx")) {
                varData = mem->getDisplayValue().toString();
            }

            if (varName == "typeflange1_id" || varName == "typeflange2_id") {
                int type = mem->getDisplayValue().toInt();

                switch (type) {
                case PCALC2::FlangeBlind :
                    varData = "Blind";
                    break;
                case PCALC2::FlangeIntegral :
                    varData = "Integral";
                    break;
                case PCALC2::FlangeLoose :
                    varData = "Loose";
                    break;
                default :
                    varData = "Invalid";
                    break;
                }
            }
        }

        report.replace("id=\"{$" + varName + "}\">&nbsp;</td>",
                       "id=\"{$" + varName + "}\"><div align=\"right\">"
                       + varData + "</div></td>");
    }

}

void PCALC_EN1591Widget::insertReportLoadCaseData(QString& report,
                                                  RB_ObjectBase* obj) {
    QString varName = "";
    double varDouble = 0.0;
    QString varData = "";
    int loadCaseNo = obj->getValue("loadcaseno").toInt();
    int memberCount = obj->memberCount();

    for (int i = 0; i < memberCount; ++i) {
        RB_ObjectMember* mem = obj->getMember(i);
        varName = mem->getName();
        bool success = false;
        varDouble = mem->getValue().toDouble(&success);

        if (success) {
            varData = QString::number(varDouble);
        } else {
            // variable value is a string such as a material name
            varData = mem->getValue().toString();
        }

        if (loadCaseNo < 0) {
            report.replace("id=\"{$" + varName + "}\">&nbsp;</td>",
                           "id=\"{$" + varName + "}\"><div align=\"right\">"
                           + varData + "</div></td>");
        } else {
            report.replace("id=\"{$" + varName
                           + "[" + QString::number(loadCaseNo) + "]"
                           + "}\">&nbsp;</td>",
                           "id=\"{$" + varName
                           + "[" + QString::number(loadCaseNo) + "]"
                           + "}\"><div align=\"right\">"
                           + varData + "</div></td>");
        }
    }

}

void PCALC_EN1591Widget::insertReportCalculationData(QString& report,
                                                     RB_ObjectBase* obj) {
    QString varName = obj->getValue("variablename").toString();
    double result = obj->getValue("result").toDouble();
    int loadCaseNo = obj->getValue("loadcaseno").toInt();

    if (loadCaseNo < 0) {
        report.replace("id=\"{$" + varName + "}\">&nbsp;</td>",
                       "id=\"{$" + varName + "}\"><div align=\"right\">"
                       + QString::number(result) + "</div></td>");
    } else {
        report.replace("id=\"{$" + varName
                       + "[" + QString::number(loadCaseNo) + "]"
                       + "}\">&nbsp;</td>",
                       "id=\"{$" + varName
                       + "[" + QString::number(loadCaseNo) + "]"
                       + "}\"><div align=\"right\">"
                       + QString::number(result) + "</div></td>");
    }
}

void PCALC_EN1591Widget::createMaterialReport() {
    // Prevent updated of materials in refreshAllProperties
    mIsUpdateMaterial = false;
    PR->clear();
    getTextEdit()->clear();
    setReportSettings();
    refreshAllProperties();
    writeValidationReport();
    mIsUpdateMaterial = true;
}

/**
 * @brief PCALC_EN1591Widget::createValidationReport,
 * last and all validation report
 */
void PCALC_EN1591Widget::createValidationReport() {
    PR->clear();
    getTextEdit()->clear();
    setReportSettings();
    setInput();
    Biluna::Calc::EN1591::EN1591Handler handler;
    handler.exec();
    writeValidationReport();
}

/**
 * @brief PCALC_EN1591Widget::writeValidationReport, write report based
 * on input output object.
 */
void PCALC_EN1591Widget::writeValidationReport() {

    QString po = "<p>";
    QString pc = "</p>";
    QString tbo = "<table border='0'>";
    QString tbc = "</table>";
    QString tro = "<tr>";
    QString trc = "</tr>";
    QString tdo = "<td>";
    QString tdc = "</td>";

    QString dateTimeStr = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Create output report

    QString outputStr;
    outputStr.append(po + "-- Start calculation: " + dateTimeStr + pc);
    outputStr.append(tbo);
    outputStr.append(tro + "<td width='12%'>&nbsp;</td>"
                           "<td width='7%'><strong>CALCULATION</strong></td>"
                           "<td width='8%'>&nbsp;</td>"
                           "<td width='*'>&nbsp;</td>"
                     + trc);
    if (cbCalculationReportType->currentIndex() == 4) { // all iterations + input
        outputStr.append(tro + tdo + tdc
                         + tdo + "<strong>INPUT</strong>" + tdc
                         + tdo + tdc+ tdo + tdc + trc);
        // input
        RB_ObjectContainer* inList
                = PR->getInOutContainer()->getContainer("PCALC_InputList");
        RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");
        int memberCount = in->memberCount();

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
                PR->getInOutContainer()->getContainer("PCALC_LoadCaseList");
        loadCaseList->sort(0, RB2::SortOrderAscending, RB2::MemberInteger);
        RB_ObjectIterator* iterLoad = loadCaseList->createIterator();

        for (iterLoad->first(); !iterLoad->isDone(); iterLoad->next()) {
            in = iterLoad->currentObject();
            memberCount = in->memberCount();

            for (int i = 0; i < memberCount; ++i) {
                RB_ObjectMember* mem = in->getMember(i);
                outputStr.append(tro + tdo + "Input:"
                                 + " [" + in->getValue("loadcaseno").toString() + "]" + tdc
                            + tdo + mem->getName() + " = " + tdc
                            + tdo + mem->getValue().toString()) + tdc + trc;
            }
        }

        delete iterLoad;
    }

    outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc + tdo + tdc + trc);
    outputStr.append(tro + tdo + tdc + tdo + "<strong>OUTPUT</strong>"
                     + tdc + tdo + tdc + tdo + tdc + trc);

    // output
    RB_ObjectContainer* outList
            = PR->getInOutContainer()->getContainer("PCALC_OutputList");
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
}

void PCALC_EN1591Widget::createUnitTestSummary() {
    PR->clear();
    getTextEdit()->clear();
    setReportSettings();

    Biluna::Calc::EN1591::EN1591_UnitTestFactory en1591TestFactory;
    en1591TestFactory.exec();
    STD_UnitTestFactory stdTestFactory;
    stdTestFactory.exec();

    RB_ObjectContainer* outList =
            PR->getInOutContainer()->getContainer("PCALC_OutputList");
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
}

void PCALC_EN1591Widget::createUnitTestDetail() {
    QString po = "<p>";
    QString pc = "</p>";
    QString tbo = "<table border='0'>";
    QString tbc = "</table>";
    QString tro = "<tr>";
    QString trc = "</tr>";
    QString tdo = "<td>";
    QString tdc = "</td>";
    int passedCount = 0;
    int failedCount = 0;

    PR->clear();
    getTextEdit()->clear();
    setReportSettings();

    Biluna::Calc::EN1591::EN1591_UnitTestFactory testFactory;
    testFactory.exec();
    STD_UnitTestFactory stdTestFactory;
    stdTestFactory.exec();

    QString dateTimeStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString outputStr;
    outputStr.append(po + "-- Start UnitTest: " + dateTimeStr + pc);
    outputStr.append(tbo);
    outputStr.append(tro + "<td width='12%'>&nbsp;</td>"
                           "<td width='7%'><strong>UNIT TEST</strong></td>"
                           "<td width='8%'>&nbsp;</td>"
                           "<td width='*'>&nbsp;</td>"
                     + trc);
    outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc + tdo + tdc + trc);
    outputStr.append(tro + tdo + tdc + tdo + "<strong>OUTPUT</strong>"
                     + tdc + tdo + tdc + tdo + tdc + trc);

    // output
    RB_ObjectContainer* outList
            = PR->getInOutContainer()->getContainer("PCALC_OutputList");
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

        str = obj->getValue("note").toString();

        if (!str.isEmpty()) {
            outputStr.append(tro + tdo + tdc + tdo + tdc + tdo + tdc
                             + tdo + str + tdc + trc);

            if (str.contains("- test OK")) {
                ++passedCount;
            } else {
                ++failedCount;
            }
        }
    }

    delete iter;
    outputStr.append(tbc);
    outputStr.append(po + "Tests Passed: " + QString::number(passedCount) + pc);
    outputStr.append(po + "Tests Failed: " + QString::number(failedCount) + pc);
    outputStr.append(po + "-- End UnitTest:"
                     + QDateTime::currentDateTime().toString(Qt::ISODate) + pc);
    getTextEdit()->setHtml(outputStr);
}

void PCALC_EN1591Widget::setBlindFlange1EnData(RB_ObjectBase* compObj,
                                             RB_ObjectBase* facingObj) {


}

void PCALC_EN1591Widget::setBlindFlange2EnData(RB_ObjectBase* compObj,
                                             RB_ObjectBase* facingObj) {

}

void PCALC_EN1591Widget::setIntegralFlange1EnData(RB_ObjectBase* compObj,
                                                  RB_ObjectBase* facingObj) {

}

void PCALC_EN1591Widget::setIntegralFlange2EnData(RB_ObjectBase* compObj,
                                                  RB_ObjectBase* facingObj) {

}

void PCALC_EN1591Widget::setLooseFlange1EnData(RB_ObjectBase* compObj,
                                               RB_ObjectBase* facingObj) {

}

void PCALC_EN1591Widget::setLooseFlange2EnData(RB_ObjectBase* compObj,
                                             RB_ObjectBase* facingObj) {

}

void PCALC_EN1591Widget::setBlindFlange1AsmeData(RB_ObjectBase* compObj,
                                                 RB_ObjectBase* facingObj)
{

}

void PCALC_EN1591Widget::setBlindFlange2AsmeData(RB_ObjectBase* compObj,
                                                 RB_ObjectBase* facingObj)
{

}

void PCALC_EN1591Widget::setIntegralFlange1AsmeData(RB_ObjectBase* flangeObj,
                                                    RB_ObjectBase* facingObj,
                                                    STD2::AsmeFlangeType flangeType,
                                                    STD2::AsmeFlangeFacingType facingType) {
//    RB_DEBUG->printObject(flangeObj);
//    RB_DEBUG->printObject(facingObj);

    setModelVariable(mFlangeModel, "nb", flangeObj->getValue("nob").toInt());

    setModelVariable(mFlangeModel, "typeflange1_id", 1);
    setModelVariable(mFlangeModel, "d31", flangeObj->getValue("w").toDouble());
    setModelVariable(mFlangeModel, "d41", flangeObj->getValue("o").toDouble());

    if (facingType == STD2::AsmeFlangeFacingRTJ) {
        setModelVariable(mFlangeModel, "dx1",
                         facingObj->getValue("p").toDouble()
                         + facingObj->getValue("f").toDouble());
    } else {
        setModelVariable(mFlangeModel, "dx1", 0.0);
    }

    setModelVariable(mFlangeModel, "drf1", facingObj->getValue("r").toDouble());

    if (facingType == STD2::AsmeFlangeFacingSRF || facingType == STD2::AsmeFlangeFacingTGF) {
        // TODO: Biluna model for tongue-groove spigot-recess not complete and clear for diameters
        setModelVariable(mFlangeModel, "drec1", 0.0);
    } else {
        setModelVariable(mFlangeModel, "drec1", 0.0);
    }

    setModelVariable(mFlangeModel, "efb1", flangeObj->getValue("tf1").toDouble());
    setModelVariable(mFlangeModel, "erf1", facingObj->getValue("hrf").toDouble());

    if (facingType == STD2::AsmeFlangeFacingSRF || facingType == STD2::AsmeFlangeFacingTGF) {
        // TODO: Biluna model for tongue-groove spigot-recess not complete and clear for diameters
        setModelVariable(mFlangeModel, "erec1", 0.0);
    } else {
        setModelVariable(mFlangeModel, "erec1", 0.0);
    }

    setModelVariable(mFlangeModel, "eq1", 0.0);

    if (facingType == STD2::AsmeFlangeFacingRTJ) {
        setModelVariable(mFlangeModel, "ex1",
                         flangeObj->getValue("tf1").toDouble()
                         - facingObj->getValue("e").toDouble());
    } else {
        setModelVariable(mFlangeModel, "ex1", 0.0);
    }

    // bolthole
    setModelVariable(mFlangeModel, "d51", STD2::inchToMm(flangeObj->getValue("diambh").toDouble()));
    setModelVariable(mFlangeModel, "blindhole1", 0);
    setModelVariable(mFlangeModel, "d5t1", 0.0);
    setModelVariable(mFlangeModel, "l5t1", 0.0);

    // blind only
    if (flangeType == STD2::AsmeFlangeBLD) {
        setModelVariable(mFlangeModel, "d91", 0.0);
        setModelVariable(mFlangeModel, "e01",
                         flangeObj->getValue("tf1").toDouble()
                         + facingObj->getValue("hrf").toDouble());
    } else {
        setModelVariable(mFlangeModel, "d91", 0.0);
        setModelVariable(mFlangeModel, "e01", 0.0);
    }

    double b3 = flangeObj->getValue("b3").toDouble();

    if (b3 <= 0.0) {
        QString value = PCALC_DIALOGFACTORY->requestTextInputDialog(
                    "Internal bore not standard",
                    "Specify internal bore B3 [mm]",
                    "0.0");
        b3 = value.toDouble();
    }

    // integral, loose only
    if (flangeType != STD2::AsmeFlangeBLD) {
        setModelVariable(mFlangeModel, "d01", b3);
        // Threaded is actually depending the inside diameter of the pipe
    } else {
        setModelVariable(mFlangeModel, "d01", 0.0);
    }

    // integral, loose (if hub) only
    if (flangeType != STD2::AsmeFlangeBLD) {

        if (b3 > 0.0) {
            setModelVariable(mFlangeModel, "d11",
                             (flangeObj->getValue("ah").toDouble() + b3) / 2);
            setModelVariable(mFlangeModel, "d21",
                             (flangeObj->getValue("x").toDouble() + b3) / 2);
            setModelVariable(mFlangeModel, "e11",
                             (flangeObj->getValue("ah").toDouble() - b3) / 2);
            setModelVariable(mFlangeModel, "e21",
                             (flangeObj->getValue("x").toDouble() - b3) / 2);
        } else {
            setModelVariable(mFlangeModel, "d11", 0.0);
            setModelVariable(mFlangeModel, "d21", 0.0);
            setModelVariable(mFlangeModel, "e11", 0.0);
            setModelVariable(mFlangeModel, "e21", 0.0);
        }

        setModelVariable(mFlangeModel, "lh1",
                         flangeObj->getValue("y3").toDouble()
                         - flangeObj->getValue("tf1").toDouble());
    } else {
        setModelVariable(mFlangeModel, "d11", 0.0);
        setModelVariable(mFlangeModel, "d21", 0.0);
        setModelVariable(mFlangeModel, "e11", 0.0);
        setModelVariable(mFlangeModel, "e21", 0.0);
        setModelVariable(mFlangeModel, "lh1", 0.0);
    }

    // loose only
    if (flangeType == STD2::AsmeFlangeLPD) {
        // TODO:
        setModelVariable(mFlangeModel, "b01", flangeObj->getValue("").toDouble());
        setModelVariable(mFlangeModel, "d61", flangeObj->getValue("").toDouble());
        setModelVariable(mFlangeModel, "d81", flangeObj->getValue("").toDouble());
        setModelVariable(mFlangeModel, "el1", flangeObj->getValue("").toDouble());
    } else {
        setModelVariable(mFlangeModel, "b01", 0.0);
        setModelVariable(mFlangeModel, "d61", 0.0);
        setModelVariable(mFlangeModel, "d81", 0.0);
        setModelVariable(mFlangeModel, "el1", 0.0);
    }

//    setModelVariable(mFlangeModel, "materialflange1_idx", flangeObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "materialloosering1_idx", flangeObj->getValue("").toDouble());

}

void PCALC_EN1591Widget::setIntegralFlange2AsmeData(RB_ObjectBase* compObj,
                                                    RB_ObjectBase* facingObj) {
    setModelVariable(mFlangeModel, "typeflange2_id", 1);
    setModelVariable(mFlangeModel, "d32", compObj->getValue("w").toDouble());
    setModelVariable(mFlangeModel, "d42", compObj->getValue("o").toDouble());
//    setModelVariable(mFlangeModel, "dx2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "drf2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "drec2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "efb2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "erf2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "erec2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "eq2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "ex2", compObj->getValue("").toDouble());
//    // bolthole
//    setModelVariable(mFlangeModel, "d52", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "blindhole2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "d5t2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "l5t2", compObj->getValue("").toDouble());
//    // blind only
//    setModelVariable(mFlangeModel, "d92", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "e02", compObj->getValue("").toDouble());
//    // integral, loose only
//    setModelVariable(mFlangeModel, "d02", compObj->getValue("").toDouble());
//    // integral, loose (if hub) only
//    setModelVariable(mFlangeModel, "d12", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "d22", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "e12", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "e22", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "lh2", compObj->getValue("").toDouble());
//    // loose only
//    setModelVariable(mFlangeModel, "b02", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "d62", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "d82", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "el2", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "materialflange2_idx", compObj->getValue("").toDouble());
//    setModelVariable(mFlangeModel, "materialloosering2_idx", compObj->getValue("").toDouble());

}

void PCALC_EN1591Widget::setLooseFlange1AsmeData(RB_ObjectBase* compObj,
                                                 RB_ObjectBase* facingObj)
{

}

void PCALC_EN1591Widget::setLooseFlange2AsmeData(RB_ObjectBase* compObj,
                                                 RB_ObjectBase* facingObj)
{

}

void PCALC_EN1591Widget::setBoltAsmeData(RB_ObjectBase* boltObj,
                                         RB_ObjectBase* /*nutObj*/) {
    RB_DEBUG->printObject(boltObj);
    //    RB_DEBUG->printObject(nutObj);
    setModelVariable(mBoltNutWasherModel, "bolttype_id", 1); // Stud bolt
    setModelVariable(mBoltNutWasherModel, "tensionertype_id", 3); // Torque wrench
    double dB0 = STD2::inchToMm(boltObj->getValue("nomsize").toDouble());
    setModelVariable(mBoltNutWasherModel, "db0", dB0);
    double dB2 = STD2::inchToMm(boltObj->getValue("d2").toDouble());
    setModelVariable(mBoltNutWasherModel, "db2", dB2);
    // nut width accross flats
    double fBasic = STD2::inchToMm(boltObj->getValue("fbasic").toDouble());
    setModelVariable(mBoltNutWasherModel, "db4", fBasic);
    double pt = STD2::inchToMm(1.0) / boltObj->getValue("thr").toDouble();
    double dBe = dB0 - 0.9743 * pt;
    setModelVariable(mBoltNutWasherModel, "dbe", dBe); //
    setModelVariable(mBoltNutWasherModel, "dbs", 0.0);
    setModelVariable(mBoltNutWasherModel, "dn", 0.0);
    // nut thickness
    double hBasic = STD2::inchToMm(boltObj->getValue("hbasic").toDouble());
    setModelVariable(mBoltNutWasherModel, "en", hBasic);
    setModelVariable(mBoltNutWasherModel, "ls", 0.0);
    setModelVariable(mBoltNutWasherModel, "mun", 0.14);
    setModelVariable(mBoltNutWasherModel, "mut", 0.12);
    setModelVariable(mBoltNutWasherModel, "pt", pt);
}

void PCALC_EN1591Widget::setBoltEnData(RB_ObjectBase* boltObj,
                                       RB_ObjectBase* nutObj) {
    RB_DEBUG->printObject(boltObj);

}

void PCALC_EN1591Widget::setModelVariable(RB_MmProxy* model,
                                          const QString& fieldName,
                                          double value) {
    QModelIndex idx = model->index(
                model->getCurrentIndex().row(),
                model->fieldIndex(fieldName));
    model->setData(idx, value);
}

void PCALC_EN1591Widget::addObjectMemberVariable(RB_ObjectBase* obj,
                                            const QString& variableName,
                                            const QString& unit,
                                            RB_MmProxy* model) {
    QModelIndex idx = model->index(
                model->getCurrentIndex().row(),
                model->fieldIndex(variableName));
    if (variableName.endsWith("_idx")) {
        obj->addMember(variableName, unit,
                       model->data(idx, RB2::RoleOrigData).toString()
                       + model->data(idx, Qt::DisplayRole).toString());
    } else {
        obj->addMember(variableName, unit, model->data(idx, RB2::RoleOrigData));
    }
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

void PCALC_EN1591Widget::updateAllowStress(const QString& materialId,
                                           const QString& temperatureField,
                                           const QString& allowStressField,
                                           STD2::CompType compType) {
    STD_MATERIALUTILITY->setCurrentMaterial(materialId);

    if (!STD_MATERIALUTILITY->isValid()) {
        PCALC_DIALOGFACTORY->commandMessage("Update allowable stress: "
                                            "material not valid");
        return;
    }

    STD2::CompType useCompType = compType;
    bool useTestForAssemblyCondition = false;

    if (cbDisregardBoltRequirement->currentIndex() == 1
            && useCompType == STD2::CompBolt) {
        // disregards EN13445 11.4.3
        useCompType = STD2::CompDefault;
    } else if (cbDisregardBoltRequirement->currentIndex() == 2) {
        // disregards EN13445 11.4.3 and use test values for assembly condition
        useTestForAssemblyCondition = true;

        if (useCompType == STD2::CompBolt) {
            useCompType = STD2::CompDefault;
        }
    }

    int row = 0;
    int rowCount = mLoadCaseModel->rowCount();
    int colTemperature = mLoadCaseModel->fieldIndex(temperatureField);
    int colAllowStress = mLoadCaseModel->fieldIndex(allowStressField);
    QModelIndex index;
    double designTemp = 0.0;
    double allowStress = 0.0;
    int loadCaseNo = -1;

    for (row = 0; row < rowCount; row++) {
        index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("loadcaseno"));
        loadCaseNo = mLoadCaseModel->data(index).toInt();
        // get design temperature and corresponding allowable stress
        index = mLoadCaseModel->index(row, colTemperature);
        designTemp = mLoadCaseModel->data(index).toDouble();

        if (loadCaseNo == 1) {
            // Regular test loadcase conditions
            allowStress = STD_MATERIALUTILITY->allowableTestStress(
                        designTemp, useCompType, loadCaseNo, allowStressField);
        } else if (loadCaseNo == 0 && useTestForAssemblyCondition) {
            // Use test loadcase conditions for assembly
            allowStress = STD_MATERIALUTILITY->allowableTestStress(
                        designTemp, useCompType, loadCaseNo, allowStressField);
        } else {
            allowStress = STD_MATERIALUTILITY->allowableDesignStress(
                        designTemp, useCompType, loadCaseNo, allowStressField);
        }

        if (mIsUpdateMaterial) {
            // set allowable stress
            index = mLoadCaseModel->index(row, colAllowStress);
            mLoadCaseModel->setData(index, allowStress);
        }
    }
}

void PCALC_EN1591Widget::updateElasModul(const QString& materialId,
                                         const QString& temperatureField,
                                         const QString& elasModulField) {
    STD_MATERIALUTILITY->setCurrentMaterial(materialId);

    if (!STD_MATERIALUTILITY->isValid()) {
        PCALC_DIALOGFACTORY->commandMessage("Update elasticity modulus: "
                                            "material not valid");
        return;
    }

    int row = 0;
    int rowCount = mLoadCaseModel->rowCount();
    int colTemperature = mLoadCaseModel->fieldIndex(temperatureField);
    int colElasModul = mLoadCaseModel->fieldIndex(elasModulField);
    QModelIndex index;
    double designTemp = 0.0;
    double elasModul = 0.0;
    int loadCaseNo = -1;

    for (row = 0; row < rowCount; row++) {
        index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("loadcaseno"));
        loadCaseNo = mLoadCaseModel->data(index).toInt();
        // get design temperature and corresponding elasticity modulus
        index = mLoadCaseModel->index(row, colTemperature);
        designTemp = mLoadCaseModel->data(index).toDouble();
        elasModul = STD_MATERIALUTILITY->elasticityModulus(
                    designTemp, loadCaseNo, elasModulField);

        if (mIsUpdateMaterial) {
            // set elasticity modulus
            index = mLoadCaseModel->index(row, colElasModul);
            mLoadCaseModel->setData(index, elasModul);
        }
    }
}

void PCALC_EN1591Widget::updateThermExp(const QString& materialId,
                                        const QString& temperatureField,
                                        const QString& thermExpField) {
    STD_MATERIALUTILITY->setCurrentMaterial(materialId);

    if (!STD_MATERIALUTILITY->isValid()) {
        PCALC_DIALOGFACTORY->commandMessage("Update thermal expansion: "
                                            "material not valid");
        return;
    }

    int row = 0;
    int rowCount = mLoadCaseModel->rowCount();
    int colTemperature = mLoadCaseModel->fieldIndex(temperatureField);
    int colThermExp = mLoadCaseModel->fieldIndex(thermExpField);
    QModelIndex index;
    double designTemp = 0.0;
    double thermExp = 0.0;
    int loadCaseNo = -1;

    for (row = 0; row < rowCount; row++) {
        index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("loadcaseno"));
        loadCaseNo = mLoadCaseModel->data(index).toInt();
        // get design temperature and corresponding allowable stress
        index = mLoadCaseModel->index(row, colTemperature);
        designTemp = mLoadCaseModel->data(index).toDouble();
        thermExp = STD_MATERIALUTILITY->thermalExpansion(
                    designTemp, loadCaseNo, thermExpField);

        if (mIsUpdateMaterial) {
            // set thermal expansion
            index = mLoadCaseModel->index(row, colThermExp);
            mLoadCaseModel->setData(index, thermExp);
        }
    }
}

void PCALC_EN1591Widget::refreshAllProperties() {
    QModelIndex index = mFlangeModel->getCurrentIndex();

    if (!index.isValid()) {
        RB_DEBUG->debug("PCALC_EN1591Widget::refreshAllProperties() "
                        "index not valid");
        return;
    }

    int row = index.row();
    index = mFlangeModel->index(
                row, mFlangeModel->fieldIndex("flange2equal"));
    bool flange2equal = 1 == index.data().toInt();
    index = mFlangeModel->index(
                row, mFlangeModel->fieldIndex("typeflange1_id"));
    bool flange1blind = 0 == index.data().toInt();
    bool flange1loose = 2 == index.data().toInt();
    index = mFlangeModel->index(
                row, mFlangeModel->fieldIndex("typeflange2_id"));
    bool flange2blind = 0 == index.data().toInt();
    bool flange2loose = 2 == index.data().toInt();
    index = mShellModel->index(
                row, mShellModel->fieldIndex("shell2equal"));
    bool shell2equal = 1 == index.data().toInt();

    index = mFlangeModel->index(
                row, mFlangeModel->fieldIndex("materialflange1_idx"));
    QString materialId = index.data(RB2::RoleOrigData).toString();
    updateAllowStress(materialId, "tf1", "ff1", STD2::CompFlange);
    updateElasModul(materialId, "tf1", "ef1");
    updateThermExp(materialId, "tf1", "alphaf1");

    if (flange1loose) {
        index = mFlangeModel->index(
                    row, mFlangeModel->fieldIndex("materialloosering1_idx"));
        materialId = index.data(RB2::RoleOrigData).toString();
        updateAllowStress(materialId, "tl1", "fl1", STD2::CompFlange);
        updateElasModul(materialId, "tf1", "el1");
        updateThermExp(materialId, "tf1", "alphal1");
    }

    if (!flange2equal) {
        index = mFlangeModel->index(
                    row, mFlangeModel->fieldIndex("materialflange2_idx"));
        materialId = index.data(RB2::RoleOrigData).toString();
        updateAllowStress(materialId, "tf2", "ff2", STD2::CompFlange);
        updateElasModul(materialId, "tf2", "ef2");
        updateThermExp(materialId, "tf2", "alphaf2");

        if (flange2loose) {
            index = mFlangeModel->index(
                        row, mFlangeModel->fieldIndex("materialloosering2_idx"));
            materialId = index.data(RB2::RoleOrigData).toString();
            updateAllowStress(materialId, "tl2", "fl2", STD2::CompFlange);
            updateElasModul(materialId, "tf2", "el2");
            updateThermExp(materialId, "tf2", "alphal2");
        }
    }

    index = mBoltNutWasherModel->getCurrentIndex();
    row = index.row();
    index = mBoltNutWasherModel->index(
                row, mBoltNutWasherModel->fieldIndex("materialbolt_idx"));
    materialId = index.data(RB2::RoleOrigData).toString();
    updateAllowStress(materialId, "tb", "fb", STD2::CompBolt);
    updateAllowStress(materialId, "tb", "fn", STD2::CompBolt); // no separate material yet
    updateElasModul(materialId, "tb", "eb");
    updateThermExp(materialId, "tb", "alphab");
    index = mBoltNutWasherModel->index(
                row, mBoltNutWasherModel->fieldIndex("materialwasher_idx"));
    materialId = index.data(RB2::RoleOrigData).toString();

    if (RB2::isValidId(materialId)) {
        updateAllowStress(materialId, "tw1", "fw1", STD2::CompWasher);
        updateElasModul(materialId, "tw1", "ew1");
        updateThermExp(materialId, "tw1", "alphaw1");
        updateAllowStress(materialId, "tw2", "fw2", STD2::CompWasher);
        updateElasModul(materialId, "tw2", "ew2");
        updateThermExp(materialId, "tw2", "alphaw2");
    }

    index = mShellModel->getCurrentIndex();
    row = index.row();

    if (!flange1blind) {
        index = mShellModel->index(
                    row, mShellModel->fieldIndex("materialshell1_idx"));
        materialId = index.data(RB2::RoleOrigData).toString();
        updateAllowStress(materialId, "tf1", "fs1", STD2::CompCylinder);
    }

    if (!shell2equal && !flange2blind) {
        index = mShellModel->index(
                    row, mShellModel->fieldIndex("materialshell2_idx"));
        materialId = index.data(RB2::RoleOrigData).toString();
        updateAllowStress(materialId, "tf2", "fs2", STD2::CompCylinder);
    }
}

void PCALC_EN1591Widget::syncTemperatures(int row) {
    QModelIndex index = mLoadCaseModel->index(
                row, mLoadCaseModel->fieldIndex("t0"));
    double designTemp = mLoadCaseModel->data(index).toDouble();
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tb"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tg"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tf1"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tl1"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tw1"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tf2"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tl2"));
    mLoadCaseModel->setData(index, designTemp);
    index = mLoadCaseModel->index(row, mLoadCaseModel->fieldIndex("tw2"));
    mLoadCaseModel->setData(index, designTemp);
}

