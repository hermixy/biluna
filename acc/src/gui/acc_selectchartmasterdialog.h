/*****************************************************************
 * $Id: acc_selectchartmasterdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 28, 2010 12:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCHARTMASTER_H
#define ACC_SELECTCHARTMASTER_H

#include <QtWidgets>
#include "acc_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class ACC_SelectChartMasterDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectChartMasterDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SelectChartMasterDialog");
    }

    virtual ~ACC_SelectChartMasterDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelChartMaster, false);
        // Relation
        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
               RB_SqlRelation("ACC_AccountGroup", "id", "groupname"));
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("ACC Select from chart of accounts"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Chart of accounts"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_SelectChartMasterDialog");
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

        tableView->sortByColumn(mModel->fieldIndex("accountcode"), Qt::AscendingOrder);

    }
};

#endif // ACC_SELECTCHARTMASTER_H
