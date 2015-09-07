/*****************************************************************
 * $Id: crm_projecteditdialog.cpp 1834 2012-12-16 12:44:34Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_projecteditdialog.h"

#include "db_modelfactory.h"
#include "crm_modelfactory.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
CRM_ProjectEditDialog::CRM_ProjectEditDialog(QWidget *parent)
        : RB_ProjectEditDialog(parent) {
    // Write settings, otherwise RB_ProjectEditDialog
    this->setObjectName("CRM_ProjectEditDialog");
}

/**
 * Destructor
 */
CRM_ProjectEditDialog::~CRM_ProjectEditDialog() {
    RB_DEBUG->print("CRM_ProjectEditDialog::~CRM_ProjectEditDialog() OK");
}

/**
 * Initialize the dialog with model
 */
void CRM_ProjectEditDialog::init() {
    setWindowTitle(tr("Edit Projects[*]"));
    // setHelpSubject() only required when .cpp file is not used

    //
    // 1. Set model with ID
    //
    mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelProjectEdit, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leNumber, mModel->fieldIndex("code"));
    sbRevision->hide(); // mMapper->addMapping(sbRevision, mModel->fieldIndex("revision"));
    lblRevision->hide();
    mMapper->addMapping(pteDescription, mModel->fieldIndex("description"));
    leCompany->hide(); // mMapper->addMapping(leCompany, mModel->fieldIndex("company"));
    lblCompany->hide();
    leLocation->hide(); // mMapper->addMapping(leLocation, mModel->fieldIndex("location"));
    lblLocation->hide();

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
void CRM_ProjectEditDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("code"), QModelIndex());
    mModel->setData(idx, "<NEW>", Qt::EditRole);
//    idx = mModel->index(row, mModel->fieldIndex("revision"), QModelIndex());
//    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    leNumber->setFocus();
    leNumber->selectAll();
}