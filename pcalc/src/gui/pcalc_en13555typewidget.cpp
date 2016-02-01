/*****************************************************************
 * $Id: pcalc_ten13555typewidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 01, 2016 21:25:40 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en13555typewidget.h"

PCALC_EN13555TypeWidget::PCALC_EN13555TypeWidget(QWidget* parent)
                : DB_TableWidget(parent) {
    // HACK: for write settings otherwise RB_DialogBase
    this->setObjectName("PCALC_EN13555TypeWidget");
}

PCALC_EN13555TypeWidget::~PCALC_EN13555TypeWidget() {
    // Dialogs create and delete models by themselves
    if (mModel) {
        delete mModel;
        mModel = NULL;
    }
}

RB_String PCALC_EN13555TypeWidget::getName() const {
    return(tr("Select EN13555 Gasket Type[*]"));
}

void PCALC_EN13555TypeWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(PCALC_ModelFactory::ModelEN13555Gasket, false);
    mModel->setRoot(PCALC_MODELFACTORY->getStoredId());
    mModel->select();

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        tableView->hideColumn(i);
    }

    // lblRoot->setText(tr("Plant Engineering Project"));
    lblRoot->hide();
    leRoot->hide();

    gbTable->setTitle(tr("List of gaskets"));
    lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

    this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

    this->setHelpSubject("PCALC_EN13555TypeWidget");

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        tableView->hideColumn(i);
    }

    // Make sure there is one row
    if (mModel->rowCount() < 1) {
        on_pbAdd_clicked();
    }

    readSettings();
}

void PCALC_EN13555TypeWidget::on_pbAdd_clicked() {
    mModel->undoFilterSort();
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("type"),
                                    QModelIndex());
    mModel->setData(idx, tr("<NEW>"), Qt::EditRole);
    // end NOTE

    tableView->setCurrentIndex(mModel->index(row, mModel->fieldIndex("type"),
                                             QModelIndex()));
    tableView->scrollTo(mModel->index(row, mModel->fieldIndex("type")));
}


