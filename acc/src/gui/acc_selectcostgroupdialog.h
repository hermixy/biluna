/*****************************************************************
 * $Id: acc_selectcostgroupdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Jun 21, 2012 15:08:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCOSTGROUPDIALOG_H
#define ACC_SELECTCOSTGROUPDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_modelfactory.h"
#include "db_treedialog.h"
#include "rb_sqlrelation.h"

class ACC_SelectCostGroupDialog : public DB_TreeDialog {

    Q_OBJECT

public:
    ACC_SelectCostGroupDialog(QWidget* parent) : DB_TreeDialog(parent) {
        this->setObjectName("ACC_SelectCostGroupDialog");
    }

    virtual ~ACC_SelectCostGroupDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostGroup, false);
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Select cost group"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Cost Center Groups"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTrvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_SelectCostGroupDialog");
        this->readSettings();

        // Hidden columns only for tableviews
        int colCount = mModel->columnCount();
        for (int i = 0; i < colCount; ++i) {
            treeView->showColumn(i);

            if (treeView->columnWidth(i) < 5) {
                treeView->setColumnWidth(i, 100);
            }
        }

        // treeView->sortByColumn(mModel->fieldIndex("accountcode"), Qt::AscendingOrder);

    }
};

#endif // ACC_SELECTCOSTGROUPDIALOG_H
