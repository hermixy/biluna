/*****************************************************************
 * $Id: peng_typeselectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 16, 2011 21:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_TYPESELECTDIALOG_H
#define PENG_TYPESELECTDIALOG_H

#include <QtWidgets>
#include "peng_modelfactory.h"
#include "db_modelfactory.h"
#include "db_tabledialog.h"

class PENG_TypeSelectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PENG_TypeSelectDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("PENG_TypeSelectDialog");
    }

    virtual ~PENG_TypeSelectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipmentType, false);
        mModel->setRoot(PENG_MODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("Select Equipment Type"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of equipment types"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("PENG_TypeSelectDialog");
        this->readSettings();

        for (int i = 0; i < RB2::HIDDENCOLUMNS; i++) {
            tableView->hideColumn(i);
        }
    }
};

#endif // PENG_TYPESELECTDIALOG_H
