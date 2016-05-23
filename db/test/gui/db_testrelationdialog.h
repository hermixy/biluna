/*****************************************************************
 * $Id: db_testrelationdialog.h 1435 2011-07-14 17:15:58Z rutger $
 * Created: Aug 9, 2010 3:04:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTRELATIONDIALOG_H
#define DB_TESTRELATIONDIALOG_H

#include <QtGui>
#include "db_modelfactory.h"
#include "db_tabledialog.h"
#include "db_testmodelfactory.h"


class DB_TestRelationDialog : public DB_TableDialog {

    Q_OBJECT

public:
    DB_TestRelationDialog(QWidget* parent) : DB_TableDialog(parent) {
        // nothing
    }

    virtual ~DB_TestRelationDialog() {
        // Dialogs create and delete models by themselves
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//        mf->removeModel(mModel->getModelType());
        delete mModel;
    }

    void init() {
        RB_ObjectBase* root = DB_TESTMODELFACTORY->getRoot();
        leRoot->setText(root->getValue("number").toString());

        // set model
        mModel = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTestRelation);
        mModel->setRoot(DB_TESTMODELFACTORY->getRootId());
        mModel->select();

        setWindowTitle(tr("Select test project"));
        lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();
        gbTable->setTitle(tr("GroupBox Blah Blah"));
        pbUp->hide();
        pbDown->hide();
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("DB_TestRelationDialog");
    }

};
#endif // DB_TESTRELATIONDIALOG_H
