/*****************************************************************
 * $Id: std_materialwidget.cpp 0 2016-06-01 15:23:20Z rutger $
 * Created: June 1, 2016 15:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "std_materialwidget.h"
#include "ui_std_materialwidget.h"

#include "pcalc_modelfactory.h"

STD_MaterialWidget::STD_MaterialWidget(QWidget *parent)
        : RB_Widget(parent), ui(new Ui::STD_MaterialWidget) {
    ui->setupUi(this);
}

STD_MaterialWidget::~STD_MaterialWidget() {
    delete mRp02Model;
    delete mRp10Model;
    delete mRmMinModel;
    delete mModel;
    delete ui;
    RB_DEBUG->debug("STD_MaterialWidget::~STD_MaterialWidget()");
}

QString STD_MaterialWidget::getName() const {
    return "Material Elasticity Modulus Tables";
}

RB2::PerspectiveType STD_MaterialWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}

void STD_MaterialWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMaterial, false);
    mModel->setRoot("");
    mModel->setWhere("id <> '0'");
    mModel->select();

    formatTableView(ui->tvMaterial, mModel);
    ui->tbbMaterial->initEdit(false, true, true, false);
    ui->tvMaterial->setToolButtonBar(ui->tbbMaterial);

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        ui->tvMaterial->hideColumn(i);
    }

    mRp02Model = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRp02, false);

    formatTableView(ui->tvRp02, mRp02Model);
    ui->tbbRp02->initSlimTable(); // initEdit(false, false, false, false);
    ui->tvRp02->setToolButtonBar(ui->tbbRp02);

    for (int i = 0; i < count; i++) {
        ui->tvRp02->hideColumn(i);
    }

    mRp10Model = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRp10, false);

    formatTableView(ui->tvRp10, mRp10Model);
    ui->tbbRp10->initSlimTable();
    ui->tvRp10->setToolButtonBar(ui->tbbRp10);

    for (int i = 0; i < count; i++) {
        ui->tvRp10->hideColumn(i);
    }

    mRmMinModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRmMin, false);

    formatTableView(ui->tvRmMin, mRmMinModel);
    ui->tbbRmMin->initSlimTable();
    ui->tvRmMin->setToolButtonBar(ui->tbbRmMin);

    for (int i = 0; i < count; i++) {
        ui->tvRmMin->hideColumn(i);
    }

    initModelMapping();
    readSettings();
}

bool STD_MaterialWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        if (mRp02Model->submitAllAndSelect()
                && mRp10Model->submitAllAndSelect()
                && mRmMinModel->submitAllAndSelect()
                && mModel->submitAllAndSelect()) {
            setWindowModified(false);
            return true;
        }
    } else {
        if (mRp02Model->submitAll()
                && mRp10Model->submitAll()
                && mRmMinModel->submitAll()
                && mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->error("STD_MaterialWidget ERROR");
    return false;
}

void STD_MaterialWidget::fileRevert() {
    mRp02Model->revertAll();
    mRp10Model->revertAll();
    mRmMinModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);
}

void STD_MaterialWidget::initModelMapping() {
    RB_DataWidgetMapper* mapper = mModel->getMapper();
    mapper->addMapping(ui->leName, mModel->fieldIndex("mname"));
    mapper->addMapping(ui->leNumber, mModel->fieldIndex("number"));

    QStringList items;
    items << "NONE" << "BAR" << "BOLTING" << "CASTING" << "FITTING"
          << "FORGING" << "NUT" << "PIPE" << "PLATE" << "ROD" << "TUBE";
    ui->cbType->setModel(new QStringListModel(items, this));
    mapper->addMapping(ui->cbType, mModel->fieldIndex("type_id"), "currentIndex");

    items.clear();
    items << "NONE" << "CARBON STEEL" << "HIGH ALLOY" << "LOW ALLOY"
          << "STAINLESS STEEL" << "NON-FERROUS";
    ui->cbClass->setModel(new QStringListModel(items, this));
    mapper->addMapping(ui->cbClass, mModel->fieldIndex("class_id"), "currentIndex");

    items.clear();
    items << "None" << "Ferritic" << "Austenitic" << "Martensitic"
          << "Austenitic-Ferritic" << "Non-Ferrous" << "other";
    ui->cbStructure->setModel(new QStringListModel(items, this));
    mapper->addMapping(ui->cbStructure, mModel->fieldIndex("structure_id"), "currentIndex");

    mapper->addMapping(ui->leComposition, mModel->fieldIndex("composition"));
    mapper->addMapping(ui->leDocument, mModel->fieldIndex("document"));
    mapper->addMapping(ui->leIssue, mModel->fieldIndex("issue"));

    addComboBoxMapping(mModel, "elasmodultable_id",
                       "std_elasmodultable", "id",
                       "mname", ui->cbElasModulTable, mapper);
    addComboBoxMapping(mModel, "thermexptable_id",
                       "std_thermexptable", "id",
                       "mname", ui->cbThermExpTable, mapper);
    addComboBoxMapping(mModel, "creeptable_id",
                       "std_creeptable", "id",
                       "mname", ui->cbCreepTable, mapper);
    addComboBoxMapping(mModel, "extpresstable_id",
                       "std_extpresstable", "id",
                       "mname", ui->cbExtPressTable, mapper);

//    int relationColumn = mModel->fieldIndex("elasmodultable_id");
//    mModel->setRelation(relationColumn,
//           RB_SqlRelation("std_elasmodultable", "id", "mname"));


/*
    addMember("mname", "-", "", RB2::MemberChar125);
    addMember("number", "-", "", RB2::MemberChar125);
    addMember("type_id", "-", "", RB2::MemberInteger);
    addMember("class_id", "-", "", RB2::MemberInteger);
    addMember("structure_id", "-", 0, RB2::MemberInteger);
    addMember("composition", "-", "", RB2::MemberChar125);
    addMember("document", "-", "", RB2::MemberChar125);
    addMember("issue", "-", "", RB2::MemberChar40);

    addMember("elasmodultable_id", "-", "", RB2::MemberChar40);
    addMember("thermexptable_id", "-", "", RB2::MemberChar40);
    addMember("creeptable_id", "-", "", RB2::MemberChar40);
    addMember("extpresstable_id", "-", "", RB2::MemberChar40);

    addMember("yield20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("yield1p20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("tensile20c", "MPa", 0.0, RB2::MemberDouble);
    addMember("specificgravity", "kg/dm3", 0.0, RB2::MemberDouble);
    addMember("elongafterrupt", "percent", 0.0, RB2::MemberDouble);
    addMember("comment", "-", "", RB2::MemberChar255);
    addMember("wstgruppe", "-", "", RB2::MemberChar125);
    addMember("form_en", "-", "", RB2::MemberChar125);
*/
}
