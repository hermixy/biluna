/*****************************************************************
 * $Id: sail_selectsymboldialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SELECTSYMBOLDIALOG_H
#define SAIL_SELECTSYMBOLDIALOG_H

#include <QtWidgets>
#include "sail_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class SAIL_SelectSymbolDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SAIL_SelectSymbolDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SAIL_SelectSymbolDialog");
    }

    virtual ~SAIL_SelectSymbolDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelSymbol, false);
        mModel->setRoot("syssetting");
        mModel->select();


        setWindowTitle(tr("SAIL Select symbol"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Symbols"));
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

        this->setHelpSubject("SAIL_SelectSymbolDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == 1 || i == RB2::HIDDENCOLUMNS + 1
                    || i == RB2::HIDDENCOLUMNS + 2) {
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

#endif // SAIL_SELECTSYMBOLDIALOG_H
