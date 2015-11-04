/*****************************************************************
 * $Id: sail_selectmapdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SELECTMAPDIALOG_H
#define SAIL_SELECTMAPDIALOG_H

#include <QtWidgets>
#include "sail_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class SAIL_SelectMapDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SAIL_SelectMapDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SAIL_SelectMapDialog");
    }

    virtual ~SAIL_SelectMapDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelMap, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();



        setWindowTitle(tr("SAIL Select map"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Groups"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("SAIL_SelectMapDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == RB2::HIDDENCOLUMNS || i == RB2::HIDDENCOLUMNS + 1) {
                tableView->showColumn(i);
            } else {
                tableView->hideColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }
    }
};

#endif // ACC_SELECTCUSTOMERDIALOG_H
