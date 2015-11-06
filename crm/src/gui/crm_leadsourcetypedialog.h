/*****************************************************************
 * $Id: crm_leadsourcetypedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Mar 3, 2012 15:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_LEADSOURCETYPEDIALOG_H
#define CRM_LEADSOURCETYPEDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"


class CRM_LeadSourceTypeDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_LeadSourceTypeDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("CRM_LeadSourceTypeDialog");
    }

    virtual ~CRM_LeadSourceTypeDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelLeadSourceType, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("CRM Lead Source Type[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of lead source types"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditOnly();
        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("CRM_LeadSourceTypeDialog");

        this->readSettings();

        // Hidden columns only for tableviews
        int colCount = mModel->columnCount();
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

    void on_pbAdd_clicked() {
        // insert at the beginning
        int row = 0;
        mModel->insertRows(row, 1, QModelIndex());
        mModel->undoFilterSort();

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
//        QModelIndex idx;
//        idx = mModel->index(row, mModel->fieldIndex("currencydefault_id"), QModelIndex());
//        mModel->setData(idx, "0", Qt::EditRole);
//        mModel->submit();
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
        tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS));
    }

};

#endif // CRM_LEADSOURCETYPEDIALOG_H
