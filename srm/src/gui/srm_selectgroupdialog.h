/*****************************************************************
 * $Id: srm_selectgroupdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTGROUPDIALOG_H
#define ACC_SELECTGROUPDIALOG_H

#include <QtWidgets>
#include "srm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class SRM_SelectGroupDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SRM_SelectGroupDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("SRM_SelectGroupDialog");
    }

    virtual ~SRM_SelectGroupDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelGroup, false);
//        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroups", "id", "groupname"));
        mModel->setRoot(SRM_MODELFACTORY->getRootId());
        mModel->select();



        setWindowTitle(tr("SRM Select group"));
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

        this->setHelpSubject("SRM_SelectGroupDialog");
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
