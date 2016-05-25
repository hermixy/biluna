/*****************************************************************
 * $Id: rb_projecteditwidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_projecteditwidget.h"
#include "ui_rb_projecteditwidget.h"


RB_ProjectEditWidget::RB_ProjectEditWidget(QWidget *parent) :
        RB_Widget(parent), ui(new Ui::RB_ProjectEditWidget) {
    ui->setupUi(this);

    mModel = NULL;
    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;
}

RB_ProjectEditWidget::~RB_ProjectEditWidget() {
    delete ui;
    RB_DEBUG->debug("RB_ProjectEditWidget::~RB_ProjectEditWidget() OK");
}

//RB_String RB_ProjectEditWidget::getName() const {
//    return "RB_ProjectEditWidget";
//}

//RB2::PerspectiveType RB_ProjectEditWidget::getPerspectiveType() const {
//    return RB2::PerspectiveDB;
//}

void RB_ProjectEditWidget::init() {
    setWindowTitle(tr("Edit Projects[*]"));

    //
    // 1. Set model with ID by implementation
    //
//    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelProjectEdit, false);
//    mModel->setRoot(""); // TODO: depending user rights

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(ui->leNumber, mModel->fieldIndex("number"));
    mMapper->addMapping(ui->pteDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(ui->leCompany, mModel->fieldIndex("company"));
    mMapper->addMapping(ui->leLocation, mModel->fieldIndex("location"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    formatTableView(ui->tableView, mModel);
    ui->tbb->initEdit(false, true, true, false);
    ui->tableView->setToolButtonBar(ui->tbb);

    connect(ui->tbb, SIGNAL(addClicked()),
            this, SLOT(pbAddclicked()));

    readSettings();

    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            ui->tableView->hideColumn(i);
        } else {
            ui->tableView->showColumn(i);

            if (ui->tableView->columnWidth(i) < 5) {
                ui->tableView->setColumnWidth(i, 100);
            }
        }
    }
}

bool RB_ProjectEditWidget::fileSave(bool withSelect) {
    if (!mModel) return false;
    beforeFileSave();

    mCurrentModelIndex = ui->tableView->currentIndex();

    if (!mCurrentModelIndex.isValid()) {
        mCurrentObject = NULL;
    } else {
        // for selection windows
        setCurrentObject();
    }

    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->print("DB_TableWidget::fileSave() ERROR");
    return false;
}

void RB_ProjectEditWidget::fileRevert() {
    if (mModel) {
        mModel->revertAll();
        setWindowModified(false);
    }
}

void RB_ProjectEditWidget::setHelpSubject(const QString& subject) {
    mHelpSubject = subject;

}

RB_ObjectBase* RB_ProjectEditWidget::getCurrentObject() {
    return mCurrentObject;
}

void RB_ProjectEditWidget::setCurrentObject() {
    if (ui->tableView->currentIndex().isValid()) {
        mCurrentObject = mModel->getCurrentObject();
    } else {
        mCurrentObject = NULL;
    }
}

void RB_ProjectEditWidget::pbAddclicked() {
    // undo filter and add row already done
    ui->leNumber->setFocus();
    ui->leNumber->selectAll();
}
