/*****************************************************************
 * $Id: std_selectmaterialwidget.cpp 0 2016-06-01 15:23:20Z rutger $
 * Created: June 18, 2016 15:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "std_selectmaterialwidget.h"
#include "ui_std_selectmaterialwidget.h"

#include "pcalc_modelfactory.h"
#include "rb_cmbdelegate.h"

STD_SelectMaterialWidget::STD_SelectMaterialWidget(QWidget *parent)
        : RB_Widget(parent), ui(new Ui::STD_SelectMaterialWidget) {
    ui->setupUi(this);
}

STD_SelectMaterialWidget::~STD_SelectMaterialWidget() {
    delete mModel;
    delete ui;
    RB_DEBUG->debug("STD_SelectMaterialWidget::~STD_SelectMaterialWidget()");
}

QString STD_SelectMaterialWidget::getName() const {
    return "Select Material";
}

RB2::PerspectiveType STD_SelectMaterialWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}

void STD_SelectMaterialWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMaterial, false);
    mModel->setRoot("");
//    mModel->setWhere("id <> '0'");
    initModelMapping();
    mModel->select();

    formatTableView(ui->tvMaterial, mModel);
    ui->tbbMaterial->initSelect();
    ui->tvMaterial->setToolButtonBar(ui->tbbMaterial);

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        ui->tvMaterial->hideColumn(i);
    }

    readSettings();
}

RB_ObjectBase *STD_SelectMaterialWidget::getCurrentObject() {
    if (ui->tvMaterial->currentIndex().isValid()) {
        return mModel->getCurrentObject();
    }

    return nullptr;
}

void STD_SelectMaterialWidget::initModelMapping() {
    QStringList items;
    items << "NONE" << "BAR" << "BOLTING" << "CASTING" << "FITTING"
          << "FORGING" << "NUT" << "PIPE" << "PLATE" << "ROD" << "TUBE";
    ui->tvMaterial->setItemDelegateForColumn(
                mModel->fieldIndex("type_id"),
                new RB_CmbDelegate(this, items, mModel, false));
    mModel->setTextList(mModel->fieldIndex("type_id"), items); // delegate not yet complete
    items.clear();
    items << "NONE" << "CARBON STEEL" << "HIGH ALLOY" << "LOW ALLOY"
          << "STAINLESS STEEL" << "NON-FERROUS";
    ui->tvMaterial->setItemDelegateForColumn(
                mModel->fieldIndex("class_id"),
                new RB_CmbDelegate(this, items, mModel, false));
    mModel->setTextList(mModel->fieldIndex("class_id"), items);
    items.clear();
    items << "None" << "Ferritic" << "Austenitic" << "Martensitic"
          << "Austenitic-Ferritic" << "Casting" << "Non-Ferrous" << "other";
    ui->tvMaterial->setItemDelegateForColumn(
                mModel->fieldIndex("structure_id"),
                new RB_CmbDelegate(this, items, mModel, false));
    mModel->setTextList(mModel->fieldIndex("structure_id"), items);

    // Instead of add
    //    addComboBoxMapping(mModel, "elasmodultable_id",
    //                       "std_elasmodultable", "id",
    //                       "mname", ui->cbElasModulTable, mapper);
    int relationColumn = mModel->fieldIndex("elasmodultable_id");
    mModel->setRelation(relationColumn,
           RB_SqlRelation("std_elasmodultable", "id", "mname"));
    relationColumn = mModel->fieldIndex("thermexptable_id");
    mModel->setRelation(relationColumn,
           RB_SqlRelation("std_thermexptable", "id", "mname"));
    relationColumn = mModel->fieldIndex("creeptable_id");
    mModel->setRelation(relationColumn,
           RB_SqlRelation("std_creeptable", "id", "mname"));
    relationColumn = mModel->fieldIndex("extpresstable_id");
    mModel->setRelation(relationColumn,
           RB_SqlRelation("std_extpresstable", "id", "mname"));
}
