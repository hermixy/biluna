/*****************************************************************
 * $Id: sail_projecteditdialog.cpp 1834 2012-12-16 12:44:34Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_projecteditdialog.h"

#include "db_modelfactory.h"
#include "sail_modelfactory.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
SAIL_ProjectEditDialog::SAIL_ProjectEditDialog(QWidget *parent)
        : RB_ProjectEditDialog(parent) {
    // Write settings, otherwise RB_ProjectEditDialog
    this->setObjectName("SAIL_ProjectEditDialog");
}

/**
 * Destructor
 */
SAIL_ProjectEditDialog::~SAIL_ProjectEditDialog() {
    RB_DEBUG->print("SAIL_ProjectEditDialog::~SAIL_ProjectEditDialog() OK");
}

/**
 * Initialize the dialog with model
 */
void SAIL_ProjectEditDialog::init() {
    setWindowTitle(tr("Edit Projects[*]"));
    // setHelpSubject() only required when .cpp file is not used

    //
    // 1. Set model with ID
    //
    mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leNumber, mModel->fieldIndex("number"));
    mMapper->addMapping(sbRevision, mModel->fieldIndex("revision"));
    mMapper->addMapping(pteDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(leCompany, mModel->fieldIndex("company"));
    mMapper->addMapping(leLocation, mModel->fieldIndex("location"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tableView, mModel);
    readSettings();

    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tableView->hideColumn(i);
        } else {
            tableView->showColumn(i);

            if (tableView->columnWidth(i) < 5) {
                tableView->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Button add clicked
 */
void SAIL_ProjectEditDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("number"), QModelIndex());
    mModel->setData(idx, "<NEW>", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("revision"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    leNumber->setFocus();
    leNumber->selectAll();
}
