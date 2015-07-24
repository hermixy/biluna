/*****************************************************************
 * $Id: sail_selectcoordinatedialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Aug 6, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_SELECTCOORDINATEDIALOG_H
#define SAIL_SELECTCOORDINATEDIALOG_H

#include <QtWidgets>
#include "sail_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"


class SAIL_SelectCoordinateDialog : public DB_TableDialog {

    Q_OBJECT

public:
    SAIL_SelectCoordinateDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("SAIL_SelectCoordinateDialog");
    }

    virtual ~SAIL_SelectCoordinateDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelCoordinate, false);
        mModel->setRoot(SAIL_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("SAIL Select coordinate[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of coordinates"));
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
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("SAIL_SelectCoordinateDialog");

        this->readSettings();
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

};

#endif // SAIL_SELECTCOORDINATEDIALOG_H
