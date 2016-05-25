/*****************************************************************
 * $Id: db_testrelationwidget.cpp 1435 2011-07-14 17:15:58Z rutger $
 * Created: Aug 9, 2010 3:04:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testrelationwidget.h"

#include "db_modelfactory.h"

DB_TestRelationWidget::DB_TestRelationWidget(QWidget *parent)
        : DB_TableWidget(parent) {
    // nothing
}

DB_TestRelationWidget::~DB_TestRelationWidget() {
    // Dialogs create and delete models by themselves
    //        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
    //        mf->removeModel(mModel->getModelType());
    delete mModel;
}

void DB_TestRelationWidget::init() {
    RB_ObjectBase* root = DB_MODELFACTORY->getRoot();
    leRoot->setText(root->getValue("number").toString());

    // set model
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestRelation);
    mModel->setRoot(DB_MODELFACTORY->getRootId());
    mModel->select();

    setWindowTitle(tr("Test relation dialog"));
    lblRoot->setText(tr("Root"));
    lblRoot->hide();
    leRoot->hide();
    gbTable->setTitle(tr("Relation Blah Blah 2"));

    this->initEditSort();
    //        this->initEditUpDown();
    //        this->initSelectionOnly();
    //        this->initTest();

    this->setHelpSubject("DB_TestRelationWidget");
    readSettings();
}
