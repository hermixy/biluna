/*****************************************************************
 * $Id: acc_selectstockcategorydialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 28, 2011 12:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTSTOCKCATEGORYDIALOG_H
#define ACC_SELECTSTOCKCATEGORYDIALOG_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_treedialog.h"
#include "rb_sqlrelation.h"

class ACC_SelectStockCategoryDialog : public DB_TreeDialog {

    Q_OBJECT

public:
    ACC_SelectStockCategoryDialog(QWidget* parent) : DB_TreeDialog(parent) {
        this->setObjectName("ACC_SelectStockCategoryDialog");
    }

    virtual ~ACC_SelectStockCategoryDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockCategory, false);
        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroup", "id", "groupname"));
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Select from stock category"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Stock Category"));
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

        this->setHelpSubject("ACC_SelectStockCategoryDialog");
        this->readSettings();

        // Hidden columns only for tableviews, now treeview
        int colCount = mModel->columnCount();
        for (int i = 0; i < colCount; ++i) {
            if (i >= 2) {
                treeView->hideColumn(i);
            } else {
                treeView->showColumn(i);

                if (treeView->columnWidth(i) < 5) {
                    treeView->setColumnWidth(i, 100);
                }
            }
        }

    }
};

#endif // ACC_SELECTSTOCKCATEGORYDIALOG_H
