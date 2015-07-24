/*****************************************************************
 * $Id: sail_selectcoordinatedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SELECTCONTACTDIALOG_H
#define MRP_SELECTCONTACTDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "mrp_modelfactory.h"


class MRP_SelectContactDialog : public DB_TableDialog {

    Q_OBJECT

public:
    MRP_SelectContactDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("MRP_SelectContactDialog");
        mParentId = "";
    }

    virtual ~MRP_SelectContactDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void setParentId(const RB_String& parentId) {
        mParentId = parentId;

        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = MRP_MODELFACTORY->getModel(MRP_ModelFactory::ModelContact, false);
        mModel->setRoot(mParentId);
        mModel->select();

        this->initSelectionOnly();
        this->readSettings();
        int colCount = mModel->columnCount(QModelIndex());

        for (int i = 0; i < colCount; ++i) {
            if (i == RB2::HIDDENCOLUMNS || i == RB2::HIDDENCOLUMNS + 1) {
                tableView->hideColumn(i);
            } else {
                tableView->showColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }
    }

    void init() {
        setWindowTitle(tr("MRP Select contact[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of contacts"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditOnly();
//        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("MRP_SelectContactDialog");

    }

private:
    RB_String mParentId;
};

#endif // MRP_SELECTCONTACTDIALOG_H
