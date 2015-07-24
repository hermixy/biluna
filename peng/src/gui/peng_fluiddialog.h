/*****************************************************************
 * $Id: peng_fluiddialog.h 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_FLUIDDIALOG_H
#define PENG_FLUIDDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "peng_modelfactory.h"
#include "rb_cmbdelegate.h"

class PENG_FluidDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PENG_FluidDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("PENG_FluidDialog");
    }

    virtual ~PENG_FluidDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelFluid);

        RB_StringList items;
        items << tr("(none)") << tr("Process") << tr("Utility");
        mModel->setTextList(mModel->fieldIndex("processutility_id"), items);

        int relationColumn = mModel->fieldIndex("hazardgroup_id");
        mModel->setRelation(relationColumn,
                            RB_SqlRelation("peng_hazardgroup", "id", "code"));
        relationColumn = mModel->fieldIndex("stability_id");
        mModel->setRelation(relationColumn,
                            RB_SqlRelation("peng_fluidstability", "id", "code"));

        mModel->setRoot(PENG_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("PENG Edit Fluids[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of fluids"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        tableView->setItemDelegateForColumn(mModel->fieldIndex("processutility_id"),
                                            new RB_CmbDelegate(this, items, mModel));


        this->setHelpSubject("PENG_FluidDialog");
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
};

#endif // PENG_FLUIDDIALOG_H
