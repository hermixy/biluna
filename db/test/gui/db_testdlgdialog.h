/*****************************************************************
 * $Id: db_testdlgdialog.h 1309 2010-10-18 21:58:58Z rutger $
 * Created: Oct 31, 2009 3:04:40 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTDLGDIALOG_H
#define DB_TESTDLGDIALOG_H

#include <QtGui>
#include "db_modelfactory.h"
#include "db_testmodelfactory.h"
#include "rb_dialogbase.h"

class DB_TestDlgDialog : public RB_DialogBase {

    Q_OBJECT

public:
    DB_TestDlgDialog(QWidget* parent) : RB_DialogBase(parent) {
        // nothing
    }

    virtual ~DB_TestDlgDialog() {
        // Dialogs create and delete models by themselves
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//        mf->removeModel(mModel->getModelType());
        delete mModel;
    }

    void init() {
        RB_ModelFactory* mf = DB_MODELFACTORY;

        // Get parent object data
        RB_MmProxy* pMm = mf->getParentModel(DB_TestModelFactory::ModelTestDialog);
        if (!pMm) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "Dialog::init() no parent model ERROR");
            return;
        }

        QModelIndex index = pMm->getCurrentIndex(); // gives actual index
        if (!index.isValid()) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                            "Dialog::init() no valid index ERROR");
            return;
        }
        const RB_MmSource* mM = dynamic_cast<const RB_MmSource*>(index.model());
        leRoot->setText(mM->data(mM->index(index.row(), 3, index.parent()),
                                           Qt::DisplayRole).toString());

        // set model
        mModel = mf->getModel(DB_TestModelFactory::ModelTestDialog, false);

        setWindowTitle(tr("Dialog"));
        lblRoot->setText(tr("Root"));
        gbTable->setTitle(tr("GroupBox Blah Blah"));
        pbAdd->setText(tr("&Add"));
        pbDelete->setText(tr("&Delete"));
        pbUp->setText(tr("&Up"));
        pbDown->setText(tr("D&own"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
        this->initTest();

        this->setHelpSubject("DB_TestDlgDialog");
    }

};
#endif // DB_TESTDLGDIALOG_H
